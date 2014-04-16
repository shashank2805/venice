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
#include <cmath>
#include "Substrate_network.hpp"
#include "User.h"
#include "Request.hpp"
#include "RequestLoader.h"
#include "SubstrateLoader.h"
#include "mapping.hpp"
#include "Mapping_trial.h"
#include "provisioning_module.h"
#include "ConfigLoader.h"
#include "Config.h"
#include "VDCPlannerMysqlConnector.h"


#define PORT 8080

#include "PlannerEngine.h"

//this struct is used to send multiple parameters to the function1 from pthread_create
struct arg_struct {
	Substrate_network * net;
	Request * req;
};

struct arg_struct2 {
	User * user;
	Request * req;
};

void * function1(void * pars) {

	struct arg_struct *args = (struct arg_struct *) pars;
	Request * request = args->req;
	Substrate_network * net = args->net;
	cout << " Starts counting\n";
	sleep(request->GetDuration());
	//sleep(10);
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&mutex);
	cout << " State Mapping = " << request->GetStatus();
	if (request->GetStatus() != STATE_ABORTED) {
		request->FreeMapping(net);
	}
	pthread_mutex_unlock(&mutex);
	//net->DisplaySubstrateEmbedding();
	return 0;
}

void * function2(void * pars) {

	struct arg_struct2 *args = (struct arg_struct2 *) pars;
	Request * request = args->req;
	User * user = args->user;
	cout << " Starts counting\n";
	sleep(QUEING_DELAY);
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&mutex);
	cout << " We remove the request from the waiting list = " << endl;
	cout << " size before = " << user->GetWaitingRequests()->size() << endl;
	user->RemoveWaitingRequest(request->GetRequestNumber());
	cout << " size after = " << user->GetWaitingRequests()->size() << endl;
	pthread_mutex_unlock(&mutex);
	//net->DisplaySubstrateEmbedding();
	return 0;
}

bool existsBySession(std::list<User>* users, std::string session) {
	bool found = false;
	std::list<User>::iterator it1 = users->begin();
	while (!found && it1 != users->begin()) {
		if (it1->GetSession() == session) {
			found = true;
		} else {
			it1++;
		}
	}
	return found;
}

User * getUserBySession(std::list<User>* users, std::string session) {
	bool found = false;
	std::list<User>::iterator it1 = users->begin();
	while (!found && it1 != users->begin()) {
		if (it1->GetSession() == session) {
			found = true;
		} else {
			it1++;
		}
	}
	return &(*it1);
}

string Config::image_id="";
string Config::key_pair="";
string Config::portal_address="";
string Config::topology_file="";
string Config::vdc_planner_address="";
// database related information
string Config::mysql_server_address="";
int Config::mysql_server_port=0;
string Config::mysql_server_user="";
string Config::mysql_server_password="";
string Config::mysql_database_name="";

