#ifndef _PROVISIONING_MODULE_H_
#define _PROVISIONING_MODULE_H_
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

	public:
		void createVM(string vm_name, string targetNode, int flavor_id, string image, string key_name="mykey");
		void deleteVM(string vm_name);
		void provisionLinks(string vdc_name, vector<string> links,string application);
		void migrateVM(string vm_name, string targetNode);
		string sendStatusVM(string vm_name);	
		string sendStatusVDC(string vdc_name);	
		string sendStatusPMs();
		string sendLinkStatus(string srcIP, string dstIP, string linkID);

};
#endif /*_PROVISIONING_MODULE_H_*/
