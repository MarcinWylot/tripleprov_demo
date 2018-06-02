/*
 * LUBMnetwork.h
 *
 *  Created on: 28-03-2013
 *      Author: marcin
 */

#ifndef LUBMNETWORK_H_
#define LUBMNETWORK_H_

#include	"diplodocus.h"
#include	"KeyManager.h"
#include 	"TypesHierarchy.h"
#include	"TemplateManager.h"
#include	"Molecules.h"
#include	"TypesManager.h"
#include	"Client.h"
#include	"Server.h"

namespace queries {

class LUBM_network {
public:
	void q01_n(char *data);
	void q01_m();
	void q02s1_n(char *data);
	void q02s1_m();
	void q02s2_n(char *data);
	void q02s2_m();
	void q03_n(char *data);
	void q03_m();
	void q04_n(char *data);
	void q04_m();
	void q05_n(char *data);
	void q05_m();
	void q06_n(char *data);
	void q06_m();
	void q07_n(char *data);
	void q07_m();
	void q08s1_n(char *data);
	void q08s1_m();
	void q08s2_n(char *data);
	void q08s2_m();
	void q09s1_n(char *data);
	void q09s1_m();
	void q09s2_n(char *data);
	void q09s2_m();
	void q10_n(char *data);
	void q10_m();
	void q11s1_n(char *data);
	void q11s1_m();
	void q11s2_n(char *data);
	void q11s2_m();
	void q12_n(char *data);
	void q12_m();
	void q13_n(char *data);
	void q13_m();
	void q14_n(char *data);
	void q14_m();
	void q15_n(char *data);
	void q15_m();
	void q16s1_n(char *data);
	void q16s1_m();
	void q16s2_n(char *data);
	void q16s2_m();
	void q17_n(char *data);
	void q17_m();
	LUBM_network();
	virtual ~LUBM_network();
};

} /* namespace queries */
#endif /* LUBMNETWORK_H_ */
