/*
 * Config.cpp
 *
 *  Created on: 2013-07-27
 *      Author: mjabri
 */

#include "Config.h"

Config::Config() {
	// TODO Auto-generated constructor stub

}

Config::~Config() {
	// TODO Auto-generated destructor stub
}

void Config::Display() {
	// TODO Auto-generated constructor stub
	cout<<"********* Configuration file *********"<<endl;
	cout<<"key_pair : "<<this->key_pair<<endl;
	cout<<"image_id : "<<this->image_id<<endl;
	cout<<"topology_file : "<<this->topology_file<<endl;
	cout<<"vdc_planner_address : "<<this->vdc_planner_address<<endl;
	cout<<"portal_address : "<<this->portal_address<<endl;
}
