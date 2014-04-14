/*
 * PlannerEngine.h
 *
 *  Created on: 2013-07-09
 *      Author: mjabri
 */

#ifndef PLANNERENGINE_H_
#define PLANNERENGINE_H_
//#pragma comment(lib, "ws2_32.lib")
#if defined (WIN32)
#include <winsock2.h>
typedef int socklen_t;
#elif defined (linux)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif
#include <iostream>
#include <stdio.h>
using namespace std;
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "Substrate_network.hpp"
#include "User.h"
#include "Request.hpp"
#include "RequestLoader.h"
#include "SubstrateLoader.h"
#include "mapping.hpp"
#include "Mapping_trial.h"
#include "provisioning_module.h"
#include "def.h"

#define PORT 8080

class PlannerEngine {
public:
	PlannerEngine();
	PlannerEngine(int,int,int,int);
	void TreatRequests();// this is the main function
	string ParseRequest(char*,int);
	bool ExistsBySession(std::string);
	User * GetUserBySession(std::string) ;
	string MappAvailability_ON(Request*,User *);
	virtual ~PlannerEngine();
private:
	int tracing;//ON or OFF
	int migration;//ON or OFF
	int availability;//ON or OFF
	int consolidation;//ON or OFF
	Substrate_network subnetwork;
	std::list<User> users;


};

#endif /* PLANNERENGINE_H_ */
