/*
 * Partitioner.cpp
 *
 *  Created on: 25-03-2013
 *      Author: marcin
 */

#include "Partitioner.h"

Partitioner::Partitioner() {
	lastNetworkNode = 0;
	nodes.resize(1);
}

void Partitioner::PartitionMolecules(unordered_set<KEY_ID> *_molecules) {
	nodes.resize(diplo::nbOfClients);
	size_t networkNode;
	for (unordered_set<KEY_ID>::iterator it_tripleIDs = _molecules->begin(); it_tripleIDs != _molecules->end(); it_tripleIDs++) {
		networkNode = getNextNetworkNode();
		nodes[networkNode].push_back(*it_tripleIDs);

		for (int i = 1; i < diplo::PartitionerRange; i++) {
			it_tripleIDs++;
			if ( it_tripleIDs == _molecules->end() ) return;
			nodes[networkNode].push_back(*it_tripleIDs);
		}
	}
}

size_t Partitioner::getNextNetworkNode() {
	size_t n = lastNetworkNode;
	if ( lastNetworkNode < nodes.size()-1 ) lastNetworkNode++;
	else lastNetworkNode = 0;
	return n;
}

void Partitioner::Display() {
	for (size_t i = 0; i < nodes.size(); i++) {
		cout << "node: " << i << "\t number of molecules: " << nodes[i].size() << endl;
	}
}

void Partitioner::SendMolecules() {
//	for (size_t i = 0; i < nodes.size(); i++) {
//		size_t size = nodes[i].size();
//		diplo::SRV.Send((char*) &size,  sizeof(size), i);
//		diplo::SRV.Send((char*) nodes[i].data(), size*sizeof(KEY_ID), i);
//	}
	Display();
	for (size_t i = 0; i < nodes.size(); i++) {
		size_t size = nodes[i].size();
		fstream f;
		try {
			f.open( diplo::dbDir+"node" + to_string(i), ios::binary | ios::out);
			f.write((char*) &size,  sizeof(size));
			f.write((char*) nodes[i].data(), nodes[i].size()*sizeof(KEY_ID));
			f.close();

		} catch (fstream::failure &e) {
			cerr << "Exception writing file";
		}
	}
}

Partitioner::~Partitioner() {
	// TODO Auto-generated destructor stub
}


