#pragma once
#include "Substrate_node.hpp"

class Sub_node_type
{
private:
	int id;
	double availability;
	std::list<class Substrate_node> nodes;
public:
	Sub_node_type(int,double);
	Sub_node_type(void);
	~Sub_node_type(void);
	int GetId();
	void SetId(int);
	double GetAvailability();
	void GetAvailability(double);
	std::list<class Substrate_node>* GetSubstrateNodes();
	std::list<class Substrate_node>* GetActiveNodes();
};

