#include "ConfigLoader.h"

ConfigLoader::ConfigLoader(void) {
}

ConfigLoader::~ConfigLoader(void) {
}

void ConfigLoader::Load() {

	//variables
	char config_filename[200];
	FILE * config_fp;
	char label[100], value[100];
	this->inputName = "config/venice_config.dat";

	//operations
	sprintf(config_filename, (this->inputName).c_str());
	config_fp = fopen(config_filename, "r");
	Config config;
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
	config.Display();

	fclose (config_fp);
}

