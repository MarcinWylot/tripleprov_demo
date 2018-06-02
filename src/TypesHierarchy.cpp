/*
 * TypesHierarchy.cpp
 *
 *  Created on: 07-08-2012
 *      Author: marcin
 */

#include "TypesHierarchy.h"

TypesHierarchy::TypesHierarchy() {
	CompositeStartFrom = 0;
//	types.push_back( vector<TYPE_ID>() );
}

void TypesHierarchy::Add(TYPE_ID type) {
	types[type] = tmprow;
	CompositeStartFrom = types.size();
}

void TypesHierarchy::AddRelation(TYPE_ID type, TYPE_ID parent) {
//	types[type].push_back(parent);
	types[parent].push_back(type);
}

void TypesHierarchy::SetNublerOfTypes(size_t nb) {
	types.resize(nb+3);
}

void TypesHierarchy::Display() {
	for (size_t i = 3; i < types.size(); i++) {
		cout << "[ "<< i << " ]";
		if ( i < CompositeStartFrom) cout << diplo::KM.Get( KeyManager::GetId(i, 1) );
		else cout << "Composite Type";
		cout << " =========> " ;
		for (size_t j = 0; j < types[i].size(); j++ ) {
			cout << diplo::KM.Get( KeyManager::GetId(types[i][j], 1) )  << "( "<< types[i][j] << " )" << "\t" ; //types[i][j] << ", " ;
		}
		cout << endl;
	}
}

TYPE_ID TypesHierarchy::GiveSubClass(TYPE_ID oldone, TYPE_ID newone) {
	vector<TYPE_ID>::iterator it;
	TYPE_ID t=0;

if (types.size() >= oldone) return newone; //hack

	for (it=types[oldone].begin(); it!=types[oldone].end(); it++) {
//		cerr << "====> new: " << newone << " ====> old: " << oldone << " ===> *it " << *it << endl;
			if (*it == newone) {
				t = newone;
			} else {
				t = GiveSubClass(*it, newone);
				if (t == *it) t = newone;
			}

			if (t) break;
	}
	return t;

}

set<TYPE_ID> TypesHierarchy::GetHierarchyForType(TYPE_ID _type) {
	vector<TYPE_ID>::iterator it2;
	set<TYPE_ID> vect;
	set<TYPE_ID> tmp;
	set<TYPE_ID> composites;

	for (it2 = types[_type].begin(); it2 != types[_type].end(); it2++) {
		tmp = GetHierarchyForType(*it2);
		for (set<TYPE_ID>::iterator i = tmp.begin(); i != tmp.end(); i++) {
			composites = GetCompositesForTypes(*i);
			vect.insert(composites.begin(), composites.end());
		}
		vect.insert(tmp.begin(), tmp.end());
		vect.insert(*it2);
	}

	tmp = GetCompositesForTypes(_type);
	vect.insert(tmp.begin(), tmp.end());

	return vect;
}

TYPE_ID TypesHierarchy::AddCompisite(set<TYPE_ID>* tmptypes) {
	TYPE_ID newType;
	set <TYPE_ID>::iterator it_type;
	vector<TYPE_ID> tmprow;

	for ( it_type = tmptypes->begin(); it_type != tmptypes->end(); it_type++)
		tmprow.push_back(*it_type);


	if (CompositeStartFrom != 0)
		for ( size_t i = CompositeStartFrom; i <= types.size(); i++)
			if( types[i] == tmprow ) return i;


	types.push_back(tmprow);
	newType = types.size()-1;

	if (CompositeStartFrom == 0) CompositeStartFrom = newType;
	return newType;
}

set<TYPE_ID> TypesHierarchy::GetCompositesForTypes(TYPE_ID type) {
	vector<TYPE_ID>::iterator it2;
	set<TYPE_ID> vect;
	set<TYPE_ID> tmp;

	for (size_t i = CompositeStartFrom; i < types.size(); i++) {
		for (it2 = types[i].begin(); it2 != types[i].end(); it2++) {
			if (*it2 == type) {
//				cerr << i << endl;
//				tmp = GetGenericTypes(*it2);
//				vect.insert( tmp.begin(), tmp.end() );
				vect.insert(i);
			}
		}
	}
	vect.insert(type);
	return vect;
}

void TypesHierarchy::Store() {
	fstream file;
	size_t s;
	size_t typesManagerSize = diplo::KM.GetNbOfTypes();
	try {
		file.open(diplo::dbDir+"types", ios::binary | ios::out);
		file.write((char*) &typesManagerSize, sizeof(typesManagerSize));
		file.write((char*) &CompositeStartFrom, sizeof(CompositeStartFrom));

		s = types.size();
		file.write((char*) &s, sizeof(s)); //number of elements in templates

		for (size_t i = 0; i < types.size(); i++) {
			s = types[i].size();
			file.write((char*) &s, sizeof(s)); //number of elements in <set> for each template
			file.write((char*) types[i].data(), sizeof(TYPE_ID)*s); //number of elements in <set> for each template
		}
		file.close();
	} catch (fstream::failure &e) {
		cerr << "Exception writing file";
	}
}

void TypesHierarchy::Restore() {
	fstream file;
	size_t s1,s2;
	size_t typesManagerSize;
	try {
		file.open(diplo::dbDir+"types", ios::binary | ios::in);
		file.read((char*) &typesManagerSize, sizeof(typesManagerSize));
		diplo::TypesM.SeizeIt(typesManagerSize);
		file.read((char*) &CompositeStartFrom, sizeof(CompositeStartFrom));
		file.read((char*) &s1, sizeof(s1)); //number of elements in templates
		types.resize(s1);
		for (size_t i = 0; i < s1; i++) {
			file.read((char*) &s2, sizeof(s2)); //number of elements in <set> for each template
			types[i].resize(s2);
			file.read((char*) types[i].data(), sizeof(TYPE_ID)*s2); //number of elements in <set> for each template
		}
		file.close();
	} catch (fstream::failure &e) {
		cerr << "Exception writing file";
	}
}

TypesHierarchy::~TypesHierarchy() {
	// TODO Auto-generated destructor stub
}

