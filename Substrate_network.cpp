//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : SAVI/VDCPlanner
//  @ File Name : Substrate_node.hpp
//  @ Date : 17/04/2013
//  @ Author : Maissa Jabri
//
//

#include "Substrate_network.hpp"

Substrate_network::Substrate_network(void) {

}

Substrate_network::Substrate_network(int id) {
	this->id = id;
}

Substrate_network::~Substrate_network(void) {

}

int Substrate_network::GetId() {
	return this->id;
}

void Substrate_network::SetId(int id) {
	this->id = id;
}

/*
 This function implements the algorithm of Floyd which was suggested and implemented by ZMF in the VDCPlanner V02
 Floyd's All pairs shortest path algorithm (O (n^3) )
 input is adjacency matrix
 output is matrix of shortest paths
 C is the adjacency matrix
 n is the order of the square matrix
 A is the all pairs shortest paths matrix
 */
void Substrate_network::ComputeAdjMatrix() {
	//variables
	int i, j, k, n;
	std::list<Substrate_link>::iterator it2;

	//operations
	n = this->GetNumberNodes();

	//this->adjMatrix == C
	this->adjMatrix = new AdjNode*[n];
	for (i = 0; i < n; ++i)
		this->adjMatrix[i] = new AdjNode[n];

	int test1 = 0, test2 = 0, test3 = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (i == j) {
				this->adjMatrix[i][j].SetLength(0);
				this->adjMatrix[i][j].SetNext(-1);
			} else {
				this->adjMatrix[i][j].SetLength(999999);
				this->adjMatrix[i][j].SetNext(-1);
			}
		}
	}

	k = 0;
	for (it2 = this->links.begin(); it2 != this->links.end(); it2++) {
		i = it2->GetSourceNode()->GetId();
		j = it2->GetDestinationNode()->GetId();
		if (i == -1 && j == -1)
			continue;
		//else
		this->adjMatrix[i][j].SetLength(1);
		this->adjMatrix[i][j].SetNext(j);
		this->adjMatrix[j][i].SetLength(1);
		this->adjMatrix[j][i].SetNext(i);
		k++;
	}

	//this->adjMatrix == A
	for (k = 0; k < n; k++) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				// if A[i][j] > A[i][k] + A[k][j];
				if (this->adjMatrix[i][k].GetLength() + this->adjMatrix[k][j].GetLength() < this->adjMatrix[i][j].GetLength()) {
					// A[i][j] = A[i][k] + A[k][j];
					this->adjMatrix[i][j].SetLength(this->adjMatrix[i][k].GetLength() + this->adjMatrix[k][j].GetLength());
					this->adjMatrix[i][j].SetNext(this->adjMatrix[i][k].GetNext());
				}
			}
		}
	}

}

AdjNode ** Substrate_network::GetAdjMatrix() {
	return this->adjMatrix;
}

void Substrate_network::DisplayAdjMatrix() {
	//variables
	int i, j, n;
	std::list<Substrate_link>::iterator it2;

	//operations
	n = this->GetNumberNodes();
	for (i = 0; i < n; i++) {
		cout << "-------------------\n";
		for (j = 0; j < n; j++) {
			cout << "-- " << this->adjMatrix[i][j].GetLength() << " --\t";
		}
		cout << "-------------------\n";
	}
}

//id = index + 1
Sub_node_type * Substrate_network::GetTypeById(int id) {
	list<Sub_node_type>::iterator it1;
	it1 = this->types.begin();
	std::advance(it1, id - 1);
	return &(*it1);
}

std::list<class Sub_node_type>* Substrate_network::GetTypes() {
	return &this->types;
}

Substrate_node* Substrate_network::GetNodeById(int id) {

	std::list<Substrate_node>::iterator it1;
	std::list<Sub_node_type>::iterator it3 = this->types.begin();
	bool found = false;

	while (!found && it3 != this->types.end()) {
		it1 = it3->GetSubstrateNodes()->begin();
		while (!found && it1 != it3->GetSubstrateNodes()->end()) {
			int l_id = it1->GetId();
			if (l_id == id) {
				found = true;
				break;
			}

			it1++;
		}

		it3++;
	}
	if (found)
		return &(*it1);
}

