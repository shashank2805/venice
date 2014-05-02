#pragma once
#include "mapping.hpp"
#include "Path.hpp"
#include <stdio.h>
using namespace std;

class Mapping_trial : public mapping
{
public:
	Mapping_trial(class Request *,int);
	~Mapping_trial(void);
	bool Mapp(class Substrate_network* sub);
	bool MappOnType(class Virtual_node*,class Substrate_network*,int);
	bool FindPaths(Substrate_network*);
	bool MapShortestPath(Path*,Substrate_network*);
	void FreeMapp(class Substrate_network* sub);
	void FreeMappPartial(class Substrate_network* sub,int);
	void FreeMappNodePartial(class Substrate_network* sub,int);
	void FreeMappPathPartial(class Substrate_network* sub,int);
	int* GetNodeMapp();
	std::list<int> * GetPathMapp();
	int* GetPathLength();
	double avaiMapp(Substrate_network* sub);
	double avaiMappNode(Substrate_network* sub);
	double avaiMappPath(Substrate_network* sub);


private:
	int * node_mapp;
	std::list<int> * path_mapp;//tableu de listes d'entiers
	int * path_length;
};

