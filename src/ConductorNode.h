/*
 * ConductorNode.h
 *
 *  Created on: 22-03-2013
 *      Author: marcin
 */

#ifndef CONDUCTORNODE_H_
#define CONDUCTORNODE_H_

#include	"diplodocus.h"
#include	"KeyManager.h"
#include 	"TypesHierarchy.h"
#include	"TemplateManager.h"
#include	"Molecules.h"
#include	"TypesManager.h"
#include	"Server.h"
#include	"Client.h"
#include	"LUBMnetwork.h"
#include	"DBPnetwork.h"



class ConductorNode {
	unordered_set<KEY_ID> myMolecules;
	void readMyMolecules();
public:
	void LoadData();
	void HandleQueries();
	ConductorNode();
	virtual ~ConductorNode();
};

#endif /* CONDUCTORNODE_H_ */
