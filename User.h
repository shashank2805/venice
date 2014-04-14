/*
 * User.h
 *
 *  Created on: 8 juin 2013
 *      Author: maissa jabri
 */

#ifndef USER_H_
#define USER_H_
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "Request.hpp"

class User {
public:
	User();
	User(int);
	User(int,std::string);
	void SetId(int);
	int GetId();
	void SetSession(std::string);
	std::string GetSession();
	class Request * GetRequestById(int);
	Request * GetRequestByIndex (int);
	std::list<Request>* GetRequests();
	std::list<Request>* GetWaitingRequests();
	void RemoveRequest(int);
	void RemoveWaitingRequest(int);
	//std::list<Request*> * GetRequests();
	virtual ~User();

private:
	int id;
	std::string session;
	//std::list<Request *> requests;
	std::list<Request> requests;
	std::list<Request> waiting_requests;
};

#endif /* USER_H_ */
