/*
 * User.cpp
 *
 *  Created on: 8 juin 2013
 *      Author: root
 */

#include "User.h"

User::User() {
	// TODO Auto-generated constructor stub
	std::cout << "New User created";
}

User::User(int id) {
	// TODO Auto-generated constructor stub
	this->id = id;
	//for testing -----------
	std::cout << "New User created with id = " << id << " \n";
}

User::User(int id, std::string session) {
	// TODO Auto-generated constructor stub
	this->id = id;
	this->session = session;
	//for testing -----------std::cout<<"New User created with id = "<<id<<" and session = "<<session;
}

void User::SetId(int id) {
	// TODO Auto-generated constructor stub
	this->id = id;
}

int User::GetId() {
	// TODO Auto-generated constructor stub
	return this->id;
}

void User::SetSession(std::string session) {
	// TODO Auto-generated constructor stub
	this->session = session;
}

std::string User::GetSession() {
	// TODO Auto-generated constructor stub
	return this->session;
}

Request * User::GetRequestById(int reqNumber) {

	std::list<Request>::iterator it1;
	bool found;
	it1 = (this->requests).begin();
	while (!found && it1 != (this->requests).end()) {
		if (reqNumber == it1->GetRequestNumber()) {
			found = true;
		} else {
			it1++;
		}
	}
	if (found)
		return &(*it1);
	else
		return NULL;
}

Request * User::GetRequestByIndex(int index) {

	std::list<Request>::iterator it1;
	it1 = (this->requests).begin();
	std::advance(it1, index);
	return &(*it1);
}

std::list<Request>* User::GetRequests() {
	return &(this->requests);
}

std::list<Request>* User::GetWaitingRequests() {
	return &(this->waiting_requests);
}

void User::RemoveRequest(int reqNumber) {
	std::list<Request>::iterator it1;
	bool found=false;
	it1 = (this->requests).begin();
	while (!found && it1 != (this->requests).end()) {
		if (reqNumber == it1->GetRequestNumber()) {
			found = true;
			(this->requests).erase(it1);
		} else {
			it1++;
		}
	}
}

void User::RemoveWaitingRequest(int reqNumber) {
	std::list<Request>::iterator it1;
	bool found=false;
	it1 = (this->waiting_requests).begin();
	while (!found && it1 != (this->waiting_requests).end()) {
		if (reqNumber == it1->GetRequestNumber()) {
					found = true;
					(this->waiting_requests).erase(it1);
				}
else
{
				it1++;
			}
		}

}

User::~User() {
// TODO Auto-generated destructor stub
}

