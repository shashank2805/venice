//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : SAVI/VDCPlanner
//  @ File Name : Node.cpp
//  @ Date : 17/04/2013
//  @ Author : Maissa Jabri
//
//


#include "Node.hpp"

int Node::GetType() {
	return this->type;
}

void Node::SetType(int NodeType) {
	this->type=NodeType;
}

double Node::GetMemory() {
	return this->memory;
}

void Node::SetMemory(double memory) {
	this->memory=memory;
}

double Node::GetCpu() {
	return this->cpu;
}

void Node::SetCpu(double cpu) {
	this->cpu=cpu;
}

double Node::GetDisk(){
	return this->disk;
}

void Node::SetDisk(double disk){
	this->disk=disk;
}

