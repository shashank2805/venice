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
};

#endif /* CONFIG_H_ */
