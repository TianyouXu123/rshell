rshell

We got five functions:
1. Command_Parsing{recognize command};
2. Comment_Parsing{recognize # as comment};
3. Execution_Parsing{split different commands and return flags for executions};
4. Command_Connector{recognize connectors};
5. Do_Execution{have pid objects for processing executions};
6. Command_Flag{simplify main function return a value to determine whtether the Rshell is terminated or not}

implement test command and preeendence operator.
Mistake(bug) we found: our rshell will output the "test command" twice.

Unable to track all the connectors in a long command, 
we decide to track the connectors by comparing the 
command one by one. 

