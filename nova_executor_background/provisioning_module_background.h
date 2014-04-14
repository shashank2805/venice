#ifndef _PROVISIONING_MODULE_BACK_H_
#define _PROVISIONING_MODULE_BACK_H_
#include "command_executor.h"
#include <map>

struct neighbor
{
        string neighbor_ip;
        string bandwidth;
        string link_id;
};

class ProvisioningModule
{	
	CommandExecutor cmdExec;
	
	void parseVDCStatus(string vdc_status, map<string, string> &vm_ip_map);
        void parseLink(string input, string &src, string &dst, string &bandwidth, string &linkId);
        void generateFileAndExecute(map<string, vector<neighbor> > &link_map);
	string sendStatusVDC(string vdc_name);

	public:
		void provisionLinks(string vdc_name, vector<string> links, string application);

};
#endif /*_PROVISIONING_MODULE_BACK_H_*/
