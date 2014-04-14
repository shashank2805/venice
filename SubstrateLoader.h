#pragma once
#include <iostream> 
#include "DataLoader.h"
#include "Substrate_network.hpp"
#include "Substrate_node.hpp"
#include "Substrate_link.hpp"
#include "Config.h"
class SubstrateLoader :
	public DataLoader
{
public:
	SubstrateLoader(void);
	~SubstrateLoader(void);
	void Load(Substrate_network *);
};

