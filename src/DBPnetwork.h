/*
 * DBPnetwork.h
 *
 *  Created on: 26-04-2013
 *      Author: marcin
 */

#ifndef DBPNETWORK_H_
#define DBPNETWORK_H_

#include	"diplodocus.h"
#include	"KeyManager.h"
#include 	"TypesHierarchy.h"
#include	"TemplateManager.h"
#include	"Molecules.h"
#include	"TypesManager.h"
#include	"Client.h"
#include	"Server.h"

namespace queries {

class DBP_network {
public:
	void q01_n(char *data);
	void q01_m();
	void q02_n(char *data);
	void q02_m();
	void q03_n(char *data);
	void q03_m();
	void q04_n(char *data);
	void q04_m();
	void q05_n(char *data);
	void q05_m();
	DBP_network();
	virtual ~DBP_network();
};

} /* namespace queries */
#endif /* DBPNETWORK_H_ */
