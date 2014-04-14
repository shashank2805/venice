/*
 * VDCPlannerMysqlConnector.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: ahmedamokrane
 */

#include "VDCPlannerMysqlConnector.h"

VDCPlannerMysqlConnector::VDCPlannerMysqlConnector(string mysqlServerAddress, int mysqlServerPort, string user, string pass, string dataBaseName) {
	this->mysqlServerPort = mysqlServerPort;
	this->mysqlServerAddress = string("");
	this->mysqlServerAddress.append(mysqlServerAddress);
	this->dataBaseName = string("");
	this->dataBaseName.append(dataBaseName);
	this->userName = string("");
	this->userName.append(user);
	this->password = string("");
	this->password.append(pass);
}

VDCPlannerMysqlConnector::~VDCPlannerMysqlConnector() {

}


int VDCPlannerMysqlConnector::createDataBase() {

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;

		/* Create a connection */
		driver = get_driver_instance();

		string connectTo("tcp://");
		connectTo.append(this->mysqlServerAddress);
		connectTo.append(":");
		stringstream ss1;
		ss1 << this->mysqlServerPort;
		connectTo.append(ss1.str());
		con = driver->connect(connectTo, this->userName, this->password);
		//con = driver->connect("tcp://127.0.0.1:3306", "root", "");

		stmt = con->createStatement();
		//Create the data base
		string s1("DROP DATABASE IF EXISTS ");
		s1.append(this->dataBaseName);
		stmt->execute(s1);

		string s2("CREATE DATABASE ");
		s2.append(this->dataBaseName);
		stmt->execute(s2);

		con->setSchema(this->dataBaseName);

		/*
		 * 			int id;
					std::string session;
					//std::list<Request *> requests;
					std::list<Request> requests;
					std::list<Request> waiting_requests;
		 */
		stmt->execute("CREATE TABLE IF NOT EXISTS users ("
				"userID    INT UNSIGNED  NOT NULL,"
				"userName VARCHAR(30),"
				"session VARCHAR(30),"
				"PRIMARY KEY  (userID) )"
		);


		/*
				  	A Request
				  	int reqNumber;
					string name; // VDC identifier in Openstack
					string userName; // the name given to the vdc by the user
					int type;
					int split;
					time_t arrTime;
					int topo;
					string application;
					double duration;
					double revenue;
					double availability;
					int numberNodes;
					int status;
		 */

		//vdc_requests (requestID, arrival, duration, revenue, numberNodes, numberLinks, limitCarbonEmission, split, type, topo, availability, status, name, userName, application)
		stmt->execute("CREATE TABLE IF NOT EXISTS vdc_requests ("
				"requestID    INT UNSIGNED  NOT NULL,"
				"arrival   DOUBLE(20,10) NOT NULL,"
				"duration   DOUBLE(20,10) NOT NULL,"
				"revenue  DOUBLE(20,10) NOT NULL,"
				"numberNodes INT NOT NULL,"
				"numberLinks INT NOT NULL,"
				"limitCarbonEmission   DOUBLE(20,10) NOT NULL,"
				"split INT,"
				"type INT,"
				"topo INT,"
				"availability DOUBLE(20,10),"
				"status INT,"
				"name VARCHAR(30),"
				"userName VARCHAR(30),"
				"application VARCHAR(30),"
				"PRIMARY KEY  (requestID) )"
		);


		/*
				 	int id;
					std::string name;
					string status;
					double bandwidth;
					Virtual_node *from;
					Virtual_node *to;
					int from_id;
					int to_id;
					//std::list<class Substrate_node>* substrateNodes;
					std::list<class Substrate_link> substrateLinks;
					int length;// 0 => from==path, -1=> no path
		 */

		//virtual_links(requestID, linkID, nodeSource, nodeDestination, partitionSource, partitionDestination, bandwidth, delay,status, length, name)
		stmt->execute("CREATE TABLE IF NOT EXISTS virtual_links ("
				"requestID INT UNSIGNED  NOT NULL,"
				"linkID  INT UNSIGNED  NOT NULL,"
				"nodeSource	  INT UNSIGNED  NOT NULL,"
				"nodeDestination INT UNSIGNED  NOT NULL,"
				"partitionSource	  INT UNSIGNED  NOT NULL,"
				"partitionDestination INT UNSIGNED  NOT NULL,"
				"bandwidth  DOUBLE(20,10) NOT NULL,"
				"delay  DOUBLE(20,10) NOT NULL,"
				"status VARCHAR(30),"
				"length INT,"
				"name VARCHAR(30),"
				"PRIMARY KEY  (requestID,linkID),"
				"UNIQUE (requestID,nodeSource,nodeDestination))"
		);


		/*
					A virtual machine or virtual node
					int type;
					double memory;
					double cpu;
					double disk;
					int flavor;
					string task_state;
					string power_state;
					string public_ip;
					string private_ip;
					string embeddingNode_name;
		 */

		//virtual_machines(requestID, nodeID, partitionID, cpu, memory, disk, flavor, taskState, powerState, publicIP, privateIP, embeddingNodeName, priorityGroup)
		stmt->execute("CREATE TABLE IF NOT EXISTS virtual_machines ("
				"requestID INT UNSIGNED  NOT NULL,"
				"nodeID INT UNSIGNED  NOT NULL,"
				"partitionID INT UNSIGNED,"
				"cpu  DOUBLE(20,10) NOT NULL,"
				"memory  DOUBLE(20,10) NOT NULL,"
				"disk  DOUBLE(20,10) NOT NULL,"
				"flavor INT NOT NULL,"
				"taskState VARCHAR(30),"
				"powerState VARCHAR(30),"
				"publicIP VARCHAR(30),"
				"privateIP VARCHAR(30),"
				"embeddingNodeName VARCHAR(30),"
				"priorityGroup INT NOT NULL,"
				"PRIMARY KEY  (requestID,nodeID))"
		);


		/*

				 	double availability;
					Substrate_node * from;
					int from_id;
					Substrate_node * to;
					int to_id;
					std::list<Path> embeddedPaths;
					int id;
					std::string name;
					string status;
					double bandwidth;
		 */

		//physical_links(linkID, name, nodeSource, nodeDestination, bandwidth, delay, status, availability)
		stmt->execute("CREATE TABLE IF NOT EXISTS physical_links ("
				"linkID  INT UNSIGNED NOT NULL,"
				"name VARCHAR(30),"
				"nodeSource	  INT UNSIGNED  NOT NULL,"
				"nodeDestination INT UNSIGNED  NOT NULL,"
				"bandwidth  DOUBLE(20,10) NOT NULL,"
				"delay  DOUBLE(20,10) NOT NULL,"
				"status VARCHAR(30),"
				"availability DOUBLE(20,10) NOT NULL,"
				"PRIMARY KEY  (linkID),"
				"UNIQUE  (nodeSource,nodeDestination))"
		);


		/*
					int active;
					double availability;
					string name;
					int type;
					int flavor;
					double memory;
					double cpu;
					double disk;
		 */
		//physical_nodes(nodeID, name, type, flavor, cpu, memory, disk, availability)
		stmt->execute("CREATE TABLE IF NOT EXISTS physical_nodes ("
				"name VARCHAR(30),"
				"nodeID  INT UNSIGNED  NOT NULL,"
				"type INT,"
				"flavor INT NOT NULL,"
				"cpu  DOUBLE(20,10) NOT NULL,"
				"memory  DOUBLE(20,10) NOT NULL,"
				"disk  DOUBLE(20,10) NOT NULL,"
				"availability  DOUBLE(20,10) NOT NULL,"
				"PRIMARY KEY  (nodeID))"
		);


		//mapping_virtual_machine (requestID,VMID, subsrateNodeID, mappingBegin, mappingEnd)
		stmt->execute("CREATE TABLE IF NOT EXISTS mapping_virtual_machine ("
				"requestID INT UNSIGNED  NOT NULL,"
				"VMID  INT UNSIGNED  NOT NULL,"
				"subsrateNodeID INT UNSIGNED  NOT NULL,"
				"mappingBegin   DOUBLE(20,10) NOT NULL,"
				"mappingEnd   DOUBLE(20,10),"
				"PRIMARY KEY  (requestID,VMID,mappingBegin))"
		);

		//mapping_virtual_link (requestID, virtualLinkID, virtualNodeSource, virtualNodeDestination, physicalLinkID, physicalNodeSource, physicalNodeDestination,  mappingBegin, mappingEnd)
		stmt->execute("CREATE TABLE IF NOT EXISTS mapping_virtual_link ("
				"requestID INT UNSIGNED  NOT NULL,"
				"virtualLinkID INT UNSIGNED  NOT NULL,"
				"virtualNodeSource  INT UNSIGNED  NOT NULL,"
				"virtualNodeDestination INT UNSIGNED  NOT NULL,"
				"physicalLinkID INT UNSIGNED,"
				"physicalNodeSource INT UNSIGNED,"
				"physicalNodeDestination INT UNSIGNED,"
				"mappingBegin   DOUBLE(20,10) NOT NULL,"
				"mappingEnd   DOUBLE(20,10),"
				"UNIQUE  (requestID, virtualLinkID, physicalLinkID, mappingBegin),"
				"UNIQUE  (requestID,virtualNodeSource, virtualNodeDestination, physicalNodeSource, physicalNodeDestination, mappingBegin))"
		);


		/*
		 * Create the database for removed VDC requests (Keep trace of the old users that submitted the requests)
		 */


		//Create the TRIGGERS
		// The Triggers do some stuff whenever an event occurs in the data base. For instance, if we remove a request from the DB, we should also remove the VMs and links that belong to this request, which are located in other tables


		stmt->execute("CREATE TRIGGER delete_links_and_vms_on_delete_request AFTER DELETE on vdc_requests "
				"FOR EACH ROW "
				"BEGIN "
				"DELETE FROM virtual_machines "
				"WHERE virtual_machines.requestID = old.requestID "
				";"
				"DELETE FROM virtual_links "
				"WHERE virtual_links.requestID = old.requestID "
				";"
				"INSERT INTO vdc_requests_archive "
				"VALUES (old.requestID, old.arrival, old.duration, old.revenue, old.numberNodes, old.numberLinks, old.limitCarbonEmission, old.split, old.type, old.topo, old.availability, old.status, old.name, old.userName, old.application) "
				";"
				"END"
		);

		stmt->execute("CREATE TRIGGER delete_mappings_on_delete_virtual_machine AFTER DELETE on virtual_machines "
				"FOR EACH ROW "
				"BEGIN "
				"INSERT INTO virtual_machines_archive "
				"VALUES (old.requestID, old.nodeID, old.partitionID, old.cpu, old.memory, old.disk, old.flavor, old.taskState, old.powerState, old.publicIP, old.privateIP, old.embeddingNodeName, old.priorityGroup) "
				";"
				"END"
		);

		stmt->execute("CREATE TRIGGER delete_mappings_on_delete_virtual_link AFTER DELETE on virtual_links "
				"FOR EACH ROW "
				"BEGIN "
				"DELETE FROM mapping_virtual_link "
				"WHERE mapping_virtual_link.requestID = old.requestID "
				"AND mapping_virtual_link.virtualNodeSource = old.nodeSource "
				"AND mapping_virtual_link.virtualNodeDestination = old.nodeDestination"
				";"
				"INSERT INTO virtual_links_archive "
				"VALUES (old.requestID, old.linkID, old.nodeSource, old.nodeDestination, old.partitionSource, old.partitionDestination, old.bandwidth, old.delay, old.status, old.length, old.name) "
				";"
				"END"
		);


		stmt->execute("CREATE TRIGGER insert_archive_mappings_on_delete_mapping_vm AFTER DELETE on mapping_virtual_machine "
				"FOR EACH ROW "
				"BEGIN "
				"INSERT INTO mapping_virtual_machine_archive "
				"VALUES (old.requestID, old.VMID, old.subsrateNodeID, old.mappingBegin, old.mappingEnd) "
				";"
				"END"
		);

		stmt->execute("CREATE TRIGGER insert_archive_mappings_on_delete_mapping_virtual_link AFTER DELETE on mapping_virtual_link "
				"FOR EACH ROW "
				"BEGIN "
				"INSERT INTO mapping_virtual_link_archive "
				"VALUES(old.requestID, old.virtualLinkID, old.virtualNodeSource, old.virtualNodeDestination, old.physicalLinkID, old.physicalNodeSource, old.physicalNodeDestination,  old.mappingBegin, old.mappingEnd) "
				";"
				"END"
		);


		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<" )" << endl;
	}

	cout << endl;

	return EXIT_SUCCESS;
}


