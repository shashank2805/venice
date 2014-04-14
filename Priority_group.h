#pragma once
#include "Virtual_node.hpp"
using namespace std;


class Priority_group
{
private:
	int id;
	std::list<class Virtual_node> nodes;

public:
	Priority_group(int);
	~Priority_group(void);
	int GetId();
	void SetId(int);
	std::list<class Virtual_node>* GetNodes();
	void SetNodes(std::list<class Virtual_node>* nodes);
};

