/*
 * Molecules.h
 *
 *  Created on: 02-11-2012
 *      Author: marcin
 */

#ifndef MOLECULES_H_
#define MOLECULES_H_

#include	"diplodocus.h"
#include	"KeyManager.h"
#include	"TemplateManager.h"
#include	"TypesManager.h"
#include <unordered_map>


using namespace std;
class Molecules;
class Entity {
public:
	KEY_ID obj; // subject for what is in "next"
//	multimap<size_t,Entity> next; //triple template (size_t) and multiple objects for which 'obj' is a subject

	unordered_multimap< KEY_ID, multimap<size_t,Entity> > next;

	bool CheckIfEntityExists(unordered_set<size_t> *tripleTemplates, KEY_ID o, unsigned short int scope, unordered_set<KEY_ID>* prov);
	void FindEntityAndAddNext(size_t tripleTmpl, KEY_ID o, KEY_ID s, KEY_ID prov, unsigned short int scope, class Molecule *molecule);
	void GetEntity(unordered_set<size_t> *tripleTemplates, KEY_ID s, unsigned short int scope, unordered_set<KEY_ID> *result, unordered_set<KEY_ID>* prov);
	void GetAllEntities(unsigned short int scope, unordered_multimap<size_t, KEY_ID> *result, KEY_ID s);
	void Display();
	Entity() {obj=0;}
	Entity(KEY_ID _obj) {obj=_obj;}
	~Entity();
	void MaterializeMoleculesForProvenance(KEY_ID id, Molecules &M);
};

class Molecule {
//	multimap<size_t,Entity> next; //triple template and multiple objects

//	unordered_map< KEY_ID, multimap<size_t,Entity> > next;
		//provenance -> triple template -> object (and further elements on the path)
//	vector< pair<KEY_ID, multimap<size_t,Entity> >> next;


public:
	vector< pair<KEY_ID, vector<pair<size_t,Entity> > >> next;
	void AddTriple(size_t tripleTmpl, KEY_ID o, KEY_ID prov);
	bool CheckIfEntityExists(unordered_set<size_t> *tripleTemplates, KEY_ID o, unsigned short int scope, unordered_set<KEY_ID>* prov);
	void GetObjects(size_t tripleTmpl, set<KEY_ID>* result);
	void FindEntityAndAddNext(size_t tripleTmpl, KEY_ID o, KEY_ID s, KEY_ID prov, unsigned short int scope);
	void GetEntity(unordered_set<size_t> *tripleTemplates, KEY_ID s, unsigned short int scope, unordered_set<KEY_ID> *result, unordered_set<KEY_ID>* prov);
	void GetAllEntities(unsigned short int scope, unordered_multimap<size_t, KEY_ID> *result, KEY_ID s);
	void Display();
	Molecule();
	~Molecule();
	void MaterializeMoleculesForProvenance(KEY_ID molec, Molecules &M2);
};

//typedef map <KEY_ID, set< pair<const KEY_ID, Molecule>* > > Tinv_molecules;
//typedef pair < map <KEY_ID, set< pair<const KEY_ID, Molecule>* > >::iterator, map <KEY_ID, set< pair<const KEY_ID, Molecule>* > >::iterator > Tit_inv_molecules;

class Molecules {
//	unordered_map<string, int> months;

	map <KEY_ID, set< pair<const KEY_ID, Molecule>* > > inv_molecules;
public:
	unordered_map<KEY_ID, Molecule > molecules;
	void CreateMolecule(KEY_ID s, KEY_ID p, KEY_ID o,  KEY_ID prov);
	void ReconstructMolecule(KEY_ID s, size_t tripleTmpl, KEY_ID o,  KEY_ID prov);
	bool IsItMyObject(KEY_ID s);
	set< pair<const KEY_ID, Molecule>* >* GetMoleculesFromInvertedIdx(KEY_ID o);
	void AddTriple(KEY_ID s, KEY_ID p, KEY_ID o,  KEY_ID prov, unsigned short int scope);
	Molecule* Get(KEY_ID s);
	pair<const KEY_ID, Molecule>* GetPair(KEY_ID s);
	void Display();
	size_t nbMolecules();
	Molecules();
	~Molecules();
	static bool provenanceTriger(KEY_ID oid);
	void MaterializeMoleculesForProvenance(Molecules &M2);
};

namespace diplo {
	extern class Molecules M;
	extern unordered_set<KEY_ID> ProvUris;
	extern unordered_set<KEY_ID> ProvMolecules;
	extern unordered_map<KEY_ID, unordered_set<KEY_ID>> ProvIdx;
	extern unordered_map<KEY_ID,int> contextTriples;
}

#endif /* MOLECULES_H_ */
