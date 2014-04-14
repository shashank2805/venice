#include "provisioning_module_background.h"
#include <string>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

int main(int argc, char*argv[])
{
	ProvisioningModule provMod;

	//provMod.createVM("FromCPP1", "cn217", 1, "f927d3ce-8609-48c1-95ef-76b4fc17d89c");
	//sleep(30);
	//provMod.createVM("FromCPP2", "cn115", 1, "f927d3ce-8609-48c1-95ef-76b4fc17d89c", "mykey");
	//sleep(30);
	//provMod.deleteVM("FromCPP1");
	//sleep(30);
	//provMod.migrateVM("FromCPP2", "cn219");
	
	string vdc_name;
	vdc_name.append(argv[1]);
	cout << vdc_name << endl;
	string application;
	application.append(argv[2]);
	cout << application << endl;
	//cout << argv[2] << endl;
	//cout << argv[3] << endl;
	vector<string> links;
	for(int i = 0; i < argc-3; i++)
		 links.push_back(argv[i+3]);
	sleep(120);
	provMod.provisionLinks(vdc_name, links, application);	


	return 0;
}
