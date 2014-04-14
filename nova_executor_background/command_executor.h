#ifndef COMMAND_EXECUTOR_H_
#define COMMAND_EXECUTOR_H_
#include <string>
#include <vector>

using namespace std;

class CommandExecutor
{
public:
	string execute(string command);
	void executeScript(string fileName);
};
#endif/*COMMAND_EXECUTOR_H_*/
