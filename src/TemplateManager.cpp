/*
 * TemplateManager.cpp
 *
 *  Created on: 08-10-2012
 *      Author: marcin
 */

#include "TemplateManager.h"

TemplateManager::TemplateManager() {
}

void TemplateManager::AddTemplate(TripleTemplate triple) {
	if (triple.subjectType < 2)
		return; //because 0 is error code, 1 is for types, and 2 id unclassified
	map<TYPE_ID, set<TripleTemplate> >::iterator it_templates;
//check if there is already template for this root
	it_templates = templates.find(triple.subjectType);
	if (it_templates != templates.end()) {
		triple.ID = it_templates->second.size() + 1;
		it_templates->second.insert(triple);
		return;
	}
	set<TripleTemplate> tmp;
	triple.ID = 1;
	tmp.insert(triple);
	templates.insert(pair<TYPE_ID, set<TripleTemplate> >(triple.subjectType, tmp));
}


void TemplateManager::AddTriple(TripleTemplate triple, unsigned short int scope) {
	map<TYPE_ID, set<TripleTemplate> >::iterator it_templates;
	set<TripleTemplate>::iterator it_triples;
	vector<TripleTemplate> triples_tmp;

	for (it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
		for (it_triples = it_templates->second.begin(); it_triples != it_templates->second.end(); it_triples++) {
			if (it_triples->scope == scope - 1 and it_triples->objectType == triple.subjectType) {
				triple.scope = scope;
				triple.ID = it_templates->second.size() + 1;
				it_templates->second.insert(triple);

			}
		}

	}
}

void TemplateManager::AddTripleInScope(TripleTemplate triple, TYPE_ID root, unsigned short int scope) {
	map< TYPE_ID, set<TripleTemplate> >::iterator it_templates;
	set<TripleTemplate>::iterator it_triples;

	for (it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
		if (it_templates->first == root) {
			for (it_triples = it_templates->second.begin(); it_triples != it_templates->second.end(); it_triples++) {

				if ( it_triples->scope == scope - 1 and it_triples->objectType == triple.subjectType) {
					triple.scope = scope;
					triple.ID = it_templates->second.size()+1;
					it_templates->second.insert(triple);

				}
			}

		}
	}
}

void TemplateManager::Display() {
	map< TYPE_ID, set<TripleTemplate> >::iterator it_templates;
	set<TripleTemplate>::iterator it_triples;
	vector<TripleTemplate> triples_tmp;
	for (it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
		cout << "Temple ID: " << it_templates->first << endl;
		for (it_triples = it_templates->second.begin(); it_triples != it_templates->second.end(); it_triples++) {
			it_triples->Display();
		}
		cout << endl;
	}
	cout << "number od templates: " << templates.size() << endl;
}

bool TemplateManager::CheckIfRoot(TYPE_ID t) {
	map< TYPE_ID, set<TripleTemplate> >::iterator it_templates;

	for (it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
		if (it_templates->second.begin()->subjectType == t)
			return true;
	}

	return false;
}

//void TemplateManager::FindTripleInScopeAndAdd(TripleTemplate triple, unsigned short int scope, vector<TripleTemplate> *triples) {
//	vector<TripleTemplate>::iterator it_triples;
//	for (it_triples = triples->begin(); it_triples != triples->end(); it_triples++) {
//
//	}
//}

void TemplateManager::AddCompisite(TYPE_ID type, set<TYPE_ID>* tmptypes) {

	for (set<TYPE_ID>::iterator it_type = tmptypes->begin(); it_type != tmptypes->end(); it_type++) {
		for (map< TYPE_ID, set<TripleTemplate> >::iterator it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
			if (it_templates->second.begin()->subjectType == *it_type) {
				TripleTemplate t;
				vector<TripleTemplate> tmp;
				for (set<TripleTemplate>::iterator it_triples = it_templates->second.begin(); it_triples != it_templates->second.end(); it_triples++) {
					t = *it_triples;
					if (t.subjectType == *it_type) t.subjectType = type; //loosing information from which type sertain relation is, for now OK, but FIXME
					tmp.push_back(t);
				}
				break;
			}
		}
	}


}

