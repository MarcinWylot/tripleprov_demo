/*
 * BTCprov.h
 *
 *  Created on: Jul 18, 2013
 *      Author: marcin
 */

#ifndef BTCPROV_H_
#define BTCPROV_H_

#include	"diplodocus.h"
#include	"KeyManager.h"
#include 	"TypesHierarchy.h"
#include	"TemplateManager.h"
#include	"Molecules.h"
#include	"API.h"
#include <algorithm>
using namespace diplo;
namespace queries {

class BTCprov {
	ofstream provOutput;
public:
	BTCprov();
	virtual ~BTCprov();
	void mixer();
	void TPDemo();
	void q01();
	void q02();
	void q03();
	void q04();
	void q05();
	void q06();
	void q07();
	void q08();
	void q09();
	void q10();
	void runthemall();
	void benchmark();
	void ProvQuery();
};

} /* namespace queries */
#endif /* BTCPROV_H_ */
