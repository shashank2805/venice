#include "Mapping_trial.h"

Mapping_trial::Mapping_trial(Request * req, int id):mapping(req)
{
	this->id=id;
	this->node_mapp = new int[this->request->GetNumberNodes()];
	this->path_length = new int[this->request->GetLinks()->size()];
	this->path_mapp = new std::list<int>[this->request->GetLinks()->size()];
}

Mapping_trial::~Mapping_trial(void)
{
}

bool Mapping_trial::Mapp(Substrate_network* sub){

	bool mapped=false,abort=false,abort2=false;
	std::list<Virtual_node>::iterator it1;
	size_t i =1;
	while (!abort && i<=this->request->GetGroups()->size())
	{
		it1=this->request->GetGroupById(i)->GetNodes()->begin();
		while (!abort && it1!=this->request->GetGroupById(i)->GetNodes()->end())
			{
				//for testing-----------cout<<" Attempt to map node with id : "<<it1->GetId()<<"\n";
				switch(this->id)
				{
				case 1:
				mapped=MappOnType(&(*it1),sub,1);
				if(!mapped)
				mapped=MappOnType(&(*it1),sub,2);
				if(!mapped)
				mapped=MappOnType(&(*it1),sub,3);
				if(!mapped)
				mapped=MappOnType(&(*it1),sub,4);
				break;
				case 2:
				mapped=MappOnType(&(*it1),sub,2);
				if(!mapped)
				mapped=MappOnType(&(*it1),sub,3);
				if(!mapped)
				mapped=MappOnType(&(*it1),sub,4);
				break;
				case 3:
				mapped=MappOnType(&(*it1),sub,3);
				if(!mapped)
				mapped=MappOnType(&(*it1),sub,4);
				break;
				case 4:
				mapped=MappOnType(&(*it1),sub,4);
				break;
				}
				if(!mapped)// this means the mapping is impossible the mapping fails
				{
					abort=true;
					this->stateMapping=STATE_MAP_NODE_FAIL;
					cout<<"--STATE_MAP_NODE_FAIL--for Trial "<<this->id<<"\n";
					// if the embedding has failed from the first try this means no previous mapping are done
					// otherwise to be done later on ------------------------------------------------------------------
					if(it1->GetId() != this->request->GetGroupById(1)->GetNodes()->begin()->GetId())// 1 is the id of the first Group
					{
						//for testing-----------cout<<" Attempt to map node with id : "<<it1->GetId()<<" failed!!!!! and it's not the first node which has id : "<<this->request->GetGroupById(1)->GetNodes()->begin()->GetId()<<"\n";
						//if we're here this means at least the previous node is embedded so let's go back fo a step
						this->FreeMappPartial(sub,it1->GetId()-1);
					}
				}
				else
				{
					it1++;
				}
			}
			
		i++;
	}
	if(!abort)// this means the node mapping was successful let's mapp links for now
	{
		this->stateMapping=STATE_MAP_NODE;
		abort2 = !this->FindPaths(sub);
		if(!abort2)
		{
			cout<<"--STATE_MAP_LINK--for Trial "<<this->id<<"\n";
			this->stateMapping=STATE_MAP_LINK;
		}
		else
		{
			cout<<"--STATE_MAP_LINK_FAIL-- for Trial "<<this->id<<"\n";
			this->stateMapping=STATE_MAP_LINK_FAIL;
		}
		this->availability=avaiMapp(sub);
		return abort2;
	}
	this->availability=avaiMapp(sub);
	return abort;
	
	
}

bool Mapping_trial::MappOnType(Virtual_node* node,Substrate_network* sub,int type){

	bool mapped = false;
	list<Substrate_node>::iterator it1;

	it1 = sub->GetTypeById(type)->GetSubstrateNodes()->begin();
	while (!mapped && it1 != sub->GetTypeById(type)->GetSubstrateNodes()->end())
	{
		//for testing-----------
		//cout<<" Attempt to map node with id : "<<node->GetId()<<" failed on type"<<type<<" \n";
		//cout<<" bw : "<<node->GetBandwidth()<<" vs restBw : "<<it1->GetRestBandwidth()<<" \n";
		//cout<<" cpu : "<<node->GetCpu()<<" vs restCpu : "<<it1->GetRestCpu()<<" \n";
		//cout<<" mem : "<<node->GetMemory()<<" vs restMem : "<<it1->GetRestMemory()<<" \n";
		// we map only on servers
		if(it1->GetType() == 1 && it1->GetRestBandwidth()>=node->GetBandwidth() && it1->GetRestCpu()>=node->GetCpu() && it1->GetRestMemory()>=node->GetMemory())
		{ 
			mapped=true;
			//this is a temporary allocation to update ressources availability
			it1->GetEmbeddedNodes()->push_back(*node);
			node->SetEmbeddingNode(&(*it1));
			if(it1->GetActive()==0)
			{
				it1->SetActive(1);
			}
			//----------------------------------------------------------------
			this->node_mapp[node->GetId()]=it1->GetId();
		}
		it1++;
	}
	return mapped;
	
}