bool VDCPlannerMysqlConnector::doesDataBaseExist() {
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::PreparedStatement *pstmt;
		sql::ResultSet *res;

		/* Create a connection */
		driver = get_driver_instance();

		string connectTo("tcp://");
		connectTo.append(this->mysqlServerAddress);
		connectTo.append(":");
		stringstream ss1;
		ss1 << this->mysqlServerPort;
		connectTo.append(ss1.str());
		con = driver->connect(connectTo, this->userName, this->password);
		//con = driver->connect("tcp://127.0.0.1:3306", "root", "root");

		stmt = con->createStatement();
		string msg =("show databases like \"");
		msg.append(this->dataBaseName);
		msg.append("\"");
		pstmt = con->prepareStatement(msg);
		res = pstmt->executeQuery();

		bool toReturn = false;
		if(res->next()){
			toReturn = true;
		}

		delete stmt;
		delete pstmt;
		delete con;

		return toReturn;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<" )" << endl;
	}

	cout << endl;
	return false;
}



int VDCPlannerMysqlConnector::writeVDCRequestToDataBase(Request* request) {

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement *pstmt;

		/* Create a connection */
		driver = get_driver_instance();

		string connectTo("tcp://");
		connectTo.append(this->mysqlServerAddress);
		connectTo.append(":");
		stringstream ss1;
		ss1 << this->mysqlServerPort;
		connectTo.append(ss1.str());
		con = driver->connect(connectTo, this->userName, this->password);
		//con = driver->connect("tcp://127.0.0.1:3306", "root", "root");

		con->setSchema(this->dataBaseName);

		//Write the request
		//vdc_requests (requestID, arrival, duration, revenue, numberNodes, numberLinks, limitCarbonEmission, split, type, topo, availability, status, name, userName, application)
		pstmt = con->prepareStatement("INSERT IGNORE INTO vdc_requests VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		pstmt->setInt(1, request->GetRequestNumber());
		pstmt->setDouble(2, request->GetArrTime());
		pstmt->setDouble(3, request->GetDuration());
		pstmt->setDouble(4, request->GetRevenue());
		pstmt->setInt(5, request->GetNumberNodes());
		pstmt->setInt(6, request->GetLinks()->size());
		pstmt->setDouble(7, 10000); //There is no limit for carbon emission, a high value is enough
		pstmt->setInt(8, request->GetSplit());
		pstmt->setInt(9, 1); // there is no access the member type in the Request class, set it to 1 by default unless you know what it means
		pstmt->setInt(10, request->GetTopo());
		pstmt->setDouble(11, request->GetAvailability());
		pstmt->setInt(12, request->GetStatus());
		pstmt->setString(13, request->GetName());
		pstmt->setString(14, request->GetUserName());
		pstmt->setString(15, request->GetApplication());

		pstmt->executeUpdate();


		//Write the VMs and their corresponding mappings
		std::list<Priority_group>*grp_list = request->GetGroups();
		int i = 1; //TODO(Aimal): start at 1?
		for (std::list<Priority_group>::iterator itPrioGroup = grp_list->begin(); itPrioGroup != grp_list->end(); itPrioGroup++){
			Priority_group gr = *itPrioGroup;
			for (std::list<Virtual_node>::iterator itVirtualNode = gr.GetNodes()->begin(); itVirtualNode != gr.GetNodes()->end(); itVirtualNode++){
				Virtual_node vm = *itVirtualNode;
				//Add the VM to the data base
				//virtual_machines(requestID, nodeID, partitionID, cpu, memory, disk, flavor, taskState, powerState, publicIP, privateIP, embeddingNodeName, priorityGroup)
				pstmt = con->prepareStatement("INSERT IGNORE INTO virtual_machines VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");
				pstmt->setInt(1, request->GetRequestNumber());
				pstmt->setInt(2, vm.GetId());
				pstmt->setInt(3, vm.GetId()); //Set the partition ID to whatever makes sense for you
				pstmt->setDouble(4, vm.GetCpu());
				pstmt->setDouble(5, vm.GetMemory());
				pstmt->setDouble(6, vm.GetDisk());
				pstmt->setInt(7, vm.GetFlavor());
				pstmt->setString(8, vm.GetTaskState());
				pstmt->setString(9, vm.GetPowerState());
				pstmt->setString(10, vm.GetPublicIp());
				pstmt->setString(11, vm.GetPrivateIp());
				pstmt->setString(12, "NIL"); // placeholder name
				pstmt->setInt(13 , i); //The priorityGroup
				pstmt->executeUpdate();

				//Add the VM mapping (virtual machine to physical node) to the data base
				//mapping_virtual_machine (requestID, VMID, subsrateNodeID, mappingBegin, mappingEnd)
#if 0
				if(vm.GetEmbeddingNode() != NULL){
printf("EmbeddingNODES\n");
					pstmt = con->prepareStatement("INSERT IGNORE INTO mapping_virtual_machines VALUES (?,?,?,?,?)");
					pstmt->setInt(1, request->GetRequestNumber());
					pstmt->setInt(2, vm.GetId());
					pstmt->setInt(3, vm.GetEmbeddingNode()->GetId());
					pstmt->setDouble(4, 0); //There is no notion of the time of embedding in the Request as far as I understood, in this case these two columns are meaningless
					pstmt->setDouble(5, 0);
					pstmt->executeUpdate();
				}
#endif
			}
			i++; // increment the priority group
		}

printf("Begin writing Links to db\n");
		//Write the Links and their corresponding mappings
		for (std::list<Path>::iterator it = request->GetLinks()->begin(); it != request->GetLinks()->end(); it++){
			Path link = *it;
printf("write Links to db\n");
			//Write the link to the database
			//virtual_links(requestID, linkID, nodeSource, nodeDestination, partitionSource, partitionDestination, bandwidth, delay, status, length, name)
			pstmt = con->prepareStatement("INSERT IGNORE INTO virtual_links VALUES (?,?,?,?,?,?,?,?,?,?,?)");
			pstmt->setInt(1, request->GetRequestNumber());
			pstmt->setInt(2, link.GetId());
			pstmt->setInt(3, link.GetSourceNode_id());
			pstmt->setInt(4, link.GetDestinationNode_id());
			pstmt->setInt(5, 0);
			pstmt->setInt(6, 0); //No meaning for the partition in this case
			pstmt->setDouble(7,link.GetBandwidth());
			pstmt->setDouble(8,10000); //The delay is not mentioned so far in the Request, put a high value
			pstmt->setString(9,link.GetStatus()); //TODO(Aimal): GetStatus is not implemented
			pstmt->setInt(10, link.GetLength());
			pstmt->setString(11,link.GetName());
			pstmt->executeUpdate();

			//Write the mapping of the link to the database
#if 0
			if(link.GetSubstrateLinks() != NULL){
printf("GetSubstrateLINKS\n");
				for (std::list<Substrate_link>::iterator itSubstrateLinks = link.GetSubstrateLinks()->begin(); itSubstrateLinks != link.GetSubstrateLinks()->end(); itSubstrateLinks++){
					Substrate_link subL = *itSubstrateLinks;
					//mapping_virtual_link (requestID, virtualLinkID, virtualNodeSource, virtualNodeDestination, physicalLinkID, physicalNodeSource, physicalNodeDestination,  mappingBegin, mappingEnd)
					pstmt = con->prepareStatement("INSERT IGNORE INTO mapping_virtual_link VALUES (?,?,?,?,?,?,?,?,?)");
					pstmt->setInt(1, request->GetRequestNumber());
					pstmt->setInt(2, link.GetId());
					pstmt->setInt(3, link.GetSourceNode_id());
					pstmt->setInt(4, link.GetDestinationNode_id());
					pstmt->setInt(5, subL.GetId());
					pstmt->setInt(6, subL.GetSourceNode()->GetId()); //pstmt->setInt(6, subL.GetSourceNode_id());
					pstmt->setInt(7, subL.GetDestinationNode()->GetId()); //pstmt->setInt(7, subL.GetDestinationNode_id());
					pstmt->setDouble(8, 0); //There is no notion of the time of embedding in the Request as far as I understood, in this case these two columns are meaningless
					pstmt->setDouble(9, 0);
					pstmt->executeUpdate();
				}
			}
#endif
		}
printf("End writing Links to db\n");
		delete pstmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<" )" << endl;
	}

	cout << endl;

	return EXIT_SUCCESS;
}

