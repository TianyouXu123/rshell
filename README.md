rshell

We got five functions:
1. Command_Parsing{recognize command};
2. Comment_Parsing{recognize # as comment};
3. Execution_Parsing{split different commands and return flags for executions};
4. Command_Connector{recognize connectors};
5. Do_Execution{have pid objects for processing executions};

Unable to track all the connectors in a long command, 
we decide to track the connectors by comparing the 
command one by one. 

