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

void Command_Parsing(string cmdString, vector<char*>& cmdVector, vector<int>& cntVector);
int Execution_Parsing(char* arguments);
char* Comment_Parsing(char* arguments);


int Command_Connector(const char* cmdString, const int begin, char& res);

int main (int argc, char* argv[])
{
	//obtain servername and username
	char server_name[50];
	gethostname(server_name, 64);
	char* username=getlogin();
	
	//obtain current working directory
	char directory[MAXPATHLEN];
	char* CWD=getcwd(directory,MAXPATHLEN);

	string cmdStr;
	
	while(1)
	{
		cout<<"["<<username<<"@"<<server_name<<CWD<<"]"<<"$ ";
		getline(cin, cmdStr);

		if (!cmdStr.empty())
		{
			vector<char*> cmdVector;
			vector<int> cntVector;
			Command_Parsing(cmdStr, cmdVector, cntVector);
			
			//execute command
			int last_status = 0;
			for (unsigned int i = 0; i < cmdVector.size(); ++i)
			{
				//first command or after semicolon
				if (cntVector[i] == 0)
				{
					last_status = Execution_Parsing (cmdVector[i]);
				}
				//command after &&
				else if (cntVector[i] == 1 && last_status == 0)
				{
					last_status = Execution_Parsing (cmdVector[i]);
				}
				//command after ||
				else if (cntVector[i] == 2 && last_status != 0)
				{
					last_status = Execution_Parsing (cmdVector[i]);
				}
				if (last_status == -2)
                {
                    //Execute cd
                    cout<<"Executing: cd"<<endl;
					char* temp=cmdVector[i];
					char directory [MAXPATHLEN];
					directory[0]='/';
					int j=1;
					for (unsigned int i = 3;i<strlen(temp); i++)
	          		{
		    			if(temp[i]==' '){}
		    			else
		    			{
	        				directory[j]=temp[i];
	        				j++;
		    			}
		    		}
					cout<<strcat(CWD,directory)<<endl;
		       
					int rc=1;
					if(rc<0)
					{
		    			cout<<"Error"<<endl;
					}
                }
			}
		}
	}
	return 0;
}

int Execution_Parsing (char* arguments)
{
	if (strncmp(arguments, "exit", 4) == 0)
	{
		//Exit program
		cout<<"Executing: exit"<<endl;
		exit(0);
	}

	if (strncmp(arguments, "cd", 2) == 0)
	    return -2;


	char* tmparguments = Comment_Parsing(arguments);
	vector <char*> arguments_vector;
	
	// strtok places a NULL terminator in front of the token, if found 
	char* pch = strtok (tmparguments, " ");
	
	while (pch != NULL)
	{
		arguments_vector.push_back (pch);
		
    //A second call to strtok using a NULL as the first parameter returns a pointer to the character following the token
    
		pch = strtok (NULL, " ");
	}
	arguments_vector.push_back (0);
	
	int arguments_size = arguments_vector.size ();
	char* arguments_result[10];
	
	for (int i = 0; i < arguments_size; i++)
	{
		arguments_result[i] = arguments_vector[i];
	}
	
	return Do_Execution(arguments_result);
}

void Command_Parsing(string cmdString, vector<char*>& cmdVector, vector<int>& cntVector) //2*
{
	cntVector.push_back(0);
	int i = 0;
	int j = 0;
	while (i != -1)
	{
		string tmpstring;
		char tmpRes;
		int a = 0;
		int b = 0;
		j = Command_Connector(cmdString.c_str(), i, tmpRes);
		if (j == -1)
		{
			b = -1;
		}
		else if (tmpRes == ';')
		{
			cntVector.push_back(0);
			b = j+2;
		}
		else if (tmpRes == '&')
		{
			cntVector.push_back(1);
			a = -1;
			b = j+3;
		}
		else if (tmpRes == '|')
		{
			cntVector.push_back(2);
			a = -1;
			b = j+3;
		}
		tmpstring = cmdString.substr(i, j - i + a);
		char* tmpcmdString = new char[tmpstring.length () + 1];
		strcpy (tmpcmdString, tmpstring.c_str());
		tmpcmdString[tmpstring.length ()] = '\0';
		cmdVector.push_back(tmpcmdString);
		i = b;
	}
}

char* Comment_Parsing(char* arguments)
{
	int i = 0;
	
	while (arguments[i] != '\0')
	{
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


int Do_Execution (char* arguments[]) //3*
{
	//Execute the command
	
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
		cout<<"Executing: %s\n"<<arguments[0];
		execvp (arguments[0], arguments);
		perror ("Execve Faild");
		exit (1);
	}
	else if (c_pid > 0)
	{
		if ((pid = wait (&status)) < 0)
		{
			perror ("wait");
			exit (1);
		}
		return status;
	}

	return status;
}


int Command_Connector(const char* cmdString, const int begin, char& res) //1*
{
	int i = begin;
	while (cmdString[i] != '\0')
	{
		res = cmdString[i];
		if (cmdString[i] == ';')
			return i;
		else if ((cmdString[i] == '&') && (cmdString[i + 1] == '&'))
			return i;
		else if ((cmdString[i] == '|') && (cmdString[i + 1] == '|'))
			return i;
        else if (cmdString[i] == '(')
            return i;
        else if ((cmdString[i] == 't') && (cmdString[i + 1] =='e') && (cmdString[i + 2] =='s') && (cmdString[i + 3] =='t'))
            return i;
        else if ((cmdString[i] == ')') && (cmdString[i + 1] !='\0'))
            return i+1;
        else if (cmdString[i] == '[')
            return i;
        else if ((cmdString[i] == ']') && (cmdString[i + 1] !='\0'))
            return i+1;
		i++;
	}
	res = '\0';
	return -1;
}

