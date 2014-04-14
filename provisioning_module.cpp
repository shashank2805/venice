#include "provisioning_module.h"
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

//Changed
void ProvisioningModule::createVM(string vm_name, string targetNode, int flavor_id, string image, string key_name)
{
	ostringstream command;
	command << "nova boot --image " << image << " --flavor " << flavor_id 
		<< " --key_name " << key_name 
		<< " --hint force_hosts=" << targetNode
		<< " " << vm_name << endl;

	this->cmdExec.execute(command.str());
}
//Changed
void ProvisioningModule::deleteVM(string vm_name)
{
	ostringstream command;
	
	command << "nova delete " << vm_name << endl;

	this->cmdExec.execute(command.str());
}
/*
void ProvisioningModule::parseVDCStatus(string vdc_status, map<string, string> &vm_ip_map)
{
	stringstream tokenizer(vdc_status);
	string line;

	while(getline(tokenizer, line, '\n'))//parsing each line
	{
		//cout << "*******each line:" << line << endl;
		stringstream lineTokenizer(line);
		string eachPart;
		string vm_name;
		string vm_ip;
		int count = 0;
		while(getline(lineTokenizer, eachPart, ';'))//parsing each field delimitted by semicolon
		{
			switch(count)
			{
				case 0:
					vm_name = eachPart;	
					break;
				case 4:
					stringstream ipTokenizer(eachPart);

					while(getline(ipTokenizer, vm_ip, ','))//parsing ip field
					{
						break;
					}
					
					break;
			}

			count ++;
		}
		//cout << vm_name << " " << vm_ip << endl;
		vm_ip_map.insert(make_pair(vm_name, vm_ip));
	}
	
}

void ProvisioningModule::parseLink(string input, string &src, string &dst, string &bandwidth, string &linkId)
{

	stringstream tokenizer(input);
	string eachPart;
	int count = 0;
	while(getline(tokenizer, eachPart, ';'))
	{
		switch(count)
		{
			case 0:
				src = eachPart;
				break;
		
			case 1:
				dst = eachPart;
				break;

			case 2:
				bandwidth = eachPart;
				break;
			
			case 3:
				linkId = eachPart;
				break;
		}
		
		count ++;
	}

}
//Not used
void ProvisioningModule::generateFileAndExecute(map<string, vector<neighbor> > &link_map)
{
	ofstream aggregatedCommandFile("provisionLinks.sh");
	if(aggregatedCommandFile.is_open())
	{
		aggregatedCommandFile << "#!/bin/sh" << endl;
		
		for(map<string, vector<neighbor> >::iterator links_it = link_map.begin();
				links_it != link_map.end(); links_it ++)
		{
			ostringstream fileName;
			fileName << "limit_from_" << links_it->first <<".sh";
			cout << fileName.str() << endl;
			
			ofstream commandFile(fileName.str().c_str());
			if(commandFile.is_open())
			{
				commandFile << "#!/bin/bash" << endl;
				commandFile << "echo start" << endl;
				commandFile << "sudo tc qdisc add dev eth0 root handle 1:0 htb default 100; #max 100 links" << endl;
				commandFile << "sudo tc class add dev eth0 parent 1:0 classid 1:1 htb rate 1000mbit; #should be the sum of all virtual links" << endl;

				int count = 2;
				for(vector<neighbor>::iterator neighbor_it = links_it->second.begin();
					neighbor_it != links_it->second.end(); neighbor_it++)
				{
					//cout << links_it->first << " " << neighbor_it->neighbor_ip << " "
					//	<< neighbor_it->bandwidth << " " << neighbor_it->link_id << endl;

					commandFile << "sudo tc class add dev eth0 parent 1:1 classid 1:"<< count << " htb rate " << neighbor_it->bandwidth
							<< "mbit ceil " << neighbor_it->bandwidth << "mbit; #" << neighbor_it->link_id << endl;

					commandFile << "sudo tc filter add dev eth0 parent 1:0 protocol ip prio 1 u32 match ip dst " << neighbor_it->neighbor_ip
							<< "/32 flowid 1:" << count << "; #" << neighbor_it->link_id << endl;
					count ++;
				}
				commandFile << "sudo tc class add dev eth0 parent 1:1 classid 1:100 htb rate 1000mbit ceil 1000mbit; #100 is the default queue" << endl;
				commandFile << "echo end" << endl;

				commandFile.close();

				ostringstream command;
				command << "chmod +x " << fileName.str() << " " << endl;
				//command << "ssh-keygen -R " << links_it->first << " 2>/dev/null" << endl;// remove the previous mapping IP to device
				//command << "scp -o 'StrictHostKeyChecking no' -i ~/mykey.pem " << fileName.str() << " ubuntu@" << links_it->first << ":~ 2>/dev/null" << endl;
				//command << "ssh -o 'StrictHostKeyChecking no' -i ~/mykey.pem ubuntu@" << links_it->first << " \"./" << fileName.str() << "\" 2>/dev/null" << endl;
				
				command << "ssh-keygen -R " << links_it->first << "" << endl;// remove the previous mapping IP to device
				command << "scp -o 'StrictHostKeyChecking no' -i ~/mykey.pem " << fileName.str() << " ubuntu@" << links_it->first << ":~ " << endl;
				command << "ssh -o 'StrictHostKeyChecking no' -i ~/mykey.pem ubuntu@" << links_it->first << " \"./" << fileName.str() << "\" " << endl;
				
		
	
				cout << command.str();
				
				aggregatedCommandFile << command.str();

			}
			else
				cout << "Unable to open file" << endl;

		}
		aggregatedCommandFile.close();
		this->cmdExec.execute("chmod +x provisionLinks.sh");
		sleep(300);
		this->cmdExec.executeScript("./provisionLinks.sh &");
	}
	else
		cout << "Aggregated command file cannot be opened" << endl;
}
*/
//Changed
void ProvisioningModule::provisionLinks(string vdc_name, vector<string> links, string application)
{
	ostringstream command;
	command << "./nova_executor_background/linkProvBackground " << "129.97.119.133" << " "  << vdc_name << " " << application << " ";

	for(vector<string>::iterator links_it = links.begin(); links_it != links.end(); links_it++)
		command << "\"" << *links_it << "\" ";
	command << "&";
	cout << command.str() << endl;

	this->cmdExec.executeScript(command.str());
}

