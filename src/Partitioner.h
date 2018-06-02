/*
 * Partitioner.h
 *
 *  Created on: 25-03-2013
 *      Author: marcin
 */

#include	"diplodocus.h"
#include	"Server.h"
#ifndef PARTITIONER_H_
#define PARTITIONER_H_



class Partitioner {
	vector< vector<KEY_ID> > nodes;
	size_t lastNetworkNode;
	size_t getNextNetworkNode();
public:
	void PartitionMolecules(unordered_set<KEY_ID> *_molecules);
	void SendMolecules();
	void Display();
	Partitioner();
	virtual ~Partitioner();
};

namespace diplo {
	extern class Partitioner P;
}
#endif /* PARTITIONER_H_ */
