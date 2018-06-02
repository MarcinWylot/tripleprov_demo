/*
 * TypesManager.h
 *
 *  Created on: 30-03-2013
 *      Author: marcin
 */

#ifndef TYPESMANAGER_H_
#define TYPESMANAGER_H_

#include "KeyManager.h"

class TypesManager {
	vector<class Types> types; /*
	 0 - unclassified URI
	 1 - type
	 */
public:
	void SeizeIt(size_t nb);
	void AddElement(TYPE_ID type, KEY_ID e);
	unordered_set< KEY_ID >* GetElementsOfType( TYPE_ID _type );
	TypesManager();
	virtual ~TypesManager();
};

namespace diplo {
	extern class TypesManager TypesM;
}

#endif /* TYPESMANAGER_H_ */
