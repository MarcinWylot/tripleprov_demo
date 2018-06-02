/*
 * TypesHierarchy.h
 *
 *  Created on: 07-08-2012
 *      Author: marcin
 */

#ifndef TYPESHIERARCHY_H_
#define TYPESHIERARCHY_H_

#include	"diplodocus.h"
#include	"KeyManager.h"
#include	"TypesManager.h"

class TypesHierarchy {
	vector<TYPE_ID> tmprow;
	TYPE_ID CompositeStartFrom;

	vector< vector<TYPE_ID> > types;

public:
	TypesHierarchy();
	void Add(TYPE_ID type);
	void SetNublerOfTypes(size_t nb);
	void AddRelation(TYPE_ID type, TYPE_ID parent);
	set<TYPE_ID> GetHierarchyForType(TYPE_ID type);
	set<TYPE_ID> GetCompositesForTypes(TYPE_ID type);
	TYPE_ID GiveSubClass(TYPE_ID oldone, TYPE_ID newone);
	TYPE_ID AddCompisite(set<TYPE_ID> *types);
	void Store();
	void Restore();
	void Display();
	virtual ~TypesHierarchy();



};

namespace diplo {
//	extern class KeyManager KM;
	extern class TypesHierarchy TH;
}
#endif /* TYPESHIERARCHY_H_ */