size_t TemplateManager::GetTemplate(TYPE_ID root, TYPE_ID s, KEY_ID p, TYPE_ID o) {
	TripleTemplate tmp;
	tmp.subjectType = s;
	tmp.predicateID = p;
	tmp.objectType = o;
	map< TYPE_ID, set<TripleTemplate> >::iterator it_templates = templates.find(root);
	set<TripleTemplate>::iterator it_triple;

//	if ( s == 614 and p == 562949953500940 and o == 2 ) cerr << "1------> have it2: " << endl;

	if(it_templates != templates.end()) {
		it_triple = it_templates->second.find(tmp);
		if(it_triple != it_templates->second.end()) {
			return it_triple->ID;
		}
	}

	return -1;
}

void TemplateManager::GetTemplates(TYPE_ID s, KEY_ID p, TYPE_ID o, unsigned short int scope, multimap<TYPE_ID, size_t>* tripleTmpl) {

	map< TYPE_ID, set<TripleTemplate> >::iterator it_templates;
	set<TripleTemplate>::iterator it_triple;

	TripleTemplate tmp;
	tmp.subjectType = s;
	tmp.predicateID = p;
	tmp.objectType = o;
	tmp.scope = scope;

	if (s != (TYPE_ID) -1 and o != (TYPE_ID) -1 and p!= (KEY_ID)-1) { //FIXME, this is kinda hack, should find better way for wildcards
		if (scope == 0 and s != (TYPE_ID) -1) {
			it_templates = templates.find(s);
			if (it_templates != templates.end()) {
				it_triple = it_templates->second.find(tmp);
				if (it_triple != it_templates->second.end()) {
					tripleTmpl->insert(pair<TYPE_ID, size_t>(it_templates->first, it_triple->ID));
				}
			}
		} else {
			for (it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
				it_triple = it_templates->second.find(tmp);
				if (it_triple != it_templates->second.end()) {
					tripleTmpl->insert(pair<TYPE_ID, size_t>(it_templates->first, it_triple->ID));
				}
			}
		}
	} else {
		//FIXME this is a "slow" hack, find a "normal" solution to work with wildcards
//		cerr << "nie jest zle\ts:" << s << "\to: " << o << endl;
		for (it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
			for (it_triple = it_templates->second.begin(); it_triple != it_templates->second.end(); it_triple++) {
				if (
						(it_triple->subjectType == s or s == (TYPE_ID) -1) and
						(it_triple->objectType == o or o == (TYPE_ID) -1) and
						(it_triple->predicateID == p or p == (KEY_ID) -1)  and
						it_triple->scope == scope
						) {
					tripleTmpl->insert(pair<TYPE_ID, size_t>(it_templates->first, it_triple->ID));
				}
			}
		}

	}

}

TYPE_ID TemplateManager::GetTypeForTemplate(size_t tmpl) {
	return templates.find(tmpl)->second.begin()->subjectType;
}

size_t TemplateManager::GetTripleTemplate(TYPE_ID tmpl, TYPE_ID s, KEY_ID p, TYPE_ID o, unsigned short int scope) {
	map< TYPE_ID, set<TripleTemplate> >::iterator it_templates = templates.find(tmpl);
	if (it_templates == templates.end()) return -1;

	set<TripleTemplate>::iterator it_triples;
	for (it_triples = it_templates->second.begin(); it_triples != it_templates->second.end(); it_triples++) {
		if (
				(it_triples->subjectType == s or s == 0) and
				(it_triples->predicateID == p ) and
				(it_triples->objectType == o or o == 0) and
				it_triples->scope == scope
			)
			return it_triples->ID;
	}
	return -1;
}

size_t TemplateManager::GetTemplateForType(TYPE_ID type) {
	for (size_t j = 0; j < templates.size(); j++) {
		if (templates[j].begin()->subjectType == type) {
			return j;
		}
	}
	return -1;
}

