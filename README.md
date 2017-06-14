rshell

We got six basic functions:
1. Command_Parsing{recognize command};
2. Comment_Parsing{recognize # as comment};
3. Execution_Parsing{split different commands and return flags for executions};
4. Command_Connector{recognize connectors};
5. Do_Execution{have pid objects for processing executions};
6. Command_Flag{simplify main function return a value to determine whtether the Rshell is terminated or not}
7. Input_Output_Redirect(handle two functions: pclose,popen to process result_buffer and output it; then return a number by pclose, utilizes as flag.)

Conclusion:
The main function of the rshell: implement test command & precendence operator.
We optimized the Command_Connector function by encupslating different connectors into single bool functions so that we can maintain the rshell more easily.
We write another boolean function 'bool REDIRE' to support the rshell.

Mistake(bug) we found: our rshell will output the "test command" twice.

