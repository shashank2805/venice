#include "ConfigLoader.h"

ConfigLoader::ConfigLoader(void) {
}

ConfigLoader::~ConfigLoader(void) {
}

void ConfigLoader::Load(Config &config) {

	//variables
	char config_filename[200];
	FILE * config_fp;
	char label[100], value[100];
	this->inputName = "config/venice_config.dat";

	//operations
	sprintf(config_filename, (this->inputName).c_str());
	config_fp = fopen(config_filename, "r");

	fscanf(config_fp, "%s %s\n", &label, &value);
	config.key_pair = value;
	fscanf(config_fp, "%s %s\n", &label, &value);
	config.image_id = value;
	fscanf(config_fp, "%s %s\n", &label, &value);
	config.topology_file = value;
	fscanf(config_fp, "%s %s\n", &label, &value);
	config.vdc_planner_address = value;
	fscanf(config_fp, "%s %s\n", &label, &value);
	config.portal_address = value;
	//Database information
	fscanf(config_fp, "%s %s\n", &label, &value);
	config.mysql_server_address = value;
	fscanf(config_fp, "%s %s\n", &label, &value);
	config.mysql_server_port = atoi(value);
	fscanf(config_fp, "%s %s\n", &label, &value);
	config.mysql_server_user = value;
	fscanf(config_fp, "%s %s\n", &label, &value);
	if(strncmp("null", value, 4) == 0) config.mysql_server_password = ""; //null => ""
	else config.mysql_server_password = value;
	fscanf(config_fp, "%s %s\n", &label, &value);
	config.mysql_database_name = value;

	config.Display();

	fclose (config_fp);
}

