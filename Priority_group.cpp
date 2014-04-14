#include "Priority_group.h"


Priority_group::Priority_group(int id)
{
	this->id=id;
}


Priority_group::~Priority_group(void)
{
}

int Priority_group::GetId(){
	return this->id;
}
	
void Priority_group::SetId(int id){
	this->id=id;
}

std::list<class Virtual_node>* Priority_group::GetNodes(){
	return &this->nodes;
}

void Priority_group::SetNodes(std::list<class Virtual_node>* nodes){
	this->nodes=*nodes;
}
