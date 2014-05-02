/*
 * VDCPlannerMysqlConnector.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: ahmedamokrane
 */

#include "VDCPlannerMysqlConnector.h"

VDCPlannerMysqlConnector::VDCPlannerMysqlConnector(string mysqlServerAddress, int mysqlServerPort, string user,
		string pass, string dataBaseName) {
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

#if 0
		/*
		 * 	int id;
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
#endif

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
				"PRIMARY KEY  (requestID) )");

		/*
		 * A mapping
		 *
		 * 	int id;
		 * 	int stateMapping;
		 * 	double mapTime;
		 * 	double availability;
		 */
		//mapping_vdc_requests (requestID, mappingID, stateMapping, mapTime, availability)
		stmt->execute("CREATE TABLE IF NOT EXISTS mapping_vdc_requests ("
				"requestID    INT UNSIGNED  NOT NULL,"
				"mappingID    INT UNSIGNED  NOT NULL,"
				"stateMapping    INT UNSIGNED  NOT NULL,"
				"mapTime   DOUBLE(20,10) NOT NULL,"
				"availability DOUBLE(20,10),"
				"PRIMARY KEY  (requestID, mappingID) )");

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
				"UNIQUE (requestID,nodeSource,nodeDestination))");

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

		//virtual_nodes(requestID, nodeID, partitionID, cpu, memory, disk, flavor, taskState, powerState, publicIP, privateIP, embeddingNodeName, priorityGroup,type)
		stmt->execute("CREATE TABLE IF NOT EXISTS virtual_nodes ("
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
				"type INT UNSIGNED NOT NULL,"
				"PRIMARY KEY  (requestID,nodeID))");

		/* physical_links table

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

#if 0
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
				"PRIMARY KEY  (linkID)," /* Because the linkID is supposed to be unique among all links in the substrate */
				"UNIQUE  (nodeSource,nodeDestination))");

		/* physical_nodes table
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
				"PRIMARY KEY  (nodeID))");
#endif
		/*
		 * For Mapping a virtual machine to a physical machine, you need,
		 * Request ID, Virtual Node ID, Substrate Network ID, Physical Node ID
		 * extra things are mapping times.
		 * */
		//mapping_virtual_nodes (requestID, VMID, subsrateNodeID, mappingBegin, mappingEnd)
		stmt->execute("CREATE TABLE IF NOT EXISTS mapping_virtual_nodes ("
				"requestID INT UNSIGNED  NOT NULL,"
				"VMID  INT UNSIGNED  NOT NULL,"
				"subsrateNodeID INT UNSIGNED  NOT NULL,"
				"PRIMARY KEY  (requestID, VMID))");

		/*
		 * For a virtual link to a physical link mapping, you need,
		 * Request ID, virtual Link ID, Substrate Network ID, Physical Link ID
		 * Since the code is coming from Greenhead, you also need EndPoints for each. and also mapping times
		 * */
		//mapping_virtual_links (requestID, virtualLinkID, virtualNodeSource, virtualNodeDestination, physicalLinkID, physicalNodeSource, physicalNodeDestination,  mappingBegin, mappingEnd)
		stmt->execute("CREATE TABLE IF NOT EXISTS mapping_virtual_links ("
				"requestID INT UNSIGNED  NOT NULL,"
				"virtualLinkID INT UNSIGNED  NOT NULL,"
				"virtualNodeSource  INT UNSIGNED  NOT NULL,"
				"virtualNodeDestination INT UNSIGNED  NOT NULL,"
				"physicalLinkID INT UNSIGNED,"
				"physicalNodeSource INT UNSIGNED,"
				"physicalNodeDestination INT UNSIGNED,"
				"UNIQUE  (requestID, virtualLinkID, physicalLinkID),"
				"UNIQUE  (requestID, virtualNodeSource, virtualNodeDestination, physicalNodeSource, physicalNodeDestination))");