Request* VDCPlannerMysqlConnector::readVDCRequestClassFromDataBase(int idRequest) {

	Request* request = NULL;

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res1;
		//sql::ResultSet *res2;
		sql::PreparedStatement *pstmt1;
		//sql::PreparedStatement *pstmt2;

		/* Create a connection */
		driver = get_driver_instance();

		string connectTo("tcp://");
		connectTo.append(this->mysqlServerAddress);
		connectTo.append(":");
		stringstream ss1;
		ss1 << this->mysqlServerPort;
		connectTo.append(ss1.str());
		con = driver->connect(connectTo, this->userName, this->password);
		//con = driver->connect("tcp://127.0.0.1:3306", "root", "root");

		con->setSchema(this->dataBaseName);
printf(" READ REQUESTS\n");
		pstmt1 = con->prepareStatement("SELECT * FROM vdc_requests WHERE requestID = (?)");
		pstmt1->setInt(1,idRequest);
		res1 = pstmt1->executeQuery();
		if(!res1->next()){
			//The request does not exist in the data base
printf("NO REQUESTS\n");
		}else{
printf("REQUESTS\n");
			//The request exists in the data base, build the VDC request
			//vdc_requests (requestID, arrival, duration, revenue, numberNodes, numberLinks, limitCarbonEmission, split, type, topo, availability, status, name, userName, application)

			request = new Request();
			request->SetRequestNumber(res1->getInt("requestID"));
			request->SetArrTime(res1->getDouble("arrival"));
			request->SetDuration(res1->getDouble("duration"));
			request->SetRevenue(res1->getDouble("revenue"));
			request->SetNumberNodes(res1->getInt("numberNodes"));
			request->SetSplit(res1->getInt("split"));
			request->SetTopo(res1->getInt("topo"));
			request->SetAvailability(res1->getDouble("availability"));
			request->SetStatus(res1->getInt("status"));
			request->SetName(res1->getString("name")); //request->SetName(string(res->getString("status").c_str()));
			request->SetUserName(res1->getString("userName")); //request->SetName(string(res->getString("status").c_str()));
			request->SetApplication(res1->getString("application")); //request->SetName(string(res->getString("status").c_str()));

			//Read the VMs
			//virtual_machines(requestID, nodeID, partitionID, cpu, memory, disk, flavor, taskState, powerState, publicIP, privateIP, embeddingNodeName, priorityGroup)
printf("NODES START\n");
			//All VMs will be in the same priority group
			std::list<Virtual_node>* listVMs = new std::list<Virtual_node>();
			pstmt1 = con->prepareStatement("SELECT * FROM virtual_machines WHERE requestID = (?) ORDER BY nodeID ASC ");
			pstmt1->setInt(1,idRequest);
			res1 = pstmt1->executeQuery();
			int i = 0;
			while(res1->next()){
				Virtual_node* vm = new  Virtual_node();
				vm->SetId(res1->getInt("nodeID"));
				vm->SetCpu(res1->getDouble("cpu"));
				vm->SetMemory(res1->getDouble("memory"));
				vm->SetDisk(res1->getDouble("disk"));
				vm->SetFlavor(res1->getInt("flavor"));
				vm->SetTaskState(res1->getString("taskState"));
				vm->SetPowerState(res1->getString("powerState"));
				vm->SetPublicIp(res1->getString("publicIP"));
				vm->SetPrivateIp(res1->getString("privateIP"));

#if 0
				//Read the mapping of this VM
				//mapping_virtual_machine (requestID, VMID, subsrateNodeID, mappingBegin, mappingEnd)
				pstmt2 = con->prepareStatement("SELECT * FROM mapping_virtual_machines WHERE requestID = (?) and VMID = (?) ");
				pstmt2->setInt(1,idRequest);
				pstmt2->setInt(2,vm->GetId());
				res2 = pstmt2->executeQuery();
				//To be continued based on how the mapping is represented at this level
				if(res2->next()){
					int idSubstrateNode = res2->getInt("subsrateNodeID");
					//You have the Id of the substrate node
					//physical_nodes(nodeID, name, type, flavor, cpu, memory, disk, availability)
					pstmt2 = con->prepareStatement("SELECT * FROM physical_nodes WHERE nodeID = (?) ");
					pstmt2->setInt(1,idSubstrateNode);
					res2 = pstmt2->executeQuery();
					if(res2->next()){
						//We write the name of the physical node
						vm->SetEmbeddingNode_name(res2->getString("name"));
					}
				}
#endif
				/*
				 * TODO:(AIMAL: Discuss) What is missing for the VM is the substrate node to which it was mapped (using vm->SetEmbeddingNode())
				 * I suggest that you do this at the end of the reading of the request and the substrate network, you can then do the combination
				 */
				listVMs->push_back(*vm);
printf("NODES END\n");
			}
			//std::list<Priority_group>*listPriorityGroup = new std::list<Priority_group>();
			Priority_group group = Priority_group(1);
			group.SetId(1);
			group.SetNodes(listVMs);
			//listPriorityGroup->push_back(group);
			//TODO(AIMAL: DISCUSS): How to add this list (listPriorityGroup) to the request
			request->GetGroups()->push_back(group);


printf("START LINKS\n");
			//Read the links
			pstmt1 = con->prepareStatement("SELECT * FROM virtual_links WHERE requestID = (?)");
			pstmt1->setInt(1,idRequest);
			res1 = pstmt1->executeQuery();

			list<Path>* links = new list<Path>();
			//virtual_links(requestID, linkID, nodeSource, nodeDestination, partitionSource, partitionDestination, bandwidth, delay, status, length, name)
			while(res1->next()){

printf("LINK LOOP START\n");
				int id = res1->getInt("linkID");
				int vmFrom = res1->getInt("nodeSource");
				int vmTo = res1->getInt("nodeDestination");
				double bw = res1->getDouble("bandwidth");
				Virtual_node * from = request->GetNodeById(vmFrom); //The nodes have already been read in the list of nodes
				Virtual_node * to = request->GetNodeById(vmTo);
				Path* l = new Path(id, vmFrom, from, vmFrom , to, bw);

				l->SetStatus(res1->getString("status"));
				l->SetName(res1->getString("name"));
				l->SetLength(1);

				//TODO(Aimal: Discuss) This push of path into the path list was not done
				request->GetLinks()->push_back(*l);
				request->IncrementRevenue(bw/100);

#if 0
				//Read the list of substrate links to which this link has been mapped
				//mapping_virtual_link (requestID, virtualLinkID, virtualNodeSource, virtualNodeDestination, physicalLinkID, physicalNodeSource, physicalNodeDestination,  mappingBegin, mappingEnd)
				sql::PreparedStatement* pstmtMappings = con->prepareStatement("SELECT * FROM mapping_virtual_link WHERE requestID = (?) AND virtualNodeSource = (?) AND  virtualNodeDestination = (?)");
				pstmtMappings->setInt(1,idRequest);
				pstmtMappings->setInt(2,vmFrom);
				pstmtMappings->setInt(3,vmTo);
				sql::ResultSet *resMappings = pstmtMappings->executeQuery();
				std::list<class Substrate_link> substrateLinks = list<Substrate_link>();
				while(resMappings->next()){
					Substrate_link* substrateLink; // = new Substrate_link();
					int srcSubstrate = resMappings->getInt("physicalNodeSource");
					int dstSubstrate = resMappings->getInt("physicalNodeDestination");
					/*
					 * TODO: you have the source and destination of the link in the substrate network, how to write them into substrateLink
					 * something that should look like substrateLink->SetDestination()
					 * Then, once you are done at this level, you should combing with the physical infrastructure (substrate class) to put the correct pointers to the correst nodes
					 */
					substrateLinks.push_back(*substrateLink);
				}
				/*
				 * TODO: There should be a way to write this substrate path into the link l
				 * Something like l->GetSubstrateLinks(substrateLinks)
				 */
#endif

				//TODO(Aimal): Discuss this extra bit
				request->SetRevenue((request->GetRevenue()*request->GetDuration())/(double)60);

//				delete pstmtMappings;
//				delete resMappings;
printf("END LINKS\n");
			}//end While res1->next() //Number of links
		}

		delete res1;
		delete pstmt1;
		//delete res2;
		//delete pstmt2;
		delete con;
printf("END READ\n");

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<" )" << endl;
		delete request;
		request = NULL;
	}

	cout << endl;
