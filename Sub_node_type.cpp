#include "Sub_node_type.h"

Sub_node_type::Sub_node_type(int id,double availability){
	this->id=id;
	this->availability=availability;
}

Sub_node_type::Sub_node_type(void)
{
}


Sub_node_type::~Sub_node_type(void)
{
}

int Sub_node_type::GetId(){
	return this->id;
}
	
void Sub_node_type::SetId(int id){
	this->id=id;
}
	
double Sub_node_type::GetAvailability(){
	return this->availability;
}
	
void Sub_node_type::GetAvailability(double availability){
	this->availability=availability;
}

std::list<Substrate_node>* Sub_node_type::GetSubstrateNodes() {
	return &(this->nodes);
}

// NOT YET written
std::list<Substrate_node>* Sub_node_type::GetActiveNodes() {
	std::list<Substrate_node> active_nodes;
	return &active_nodes;
}
