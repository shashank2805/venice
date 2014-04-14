#pragma once
#include "DataLoader.h"
#include "Request.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#if defined (WIN32)
#include <windows.h>
#import <msxml6.dll> 
#elif defined (linux)
//some linux lib here
#endif

class RequestLoader :
	public DataLoader
{
public:
	RequestLoader(void);
	RequestLoader(char*);
	~RequestLoader(void);
	void Load(Request*,int,int,char*);
private:
	void LoadPoisson(Request*,int);
	void LoadGoogle(Request*,int);
	void LoadWEB(Request*,int,char*);
	void LoadXML(Request*,int);
};