#if 0
		/*
		 * Create the database for removed VDC requests (Keep trace of the old users that submitted the requests)
		 */

		//Create the TRIGGERS
		// The Triggers do some stuff whenever an event occurs in the data base. For instance, if we remove a request from the DB, we should also remove the VMs and links that belong to this request, which are located in other tables
		stmt->execute("CREATE TRIGGER delete_links_and_vms_on_delete_request AFTER DELETE on vdc_requests "
				"FOR EACH ROW "
				"BEGIN "
				"DELETE FROM virtual_nodes "
				"WHERE virtual_nodes.requestID = old.requestID "
				";"
				"DELETE FROM virtual_links "
				"WHERE virtual_links.requestID = old.requestID "
				";"
				"INSERT INTO vdc_requests_archive "
				"VALUES (old.requestID, old.arrival, old.duration, old.revenue, old.numberNodes, old.numberLinks, old.limitCarbonEmission, old.split, old.type, old.topo, old.availability, old.status, old.name, old.userName, old.application) "
				";"
				"END"
		);

		stmt->execute("CREATE TRIGGER delete_mappings_on_delete_virtual_machine AFTER DELETE on virtual_nodes "
				"FOR EACH ROW "
				"BEGIN "
				"INSERT INTO virtual_nodes_archive "
				"VALUES (old.requestID, old.nodeID, old.partitionID, old.cpu, old.memory, old.disk, old.flavor, old.taskState, old.powerState, old.publicIP, old.privateIP, old.embeddingNodeName, old.priorityGroup, old.type) "
				";"
				"END"
		);

		stmt->execute("CREATE TRIGGER delete_mappings_on_delete_virtual_link AFTER DELETE on virtual_links "
				"FOR EACH ROW "
				"BEGIN "
				"DELETE FROM mapping_virtual_links "
				"WHERE mapping_virtual_links.requestID = old.requestID "
				"AND mapping_virtual_links.virtualNodeSource = old.nodeSource "
				"AND mapping_virtual_links.virtualNodeDestination = old.nodeDestination"
				";"
				"INSERT INTO virtual_links_archive "
				"VALUES (old.requestID, old.linkID, old.nodeSource, old.nodeDestination, old.partitionSource, old.partitionDestination, old.bandwidth, old.delay, old.status, old.length, old.name) "
				";"
				"END"
		);

		stmt->execute("CREATE TRIGGER insert_archive_mappings_on_delete_mapping_vm AFTER DELETE on mapping_virtual_nodes "
				"FOR EACH ROW "
				"BEGIN "
				"INSERT INTO mapping_virtual_nodes_archive "
				"VALUES (old.requestID, old.VMID, old.subsrateNodeID, old.mappingBegin, old.mappingEnd) "
				";"
				"END"
		);

		stmt->execute("CREATE TRIGGER insert_archive_mappings_on_delete_mapping_virtual_links AFTER DELETE on mapping_virtual_links "
				"FOR EACH ROW "
				"BEGIN "
				"INSERT INTO mapping_virtual_links_archive "
				"VALUES(old.requestID, old.virtualLinkID, old.virtualNodeSource, old.virtualNodeDestination, old.physicalLinkID, old.physicalNodeSource, old.physicalNodeDestination,  old.mappingBegin, old.mappingEnd) "
				";"
				"END"
		);
