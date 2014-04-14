#pragma once
#include <iostream> 
#include "DataLoader.h"
#include "Config.h"
class ConfigLoader: public DataLoader {
public:
	ConfigLoader(void);
	~ConfigLoader(void);
	void Load(Config &config);
};

