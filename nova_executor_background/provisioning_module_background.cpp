#include "provisioning_module_background.h"
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

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
		//cout << "eachpart " ;
		//cout << eachPart << endl;
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
		//sleep(300);
		this->cmdExec.executeScript("./provisionLinks.sh &");
	}
	else
		cout << "Aggregated command file cannot be opened" << endl;
}

void ProvisioningModule::provisionLinks(string vdc_name, vector<string> links, string application)
{
	cout << "PM: PL: VDC_Name: " << vdc_name << endl;
	string vdc_status = this->sendStatusVDC(vdc_name);
	cout << vdc_status << endl;

	map<string, string> vm_ip_map;

	parseVDCStatus(vdc_status, vm_ip_map);

	//testing the map

	for(map<string, string>::iterator map_it = vm_ip_map.begin();
		map_it != vm_ip_map.end(); map_it ++)
		cout << map_it->first << " " << map_it->second << endl;
	//cout << vm_ip_map.find("VDC_0_VM0")->second << endl;
	
	//end testing map

	map<string, vector<neighbor> > link_map;

	for(vector<string>::iterator links_it = links.begin(); 
		links_it != links.end(); links_it++)
	{
		string src, dst, bandwidth, linkId;
		parseLink(*links_it, src, dst, bandwidth, linkId);
		cout << "parsing links: " << src << " " << dst << endl;				
		
		string src_ip = vm_ip_map.find(src)->second;
		string dst_ip = vm_ip_map.find(dst)->second;

		cout << "IPs: " << src_ip << " " << dst_ip << endl;

		neighbor new_neighbor;
		new_neighbor.neighbor_ip = dst_ip;
		new_neighbor.bandwidth = bandwidth;
		new_neighbor.link_id = linkId;

		map<string, vector<neighbor> >::iterator links_it = link_map.find(src_ip);

		if(links_it == link_map.end())
		{
			vector<neighbor> neighbor_list;
			neighbor_list.push_back(new_neighbor);
			link_map.insert(make_pair(src_ip, neighbor_list));
		}
		else
		{
			//cout << "Already the src ip " << endl;
			links_it->second.push_back(new_neighbor);
		}
	}


	for(map<string, vector<neighbor> >::iterator links_it = link_map.begin(); 
			links_it != link_map.end(); links_it ++)
	{
		for(vector<neighbor>::iterator neighbor_it = links_it->second.begin();
			neighbor_it != links_it->second.end(); neighbor_it++)
		{
			cout << links_it->first << " " << neighbor_it->neighbor_ip << " " 
				<< neighbor_it->bandwidth << " " << neighbor_it->link_id << endl; 
			
			
		} 
	}
	
	cout << "Provision links later" << endl;

	if(application.compare(0, 4, "None") == 0)
		return;
	else if(application.compare(0, 9, "MapReduce") == 0)
	{
		cout << "Provisioning MapReduce Cluster " << application << endl;
		ofstream nameIPMapMRFile("nameIPMapMR");
		if(nameIPMapMRFile.is_open())
		{
			cout << "Generating the ip map file" << endl;
			for(map<string, string>::iterator map_it = vm_ip_map.begin();
				map_it != vm_ip_map.end(); map_it ++)
			{
				string machine_name = map_it->first;
				replace(machine_name.begin(), machine_name.end(), '_', '-');

				nameIPMapMRFile << machine_name << " " << map_it->second << endl;
			}
			
			nameIPMapMRFile.close();
			
			cout << "Before executing hadoop cluster" << endl;

			this->cmdExec.executeScript("./provision_MR.sh");

			cout << "After executing hadoop cluster" << endl;
		}
		else
			cout << "nameIPMapMR File cannot be opened" << endl;
		
		return;
	}

	generateFileAndExecute(link_map);
}

string ProvisioningModule::sendStatusVDC(string vdc_name)
{

	/*
	input: vdc_name
	output: a list of {vm_name;status;task_state;power_state;private_ip,public_ip}
	Example command for vdc_name=FromCPP
	Shell Command:
		nova list|grep 'FromCPP'|awk '{name[NR]=$4; status[NR]=$6; task_state[NR]=$8; power_state[NR]=$10; split($12, temp, "=|,"); private_network[NR]=temp[2]; public_network[NR]=$13}END{for(i=1; i<=NR; i++){output[i]= name[i]";"status[i]";"task_state[i]";"power_state[i]";"private_network[i]; {if(public_network[i]!="|") output[i]=output[i]","public_network[i];}};{for(i = 1; i <= NR; i++) print output[i]}}'
	output:	
		FromCPP1;ACTIVE;None;Running;172.16.100.17
		FromCPP2;ACTIVE;None;Running;172.16.100.19
	*/
	ostringstream command;

	command << "nova list|grep '" << vdc_name << "'|" 
		<< "awk '{"
			<< "name[NR]=$4; status[NR]=$6; task_state[NR]=$8; power_state[NR]=$10; split($12, temp, \"=|,\"); private_network[NR]=temp[2]; public_network[NR]=$13}"
			<< "END{" 
				<< "for(i=1; i<=NR; i++)"
				<< "{"
					<< "output[i]= name[i]\";\"status[i]\";\"task_state[i]\";\"power_state[i]\";\"private_network[i];"
					<< "{"
						<<"if(public_network[i]!=\"|\")"
							<< "output[i]=output[i]\",\"public_network[i];"
					<< "}"
				<< "};"
				<< "{for(i = 1; i <= NR; i++) print output[i]}"
			<< "}'";

	return this->cmdExec.execute(command.str());
}

