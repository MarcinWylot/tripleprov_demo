/*
 * TemplateManager.h
 *
 *  Created on: 08-10-2012
 *      Author: marcin
 */

#ifndef TEMPLATEMANAGER_H_
#define TEMPLATEMANAGER_H_

#include	"diplodocus.h"
#include	"KeyManager.h"
#include <unordered_set>

class TripleTemplate {
public:
	mutable size_t ID;
	TYPE_ID subjectType;
	KEY_ID	predicateID;
	TYPE_ID objectType;
//	mutable size_t nextID;
	unsigned short int scope;
	friend int operator<(const TripleTemplate& left, const TripleTemplate& right);
	friend int operator==(const TripleTemplate& left, const TripleTemplate& right);
	void Display() const;
	TripleTemplate();

};

class TemplateManager {
//	vector< vector<TripleTemplate> > templates; //maybe i should change it to multimap< TYPE_ID, TripleTemplate >, do i really need first vector?? the ids are being used only here, in Molecules we're using TYPE_ID

	map< TYPE_ID, set<TripleTemplate> > templates;
	bool CheckIfRoot(TYPE_ID t);
public:
	TemplateManager();
	void AddTemplate(TripleTemplate triple);
	void AddTriple(TripleTemplate triple, unsigned short int scope);
	void AddTripleInScope(TripleTemplate triple, TYPE_ID root, unsigned short int scope);
	void AddCompisite(TYPE_ID type, set<TYPE_ID>* tmptypes);
	size_t GetTemplate(TYPE_ID root, TYPE_ID s, KEY_ID p, TYPE_ID o);
	KEY_ID GetPredicate(TYPE_ID root, size_t tmpl, unsigned short int scope);
	size_t GetTripleTemplate(TYPE_ID tmpl, TYPE_ID s, KEY_ID p, TYPE_ID o, unsigned short int scope);
//	void GetTripleTemplatesPO(KEY_ID p, KEY_ID o, unsigned short int scope, set<TYPE_ID> * res);
	TYPE_ID GetTypeForTemplate(size_t tmpl);
	size_t GetTemplateForType(TYPE_ID type);
	void GetTemplates(TYPE_ID s, KEY_ID p, TYPE_ID o, unsigned short int scope, multimap<TYPE_ID, size_t> *tripleTmpl);
	void GetTemplates(TYPE_ID s, KEY_ID p, TYPE_ID o, unsigned short int scope, unordered_set<size_t> *tripleTmpl);
//	void FindTripleInScopeAndAdd(TripleTemplate triple, unsigned short int scope, vector<TripleTemplate> *triples);
	void Display();
	void Display(size_t tmpl);
	void Store();
	void Restore();
	virtual ~TemplateManager();
};

namespace diplo {
//	extern class KeyManager KM;
	extern class TemplateManager TM;
}

#endif /* TEMPLATEMANAGER_H_ */
