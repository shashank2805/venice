#include <string>
#include <iostream>
#include <stdio.h>
#include "command_executor.h"
#include <stdlib.h>
using namespace std;

string CommandExecutor::execute(string command)
{
	FILE * pipe = popen(command.c_str(), "r");
	
	if(!pipe) 
		return "ERROR";
	
	char buffer[128];
	string result;

	while(!feof(pipe))
	{
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	
	pclose(pipe);

	return result;	
}

void CommandExecutor::executeScript(string fileName)
{
	system(fileName.c_str());
}