#endif

		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
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
		string msg = ("show databases like \"");
		msg.append(this->dataBaseName);
		msg.append("\"");
		pstmt = con->prepareStatement(msg);
		res = pstmt->executeQuery();

		bool toReturn = false;
		if (res->next()) {
			toReturn = true;
		}

		delete stmt;
		delete pstmt;
		delete con;

		return toReturn;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
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

		//Write Request Mapping
		//mapping_vdc_requests (requestID, mappingID, stateMapping, mapTime, availability)
		pstmt = con->prepareStatement("INSERT IGNORE INTO mapping_vdc_requests VALUES (?,?,?,?,?)");
		pstmt->setInt(1, request->GetRequestNumber());
		pstmt->setInt(2, request->GetMapping()->GetId());
		pstmt->setInt(3, request->GetMapping()->GetStateMapping());
		pstmt->setDouble(4, request->GetMapping()->GetMapTime());
		pstmt->setDouble(5, request->GetMapping()->GetAvailability());

		pstmt->executeUpdate();

		//Write the VMs and their corresponding mappings
		std::list<Priority_group>*grp_list = request->GetGroups();
		//int i = 1;
		for (std::list<Priority_group>::iterator itPrioGroup = grp_list->begin(); itPrioGroup != grp_list->end();
				itPrioGroup++) {
			Priority_group gr = *itPrioGroup;
			for (std::list<Virtual_node>::iterator itVirtualNode = gr.GetNodes()->begin();
					itVirtualNode != gr.GetNodes()->end(); itVirtualNode++) {
				Virtual_node vm = *itVirtualNode;
				//Add the VM to the data base
				//virtual_nodes(requestID, nodeID, partitionID, cpu, memory, disk, flavor, taskState, powerState, publicIP, privateIP, embeddingNodeName, priorityGroup, type)
				pstmt = con->prepareStatement("INSERT IGNORE INTO virtual_nodes VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
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
				//pstmt->setString(12, "NIL"); // placeholder name
				pstmt->setString(12, vm.GetEmbeddingNode_name());
				//pstmt->setInt(13 , i); //The priorityGroup
				pstmt->setInt(13, gr.GetId()); //The priorityGroup, according to current logic, should remain 1
				pstmt->setInt(14, vm.GetType());
				pstmt->executeUpdate();

				//Add the VM mapping (virtual machine to physical node) to the data base
				//mapping_virtual_nodes (requestID, VMID, subsrateNodeID)
				if (vm.GetEmbeddingNode() != NULL) {
					pstmt = con->prepareStatement("INSERT IGNORE INTO mapping_virtual_nodes VALUES (?,?,?)");
					pstmt->setInt(1, request->GetRequestNumber());
					pstmt->setInt(2, vm.GetId());
					pstmt->setInt(3, vm.GetEmbeddingNode()->GetId());
					pstmt->executeUpdate();
				}
			}
		}

		//Write the Links and their corresponding mappings
		for (std::list<Path>::iterator it = request->GetLinks()->begin(); it != request->GetLinks()->end(); it++) {
			Path link = *it;
			//Write the link to the database
			//virtual_links(requestID, linkID, nodeSource, nodeDestination, partitionSource, partitionDestination, bandwidth, delay, status, length, name)
			pstmt = con->prepareStatement("INSERT IGNORE INTO virtual_links VALUES (?,?,?,?,?,?,?,?,?,?,?)");
			pstmt->setInt(1, request->GetRequestNumber());
			pstmt->setInt(2, link.GetId());
			pstmt->setInt(3, link.GetSourceNode_id());
			pstmt->setInt(4, link.GetDestinationNode_id());
			pstmt->setInt(5, 0); //No meaning for the partition in this case
			pstmt->setInt(6, 0); //No meaning for the partition in this case
			pstmt->setDouble(7, link.GetBandwidth());
			pstmt->setDouble(8, 10000); // TODO: The delay is not mentioned in the Request, put a high value placeholder
			pstmt->setString(9, link.GetStatus());
			pstmt->setInt(10, link.GetLength());
			pstmt->setString(11, link.GetName());
			pstmt->executeUpdate();

			//Write the mapping of the link to the database
			if (link.GetSubstrateLinks() != NULL) {
				for (std::list<Substrate_link>::iterator itSubstrateLinks = link.GetSubstrateLinks()->begin();
						itSubstrateLinks != link.GetSubstrateLinks()->end(); itSubstrateLinks++) {
					Substrate_link subL = *itSubstrateLinks;
					//mapping_virtual_links (requestID, virtualLinkID, virtualNodeSource, virtualNodeDestination, physicalLinkID, physicalNodeSource, physicalNodeDestination)
					pstmt = con->prepareStatement("INSERT IGNORE INTO mapping_virtual_links VALUES (?,?,?,?,?,?,?)");
					pstmt->setInt(1, request->GetRequestNumber());
					pstmt->setInt(2, link.GetId());
					pstmt->setInt(3, link.GetSourceNode_id());
					pstmt->setInt(4, link.GetDestinationNode_id());
					pstmt->setInt(5, subL.GetId());
					pstmt->setInt(6, subL.GetSourceNode()->GetId()); //pstmt->setInt(6, subL.GetSourceNode_id());
					pstmt->setInt(7, subL.GetDestinationNode()->GetId()); //pstmt->setInt(7, subL.GetDestinationNode_id());
					pstmt->executeUpdate();
				}
			}
		}
		delete pstmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cout << endl;

	return EXIT_SUCCESS;
}

