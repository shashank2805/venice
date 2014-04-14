//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : SAVI/VDCPlanner
//  @ File Name : Substrate_link.cpp
//  @ Date : 17/04/2013
//  @ Author : Maissa Jabri
//
//


#include "Substrate_link.hpp"
#include "Substrate_node.hpp"

Substrate_link::Substrate_link(int id,Substrate_node * from,Substrate_node * to, double bw, double avai){
	this->id=id;
	this->bandwidth=bw;
	this->availability=avai;
	this->from=from;
	this->to=to;
}

void Substrate_link::SetAvailability(double availability){
	this->availability=availability;
}
	
double Substrate_link::GetAvailability(){
	return this->availability;
}

Substrate_node* Substrate_link::GetSourceNode() {
	return this->from;
}

Substrate_node* Substrate_link::GetDestinationNode() {
	return this->to;
}

double Substrate_link::GetUsedBandwidth() {
	double bw=0;
	std::list<Path>::iterator it1;
	if(this->embeddedPaths.empty())
		return 0;
	for (it1=this->embeddedPaths.begin(); it1!=this->embeddedPaths.end(); it1++)
	{
		bw+=it1->GetBandwidth();
	}
	return bw;
}

double Substrate_link::GetRestBandwidth() {
	double bw=this->bandwidth;
	std::list<Path>::iterator it1;
	cout<<" bw = "<<bw<<"\n";
	if(this->embeddedPaths.empty())
		return bw;
	for (it1=this->embeddedPaths.begin(); it1!=this->embeddedPaths.end(); it1++)
	{
		bw-=it1->GetBandwidth();
	}
	return bw;
}

std::list<Path>* Substrate_link::GetEmbeddedPaths() {
	return &this->embeddedPaths;
}

bool Substrate_link::removeEmbeddedPath( Path * path){
	std::list<Path>::iterator it1;
	bool found = false;
		if(this->embeddedPaths.empty())
		{
			cout<<" No Embedded Paths for the Substrate Link with id : "<<this->id<<"\n";
		}
		else
		{
			it1=this->embeddedPaths.begin();
			while(!found && it1!=this->embeddedPaths.end())
			{
				//if(it1->GetId()==path->GetId())
				if(it1->GetName()==path->GetName())
				{
					found=true;
					//cout<< " ********* name1 : "<<it1->GetName()<<" name2 : "<<path->GetName()<<endl;
					it1=this->embeddedPaths.erase(it1);
					//cout<< " **size after erase : "<<this->embeddedPaths.size()<<endl;
				}
				else
				{
					it1++;
				}
			}
		}
		return found;
}

void Substrate_link::DisplayEmdeddedPaths(){
	std::list<Path>::iterator it1;
	if(this->embeddedPaths.empty())
	{
		cout<<" No Embedded Paths for the Substrate Link with id : "<<this->id<<"\n";
	}
	else
	{
		cout<<" Embedded Paths for the Substrate Link with id : "<<this->id<<" are : \n";
			for (it1=this->embeddedPaths.begin(); it1!=this->embeddedPaths.end(); it1++)
			{
				cout<<" Path id : "<<it1->GetId()<<"\n";
			}
	}
}

void Substrate_link::Display(){
	//cout << "id : " << this->id << " availability : " << this->availability << "\n";
	cout << "id : " << this->id << " from : " << this->from->GetId()
		<< " to : " << this->to->GetId()
			<< " availability : " << this->availability << "\n";
}