request->DisplayRequest();
	return request;
}

vector<Request*>* VDCPlannerMysqlConnector::readAllVDCRequestsClassFromDataBase() {

	vector<Request*>* listResults = new vector<Request*>();
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;

		/* Create a connection */
		driver = get_driver_instance();

		string connectTo("tcp://");
		connectTo.append(this->mysqlServerAddress);
		connectTo.append(":");
		stringstream ss1;
		ss1 << this->mysqlServerPort;
		connectTo.append(ss1.str());
		con = driver->connect(connectTo, this->userName, this->password);
		//con = driver->connect("tcp://127.0.0.1:3306", "root", "");

		con->setSchema(this->dataBaseName);

		pstmt = con->prepareStatement("SELECT * FROM vdc_requests");
		res = pstmt->executeQuery();

		while(res->next()){
			int idRequest = res->getInt("requestID");
			Request* req = this->readVDCRequestClassFromDataBase(idRequest);

			 req->DisplayRequest();

			listResults->push_back(req);
			printf("welp, here we are\n");
		}//End While on the number of request

		delete res;
		delete pstmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<" )" << endl;
	}

	cout << endl;
	return listResults;

}

int VDCPlannerMysqlConnector::removeVDCRequestFromDataBase(Request* request, double t) {

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement *pstmt;

		/* Create a connection */
		driver = get_driver_instance();

		string connectTo("tcp://");
		connectTo.append(this->mysqlServerAddress);
		connectTo.append(":");
		stringstream ss1;
		ss1 << this->mysqlServerPort;
		connectTo.append(ss1.str());
		con = driver->connect(connectTo, this->userName, this->password);
		//con = driver->connect("tcp://127.0.0.1:3306", "root", "root");

		con->setSchema(this->dataBaseName);

		//Update the mapping end first

		//Update the old mapping
		pstmt = con->prepareStatement(
				"UPDATE mapping_virtual_machine SET mappingEnd = (?)"
				"WHERE requestID = (?) AND ((mappingEnd IS NULL) OR (mappingEnd = -1))"
		);
		pstmt->setDouble(1,t);
		pstmt->setInt(2,request->GetRequestNumber());
		pstmt->executeUpdate();

		pstmt = con->prepareStatement(
				"UPDATE mapping_virtual_link SET mappingEnd = (?)"
				"WHERE requestID = (?) AND ((mappingEnd IS NULL) OR (mappingEnd = -1))"
		);
		pstmt->setDouble(1,t);
		pstmt->setInt(2,request->GetRequestNumber());
		pstmt->executeUpdate();


		pstmt = con->prepareStatement("DELETE FROM vdc_requests WHERE requestID = (?)");
		pstmt->setInt(2,request->GetRequestNumber());
		pstmt->executeUpdate();

		/*
		 * This part is optional, the triggers already remove the links and VMs that belong to the request (have the same requestID)
		 *
		pstmt = con->prepareStatement("DELETE FROM virtual_links WHERE requestID = (?)");
		pstmt->setInt(2,request->GetRequestNumber());
		pstmt->executeUpdate();

		pstmt = con->prepareStatement("DELETE FROM virtual_machines WHERE requestID = (?)");
		pstmt->setInt(2,request->GetRequestNumber());
		pstmt->executeUpdate();

		pstmt = con->prepareStatement("DELETE FROM mapping_virtual_machine WHERE requestID = (?)");
		pstmt->setInt(2,request->GetRequestNumber());
		pstmt->executeUpdate();

		pstmt = con->prepareStatement("DELETE FROM mapping_virtual_link WHERE requestID = (?)");
		pstmt->setInt(2,request->GetRequestNumber());
		pstmt->executeUpdate();

		 *
		 */

		delete pstmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<" )" << endl;
		delete request;
		request = NULL;
	}

	cout << endl;

	return EXIT_SUCCESS;
}