Substrate_node* Substrate_network::GetNodeByName(string name) {

	std::list<Substrate_node>::iterator it1;
	std::list<Sub_node_type>::iterator it3 = this->types.begin();
	bool found = false;

	while (!found && it3 != this->types.end()) {
		it1 = it3->GetSubstrateNodes()->begin();
		while (!found && it1 != it3->GetSubstrateNodes()->end()) {
			if (it1->GetName() == name) {
				found = true;
				break;
			}

			it1++;
		}

		it3++;
	}
	if (found)
		return &(*it1);
}

Substrate_link* Substrate_network::GetLinkById(int id) {

	std::list<Substrate_link>::iterator it1 = this->links.begin();
	bool found = false;

	while (!found && it1 != this->links.end()) {
		int l_id = it1->GetId();
		if (l_id == id) {
			found = true;
			break;
		}

		it1++;
	}
	if (found)
		return &(*it1);
}

std::list<Substrate_link>* Substrate_network::GetSubstrateLinks() {
	return &this->links;
}

void Substrate_network::DisplaySubstrate() {
	//variables
	std::list<Substrate_node>::iterator it1;
	std::list<Substrate_link>::iterator it2;
	std::list<Sub_node_type>::iterator it3;

	//operations
	cout << "For substrate network with id = " << this->id << "\n";
	cout << "Nodes :\n";
	cout << "type 1 : " << this->GetTypeById(1)->GetSubstrateNodes()->size() << "\n";
	cout << "type 2 : " << this->GetTypeById(2)->GetSubstrateNodes()->size() << "\n";
	cout << "type 3 : " << this->GetTypeById(3)->GetSubstrateNodes()->size() << "\n";
	cout << "type 4 : " << this->GetTypeById(4)->GetSubstrateNodes()->size() << "\n";
	for (it3 = this->types.begin(); it3 != this->types.end(); it3++) {
		cout << "type : " << it3->GetSubstrateNodes()->size() << "\n";
		for (it1 = it3->GetSubstrateNodes()->begin(); it1 != it3->GetSubstrateNodes()->end(); it1++)
			it1->Display();
	}
	cout << "Links :\n";
	for (it2 = this->links.begin(); it2 != this->links.end(); it2++)
		it2->Display();

}

void Substrate_network::DisplaySubstrateEmbedding() {
	//variables
	std::list<Substrate_node>::iterator it1;
	std::list<Substrate_link>::iterator it2;
	std::list<Sub_node_type>::iterator it3;
	//operations
	debug("For Substrate Network with ID: %f", this->id);
	debug("Nodes of type1: %d, Nodes of type2: %d, Nodes of type3: %d, Nodes of type4: %d\n",
			this->GetTypeById(1)->GetSubstrateNodes()->size(), this->GetTypeById(2)->GetSubstrateNodes()->size(),
			this->GetTypeById(3)->GetSubstrateNodes()->size(), this->GetTypeById(4)->GetSubstrateNodes()->size());
	for (it3 = this->types.begin(); it3 != this->types.end(); it3++) {
		for (it1 = it3->GetSubstrateNodes()->begin(); it1 != it3->GetSubstrateNodes()->end(); it1++)
			it1->DisplayEmdeddedNodes();
	}
	debug("Links:");
	for (it2 = this->links.begin(); it2 != this->links.end(); it2++)
		it2->DisplayEmdeddedPaths();

}

int Substrate_network::GetNumberNodes() {
	return this->numberNodes;
}

void Substrate_network::SetNumberNodes(int numberNodes) {
	this->numberNodes = numberNodes;
}

Substrate_link * Substrate_network::GetLinkByFromTo(int from, int to) {
	std::list<Substrate_link>::iterator it2;
	for (it2 = this->links.begin(); it2 != this->links.end(); it2++) {
		if ((it2->GetSourceNode()->GetId() == from && it2->GetDestinationNode()->GetId() == to)
				|| (it2->GetSourceNode()->GetId() == to && it2->GetDestinationNode()->GetId() == from))
			return &(*it2);
	}
	return NULL;
}
