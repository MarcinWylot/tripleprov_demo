/*
 * WDCqueries.h
 *
 *  Created on: Sep 26, 2013
 *      Author: marcin
 */

#ifndef WDCQUERIES_H_
#define WDCQUERIES_H_
#include	"diplodocus.h"
#include	"KeyManager.h"
#include 	"TypesHierarchy.h"
#include	"TemplateManager.h"
#include	"Molecules.h"
#include	"API.h"
namespace queries {

class WDCqueries {
public:
	WDCqueries();
	virtual ~WDCqueries();
	void q01();
	void q02();
	void q03();
	void q04();
	void q05();
	void q06();
	void q07();
	void runthemall();
	void benchmark();
	void ProvQuery();

};

} /* namespace queries */
#endif /* WDCQUERIES_H_ */
