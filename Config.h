/*
 * Config.h
 *
 *  Created on: 2013-07-27
 *      Author: mjabri
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
class Config {
public:
	Config();
	virtual ~Config();
	void Display();
	static string key_pair;
	static string image_id;
	static string topology_file;
	static string vdc_planner_address;
	static string portal_address;
	// database related information
	static string mysql_server_address;
	static int mysql_server_port;
	static string mysql_server_user;
	static string mysql_server_password;
	static string mysql_database_name;
};

#endif /* CONFIG_H_ */