Request* VDCPlannerMysqlConnector::readVDCRequestClassFromDataBase(int idRequest, Substrate_network *subNetwork) {

	Request* request = NULL;

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::ResultSet *res1;
		sql::ResultSet *res2;
		sql::PreparedStatement *pstmt1;
		sql::PreparedStatement *pstmt2;

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
		pstmt1 = con->prepareStatement("SELECT * FROM vdc_requests WHERE requestID = (?)");
		pstmt1->setInt(1, idRequest);
		res1 = pstmt1->executeQuery();

		if (!res1->next()) {
			//The request with id "idRequest" does not exist in the data base
			debug("No Request: %d found in Database Table vdc_requests", idRequest);
		} else {
			//The request exists in the data base, build the VDC request
			//vdc_requests (requestID, arrival, duration, revenue, numberNodes, numberLinks, limitCarbonEmission, split, type, topo, availability, status, name, userName, application)
			request = new Request();
			request->SetRequestNumber(res1->getInt("requestID"));
			request->SetArrTime(res1->getDouble("arrival"));
			request->SetDuration(res1->getDouble("duration"));
			request->SetRevenue(res1->getDouble("revenue"));
			request->SetNumberNodes(res1->getInt("numberNodes"));
			request->SetSplit(res1->getInt("split"));
			// type is missing form Request Class
			request->SetTopo(res1->getInt("topo"));
			request->SetAvailability(res1->getDouble("availability"));
			request->SetStatus(res1->getInt("status"));
			request->SetName(res1->getString("name"));
			request->SetUserName(res1->getString("userName"));
			request->SetApplication(res1->getString("application"));

			// read/set Mapping
			pstmt1 = con->prepareStatement("SELECT * FROM mapping_vdc_requests WHERE requestID = (?)");
			pstmt1->setInt(1, idRequest);
			res1 = pstmt1->executeQuery();

			if (!res1->next()) {
				// No mapping against the request
				debug("No mapping against the request: %d found in Database Table mapping_vdc_requests", idRequest);
			} else {
				mapping* map = new mapping(request);
				request->SetMapping(map);
				//mapping_vdc_requests (requestID, mappingID, stateMapping, mapTime, availability)
				request->GetMapping()->SetRequest(request);
				request->GetMapping()->SetId(res1->getInt("mappingID"));
				request->GetMapping()->SetStateMapping(res1->getInt("stateMapping"));
				request->GetMapping()->SetMapTime(res1->getDouble("mapTime"));
				request->GetMapping()->SetAvailability(res1->getDouble("availability"));
			}

			//Read the VMs
			//virtual_nodes(requestID, nodeID, partitionID, cpu, memory, disk, flavor, taskState, powerState, publicIP, privateIP, embeddingNodeName, priorityGroup, type)
			//All VMs will be in the same priority group
			std::list<Virtual_node>* listVMs = new std::list<Virtual_node>();
			pstmt1 = con->prepareStatement("SELECT * FROM virtual_nodes WHERE requestID = (?) ORDER BY nodeID ASC ");
			pstmt1->setInt(1, idRequest);
			res1 = pstmt1->executeQuery();

			while (res1->next()) {
				Virtual_node* vm = new Virtual_node();
				vm->SetId(res1->getInt("nodeID"));
				vm->SetCpu(res1->getDouble("cpu"));
				vm->SetMemory(res1->getDouble("memory"));
				vm->SetDisk(res1->getDouble("disk"));
				vm->SetFlavor(res1->getInt("flavor"));
				vm->SetTaskState(res1->getString("taskState"));
				vm->SetPowerState(res1->getString("powerState"));
				vm->SetPublicIp(res1->getString("publicIP"));
				vm->SetPrivateIp(res1->getString("privateIP"));
				vm->SetEmbeddingNode_name(res1->getString("embeddingNodeName"));
				vm->SetType(res1->getInt("type"));
				vm->SetBandwidth(0); // set bandwidth to 0 manually

				//Read the mapping of this VM
				//mapping_virtual_nodes (requestID, VMID, subsrateNodeID)
				pstmt2 = con->prepareStatement("SELECT * FROM mapping_virtual_nodes WHERE requestID = (?) AND VMID = (?) ");
				pstmt2->setInt(1, idRequest);
				pstmt2->setInt(2, vm->GetId());
				res2 = pstmt2->executeQuery();
				// Since each VNode is Mapped to a single PNode, we can assume a single entry against each VMID
				// that is why we do not do res2->next() continually
				// Assign substrate node (PNode) to the VNode and vice versa
				if (!res2->next()) {
					// no mapping from vnode to pnode found...
					debug("No mapping from vnode to pnode found in Database Table mapping_virtual_nodes");
				} else {
					vm->SetEmbeddingNode(subNetwork->GetNodeById(res2->getInt("subsrateNodeID")));
					subNetwork->GetNodeById(res2->getInt("subsrateNodeID"))->GetEmbeddedNodes()->push_back(*vm);
					subNetwork->GetNodeById(res2->getInt("subsrateNodeID"))->GetEmbeddedNodes_id()->push_back(
							res2->getInt("VMID"));
					listVMs->push_back(*vm);
				}
			}
			Priority_group group = Priority_group(1); // always remain 1 for current implementation
			group.SetId(1);
			group.SetNodes(listVMs);
			request->GetGroups()->push_back(group);
			//Read the links
			pstmt1 = con->prepareStatement("SELECT * FROM virtual_links WHERE requestID = (?) ORDER BY linkID ASC ");
			pstmt1->setInt(1, idRequest);
			res1 = pstmt1->executeQuery();
			//list<Path>* links = new list<Path>();
			//virtual_links(requestID, linkID, nodeSource, nodeDestination, partitionSource, partitionDestination, bandwidth, delay, status, length, name)
			while (res1->next()) {
				int linkID = res1->getInt("linkID");
				int vmFrom = res1->getInt("nodeSource");
				int vmTo = res1->getInt("nodeDestination");
				double bw = res1->getDouble("bandwidth");
				Virtual_node * from = request->GetNodeById(vmFrom); //The nodes have already been read in the list of nodes
				Virtual_node * to = request->GetNodeById(vmTo);
				Path* vlink = new Path(linkID, vmFrom, from, vmTo, to, bw);
				vlink->SetStatus(res1->getString("status"));
				vlink->SetName(res1->getString("name"));
				vlink->SetLength(res1->getInt("length"));

				//Read the link mapping
				//mapping_virtual_links (requestID, virtualLinkID, virtualNodeSource, virtualNodeDestination, physicalLinkID, physicalNodeSource, physicalNodeDestination)
				pstmt2 = con->prepareStatement(
						"SELECT * FROM mapping_virtual_links WHERE requestID = (?) AND virtualLinkID = (?) ");
				pstmt2->setInt(1, idRequest);
				pstmt2->setInt(2, linkID);
				res2 = pstmt2->executeQuery();
				/* for each physical link, the virtual link is embedded in, push the physical link into a list and assign it to
				 * the virtual link. Also assign the virtual link to the physical link.
				 */
				while (res2->next()) {
					int physcialLinkId = res2->getInt("physicalLinkID");
					Substrate_link* substrateLink = subNetwork->GetLinkById(physcialLinkId);
					vlink->GetSubstrateLinks()->push_back(*substrateLink);
					subNetwork->GetLinkById(physcialLinkId)->GetEmbeddedPaths()->push_back(*vlink);
				}
				// Finally push the virtual link into the request and update bandwidth and revenue
				request->GetLinks()->push_back(*vlink);
				request->IncrementRevenue(bw / 100);
				request->SetRevenue((request->GetRevenue() * request->GetDuration()) / (double) 60);
			} //end While res1->next() //Number of links
		}

		delete res1;
		delete pstmt1;
		delete res2;
		delete pstmt2;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		delete request;
		request = NULL;
	}

	return request;
}

