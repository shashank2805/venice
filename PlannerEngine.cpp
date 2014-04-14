/*
 * PlannerEngine.cpp
 *
 *  Created on: 2013-07-09
 *      Author: mjabri
 */

#include "PlannerEngine.h"

PlannerEngine::PlannerEngine() {
	// TODO Auto-generated constructor stub
	this->availability = ON;
	this->migration = OFF;
	this->consolidation = OFF;
	this->tracing = OFF;

}

PlannerEngine::PlannerEngine(int availability, int migration, int consolidation,
		int tracing) {
	// TODO Auto-generated constructor stub
	this->availability = availability;
	this->migration = migration;
	this->consolidation = consolidation;
	this->tracing = tracing;
}

PlannerEngine::~PlannerEngine() {
	// TODO Auto-generated destructor stub
}