void TemplateManager::Store() {
	map< TYPE_ID, set<TripleTemplate> >::iterator it_templates;
	set<TripleTemplate>::iterator it_triples;
	vector<TripleTemplate> triples_tmp;
	size_t s;
	fstream file;
	try {
		file.open(diplo::dbDir+"templates", ios::binary | ios::out);
		s = templates.size();
		file.write( (char*)  &s, sizeof(s)); //number of elements in templates

		for (it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
			file.write( (char*)  &(it_templates->first), sizeof(it_templates->first));

			s = it_templates->second.size();
			file.write( (char*)  &s, sizeof(s)); //number of elements in <set> for each template

			for (it_triples = it_templates->second.begin(); it_triples != it_templates->second.end(); it_triples++) {
				file.write( (char*)  &(*it_triples), sizeof(*it_triples));
			}
		}
		file.close();

	} catch (fstream::failure &e) {
		cerr << "Exception writing file";
	}

}

void TemplateManager::Restore() {
	map< TYPE_ID, set<TripleTemplate> >::iterator it_templates;
	set<TripleTemplate>::iterator it_triples;
//	vector<TripleTemplate> triples_tmp;
	size_t s1,s2;
	TYPE_ID type;
	TripleTemplate tripleTemplate;
	set<TripleTemplate> triples_tmp;
	fstream file;
	try {
		file.open(diplo::dbDir+"templates", ios::binary | ios::in);
		file.read( (char*)  &s1, sizeof(s1)); //number of elements in templates
		for (size_t i = 0 ; i < s1; i++) {
			set<TripleTemplate> triples_tmp;
			file.read( (char*)  &type, sizeof(type));
			file.read( (char*)  &s2, sizeof(s2)); //number of elements in <set> for each template
			for (size_t j = 0 ; j < s2; j++) {
				file.read( (char*)  &tripleTemplate, sizeof(tripleTemplate));
				triples_tmp.insert(tripleTemplate);
			}
			templates.insert ( pair< TYPE_ID, set<TripleTemplate> > (type, triples_tmp) );
		}
		file.close();

	} catch (fstream::failure &e) {
		cerr << "Exception writing file";
	}


}

KEY_ID TemplateManager::GetPredicate(TYPE_ID root, size_t tmpl, unsigned short int scope) {

	map< TYPE_ID, set<TripleTemplate> >::iterator it_templates = templates.find(root);
	for (set<TripleTemplate>::iterator it_triples = it_templates->second.begin(); it_triples != it_templates->second.end(); it_triples++) {
		if ( it_triples->ID == tmpl )
			return  it_triples->predicateID;
	}
	return 0;
}

void TemplateManager::GetTemplates(TYPE_ID s, KEY_ID p, TYPE_ID o, unsigned short int scope, unordered_set<size_t>* tripleTmpl) {

	map<TYPE_ID, set<TripleTemplate> >::iterator it_templates;
	set<TripleTemplate>::iterator it_triple;

	TripleTemplate tmp;
	tmp.subjectType = s;
	tmp.predicateID = p;
	tmp.objectType = o;
	tmp.scope = scope;

	if (s != (TYPE_ID) -1 and o != (TYPE_ID) -1 and p != (KEY_ID) -1) { //FIXME, this is kinda hack, should find better way for wildcards
		if (scope == 0 and s != (TYPE_ID) -1) {
			it_templates = templates.find(s);
			if (it_templates != templates.end()) {
				it_triple = it_templates->second.find(tmp);
				if (it_triple != it_templates->second.end()) {
					tripleTmpl->insert(it_triple->ID);
				}
			}
		} else {
			for (it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
				it_triple = it_templates->second.find(tmp);
				if (it_triple != it_templates->second.end()) {
					tripleTmpl->insert(it_triple->ID);
				}
			}
		}
	} else {
		//FIXME this is a "slow" hack, find a "normal" solution to work with wildcards
//		cerr << "nie jest zle\ts:" << s << "\to: " << o << endl;
		for (it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
			for (it_triple = it_templates->second.begin(); it_triple != it_templates->second.end(); it_triple++) {
				if ((it_triple->subjectType == s or s == (TYPE_ID) -1) and (it_triple->objectType == o or o == (TYPE_ID) -1) and (it_triple->predicateID == p or p == (KEY_ID) -1)
						and it_triple->scope == scope) {
					tripleTmpl->insert(it_triple->ID);
				}
			}
		}

	}

}

