/*
 * Conductor.h
 *
 *  Created on: 19-11-2012
 *      Author: marcin
 */

#ifndef CONDUCTOR_H_
#define CONDUCTOR_H_

#include	"diplodocus.h"
#include	"KeyManager.h"
#include 	"TypesHierarchy.h"
#include	"TemplateManager.h"
#include	"Molecules.h"
#include	"Partitioner.h"
#include	"TypesManager.h"
#include	"Server.h"
#include	"Client.h"
#include	"LUBMnetwork.h"
#include	"DBPnetwork.h"


class Conductor {
public:
	void LoadData();
	void HandleQueriesLUBMnetwork();
	void HandleQueriesDBPnetwork();
	Conductor();
	~Conductor();
};



#endif /* CONDUCTOR_H_ */