bool Mapping_trial::FindPaths(Substrate_network* sub){
	//variables
	bool abort=false;
	list<Path>::iterator it1;

	//operations
	it1 = this->request->GetLinks()->begin();
	while (!abort && it1 != this->request->GetLinks()->end())
	{
		abort=!(MapShortestPath(&(*it1),sub));
		//for testing----
		if(abort)
		{
			cout<<" abort = true for "<<it1->GetId()<<"\n";
		}
		if(!abort)
		{
			it1++;
		}
	}
	return !(abort);
}


bool Mapping_trial::MapShortestPath(Path* path,Substrate_network* sub){

	//variables
	bool mapped=false,abort=false;
	int from,to;
	int nextId;

	//operations
	from=path->GetSourceNode()->GetEmbeddingNode()->GetId();
	to=path->GetDestinationNode()->GetEmbeddingNode()->GetId();
	//for testing
	if(from==to)//mapped in the same substrate node
	{
		//int the final mapping----------------path->SetLength(0);
		//for testing------cout<<" from = to for the path "<<path->GetId()<<"\n";
		this->path_length[path->GetId()]=0;
		mapped=true;
	}
	else
	{
		if(sub->GetAdjMatrix()[from][to].GetNext()==-1)//no enough bw on the link
		{
			this->path_length[path->GetId()]=-1;
			//for testing
			cout<<" no enough bw on the link for the path "<<path->GetId()<<"\n";
			mapped=false;
		}
		else
		{
			nextId=from;
			while(nextId!=to && !abort)
			{
				from=nextId;
				nextId=sub->GetAdjMatrix()[nextId][to].GetNext();
				//for testing-----------cout<<"--attempt to find substrate link from = "<<from<<" to = "<<nextId<<" \n";
				if(sub->GetLinkByFromTo(from,nextId)== NULL)
				{
					//for testing-----------cout<<"--attempt to mappp link : "<<path->GetId()<<" oops\n";
					this->path_length[path->GetId()]=-1;
					abort=true;
					mapped=false;
				}
				else
				{
					if(sub->GetLinkByFromTo(from,nextId)->GetRestBandwidth()<path->GetBandwidth())//no enough bw on the link
					{

						//for testing-----------cout<<"--attempt to mappp link : "<<path->GetId()<<" failed no enough bandwidth\n";
						//for testing-----------cout<<" rest Bw for sub link "<<sub->GetLinkByFromTo(from,nextId)->GetRestBandwidth()<<" requested Bw "<<path->GetBandwidth()<<"\n";
						this->path_length[path->GetId()]=-1;
						abort=true;
						mapped=false;
					}
					else
					{
						//for testing------------cout<<" mapping possible for the path "<<path->GetId()<<" it will take partially place at "<<sub->GetLinkByFromTo(from,nextId)->GetId()<<endl;
						this->GetPathMapp()[path->GetId()].push_back(sub->GetLinkByFromTo(from,nextId)->GetId());
						//for testing-----------cout<<"  sub ID == "<<sub->GetLinkByFromTo(from,nextId)->GetId()<<"  path ID == "<<path->GetId()<<endl;
						sub->GetLinkByFromTo(from,nextId)->GetEmbeddedPaths()->push_back((*path));
					}
				}
			}
			
			if(!abort)
			{
				//for testing------------cout<<" mapping possible for the path "<<path->GetId()<<endl;
				this->path_length[path->GetId()]=this->GetPathMapp()[path->GetId()].size();
				mapped=true;
			}
		}
	}
	return mapped;
}

double Mapping_trial::avaiMapp(Substrate_network* sub){

	double avai;
	if(this->stateMapping==STATE_MAP_NODE_FAIL ||this->stateMapping==STATE_MAP_LINK_FAIL ||this->stateMapping==STATE_NEW)
	{
		avai=0;
	}
	else
	{
		avai=1;
		avai*=avaiMappNode(sub)/100;
		avai*=avaiMappPath(sub)/100;
		
	}
	return avai*=100;
}

double Mapping_trial::avaiMappNode(Substrate_network* sub){

	//variables
	std::list<Priority_group>::iterator it1;
	std::list<Virtual_node>::iterator it2;
	double avai=1;

	//operations
	for (it1=this->request->GetGroups()->begin(); it1!=this->request->GetGroups()->end(); it1++)
	{
		for (it2=it1->GetNodes()->begin(); it2!=it1->GetNodes()->end(); it2++)
		{
			//for testing-----------cout<<(it2->GetEmbeddingNode()->GetAvailability())<<"\n";
			avai*= (it2->GetEmbeddingNode()->GetAvailability())/100;
		}
	}
	avai*=100;
	return avai;
}