void TemplateManager::Display(size_t tmpl) {
	map< TYPE_ID, set<TripleTemplate> >::iterator it_templates;
	set<TripleTemplate>::iterator it_triples;
	vector<TripleTemplate> triples_tmp;
	it_templates = templates.find(tmpl);
//	for (it_templates = templates.begin(); it_templates != templates.end(); it_templates++) {
		cout << "Temple ID: " << it_templates->first << endl;
		for (it_triples = it_templates->second.begin(); it_triples != it_templates->second.end(); it_triples++) {
			it_triples->Display();
		}
		cout << endl;
//	}
//	cout << "number od templates: " << templates.size() << endl;
}

TemplateManager::~TemplateManager() {
	// TODO Auto-generated destructor stub
}

int operator<(const TripleTemplate& left, const TripleTemplate& right) {

	bool x = false;

	if (left.subjectType < right.subjectType      )
		x = true;
	else if (left.subjectType == right.subjectType        ) {
		if (left.predicateID < right.predicateID       )
			x = true;
		else if (left.predicateID == right.predicateID       ) {
			if (left.objectType < right.objectType   )
				x = true;
			else if (left.objectType == right.objectType        ) {
				if (left.scope < right.scope)
					x = true;
			}
		}
	}

//FIXME check if -1 works OK
//	Check results for all queries



//	if (left.subjectType < right.subjectType    and (right.subjectType != (TYPE_ID)-1 and left.subjectType != (TYPE_ID)-1 )      )
//		x = true;
//	else if (left.subjectType == right.subjectType     or  (right.subjectType == (TYPE_ID)-1 or left.subjectType == (TYPE_ID)-1 )      ) {
//		if (left.predicateID < right.predicateID    and (right.predicateID != (KEY_ID)-1 and left.predicateID != (KEY_ID)-1 )      )
//			x = true;
//		else if (left.predicateID == right.predicateID      or  (right.predicateID == (KEY_ID)-1 or left.predicateID == (KEY_ID)-1 )      ) {
//			if (left.objectType < right.objectType    and (right.objectType != (TYPE_ID)-1 and left.objectType != (TYPE_ID)-1 )      )
//				x = true;
//			else if (left.objectType == right.objectType      or  (right.objectType == (TYPE_ID)-1 or left.objectType == (TYPE_ID)-1 )      ) {
//				if (left.scope < right.scope) {
//					x = true;
//				}
//			}
//		}
//	}


//	if (diplo::onlyPartition) {
////		if ( (left.subjectType == 50 or right.subjectType == 50)
////						) {
//						cerr << "<<<<<<< subjectType: " << left.subjectType << "-" << right.subjectType << "\t"
//								<< "predicateID: " << left.predicateID << "-" << right.predicateID << "\t"
//								<< "objectType: " << left.objectType << "-" << right.objectType << "\t"
//								<< "scope: " << left.scope << "-" << right.scope << "\t"
//								<< "\t result:" << x << endl;
////		}
//	}

	return x;
}

void TripleTemplate::Display() const {
	cout << "ID: [ " << ID << " ]\t" << subjectType << "\t" << diplo::KM.Get(predicateID) << " [ " << predicateID << " ] \t" << objectType << "\t"  << scope << endl;
}

int operator ==(const TripleTemplate& left, const TripleTemplate& right) {

//	if (diplo::onlyPartition) {
////			if ( (left.subjectType == 50 or right.subjectType == 50)
////							) {
//							cerr << "======= subjectType: " << left.subjectType << "-" << right.subjectType << "\t"
//									<< "predicateID: " << left.predicateID << "-" << right.predicateID << "\t"
//									<< "objectType: " << left.objectType << "-" << right.objectType << "\t"
//									<< "scope: " << left.scope << "-" << right.scope << "\t"
//									<<endl;
////			}
//		}


	if (
			( left.subjectType == right.subjectType or right.subjectType == (TYPE_ID)-1 )  and
			( left.predicateID == right.predicateID or right.predicateID == (TYPE_ID)-1 )  and
			( left.objectType == right.objectType or right.objectType == (TYPE_ID)-1 )  and
			( left.scope == right.scope or right.scope == (unsigned short int)-1 )
		)
		return true;

	return false;
}

TripleTemplate::TripleTemplate() {
	ID = 0;
	subjectType = 0;
	predicateID = 0;
	objectType = 0;
//	nextID = 0;
	scope=0;
}

