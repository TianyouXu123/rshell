#include <iostream>
#include <cstring>
#include <vector>

#include <sys/types.h> //processes IDs
#include <sys/wait.h>

#include <unistd.h> //gethostname, getcwd
#include <stdlib.h>
#include <stdio.h>

using namespace std;

bool Command_Flag(string Command_str);
void Command_Parsing(string Command_string, vector<char*>& Command_Vec, vector<int>& Connector_Vec);
int Execution_Parsing(char* arguments);
char* Comment_Parsing(char* arguments);

int Command_Connector(const char* Command_string, const int start, char& result);

int Do_Execution (char* arguments[]);

int main (int argc, char* argv[])
{
	//obtain servername and username
	char server_name[50];
	gethostname(server_name, 64);
	char* username=getlogin();

	string Command_str;
	bool finish=false;
	
	while(!finish)
	{
		cout<<username<<"@"<<server_name<<"$ ";
		getline(cin, Command_str);
        finish = Command_Flag(Command_str);
	}
	return 0;
}

bool Command_Flag(string Command_str)
{
    if (!Command_str.empty())
	{
		vector<char*> Command_Vec;
		vector<int> Connector_Vec;
		
		Command_Parsing(Command_str, Command_Vec, Connector_Vec);
		
		int flag = 0;
		int VecSize = Command_Vec.size();
		
		for (int i = 0; i < VecSize; ++i)
		{
		    //After ';'; 1st command
			if (Connector_Vec[i] == 0)
			{
				flag = Execution_Parsing (Command_Vec[i]);
			}
			//After '&&'
			else if (Connector_Vec[i] == 1 && flag == 0)
			{
				flag = Execution_Parsing (Command_Vec[i]);
			}
			//After '||'
			else if (Connector_Vec[i] == 2 && flag != 0)
			{
				flag = Execution_Parsing (Command_Vec[i]);
			}
			if (flag == -5)
			{
				return true;
			}
		}
	}
	return false;
}

int Execution_Parsing (char* arguments)
{ 
    
    if (strncmp(arguments, "exit", 4) == 0)
	{
		cout<<"Execute: exit"<<endl;
		return -5;
	}
	else if (strncmp(arguments, "(", 1) == 0)
    {
        int temp_size = strlen(arguments)-2;
        char* tmp_arguments = new char[temp_size];
        for (int i = 0; i < temp_size; ++i)
        {
            tmp_arguments[i] = arguments[i+1];
        }
        return Command_Flag(tmp_arguments);
    }
    else if (strncmp(arguments, "[", 1) == 0 || strncmp(arguments, "test", 4) == 0)
    {
        char* tmp_arguments = Comment_Parsing(arguments);
	    vector <char*> arguments_vector;
	    
        // strtok places a NULL terminator in front of the token, if found 
	    char* arguments_part = strtok (tmp_arguments, " ");
	    
        while (arguments_part != NULL)
        {
            //push test command if [ is found
            if (strncmp(arguments_part, "[", 1) == 0)
            {
                char temp[4] = {'t','e','s','t'};
                arguments_vector.push_back(temp);
            }
            //ignore if ] is found
            else if (strncmp(arguments_part, "]", 1) != 0)
            {
                arguments_vector.push_back (arguments_part);
            }
            
            //A second call to strtok using a NULL as the first parameter returns a pointer to the character following the token
            arguments_part = strtok (NULL, " ");
        }
        arguments_vector.push_back (0);
        //determine checking flags
        if (strncmp(arguments_vector[1], "-e", 2) != 0 && strncmp(arguments_vector[1], "-f", 2) != 0 && strncmp(arguments_vector[1], "-d", 2) != 0 )
        {
            char temp[2] = {'-','e'};
            vector<char*>::iterator it;
            it = arguments_vector.begin();
            it++;
            arguments_vector.insert(it, temp);
        }
        
        int arguments_size = arguments_vector.size ();
        char* arguments_result[10];
        for (int i = 0; i < arguments_size; i++)
        {
            arguments_result[i] = arguments_vector[i];
        }
        int test_result = Do_Execution(arguments_result);
        if (test_result == 0)
        {
            cout<<"(True)"<<endl;
        }
        else
        {
            cout<<"(False)"<<endl;
        }
        return Do_Execution(arguments_result);
    }
    else
    {
        char* tmp_arguments = Comment_Parsing(arguments);
        vector<char*> arguments_vector;
        
        char* arguments_part = strtok (tmp_arguments, " ");
        
        while (arguments_part != NULL)
        {
            arguments_vector.push_back (arguments_part);
            arguments_part = strtok (NULL, " ");
        }
        arguments_vector.push_back (0);
        
        int arguments_size = arguments_vector.size ();
        char * arguments_result[10];
        
        for (int i = 0; i < arguments_size; i++)
        {
            arguments_result[i] = arguments_vector[i];
        }
        
        return Do_Execution(arguments_result);
    }
}