vector<Request*>* VDCPlannerMysqlConnector::readAllVDCRequestsClassFromDataBase(Substrate_network *subNetwork) {

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

		while (res->next()) {
			int idRequest = res->getInt("requestID");
			Request* req = this->readVDCRequestClassFromDataBase(idRequest, subNetwork);

			req->DisplayRequest();

			listResults->push_back(req);
		}		//End While on the number of request

		delete res;
		delete pstmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
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

#if 0
		//Update the old mapping
		pstmt = con->prepareStatement(
				"UPDATE mapping_virtual_nodes SET mappingEnd = (?)"
				"WHERE requestID = (?) AND ((mappingEnd IS NULL) OR (mappingEnd = -1))"
		);
		pstmt->setDouble(1,t);
		pstmt->setInt(2,request->GetRequestNumber());
		pstmt->executeUpdate();
#endif

#if 0
		pstmt = con->prepareStatement(
				"UPDATE mapping_virtual_links SET mappingEnd = (?)"
				"WHERE requestID = (?) AND ((mappingEnd IS NULL) OR (mappingEnd = -1))"
		);
		pstmt->setDouble(1,t);
		pstmt->setInt(2,request->GetRequestNumber());
		pstmt->executeUpdate();
#endif

		//delete requests
		pstmt = con->prepareStatement("DELETE FROM vdc_requests WHERE requestID = (?)");
		pstmt->setInt(1, request->GetRequestNumber());
		pstmt->executeUpdate();

		//delete mapping
		pstmt = con->prepareStatement("DELETE FROM mapping_vdc_requests WHERE requestID = (?)");
		pstmt->setInt(1, request->GetRequestNumber());
		pstmt->executeUpdate();

		//delete virtual nodes
		pstmt = con->prepareStatement("DELETE FROM virtual_nodes WHERE requestID = (?)");
		pstmt->setInt(1, request->GetRequestNumber());
		pstmt->executeUpdate();

		//delete virtual nodes mapping
		pstmt = con->prepareStatement("DELETE FROM mapping_virtual_nodes WHERE requestID = (?)");
		pstmt->setInt(1, request->GetRequestNumber());
		pstmt->executeUpdate();

		//delete virtual links
		pstmt = con->prepareStatement("DELETE FROM virtual_links WHERE requestID = (?)");
		pstmt->setInt(1, request->GetRequestNumber());
		pstmt->executeUpdate();

		//delete virtual links mapping
		pstmt = con->prepareStatement("DELETE FROM mapping_virtual_links WHERE requestID = (?)");
		pstmt->setInt(1, request->GetRequestNumber());
		pstmt->executeUpdate();

		/*
		 * This part is optional, the triggers already remove the links and VMs that belong to the request (have the same requestID)
		 *
		 pstmt = con->prepareStatement("DELETE FROM virtual_links WHERE requestID = (?)");
		 pstmt->setInt(2,request->GetRequestNumber());
		 pstmt->executeUpdate();

		 pstmt = con->prepareStatement("DELETE FROM virtual_nodes WHERE requestID = (?)");
		 pstmt->setInt(2,request->GetRequestNumber());
		 pstmt->executeUpdate();

		 pstmt = con->prepareStatement("DELETE FROM mapping_virtual_nodes WHERE requestID = (?)");
		 pstmt->setInt(2,request->GetRequestNumber());
		 pstmt->executeUpdate();

		 pstmt = con->prepareStatement("DELETE FROM mapping_virtual_links WHERE requestID = (?)");
		 pstmt->setInt(2,request->GetRequestNumber());
		 pstmt->executeUpdate();

		 *
		 */

		delete pstmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
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
		for (std::list<Substrate_link>::iterator it = listLinks->begin(); it != listLinks->end(); it++) {
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
			if (std::find(listNodeIDs.begin(), listNodeIDs.end(), l.GetSourceNode()->GetId()) != listNodeIDs.end()) {
				listNodeIDs.push_back(l.GetSourceNode()->GetId());
				listNodes.push_back(l.GetSourceNode());
			}
			if (std::find(listNodeIDs.begin(), listNodeIDs.end(), l.GetDestinationNode()->GetId()) != listNodeIDs.end()) {
				listNodeIDs.push_back(l.GetDestinationNode()->GetId());
				listNodes.push_back(l.GetDestinationNode());
			}

		}

		//Write the physical nodes

		//physical_nodes(nodeID, name, type, flavor, cpu, memory, disk, availability)
		for (int i = 0; i < listNodes.size(); i++) {
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
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
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
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cout << endl;
}
