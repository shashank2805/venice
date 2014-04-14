#include "RequestLoader.h"

RequestLoader::RequestLoader(void) {
}

RequestLoader::~RequestLoader(void) {
}

RequestLoader::RequestLoader(char* ressource) {
	this->inputName = ressource;
}

void RequestLoader::Load(Request* req, int reqType, int reqNumber,
		char * request) {
	//for testing-----------printf("%d \n", reqType);

	if (reqType == POISSON) //POISSON==0
	{
		//strcpy(this->inputName, "req2");
		this->inputName="req2";
		LoadPoisson(req, reqNumber);
	} else if (reqType == 1) //(DataType==GOOGLE)
			{
		//strcpy(this->inputName, "req2");
		this->inputName="req2";
		LoadGoogle(req, reqNumber);
	} else if (reqType == 2) //(DataType==WEB)
			{
		LoadWEB(req, reqNumber, request);
	} else if (reqType == 3) //(DataType==XML)
			{
		//strcpy(this->inputName, "reqXml");
		this->inputName="reqXml";
		LoadXML(req, reqNumber);
	}
}

void RequestLoader::LoadPoisson(Request* req, int reqNumber) {

	//variables
	int i, j, id, nbNodes, nbLinks, type, from, to, split, topo, nbGrps, grp,
			step;
	char filename[200];
	double bw, cpu, mem, arrTime, duration, avai;
	FILE * fp;
	list<Virtual_node>::iterator it1;
	list<Virtual_node>::iterator it2;

	//operations
	req->SetRequestNumber(reqNumber);
	req->SetRevenue(0);
	sprintf(filename, "%s/req%d.dat", (this->inputName).c_str(), reqNumber);
	fp = fopen(filename, "r");
	fscanf(fp, "%d %d %d %lf %lf %d %lf %d\n", &nbNodes, &nbLinks, &split,
			&arrTime, &duration, &topo, &avai, &nbGrps);
	//for testing-----------printf("%d %d %d %lf %lf %d %lf %d\n", nbNodes,nbLinks,split,arrTime, duration, topo, avai,nbGrps);
	req->SetArrTime(arrTime);
	req->SetDuration(duration);
	req->SetTopo(topo);
	req->SetAvailability(avai);
	req->SetSplit(LINK_UNSPLITTABLE);
	req->SetNumberNodes(nbNodes);

	step = nbNodes / nbGrps;
	for (j = 1; j < nbGrps; j++) {
		Priority_group pr_grp(j);
		for (i = 0; i < step; i++) {
			fscanf(fp, "%d %d %lf %lf %d %d\n", &id, &cpu, &mem, &type, &grp);
			bw = 0;
			Virtual_node node(id, cpu, mem, type, bw);
			pr_grp.GetNodes()->push_back(node);
			req->IncrementRevenue(0.002315 * cpu * 4 + mem * 8 * 0.00335);
		}
		req->GetGroups()->push_back(pr_grp);
	}
	Priority_group pr_grp(j);
	for (i = 0; i < step; i++) {
		fscanf(fp, "%d %d %lf %lf %d %d\n", &id, &cpu, &mem, &type, &grp);
		bw = 0;
		Virtual_node node(id, cpu, mem, type, bw);
		pr_grp.GetNodes()->push_back(node);
		req->IncrementRevenue(0.002315 * cpu * 4 + mem * 8 * 0.00335);
	}
	req->GetGroups()->push_back(pr_grp);
	for (j = 0; j < nbLinks; j++) {
		fscanf(fp, "%d %d %d %lf\n", &id, &from, &to, &bw);
		//for testing-----------printf("%d %d %d %lf\n",id, from, to, bw);
		int f = from % step;
		int tf = (from / step) + 1;
		int t = to % step;
		int tt = (to / step) + 1;
		it1 = req->GetGroupById(tf)->GetNodes()->begin();
		std::advance(it1, f);
		it2 = req->GetGroupById(tt)->GetNodes()->begin();
		std::advance(it2, t);
		Path link(j, &(*it1), &(*it2), (double) bw / 100);
		req->GetLinks()->push_back(link);
		req->IncrementRevenue(bw / 100);

	}

	req->SetRevenue((req->GetRevenue() * req->GetDuration()) / (double) 60); //assuming the price is 1
	fclose(fp);
}