int main() {

	//Variables to use for the server
#if defined (WIN32)
	WSADATA WSAData;
	int error = WSAStartup(MAKEWORD(2,2), &WSAData);
#else
	int error = 0;
#endif

	char buffer[1000], buffer2[1000], response[1000], response2[3000];
	int sock_err, ret_val;
	std::string status;
	//Socket and serveur context
	SOCKADDR_IN sin;
	SOCKET sock;
	socklen_t recsize = sizeof(sin);

	//Socket and client context
	SOCKADDR_IN csin;
	SOCKET csock;
	socklen_t crecsize = sizeof(csin);
	int idRequest = 0;
	std::list<User> users;
	User user;
	ConfigLoader cLoader;
	Config config;
	cLoader.Load(config);

	//----------- First we load the substrate network
	Substrate_network subNetwork(0);
	SubstrateLoader nLoader;
	nLoader.Load(&subNetwork);

	if (!error) {

		// Create Database object
		// NOTE: this information must be changed and code recompiled to reflect the location of the database
		VDCPlannerMysqlConnector vdc_db(config.mysql_server_address, config.mysql_server_port,
				config.mysql_server_user, config.mysql_server_password, config.mysql_database_name);

		if(vdc_db.doesDataBaseExist()) { //if DB is pre-existing, use it.
			debug("%s \n","Database exists.");
			std::vector<Request*>* db_requests;
			std::vector<Request*>::iterator itDBRequest;
			int user_id = 1;
			debug("%s \n","Beginning to read all requests from Database");
			db_requests = vdc_db.readAllVDCRequestsClassFromDataBase();
			debug("%s: %d\n","Successfully read requests from DB:", db_requests->size());

			for (itDBRequest = db_requests->begin(); itDBRequest != db_requests->end(); itDBRequest++) {
				// debug print
				debug("%s \n","Found a relevant request:");
				(*itDBRequest)->DisplayRequest();

				// new user
				debug("%s: %d\n","creating new user", user_id);
				user.SetId(user_id++);
				user.SetSession("v2gvna14098bvdjtcpo7njh5t2"); // session ID copied from a testrun
				users.push_back(user);
				debug("Current Embedding:\n");
				subNetwork.DisplaySubstrateEmbedding();

				//let's select the best mapping option among four possible ones
				Mapping_trial mapp1(*itDBRequest, 1);
				mapp1.Mapp(&subNetwork);
				mapp1.FreeMapp(&subNetwork);
				Mapping_trial mapp2(*itDBRequest, 2);
				mapp2.Mapp(&subNetwork);
				mapp2.FreeMapp(&subNetwork);
				Mapping_trial mapp3(*itDBRequest, 3);
				mapp3.Mapp(&subNetwork);
				mapp3.FreeMapp(&subNetwork);
				Mapping_trial mapp4(*itDBRequest, 4);
				mapp4.Mapp(&subNetwork);
				mapp4.FreeMapp(&subNetwork);
				if ((mapp1.GetStateMapping() == STATE_MAP_NODE_FAIL
							|| mapp1.GetStateMapping() == STATE_MAP_LINK_FAIL)
						&& (mapp2.GetStateMapping() == STATE_MAP_NODE_FAIL
							|| mapp2.GetStateMapping() == STATE_MAP_LINK_FAIL)
						&& (mapp3.GetStateMapping() == STATE_MAP_NODE_FAIL
							|| mapp3.GetStateMapping() == STATE_MAP_LINK_FAIL)
						&& (mapp4.GetStateMapping() == STATE_MAP_NODE_FAIL
							|| mapp4.GetStateMapping() == STATE_MAP_LINK_FAIL)) {
					//the mapping was not possible we put let the request wait
					debug("%s \n","Mapping from database read request not possible.");
					user.GetWaitingRequests()->push_back(**itDBRequest);
					pthread_t t1;
					struct arg_struct2 args;
					args.user = &user;
					args.req = *itDBRequest; //we have to pass the user request otherwise we are jus passing a copy of it
					pthread_create(&t1, 0, function2, (void *) &args); // create a thread running function1
					//sprintf(response, "MAP_FAIL&");
					//send(csock, response, sizeof(response), 0);
				} else {
					debug("%s \n","Mapping from database read request is possible.");
					mapping mapp = mapping(*itDBRequest);
					debug("%s \n","Mapping achieved.");
					//We try to sellect the best mapping among trials
					double tempAvai = abs(mapp1.GetAvailability() - (*itDBRequest)->GetAvailability());
					Mapping_trial * best = &mapp1;
					debug("%s \n", "mapp1 is the temporarily selected best mapping");
					if (tempAvai > abs(mapp2.GetAvailability() - (*itDBRequest)->GetAvailability())) {
						tempAvai = abs(mapp2.GetAvailability() - (*itDBRequest)->GetAvailability());
						best = &mapp2;
						debug("%s \n", "mapp2 is the best mapping");
					}
					if (tempAvai > abs(mapp3.GetAvailability() - (*itDBRequest)->GetAvailability())) {
						tempAvai = abs(mapp3.GetAvailability() - (*itDBRequest)->GetAvailability());
						best = &mapp3;
						debug("%s \n", "mapp3 is the best mapping");
					}
					if (tempAvai > abs(	mapp4.GetAvailability()	- (*itDBRequest)->GetAvailability())) {
						tempAvai = abs(mapp4.GetAvailability() - (*itDBRequest)->GetAvailability());
						best = &mapp4;
						debug("%s \n", "mapp4 is the best mapping");
					}
					debug("%s \n","Selecting Best Mapping.");
					best->displayMapp();
					mapp.ApplyBestMapping(best->GetNodeMapp(),
							best->GetPathMapp(),
							best->GetPathLength(), &subNetwork);
					debug("%s \n","Assigning Best Mapping.");
					(*itDBRequest)->SetMapping(&mapp);
					debug("Resulting Embedding:\n");
					subNetwork.DisplaySubstrateEmbedding();
					cout	<< "The resulting availability of the best mapping is : "
							<< (*itDBRequest)->GetMapping()->GetAvailability()
							<< " while the required availability is : "
							<< (*itDBRequest)->GetAvailability() << "\n";

					sprintf(response, "MAP_DONE&%f&",
							(*itDBRequest)->GetMapping()->GetAvailability());
					user.GetRequests()->push_back(**itDBRequest);
					pthread_t t1;
					struct arg_struct args;
					args.net = &subNetwork;
					args.req = user.GetRequestById(
							(*itDBRequest)->GetRequestNumber()); //we have to pass the user request otherwise we are jus passing a copy of it
					pthread_create(&t1, 0, function1,
							(void *) &args); // create a thread running function1
					//send(csock, response, sizeof(response), 0);
				}
				idRequest++;
			}
		}
		else { // Else create a new DB that will be populated when a new request comes in
			// debug print
			debug("%s \n","NO database found, Creating a new DB");
			ret_val = vdc_db.createDataBase();
			if (ret_val == EXIT_SUCCESS) debug("%s \n","SUCCESS in Creating a new DB");
			else  debug("%s \n","FAILURE while Creating a new DB");
		}

		// Socket creation
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock != INVALID_SOCKET) {
			cout << "Socket is now open " << sock << "\n";
			// Configuration
			sin.sin_addr.s_addr = htonl(INADDR_ANY); // automatique IP Adress
			sin.sin_family = AF_INET; // IP Protocol
			sin.sin_port = htons(PORT); // Port specification
			sock_err = bind(sock, (SOCKADDR*) &sin, recsize);

			if (sock_err != SOCKET_ERROR) {
				//idRequest = 0;
				sock_err = listen(sock, 5);
				while (1) {
					if (sock_err != SOCKET_ERROR) {
						cout << "Waiting for a new request ...\n";
						csock = accept(sock, (SOCKADDR*) &csin, &crecsize);
						//New session has been opened let's create a User intance
						cout << "Request received from "
								<< inet_ntoa(csin.sin_addr) << " : "
								<< htons(csin.sin_port) << "\n";

						recv(csock, buffer, sizeof(buffer), 0);
						//cout << buffer << endl;
						strcpy(buffer2, buffer);
						char * action = strtok(buffer2, "&,");
						char * session = strtok(NULL, "&,");
						if (!existsBySession(&users, session)) {
							user.SetId(csock);
							user.SetSession(session);
							cout << "creation of a new user : session id = "<< session << "\n";
							users.push_back(user);
						} else {
							user = *(getUserBySession(&users, session));
						}
						//if (!user.GetRequests()->empty()) {user.GetRequestById(0)->DisplayRequest();}
						if (!strcmp(action, "newvdc")) {
							cout << "new VDC" << endl;

							//cout << "Session id = " << user.GetSession() << "\n";
							//for testing--------cout << "Session id = " << session << "\n";
							//let's extract the request details from the message received
							Request request;
							RequestLoader rLoader;
							printf("*****  \n%s\n********\n", buffer);
							rLoader.Load(&request, WEB, idRequest, buffer);
							//for testing-----------
							request.DisplayRequest();
							// write request to DB
							debug("%s \n","write new request to DB");
							ret_val = vdc_db.writeVDCRequestToDataBase(&request);
							if (ret_val == EXIT_SUCCESS) debug("%s \n","SUCCESS in writing new request to DB");
							else debug("%s \n","FAILURE in writing new request to DB");
							//for testing
							debug("Current Embedding:\n");
							subNetwork.DisplaySubstrateEmbedding();
							//let's select the best mapping option among four possible ones
							Mapping_trial mapp1(&request, 1);
							mapp1.Mapp(&subNetwork);
							//for testing-----------subNetwork.DisplaySubstrateEmbedding();
							mapp1.FreeMapp(&subNetwork);
							//for testing-----------
							//for testing-----------cout<<"The resulting availability of mapping 1 is : "<<mapp1.GetAvailability()<<"\n";
							//for testing---------
							Mapping_trial mapp2(&request, 2);
							mapp2.Mapp(&subNetwork);
							mapp2.FreeMapp(&subNetwork);
							//for testing-----------subNetwork.DisplaySubstrateEmbedding();
							//for testing-----------cout<<"The resulting availability of mapping 2 is : "<<mapp2.GetAvailability()<<"\n";
							Mapping_trial mapp3(&request, 3);
							mapp3.Mapp(&subNetwork);
							mapp3.FreeMapp(&subNetwork);
							//for testing-----------subNetwork.DisplaySubstrateEmbedding();
							//for testing-----------cout<<"The resulting availability of mapping 3 is : "<<mapp3.GetAvailability()<<"\n";
							Mapping_trial mapp4(&request, 4);
							mapp4.Mapp(&subNetwork);
							mapp4.FreeMapp(&subNetwork);
							//for testing-----------subNetwork.DisplaySubstrateEmbedding();

							//for testing-----------cout<<"The resulting availability of mapping 4 is : "<<mapp4.GetAvailability()<<"\n";
							if ((mapp1.GetStateMapping() == STATE_MAP_NODE_FAIL
									|| mapp1.GetStateMapping()
											== STATE_MAP_LINK_FAIL)
									&& (mapp2.GetStateMapping()
											== STATE_MAP_NODE_FAIL
											|| mapp2.GetStateMapping()
													== STATE_MAP_LINK_FAIL)
									&& (mapp3.GetStateMapping()
											== STATE_MAP_NODE_FAIL
											|| mapp3.GetStateMapping()
													== STATE_MAP_LINK_FAIL)
									&& (mapp4.GetStateMapping()
											== STATE_MAP_NODE_FAIL
											|| mapp4.GetStateMapping()
													== STATE_MAP_LINK_FAIL)) {
								//the mapping was not possible we put let the request wait
								user.GetWaitingRequests()->push_back(request);
								idRequest++;
								pthread_t t1;
								struct arg_struct2 args;
								args.user = &user;
								args.req = &request; //we have to pass the user request otherwise we are jus passing a copy of it
								pthread_create(&t1, 0, function2,
										(void *) &args); // create a thread running function1
								sprintf(response, "MAP_FAIL&");
								send(csock, response, sizeof(response), 0);
							} else {
								mapping mapp = mapping(&request);
								//We try to sellect the best mapping among trials
								double tempAvai = abs(
										mapp1.GetAvailability()
												- request.GetAvailability());
								Mapping_trial * best = &mapp1;
								debug("%s \n", "mapp1 is the default best mapping");
								if (tempAvai
										> abs(
												mapp2.GetAvailability()
														- request.GetAvailability())) {
									tempAvai =
											abs(
													mapp2.GetAvailability()
															- request.GetAvailability());
									best = &mapp2;
									debug("%s \n", "mapp2 is the best mapping");
								}
								if (tempAvai
										> abs(
												mapp3.GetAvailability()
														- request.GetAvailability())) {
									tempAvai =
											abs(
													mapp3.GetAvailability()
															- request.GetAvailability());
									best = &mapp3;
									debug("%s \n", "mapp3 is the best mapping");
								}
								if (tempAvai
										> abs(
												mapp4.GetAvailability()
														- request.GetAvailability())) {
									tempAvai =
											abs(
													mapp4.GetAvailability()
															- request.GetAvailability());
									best = &mapp4;
									debug("%s \n", "mapp4 is the best mapping");
								}
								best->displayMapp();
								//-----------------------------------------------
								//for testing-----------cout<<"The resulting availability of the best mapping is : "<<best->GetAvailability()<<" while the required availability is : "<<request.GetAvailability()<<"\n";
								mapp.ApplyBestMapping(best->GetNodeMapp(),
										best->GetPathMapp(),
										best->GetPathLength(), &subNetwork);
								//for testing-----------cout<<"The availability mapp is : "<<mapp.GetAvailability()<<"\n";
								request.SetMapping(&mapp);
								//for testing
								//for testing-----------request.GetMapping()->displayMapp();
								//for testing-----------request.FreeMapping(&subNetwork);

								debug("Resulting Embedding:\n");
								subNetwork.DisplaySubstrateEmbedding();
								cout
										<< "The resulting availability of the best mapping is : "
										<< request.GetMapping()->GetAvailability()
										<< " while the required availability is : "
										<< request.GetAvailability() << "\n";
								idRequest++;
								sprintf(response, "MAP_DONE&%f&",
										request.GetMapping()->GetAvailability());
								//for testing-----------subNetwork.DisplaySubstrateEmbedding();
								//for testing-----------cout << " DISPLAY BEFORE PUSH BACK" << endl;
								//for testing-----------request.DisplayRequest();

								user.GetRequests()->push_back(request);
								//user.GetRequests()->push_back(&request);
								//for testing-----------cout << " DISPLAY BEFORE PUSH BACK" << endl;
								//for testing-----------user.GetRequestById(0)->DisplayRequest();

								pthread_t t1;
								struct arg_struct args;
								args.net = &subNetwork;
								args.req = user.GetRequestById(
										request.GetRequestNumber()); //we have to pass the user request otherwise we are jus passing a copy of it
								pthread_create(&t1, 0, function1,
										(void *) &args); // create a thread running function1

								send(csock, response, sizeof(response), 0);
							}

						} else {
							if (!strcmp(action, "trackvdcs")) {
								cout << "track VDCs" << endl;

								sprintf(response, "TRACK_DONE");
								if (!user.GetRequests()->empty()) {
									//for testing-----------

									sprintf(response, "%s&%d", response,
											user.GetRequests()->size());
									int idRequest;
									std::list<Request>::iterator it1 =
											user.GetRequests()->begin();
									//std::list<Request>::iterator it1 =
									user.GetRequests()->begin();
									pthread_mutex_t mutex =
											PTHREAD_MUTEX_INITIALIZER;
									pthread_mutex_lock(&mutex);
									while (it1 != user.GetRequests()->end()) {
										(it1)->CalculateStatus();
										//cout<<" here "<<endl;
										int reqNumber =
												(it1)->GetRequestNumber();
										//cout<<" here "<<endl;
										int nbNodes = (it1)->GetNumberNodes();
										//cout<<" here "<<endl;
										int nbLinks = (it1)->GetLinks()->size();
										//cout<<" here "<<endl;
										double duration = (it1)->GetDuration();
										//cout<<" here "<<endl;
										double avai =
												(it1)->GetMapping()->GetAvailability();
										//cout<<" here "<<endl;
										int status = (it1)->GetStatus();

										struct tm tstruct_tmp;
										char buf_tm_tmp[80];
										time_t arrTime = (it1)->GetArrTime();

										time_t now = time(0);
										//tstruct_tmp = *localtime(&arrTime);
										strftime(buf_tm_tmp, sizeof(buf_tm_tmp),
												"%Y-%m-%d.%X",
												localtime(&arrTime));
										cout << buf_tm_tmp << endl;
										double remainTime;
										if (status == STATE_EXPIRED
												|| status == STATE_ABORTED) {
											remainTime = 0;
										} else {
											if (status == STATE_WAITING) {
												remainTime =
														(it1)->GetDuration();
											} else {
												remainTime =
														(it1)->GetDuration()
																- difftime(now,
																		(it1)->GetArrTime());
											}
										}

										//cout<<" here "<<endl;
										//for testing-----------it1->DisplayRequest();
										sprintf(response,
												"%s&%d&%s&%d&%d&%lf&%lf&%d&%s&%lf",
												response, reqNumber,
												(it1)->GetUserName().c_str(),
												nbNodes, nbLinks, duration,
												avai, status, buf_tm_tmp,
												remainTime);
										it1++;
									}

									pthread_mutex_unlock(&mutex);

								} else {
									sprintf(response, "%s&0", response);
								}
								sprintf(response, "%s&", response);

								send(csock, response, sizeof(response), 0);
							} else {
								if (!strcmp(action, "dltvdc")) {
									char * charId = strtok(NULL, "&,");
									int id = atoi(charId);
									cout << " asking to delete VDC with id "
											<< id << endl;
									//cout <<" user status : "<< user.GetRequestById(id)->GetStatus() << endl;
									if (user.GetRequestById(id)->GetStatus()==STATE_RUNNING) {
										pthread_mutex_t mutex =
												PTHREAD_MUTEX_INITIALIZER;
										pthread_mutex_lock(&mutex);
										//for testing-----------subNetwork.DisplaySubstrateEmbedding();
										user.GetRequestById(id)->FreeMapping(
												&subNetwork);
										//for testing-----------subNetwork.DisplaySubstrateEmbedding();
										user.GetRequestById(id)->SetStatus(
												STATE_ABORTED);
										//user.GetRequestById(id)->GetMapping()->displayMapp();
										pthread_mutex_unlock(&mutex);
										sprintf(response, "%s&%d", "RMV_DONE",
												id);

										// Delete VDC Request from Database
										debug("%s \n","remove a request from DB");
										ret_val = vdc_db.removeVDCRequestFromDataBase(user.GetRequestById(id), double(time(0)));
										if (ret_val == EXIT_SUCCESS) debug("%s \n","SUCCESS in removing request from DB");
										else debug("%s \n","FAILURE in removing request from DB");

									} else {
										sprintf(response, "%s&%d", "RMV_FAILED",
												id);
									}

									send(csock, response, sizeof(response), 0);
								} else {
									if (!strcmp(action, "vdcdetails")) {
										char * charId = strtok(NULL, "&,");
										int id = atoi(charId);
										cout
												<< " asking to get the details of VDC with id "
												<< id << endl;
										//cout <<" user status : "<< user.GetRequestById(id)->GetStatus() << endl;
										Request * vdc = user.GetRequestById(id);
										pthread_mutex_t mutex =
												PTHREAD_MUTEX_INITIALIZER;
										pthread_mutex_lock(&mutex);
										//for testing-----------cout<< " DISPLAY REQUEST @ BEG OF DETAILS VDC"<< endl;
										//user.GetRequestById(0)->DisplayRequest();
										//vdc->DisplayRequest();
										vdc->CalculateStatus();
										sprintf(response, "%s&%d&%s&%d&%d",
												"VDC_FOUND",
												vdc->GetRequestNumber(),
												vdc->GetUserName().c_str(),
												vdc->GetNumberNodes(),
												vdc->GetLinks()->size());
										std::list<Priority_group>::iterator it2;
										std::list<Virtual_node>::iterator it3;
										std::list<Path>::iterator it4;
										for (it2 = vdc->GetGroups()->begin();
												it2 != vdc->GetGroups()->end();
												it2++) {
											for (it3 = it2->GetNodes()->begin();
													it3
															!= it2->GetNodes()->end();
													it3++) {
												sprintf(response,
														"%s&%d&%d&%s&%s&%s&%s",
														response, it3->GetId(),
														it3->GetFlavor(),
														it3->GetStatus().c_str(),
														it3->GetEmbeddingNode_name().c_str(),
														it3->GetPrivateIp().c_str(),
														it3->GetPublicIp().c_str());
											}
										}

										for (it4 = vdc->GetLinks()->begin();
												it4 != vdc->GetLinks()->end();
												it4++) {
											int id = it4->GetId();
											int from_id =
													it4->GetSourceNode_id();
											int to_id =
													it4->GetDestinationNode_id();
											double bw = it4->GetBandwidth();
											//for testing -----printf("%d  %d  %d  %lf\n",id,from_id,to_id,bw);
											sprintf(response,
													"%s&%d&%d&%d&%lf&%s",
													response, id, from_id,
													to_id, bw,
													it4->GetStatus().c_str());
										}
										sprintf(response, "%s&%d&%d&%d&%lf&",
												response);
										//printf("%s\n", response);
										pthread_mutex_unlock(&mutex);

										send(csock, response, sizeof(response),
												0);
									} else {
										if (!strcmp(action, "subdetails")) {
											cout
													<< " asking to get the details of substrate network "
													<< endl;
											std::list<Sub_node_type>::iterator it1;
											std::list<Substrate_node>::iterator it2;
											std::list<Substrate_link>::iterator it3;
											pthread_mutex_t mutex =
													PTHREAD_MUTEX_INITIALIZER;
											pthread_mutex_lock(&mutex);
											sprintf(response2, "%d&%d&%d",
													subNetwork.GetId(),
													subNetwork.GetNumberNodes(),
													subNetwork.GetSubstrateLinks()->size());
											for (it1 =
													subNetwork.GetTypes()->begin();
													it1
															!= subNetwork.GetTypes()->end();
													it1++) {
												for (it2 =
														it1->GetSubstrateNodes()->begin();
														it2
																!= it1->GetSubstrateNodes()->end();
														it2++) {
													sprintf(response2,
															"%s&%s&%d&%lf&%lf&%lf&%lf&%lf&%lf&%d&%lf&%s",
															response2,
															it2->GetName().c_str(),
															it2->GetType(),
															it2->GetCpu(),
															it2->GetUsedCpu(),
															it2->GetMemory(),
															it2->GetUsedMemory(),
															it2->GetDisk(),
															it2->GetUsedDisk(),
															it2->GetEmbeddedNodes()->size(),
															it2->GetAvailability(),
															"State_node");
												}
											}
											for (it3 =
													subNetwork.GetSubstrateLinks()->begin();
													it3
															!= subNetwork.GetSubstrateLinks()->end();
													it3++) {
												sprintf(response2,
														"%s&%d&%s&%s&%lf&%lf",
														response2, it3->GetId(),
														it3->GetSourceNode()->GetName().c_str(),
														it3->GetDestinationNode()->GetName().c_str(),
														it3->GetBandwidth(),
														it3->GetUsedBandwidth());

											}
											pthread_mutex_unlock(&mutex);
											sprintf(response2, "%s&",
													response2);
											printf("%s\n", response2);
											send(csock, response2,
													sizeof(response2), 0);
										} else {
											if (!strcmp(action,
													"lastvdcdetails")) {
												printf("*** here *****\n");
												//cout <<" user status : "<< user.GetRequestById(id)->GetStatus() << endl;
												Request * vdc =
														user.GetRequestById(
																user.GetRequests()->size()
																		- 1);
												pthread_mutex_t mutex =
																		PTHREAD_MUTEX_INITIALIZER;
												pthread_mutex_lock(&mutex);
												//for testing-----------cout<< " DISPLAY REQUEST @ BEG OF DETAILS VDC"<< endl;
												//user.GetRequestById(0)->DisplayRequest();
												//vdc->DisplayRequest();
												//vdc->CalculateStatus();
												sprintf(response, "%s&%d&%d&%d",
														"VDC_FOUND",
														vdc->GetRequestNumber(),
														vdc->GetNumberNodes(),
														vdc->GetLinks()->size());
												std::list<Priority_group>::iterator it2;
												std::list<Virtual_node>::iterator it3;
												std::list<Path>::iterator it4;
												for (it2 =
														vdc->GetGroups()->begin();
														it2
																!= vdc->GetGroups()->end();
														it2++) {
													for (it3 =
															it2->GetNodes()->begin();
															it3
																	!= it2->GetNodes()->end();
															it3++) {
														sprintf(response,
																"%s&%d&%d&%s&%s&%s&%s",
																response,
																it3->GetId(),
																it3->GetFlavor(),
																it3->GetStatus().c_str(),
																it3->GetEmbeddingNode_name().c_str(),
																it3->GetPrivateIp().c_str(),
																it3->GetPublicIp().c_str());
													}
												}

												for (it4 =
														vdc->GetLinks()->begin();
														it4
																!= vdc->GetLinks()->end();
														it4++) {
													int id = it4->GetId();
													int from_id =
															it4->GetSourceNode_id();
													int to_id =
															it4->GetDestinationNode_id();
													double bw =
															it4->GetBandwidth();
													//for testing -----printf("%d  %d  %d  %lf\n",id,from_id,to_id,bw);
													sprintf(response,
															"%s&%d&%d&%d&%lf&%s",
															response, id,
															from_id, to_id, bw,
															it4->GetStatus().c_str());
												}
												sprintf(response,
														"%s&%d&%d&%d&%lf&",
														response);
												//printf("%s\n", response);
												pthread_mutex_unlock(&mutex);

												send(csock, response,
														sizeof(response), 0);
											} else {
												cout << "??";
											}
										}
									}
								}

							}
						}

					} else
						perror("listen");
				}
			} else {
				perror("bind");
			}

			// Closing the client socket
			cout << "Closing the client socket\n";
			closesocket(csock);
			cout << "Closing the server socket\n";
			closesocket(sock);

		} else
			perror("socket");

#if defined (WIN32)
		WSACleanup();
#endif
	}
	return fgetc(stdin);

}