int VDCPlannerMysqlConnector::writeSubstrateNetworkToDataBase(Substrate_network substrate_network) {
#if 1
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement *pstmt;

		/* Create a connection */
		driver = get_driver_instance();

		string connectTo("tcp://");
		connectTo.append(this->mysqlServerAddress);
		connectTo.append(":");
		stringstream ss1;
		ss1 << this->mysqlServerPort;
		connectTo.append(ss1.str());
		con = driver->connect(connectTo, this->userName, this->password);
		//con = driver->connect("tcp://127.0.0.1:3306", "root", "root");

		con->setSchema(this->dataBaseName);

		vector<int> listNodeIDs = vector<int>();
		vector<Substrate_node*> listNodes = vector<Substrate_node*>();

		//Write the physical links
		//physical_links(linkID, name, nodeSource, nodeDestination, bandwidth, delay, status, availability)
		list<Substrate_link>* listLinks = substrate_network.GetSubstrateLinks();
		for (std::list<Substrate_link>::iterator it = listLinks->begin(); it != listLinks->end(); it++){
			Substrate_link l = *it;
			pstmt = con->prepareStatement("INSERT IGNORE INTO physical_links VALUES (?,?,?,?,?,?,?,?)");
			pstmt->setInt(1, l.GetId());
			pstmt->setString(2, l.GetName());
			pstmt->setInt(3, l.GetSourceNode()->GetId()); // pstmt->setInt(3, l.GetSourceNode_id());
			pstmt->setInt(4, l.GetDestinationNode()->GetId());
			pstmt->setDouble(5, l.GetBandwidth());
			pstmt->setDouble(6, 0); //No delay is taken into account
			pstmt->setString(7, l.GetStatus());
			pstmt->setDouble(8, l.GetAvailability());
			pstmt->executeUpdate();

			//Build the list of substrate nodes to be inserted into the data base
			if(std::find(listNodeIDs.begin(), listNodeIDs.end(), l.GetSourceNode()->GetId()) != listNodeIDs.end()){
				listNodeIDs.push_back(l.GetSourceNode()->GetId());
				listNodes.push_back(l.GetSourceNode());
			}
			if(std::find(listNodeIDs.begin(), listNodeIDs.end(), l.GetDestinationNode()->GetId())!=listNodeIDs.end()){
				listNodeIDs.push_back(l.GetDestinationNode()->GetId());
				listNodes.push_back(l.GetDestinationNode());
			}

		}


		//Write the physical nodes

		//physical_nodes(nodeID, name, type, flavor, cpu, memory, disk, availability)
		for(int i=0; i<listNodes.size(); i++){
			Substrate_node* node = listNodes.at(i);
			pstmt = con->prepareStatement("INSERT IGNORE INTO physical_nodes VALUES (?,?,?,?,?,?,?,?)");
			pstmt->setInt(1, node->GetId());
			pstmt->setString(2, node->GetName());
			pstmt->setInt(3, node->GetType());
			pstmt->setInt(4, 0); //There is no way I could access the flavor
			pstmt->setDouble(5, node->GetCpu());
			pstmt->setDouble(6, node->GetMemory());
			pstmt->setDouble(7, node->GetDisk());
			pstmt->setDouble(8, node->GetAvailability());
			pstmt->executeUpdate();
		}

		delete pstmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<" )" << endl;
	}

	cout << endl;

#endif

	return EXIT_SUCCESS;


}



void VDCPlannerMysqlConnector::deleteDataBase() {

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;

		/* Create a connection */
		driver = get_driver_instance();

		string connectTo("tcp://");
		connectTo.append(this->mysqlServerAddress);
		connectTo.append(":");
		stringstream ss1;
		ss1 << this->mysqlServerPort;
		connectTo.append(ss1.str());
		con = driver->connect(connectTo, this->userName, this->password);
		//con = driver->connect("tcp://127.0.0.1:3306", "root", "root");

		stmt = con->createStatement();
		//Create the data base
		string s1("DROP DATABASE IF EXISTS ");
		s1.append(this->dataBaseName);
		stmt->execute(s1);

		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<" )" << endl;
	}

	cout << endl;
}