void RequestLoader::LoadGoogle(Request* req, int reqNumber) {

}

void RequestLoader::LoadWEB(Request* req, int reqNumber, char * request) {

	//variables
	int i, j, id, nbNodes, nbLinks, type, from, to, split, topo, nbGrps, grp,
			step, flavor;
	double bw, cpu, mem, disk, arrTime, duration, avai;
	ostringstream vdc_id;
	char * pch;
	char * session;
	char * user_name;
	char * application;
	list<Virtual_node>::iterator it1;
	list<Virtual_node>::iterator it2;

	//for testing-----------printf("%s \n",request);
	//operations
	req->SetRequestNumber(reqNumber);
	vdc_id << "VDC_" << reqNumber << "_";
	req->SetName(vdc_id.str());
	req->SetRevenue(0);
	pch = strtok(request, "&,"); //action = pch;
	pch = strtok(NULL, "&,");
	session = pch;
	pch = strtok(NULL, "&,");
	user_name = pch;
	req->SetUserName(user_name);
	//for testing--------cout<<"Session id = "<<session<<"\n";
	pch = strtok(NULL, "&,");
	nbNodes = atoi(pch);
	req->SetNumberNodes(nbNodes);
	pch = strtok(NULL, "&,");
	nbLinks = atoi(pch);
	pch = strtok(NULL, "&,");
	nbGrps = atoi(pch);
	pch = strtok(NULL, "&,");
	avai = atof(pch);
	req->SetAvailability(avai);
	pch = strtok(NULL, "&,");
	split = atoi(pch);
	req->SetSplit(LINK_UNSPLITTABLE);
	pch = strtok(NULL, "&,");
	arrTime = atof(pch);
	req->SetArrTime(arrTime);
	pch = strtok(NULL, "&,");
	duration = atof(pch);
	req->SetDuration(duration);
	pch = strtok(NULL, "&,");
	topo = atoi(pch);
	req->SetTopo(topo);
	pch = strtok(NULL, "&,");
	application = pch;
	req->SetApplication(application);
	req->SetStatus(STATE_RECEIVED);
	time_t now = time(0);
	req->SetArrTime(now);

	//for testing-----------
	printf("hereeeeeeuuu   %d %d %d %lf %lf %d %lf %d\n", nbNodes, nbLinks,
			split, arrTime, duration, topo, avai, nbGrps);
	step = nbNodes / nbGrps;

	for (j = 1; j < nbGrps; j++) {
		Priority_group pr_grp(j);
		for (i = 0; i < step; i++) {
			pch = strtok(NULL, "&,");
			id = atoi(pch);
			pch = strtok(NULL, "&,");
			flavor = atoi(pch);
			pch = strtok(NULL, "&,");
			cpu = atof(pch);
			pch = strtok(NULL, "&,");
			mem = atof(pch);
			pch = strtok(NULL, "&,");
			disk = atof(pch);
			pch = strtok(NULL, "&,");
			type = atoi(pch);
			pch = strtok(NULL, "&,");
			grp = atoi(pch);
			//for testing-----------printf("%d %lf %lf %d %d %d\n", id, cpu, mem, disk, type, grp,flavor);

			bw = 0;
			ostringstream vm_id;
			vm_id << "VDC_" << reqNumber << "_VM" << id;
			Virtual_node node(id, vm_id.str(), cpu, mem, disk, type, bw,
					flavor);
			pr_grp.GetNodes()->push_back(node);
			req->IncrementRevenue(0.002315 * cpu * 4 + mem * 8 * 0.00335);

		}
		req->GetGroups()->push_back(pr_grp);
	}

	Priority_group pr_grp(j);
	for (i = 0; i < step; i++) {
		pch = strtok(NULL, "&,");
		id = atoi(pch);
		pch = strtok(NULL, "&,");
		flavor = atoi(pch);
		pch = strtok(NULL, "&,");
		cpu = atof(pch);
		pch = strtok(NULL, "&,");
		mem = atof(pch);
		pch = strtok(NULL, "&,");
		disk = atof(pch);
		pch = strtok(NULL, "&,");
		type = atoi(pch);
		pch = strtok(NULL, "&,");
		grp = atoi(pch);
		//for testing-----------printf("%d %lf %lf %d %d %d\n", id, cpu, mem, disk, type, grp, flavor);

		bw = 0;
		ostringstream vm_id;
		vm_id << "VDC_" << reqNumber << "_VM" << id;
		Virtual_node node(id, vm_id.str(), cpu, mem, disk, type, bw, flavor);
		pr_grp.GetNodes()->push_back(node);
		req->IncrementRevenue(0.002315 * cpu * 4 + mem * 8 * 0.00335);

	}
	req->GetGroups()->push_back(pr_grp);

	for (j = 0; j < nbLinks; j++) {
		pch = strtok(NULL, "&,");
		id = atoi(pch);
		pch = strtok(NULL, "&,");
		from = atoi(pch);
		pch = strtok(NULL, "&,");
		to = atoi(pch);
		pch = strtok(NULL, "&,");
		bw = atof(pch);
		//for testing-----------printf("%d %d %d %lf\n", id, from, to, bw);

		int f = from % step;
		int tf = (from / step) + 1;
		int t = to % step;
		int tt = (to / step) + 1;
		it1 = req->GetGroupById(tf)->GetNodes()->begin();
		std::advance(it1, f);
		it2 = req->GetGroupById(tt)->GetNodes()->begin();
		std::advance(it2, t);
		ostringstream path_name;
		path_name << "VDC_" << req->GetRequestNumber() << "_VL" << id;
		Path link(id, path_name.str(), from, &(*it1), to, &(*it2),
				(double) bw / 1000);
		req->GetLinks()->push_back(link);
		req->IncrementRevenue(bw / 100);

	}

	req->SetRevenue((req->GetRevenue() * req->GetDuration()) / (double) 60); //assuming the price is 1
	//for testing----------

}

