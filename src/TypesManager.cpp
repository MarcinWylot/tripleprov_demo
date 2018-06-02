/*
 * TypesManager.cpp
 *
 *  Created on: 30-03-2013
 *      Author: marcin
 */

#include "TypesManager.h"

TypesManager::TypesManager() {
	// TODO Auto-generated constructor stub

}

void TypesManager::SeizeIt(size_t nb) {
	types.resize(nb);
}

void TypesManager::AddElement(TYPE_ID type, KEY_ID e) {
	types[type].elements2.insert(e);
}

unordered_set< KEY_ID >* TypesManager::GetElementsOfType(TYPE_ID _type) {
	return &(types[_type].elements2);
}

TypesManager::~TypesManager() {
	// TODO Auto-generated destructor stub
}