double Mapping_trial::avaiMappPath(Substrate_network* sub){

	//variables
	double avai=1;
	size_t i;
	std::list<int>::iterator it;

	//operations
	for (i=0; i<this->request->GetLinks()->size(); i++)
	{
		for (it=this->path_mapp[i].begin(); it!=this->path_mapp[i].end(); it++)
		{
			//for testing-----------cout<<(sub->GetLinkById(*it)->GetAvailability())<<"\n";
			avai*= sub->GetLinkById(*it)->GetAvailability()/100;
		}
	}
	avai*=100;
	return avai;
}

void Mapping_trial::FreeMapp(Substrate_network* sub){
	if(this->stateMapping==STATE_MAP_LINK)
	{
	//FreeMappPath(sub);
	//FreeMappNode(sub);
	FreeMappPathPartial(sub,this->request->GetLinks()->size()-1);
	FreeMappNodePartial(sub,this->request->GetNumberNodes()-1);//this->request->GetNumberNodes()-1 = the id of the last node successfuly mapped
	}
	//else; resources have already been liberated
}

void Mapping_trial::FreeMappPartial(Substrate_network* sub,int lastComponent){
	
	if(this->stateMapping==STATE_MAP_NODE_FAIL)
	{
		FreeMappNodePartial(sub,lastComponent);
	}
	else
		if(this->stateMapping==STATE_MAP_LINK_FAIL)
		{
			//this means all the nodes have been succesfuly
			//only the links were partially mapped
			FreeMappPathPartial(sub,lastComponent);
			FreeMappNodePartial(sub,this->request->GetNumberNodes());
		}
}

void Mapping_trial::FreeMappNodePartial(Substrate_network* sub,int lastNode)//the last successfully embedded node
{
	
	//variables
	std::list<Priority_group>::iterator it1;
	std::list<Virtual_node>::iterator it2;
	std::list<Virtual_node>::iterator it3;
	Substrate_node * embeddingNode;
	bool done=false;

	//for testing-----------cout<<"FreeMappNodePartial\n";
	it1=this->request->GetGroups()->begin();
	while(!done && it1!=this->request->GetGroups()->end())
	{
		it2=it1->GetNodes()->begin();
		while(!done && it2!=it1->GetNodes()->end())
		{
			//for testing-----------cout<<" Attempt to free node with id : "<<it2->GetId()<<"\n";
			bool found=false;
			//we have to free the substrate node from being allocated to the virtual node
			embeddingNode=it2->GetEmbeddingNode();	
			it3 = embeddingNode->GetEmbeddedNodes()->begin();
			while(!found && it3 != embeddingNode->GetEmbeddedNodes()->end())
			{
				if(it3->GetId()==it2->GetId())
				{
					found=true;
					embeddingNode->GetEmbeddedNodes()->erase(it3);
				}
				else
				{
					it3++;
				}
			}
			it2->RemoveEmbeddingNode();
			if (it2->GetId()==lastNode)
			{
				done =true;
			}
			else
			{
				it2++;
			}
		}
		it1++;
	}
	this->stateMapping=STATE_NEW;
}

void Mapping_trial::FreeMappPathPartial(Substrate_network* sub,int lastPath)//the last successfully embedded path
{
	
	//variables
	std::list<Path>::iterator it1;
	std::list<Path>::iterator it2;
	std::list<int>::iterator it3;
	Substrate_link * embeddingLink;
	int id,i;
	bool found,done=false;

	//for testing-----------cout<<"FreeMappPathPartial\n";
	it1=this->request->GetLinks()->begin();
	while(!done && it1!=this->request->GetLinks()->end())
	{
		//for testing-----------cout<<" Attempt to free path with id : "<<it1->GetId()<<"\n";
		i=0;
		it3=this->GetPathMapp()[it1->GetId()].begin();
		while(it3!=this->GetPathMapp()[it1->GetId()].end())
		{
			id=*it3;
			embeddingLink = sub->GetLinkById(id);
			found=false;
			it2=embeddingLink->GetEmbeddedPaths()->begin();
			while(!found && it2!=embeddingLink->GetEmbeddedPaths()->end())
			{

				//if(it1->GetId()==it2->GetId())
				if(it1->GetName()==it2->GetName())
				{
					found=true;
					//cout<<" size before erase "<<embeddingLink->GetEmbeddedPaths()->size()<<endl;
					it2=embeddingLink->GetEmbeddedPaths()->erase(it2);
					//cout<<" size after erase "<<embeddingLink->GetEmbeddedPaths()->size()<<endl;
					//---------in case of testing----------------- //
				}
				else
				{
					it2++;
				}
			}
			it3++;
		}
		if(it1->GetId() == lastPath)
		{
			done=true;
		}
		else
		{
					it1++;
		}
	}
}

int* Mapping_trial::GetNodeMapp(){
	return this->node_mapp;
}

std::list<int> * Mapping_trial::GetPathMapp(){
	return this->path_mapp;
}

int* Mapping_trial::GetPathLength(){
	return this->path_length;
}