//this function has to be rewritten it works for windows but not for linux

void RequestLoader::LoadXML(Request* req, int reqNumber) {
	/*
	 char filename[200];

	 HRESULT hr = CoInitialize(NULL);
	 if (SUCCEEDED(hr))
	 {
	 try
	 {
	 MSXML2::IXMLDOMDocument2Ptr xmlDoc;
	 hr = xmlDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	 // TODO: if (FAILED(hr))...

	 sprintf(filename, "%s/req%d.xml", this->inputName, reqNumber);
	 if (xmlDoc->load(filename) != VARIANT_TRUE)
	 {
	 printf("Unable to load input.xml\n");
	 }
	 else
	 {
	 printf("XML was successfully loaded\n");

	 xmlDoc->setProperty("SelectionLanguage", "XPath");
	 MSXML2::IXMLDOMNodeListPtr wheels = xmlDoc->selectNodes("/Car/Wheels");
	 printf("Car has %u wheels\n", wheels->Getlength());

	 MSXML2::IXMLDOMNodePtr node;
	 node = xmlDoc->createNode(MSXML2::NODE_ELEMENT, "Engine", "");
	 node->text = "Engine 1.0";
	 xmlDoc->documentElement->appendChild(node);
	 hr = xmlDoc->save("output.xml");
	 if (SUCCEEDED(hr))
	 printf("output.xml successfully saved\n");
	 }
	 }
	 catch (_com_error &e)
	 {
	 printf("ERROR: %ws\n", e.ErrorMessage());
	 }
	 CoUninitialize();
	 }
	 */
}

