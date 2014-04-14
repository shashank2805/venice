#include "AdjNode.hpp"


AdjNode::AdjNode(void)
{

}

AdjNode::AdjNode(int l, int n)
{
	this->length=l;
	this->next=n;
}


AdjNode::~AdjNode(void)
{
}

int AdjNode::GetLength(){
	return this->length;
}

void AdjNode::SetLength(int length){
	this->length=length;
}

int AdjNode::GetNext(){
	return this->next;
}
	
void AdjNode::SetNext(int next){
	this->next=next;
}