void Command_Parsing(string Command_string, vector<char*>& Command_Vec, vector<int>& Connector_Vec)
{
	Connector_Vec.push_back(0);
	
	int i = 0;
	int j = 0;
	
	while (i != -1)
	{
		string tmp_string;
		char tmp_Result;
		
		int a = 0;
		int b = 0;
		j = Command_Connector(Command_string.c_str(), i, tmp_Result);
		
		if (j == -1)
		{
			b = -1;
		}
		else if (tmp_Result == ';')
		{
			Connector_Vec.push_back(0);
			b = j+2;
		}
		else if (tmp_Result == '&')
		{
			Connector_Vec.push_back(1);
			a = -1;
			b = j+3;
		}
		else if (tmp_Result == '|')
		{
			Connector_Vec.push_back(2);
			a = -1;
			b = j+3;
		}
		
		tmp_string = Command_string.substr(i, j - i + a);
		char* Temp_Command_string = new char[tmp_string.length () + 1];
		strcpy (Temp_Command_string, tmp_string.c_str());
		Temp_Command_string[tmp_string.length ()] = '\0';
		Command_Vec.push_back(Temp_Command_string);
		i = b;
	}
}

char* Comment_Parsing(char* arguments)
{
	int i = 0;
	while (arguments[i] != '\0')
	{
	    //check comment sign here
		if (arguments[i] == '#')
		{
			char* tmp = new char[i+1];
			for (int j = 0; j < i; ++j)
			{
				tmp[j] = arguments[j];
			}
			tmp[i] = '\0';
			return tmp;
		}
		i++;
	}
	return arguments;
}

int Command_Connector(const char* Command_string, const int start, char& result)
{
	int i = start;
	
	//different connector condition
	
	/*bool LEFT_PARE = (Command_string[i] == '(');
	bool RIGHT_PARE = (Command_string[i] == ')');
	bool SEMICO = ((Command_string[i] == ';') && (Command_string[i + 1] == ' '));
	bool AND = ((Command_string[i - 1] == ' ') && (Command_string[i] == '&') && (Command_string[i + 1] == '&') && (Command_string[i + 2] == ' '));
	bool OR = ((Command_string[i - 1] == ' ') && (Command_string[i] == '|') && (Command_string[i + 1] == '|') && (Command_string[i + 2] == ' '));
		
	if (LEFT_PARE)
	{
	    i++;
	    while(!RIGHT_PARE)
	    {
	        i++;
	    }
	}
	while (Command_string[i] != '\0')
	{
		result = Command_string[i];
		
		if(SEMICO)
			return i;
		else if(AND)
			return i;
		else if(OR)
			return i;
        i++;
	}*/
	if (Command_string[i] == '(')
	{
	    i++;
	    while(Command_string[i] != ')')
	    {
	        i++;
	    }
	}
	while (Command_string[i] != '\0')
    {
        result = Command_string[i];
        if ((Command_string[i] == ';') && (Command_string[i + 1] == ' '))
            return i;
        else if ((Command_string[i - 1] == ' ') && (Command_string[i] == '&') && (Command_string[i + 1] == '&') && (Command_string[i + 2] == ' '))
            return i;
        else if ((Command_string[i - 1] == ' ') && (Command_string[i] == '|') && (Command_string[i + 1] == '|') && (Command_string[i + 2] == ' '))
            return i;
        i++;
    }
	result = '\0';
	return -1;
}

int Do_Execution(char* arguments[])
{
	//pid for parent and children
	pid_t pid;
	pid_t c_pid;
	
	int status = 0;
	c_pid = fork ();
	
	if (c_pid < 0)
	{
		perror ("Fork Failed");
		exit (1);
	}
	else if (c_pid == 0)
	{
	    printf ("Execute %s\n", arguments[0]);
		execvp (arguments[0], arguments);
		perror ("Execution Failed");
		exit (1);
	}
	else if (c_pid > 0)
	{
		if ((pid = wait(&status)) < 0)
		{
			perror ("Waiting");
			exit (1);
		}
		return status;
	}
	return status;
}
