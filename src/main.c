#include <iostream>
#include <cstring>
#include <vector>

#include <sys/param.h> //MAXPATHLEN
#include <sys/types.h> //processes IDs
#include <sys/wait.h>

#include <unistd.h> //gethostname, getcwd
#include <stdlib.h>
#include <stdio.h>

using namespace std;

//Will use these in the future
//const char *SEMICO = ";";
//const char *AND = "&&";
//const char *OR = "||";
//const char *TEST = "test";
//const char *L_PARA = "(";
//const char *R_PARA = ") ";
//const char *L_BRAC = "[";
//const char *R_BRAC = "] ";

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
	
	//obtain current working directory
	char directory[MAXPATHLEN];
	char* CWD=getcwd(directory,MAXPATHLEN);

	string Command_str;
	bool finish=false;
	
	while(!finish)
	{
		cout<<"["<<username<<"@"<<server_name<<CWD<<"]"<<"$ ";
		getline(cin, Command_str);

		if (!Command_str.empty())
		{
			vector<char*> Command_Vec;
			vector<int> Connector_Vec;
			
			Command_Parsing(Command_str, Command_Vec, Connector_Vec);
			
			int flag = 0;
			unsigned int VecSize = Command_Vec.size();
			
			for (unsigned int i = 0; i < VecSize; ++i)
			{
			    //First cmd or after ';'
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
					finish=true;
				}
				
				if (flag == -2)
                {
					char* tmp_command=Command_Vec[i];
					char directory [MAXPATHLEN];
					directory[0]='/';
					
					int index=1;
					unsigned int StrLen = strlen(tmp_command);
					for (unsigned int i = 3;i<StrLen; i++)
	          		{
		    			if(tmp_command[i]==' '){}
		    			else
		    			{
	        				directory[index]=tmp_command[i];
	        				index++;
		    			}
		    		}
					cout<<strcat(CWD,directory)<<endl;
                }
			}
		}
	}
	return 0;
}

int Execution_Parsing (char* arguments)
{
	if (strncmp(arguments, "cd", 2) == 0)
	    return -2;

	char* tmp_arguments = Comment_Parsing(arguments);
	vector <char*> arguments_vector;
	
	// strtok places a NULL terminator in front of the token, if found 
	char* arguments_part = strtok (tmp_arguments, " ");
	
	while (arguments_part != NULL)
	{
		arguments_vector.push_back (arguments_part);
		
    //A second call to strtok using a NULL as the first parameter returns a pointer to the character following the token
    
		arguments_part = strtok (NULL, " ");
	}
	arguments_vector.push_back (0);
	
	int arguments_size = arguments_vector.size ();
	char* arguments_result[10];
	
	for (int i = 0; i < arguments_size; i++)
	{
		arguments_result[i] = arguments_vector[i];
	}
	
	if (strncmp(arguments, "exit", 4) == 0)
	{
		cout<<"Goooooode bye!"<<endl;
		return -5;
	}
	
	return Do_Execution(arguments_result);
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
	while (Command_string[i] != '\0')
	{
		result = Command_string[i];
		
		//different connector condition
		bool ifcondition1 = (Command_string[i] == ';');
		bool ifcondition2 = ((Command_string[i] == '&') && (Command_string[i + 1] == '&'));
		bool ifcondition3 = ((Command_string[i] == '|') && (Command_string[i + 1] == '|'));
		bool ifcondition4 = ((Command_string[i] == 't') && (Command_string[i + 1] =='e') && (Command_string[i + 2] =='s') && (Command_string[i + 3] =='t'));
		bool ifcondition5 = (Command_string[i] == '(');
		bool ifcondition6 = ((Command_string[i] == ')') && (Command_string[i + 1] !='\0'));
		bool ifcondition7 = (Command_string[i] == '[');
		bool ifcondition8 = ((Command_string[i] == ']') && (Command_string[i + 1] !='\0'));
		
		if(ifcondition1)
			return i;
		else if(ifcondition2)
			return i;
		else if(ifcondition3)
			return i;
        else if(ifcondition4)
			return i;
        else if(ifcondition5)
			return i;
        else if(ifcondition6)
			return i+1;
        else if(ifcondition7)
			return i;
        else if(ifcondition8)
			return i+1;
        i++;
	}
	result = '\0';
	return -1;
}

int Do_Execution (char* arguments[])
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