//changed
string ProvisioningModule::sendLinkStatus(string srcIP, string dstIP, string linkID)
{
	//venice ip: needs to be changed 129.97.119.133
	ostringstream command_src; 
	command_src << "./checkLink_main.sh " << "129.97.119.133" 
		<< " " << srcIP;
	cout<<" command src : "<<command_src.str()<<endl;
	string srcResult = this->cmdExec.execute(command_src.str());
	
	ostringstream command_dst; 
	command_dst << "./checkLink_main.sh " << "129.97.119.133" 
		<< " " << dstIP;
	cout<<" command dst : "<<command_dst.str()<<endl;
	string dstResult = this->cmdExec.execute(command_dst.str());
	
	cout << "Result: " << srcResult << endl;
	cout << "Result: " << dstResult << endl;
	
	if(srcResult.compare(0, 8, "No Route") == 0 || dstResult.compare(0, 8, "No Route") == 0)
		return "Not Provisioned";
	
	else if ( srcResult.compare(0, 12, "Provisioning") == 0 || dstResult.compare(0, 12, "Provisioning") == 0 )
		return "Provisioning";
	
	else
		return "Provisioned";

}
//Not used
void ProvisioningModule::migrateVM(string vm_name, string targetNode)
{
	ostringstream command;
	
	command << "nova live-migration --block_migrate "<< vm_name << " " << targetNode << endl;
	
	this->cmdExec.execute(command.str());
}
//Changed
string ProvisioningModule::sendStatusVM(string vm_name)
{

	/*
	input: vm_name
	output: vm_name;status;private_ip
	Example command for vm_name=venice
	Shell Command:	
	nova list|grep 'venice'|awk '{name=$4; status=$6; split($8, private_networks, "=|,");}END{output= name";"status";"private_networks[2]; print output}'
	output: venice;ACTIVE;10.4.4.3
	*/
	ostringstream command;
	//cout << "PM: VM Status: " << vm_name << endl;
	command << "nova list|grep '" << vm_name << "'|" 
		<< "awk '{"
			<< "name=$4; status=$6; split($8, private_networks, \"=|,\");}"
			<< "END{ output = name\";\"status\";\"private_networks[2];" 
			<< "print output"
		<< "}'" << endl;
	//cout << "PM: Command: " << command.str() << endl;
	return this->cmdExec.execute(command.str());
}
//Changed
string ProvisioningModule::sendStatusVDC(string vdc_name)
{

        /*
        input: vdc_name
        output: a list of {vm_name;status;private_ip}
        Example command for vdc_name=venice
        Shell Command:
        nova list|grep 'temp'|awk '{name[NR]=$4; status[NR]=$6; split($8, temp, "=|,"); private_network[NR]=temp[2];}END{for(i=1; i<=NR; i++){output[i]= name[i]";"status[i]";"private_network[i];};{for(i = 1; i <= NR; i++) print output[i]}}'
        temp1;ACTIVE;10.4.4.4
        temp2;ACTIVE;10.4.4.5

        */
        ostringstream command;

        command << "nova list|grep '" << vdc_name << "'|"
                << "awk '{"
                        << "name[NR]=$4; status[NR]=$6; split($8, temp, \"=|,\"); private_network[NR]=temp[2];}"
                        << "END{"
                                << "for(i=1; i<=NR; i++)"
                                << "{"
                                        << "output[i]= name[i]\";\"status[i]\";\"private_network[i];"
                                << "};"
                                << "{for(i = 1; i <= NR; i++) print output[i]}"
                        << "}'";

        return this->cmdExec.execute(command.str());
}

