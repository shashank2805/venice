#include "SubstrateLoader.h"


SubstrateLoader::SubstrateLoader(void)
{
}


SubstrateLoader::~SubstrateLoader(void)
{
}

void SubstrateLoader::Load(Substrate_network * net)
{

	//variables
    char filename[200];
    FILE * fp;
    int j,id,nbNodes, nbLinks,cpu,mem,type,disk;
	int bw=0, active=0;
	Config config;
	//this->inputName="sub/openstack.dat";
	//this->inputName="sub/SAVI_WT_EDGE.dat";
	//this->inputName=(config.topology_file).c_str();
	//strcpy(this->inputName, (config.topology_file).c_str());
	this->inputName=(config.topology_file);
	cout<<"here"<<endl;
	double avai;
	char name[50],from[50],to[50];

	//operations
	sprintf(filename, (this->inputName).c_str());
    fp = fopen(filename, "r");
    fscanf(fp, "%d %d\n", &nbNodes, &nbLinks);
    printf("Loading the substrate..\n"); 
    printf("%d %d\n", nbNodes, nbLinks);

	net->SetNumberNodes(nbNodes);
	net->GetTypes()->push_back(Sub_node_type(1,90));
	net->GetTypes()->push_back(Sub_node_type(2,99));
	net->GetTypes()->push_back(Sub_node_type(3,99.9));
	net->GetTypes()->push_back(Sub_node_type(4,99.99));

    for (j = 0; j < nbNodes; j ++) 
	{
		int ret = fscanf(fp, "%d %s %d %d %d %d %lf\n", &id,&name,&cpu,&mem,&disk,&type,&avai);

		//for testing -------printf("%d name : %s cpu : %d mem : %d disk : %d type : %d avai : %lf\n", id,name,cpu,mem,disk,type,avai);
		Substrate_node node(id,(double)cpu,(double)mem,(double)disk,type,(double)bw,active,name,avai);
		if(avai == 90)
			net->GetTypeById(1)->GetSubstrateNodes()->push_back(node);
		else
			if (avai == 99)
				net->GetTypeById(2)->GetSubstrateNodes()->push_back(node);
			else
				if(avai == 99.9)
					net->GetTypeById(3)->GetSubstrateNodes()->push_back(node);
				else
					net->GetTypeById(4)->GetSubstrateNodes()->push_back(node);
    }


    for (j = 0; j < nbLinks; j ++) 
	{
      fscanf(fp, "%d %s %s %d %lf\n", &id, &from, &to, &bw,&avai);
      //for testing -------printf("%d from : %s to : %s bw : %d avai : %lf\n", id,from,to,bw,avai);
	  Substrate_node * f = net->GetNodeByName(from);
	  Substrate_node * t = net->GetNodeByName(to);
	  Substrate_link link(id,f,t,(double)bw,avai);
	  net->GetSubstrateLinks()->push_back(link);
    }

	//time to compute the adjacence matrix
	net->ComputeAdjMatrix();
	//for testing-----------net->DisplayAdjMatrix();
    fclose(fp); 
 }
/*
void SubstrateLoader::Load(Substrate_network * net)
{

	//variables
    char filename[200];
    FILE * fp;
    int j,id,test,nbNodes, nbLinks,cpu,mem,type,from,to;
	int bw=0, active=0;
	this->inputName="sub/tree.dat";
	double avai;

	//operations
	sprintf(filename, this->inputName);
    fp = fopen(filename, "r");
    fscanf(fp, "%d %d\n", &nbNodes, &nbLinks);
    printf("Loading the substrate..\n");
    printf("%d %d\n", nbNodes, nbLinks);

	net->SetNumberNodes(nbNodes);
	net->GetTypes()->push_back(Sub_node_type(1,90));
	net->GetTypes()->push_back(Sub_node_type(2,99));
	net->GetTypes()->push_back(Sub_node_type(3,99.9));
	net->GetTypes()->push_back(Sub_node_type(4,99.99));

    for (j = 0; j < nbNodes; j ++)
	{
		int ret = fscanf(fp, "%d %d %d %d %d %lf\n", &id,&cpu,&mem,&type,&test,&avai);
		Substrate_node node(id,(double)cpu,(double)mem,type,(double)bw,active,avai);
		if(avai == 90)
			net->GetTypeById(1)->GetSubstrateNodes()->push_back(node);
		else
			if (avai == 99)
				net->GetTypeById(2)->GetSubstrateNodes()->push_back(node);
			else
				if(avai == 99.9)
					net->GetTypeById(3)->GetSubstrateNodes()->push_back(node);
				else
					net->GetTypeById(4)->GetSubstrateNodes()->push_back(node);
    }


    for (j = 0; j < nbLinks; j ++)
	{
      fscanf(fp, "%d %d %d %d %d %lf\n", &id, &from, &to, &bw,&test,&avai);;
	  Substrate_node * f = net->GetNodeById(from);
	  Substrate_node * t = net->GetNodeById(to);
	  Substrate_link link(id,f,t,(double)(bw/1000),avai);
	  net->GetSubstrateLinks()->push_back(link);
    }

	//time to compute the adjacence matrix
	net->ComputeAdjMatrix();
	//for testing-----------net->DisplayAdjMatrix();
    fclose(fp);
 }*/
