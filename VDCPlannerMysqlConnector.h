/*
 * VDCPlannerMysqlConnector.h
 *
 *  Created on: Apr 10, 2014
 *      Author: ahmedamokrane
 */

#ifndef VDCPLANNERMYSQLCONNECTOR_H_
#define VDCPLANNERMYSQLCONNECTOR_H_

//#include "GreenheadMysqlConnector.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <algorithm>
#include "Substrate_network.hpp"
#include "Substrate_node.hpp"
#include "Substrate_link.hpp"
#include "User.h"
#include "Request.hpp"
#include "RequestLoader.h"
#include "SubstrateLoader.h"
#include "mapping.hpp"
#include "Mapping_trial.h"
#include "provisioning_module.h"
#include "ConfigLoader.h"
#include "Config.h"

// MySQL includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

/*
 *
 */

using namespace std;

class VDCPlannerMysqlConnector {
public:

	string mysqlServerAddress;
	string dataBaseName;
	string userName;
	string password;
	int mysqlServerPort;

	VDCPlannerMysqlConnector(string mysqlServerAddress, int mysqlServerPort, string user, string pass,
			string dataBaseName);
	virtual ~VDCPlannerMysqlConnector();

	bool doesDataBaseExist();

	int createDataBase();
	void deleteDataBase();

	/* Writes the passed request and mapping into the Data Base */
	int writeVDCRequestToDataBase(Request* request);

	//Request* readVDCRequestClassFromDataBase(int idRequest);
	/* Reads request, virtual node, vitual link and mapping (node, link) tables from Data Base and
	 * populates the subNetwork with appropriate assignment of link and node pointers.
	 * Returns: the request read from Data Base and modified Substrate_network object pointer */
	Request* readVDCRequestClassFromDataBase(int idRequest, Substrate_network *subNetwork);

	/* Reads all the requests and accompanying tables from the Data Base using readVDCRequestClassFromDataBase
	 * function
	 * Returns: std::vector containing all the requests, and a modified Substrate_network object with
	 * appropriate assignment of link and node pointers. */
	vector<Request*>* readAllVDCRequestsClassFromDataBase(Substrate_network *subNetwork);

	int removeVDCRequestFromDataBase(Request* request, double t);

	int writeSubstrateNetworkToDataBase(Substrate_network);
	Substrate_network* readSubstrateNetworkFromDataBase();

};

#endif /* VDCPLANNERMYSQLCONNECTOR_H_ */
