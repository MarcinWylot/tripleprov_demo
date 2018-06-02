/*
 * Molecules.cpp
 *
 *  Created on: 02-11-2012
 *      Author: marcin
 */

#include "Molecules.h"


void Entity::FindEntityAndAddNext(size_t tripleTmpl, KEY_ID o, KEY_ID s, KEY_ID prov, unsigned short int scope, Molecule *molecule) {
//FIXME prov
	if (scope == 1 and obj == s) { //add in one of nexts here
		unordered_map< KEY_ID, multimap<size_t,Entity> >::iterator i = next.find(prov);
		if (i != next.end())
			i->second.insert(pair<size_t, Entity>(tripleTmpl, Entity(o)));
		else {
			multimap<size_t,Entity> tmp;
			tmp.insert(pair<size_t, Entity>(tripleTmpl, Entity(o)));
			next.insert( pair< KEY_ID, multimap<size_t,Entity> > (prov,tmp) );
		}
		return;
	} else { //continue searching for it
		for (unordered_map < KEY_ID, multimap<size_t,Entity> >::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
			for (multimap<size_t, Entity>::iterator it_next = it_p->second.begin(); it_next != it_p->second.end(); it_next++) {
				it_next->second.FindEntityAndAddNext(tripleTmpl, o, s, prov, scope - 1, molecule);
			}
		}
	}
}

void Entity::Display() {
//FIXME prov
	static string spaces = "---";
//	cout << "----------------------------------------  next.size()  " << next.size() << endl;
	for (unordered_map < KEY_ID, multimap<size_t,Entity> >::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
		cout << spaces << it_p->first << "\t" << diplo::KM.Get(it_p->first) << " [prov] "<< endl;
		for (multimap<size_t, Entity>::iterator it_next = it_p->second.begin(); it_next != it_p->second.end(); it_next++) {
			cout << spaces << "-" << it_next->first << "\t"
					<< diplo::KM.Get(it_next->second.obj) << "\tnext nb: "
					<< it_next->second.next.size() << endl;
			spaces.append("-");
			it_next->second.Display();
			spaces = spaces.substr(0, spaces.length() - 1);
		}
	}
}

void Entity::GetEntity(unordered_set<size_t>* tripleTemplates, KEY_ID s, unsigned short int scope, unordered_set<KEY_ID>* result, unordered_set<KEY_ID>* prov) {
//FIXME prov

	if (scope == 0) {
		if (obj == s or s == 0) {
			for (unordered_set<size_t>::iterator templ =
					tripleTemplates->begin(); templ != tripleTemplates->end();
					templ++) {

				for (unordered_map<KEY_ID, multimap<size_t, Entity> >::iterator it_p =
							next.begin(); it_p != next.end(); it_p++) {
					if (Molecules::provenanceTriger(it_p->first)) {
						pair<multimap<size_t, Entity>::iterator,
								multimap<size_t, Entity>::iterator> it =
								it_p->second.equal_range(*templ);
						for (multimap<size_t, Entity>::iterator i = it.first;
								i != it.second; i++) {
							
							result->insert(i->second.obj);
							prov->insert(it_p->first);
						}
					}
				}
			}
		}
	} else {
		for (unordered_map<KEY_ID, multimap<size_t, Entity> >::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
			for (multimap<size_t, Entity>::iterator it_next = it_p->second.begin(); it_next != it_p->second.end(); it_next++) {

				it_next->second.GetEntity(tripleTemplates, s, scope - 1, result, prov);
			}
		}
	}
}

void Entity::GetAllEntities(unsigned short int scope, unordered_multimap<size_t, KEY_ID>* result, KEY_ID s) {
	//FIXME prov

	if (scope == 0) {
		if (obj == s or s == 0) {
			for (unordered_map<KEY_ID, multimap<size_t, Entity> >::iterator it_p =
					next.begin(); it_p != next.end(); it_p++) {
				if (Molecules::provenanceTriger(it_p->first)) {
					for (multimap<size_t, Entity>::iterator it_next =
							it_p->second.begin(); it_next != it_p->second.end();
							it_next++) {
						result->insert(
								pair<size_t, KEY_ID>(it_next->first,
										it_next->second.obj));
					}
				}
			}
		}
	} else {
		for (unordered_map<KEY_ID, multimap<size_t, Entity> >::iterator it_p =
				next.begin(); it_p != next.end(); it_p++) {
			for (multimap<size_t, Entity>::iterator it_next =
					it_p->second.begin(); it_next != it_p->second.end();
					it_next++) {
				it_next->second.GetAllEntities(scope - 1, result, s);
			}
		}
	}
}

bool Entity::CheckIfEntityExists(unordered_set<size_t> *tripleTemplates, KEY_ID o, unsigned short int scope, unordered_set<KEY_ID>* prov) {
//FIXME prov
	bool ret = false;
	if (scope == 0) {
		for (unordered_map<KEY_ID, multimap<size_t, Entity> >::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
			if ( Molecules::provenanceTriger(it_p->first) ) {
				for (unordered_set<size_t>::iterator tripleTmpl =
						tripleTemplates->begin();
						tripleTmpl != tripleTemplates->end(); tripleTmpl++) {
					pair<multimap<size_t, Entity>::iterator,
							multimap<size_t, Entity>::iterator> it =
							it_p->second.equal_range(*tripleTmpl);
					for (multimap<size_t, Entity>::iterator i = it.first;
							i != it.second; i++) {
						if (i->second.obj == o) {
							//				cout << "\t------> " << i->first << " == " << tripleTmpl << "\t" << i->second.obj << " == " << o << endl;
							ret = true;
							prov->insert(it_p->first);
						}
					}
				}
			}
		}
	} else {
		for (unordered_map<KEY_ID, multimap<size_t, Entity> >::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
			for (multimap<size_t, Entity>::iterator it_next = it_p->second.begin(); it_next != it_p->second.end(); it_next++) {
				return it_next->second.CheckIfEntityExists(tripleTemplates, o, scope - 1, prov);
			}
		}
	}
	return ret;
}

Entity::~Entity() {
}

Molecules::Molecules() {
	// TODO Auto-generated constructor stub

}

void Molecules::CreateMolecule(KEY_ID s, KEY_ID p, KEY_ID o, KEY_ID prov) {
	pair< unordered_map<KEY_ID, Molecule >::iterator, bool > it;
	it = molecules.insert( pair<KEY_ID, Molecule> ( s, Molecule() ) );

//	if ( it.second == true ) {
//	it->second.AddTriple(size_t tripleTmpl, KEY_ID o, unsigned short int scope);
	//discover template for p and o in Molecule of template s
	unsigned short int tripleTmpl = diplo::TM.GetTemplate(
			diplo::KM.GetType(s), diplo::KM.GetType(s), p, diplo::KM.GetType(o));

	if (tripleTmpl == -1) {
		cerr << diplo::KM.Get(s) << "[ " << diplo::KM.GetType(s) << " ] " << "\t" << diplo::KM.Get(p) << "[ " << p << " ] " << "\t" << diplo::KM.Get(o) << "[ " << diplo::KM.GetType(o) << " ] " << endl;
	}
//	else
//		cerr << tripleTmpl << endl;


	it.first->second.AddTriple(tripleTmpl, o, prov);
//	pair<const KEY_ID, Molecule> *ss;
//	ss = &( *(it.first) );

//	if (o == 9570149208162307)
//			cerr << "MW ---> " << it.first->first << "\t" << diplo::KM.Get(9570149208162307) << endl;


	map <KEY_ID, set< pair<const KEY_ID, Molecule>* > >::iterator it_inv_molecules = diplo::M.inv_molecules.find(o);
	if (it_inv_molecules == diplo::M.inv_molecules.end() ) {
		set< pair<const KEY_ID, Molecule>* > tt;
		tt.insert( &(*(it.first)) ) ;

		pair<KEY_ID, set< pair<const KEY_ID, Molecule>* > > x (o, tt );
//		set<pair<const KEY_ID, Molecule>*>::iterator it2 = tt.begin();
//		cerr << "-----1 > " << diplo::KM.Get(s) << "\t" << s  << "\t";
//		cerr << (*it2)->first << "\t" << endl;

		inv_molecules.insert( x ); //MW check here!!!!
	} else {
		it_inv_molecules->second.insert( &(*(it.first)) ) ;
	}


}

void Molecules::ReconstructMolecule(KEY_ID s, size_t tripleTmpl, KEY_ID o, KEY_ID prov) {
	pair< unordered_map<KEY_ID, Molecule >::iterator, bool > it;
	it = molecules.insert( pair<KEY_ID, Molecule> ( s, Molecule() ) );

	it.first->second.AddTriple(tripleTmpl, o, prov);

	map <KEY_ID, set< pair<const KEY_ID, Molecule>* > >::iterator it_inv_molecules = inv_molecules.find(o);
	if (it_inv_molecules == inv_molecules.end() ) {
		set< pair<const KEY_ID, Molecule>* > tt;
		tt.insert( &(*(it.first)) ) ;

		pair<KEY_ID, set< pair<const KEY_ID, Molecule>* > > x (o, tt );
		inv_molecules.insert( x ); //MW check here!!!!
	} else {
		it_inv_molecules->second.insert( &(*(it.first)) ) ;
	}

	auto provIT = diplo::ProvIdx.find(prov);
	if (provIT == diplo::ProvIdx.end()) {
		diplo::ProvIdx.insert( { { prov, { s } } });
	} else {
		provIT->second.insert(s);
	}
}

void Molecules::AddTriple(KEY_ID s, KEY_ID p, KEY_ID o, KEY_ID prov, unsigned short int scope) {
	multimap<TYPE_ID, size_t> tripleTmpls;
	diplo::TM.GetTemplates(diplo::KM.GetType(s), p, diplo::KM.GetType(o), scope, &tripleTmpls);


	vector<KEY_ID> molecules_to_search_in;

	map <KEY_ID, set< pair<const KEY_ID, Molecule>* > >::iterator it_inv_molecules = diplo::M.inv_molecules.find(s);
	set< pair<const KEY_ID, Molecule>*  > * molecules_where_S_exists;

	if (it_inv_molecules != diplo::M.inv_molecules.end() ) {
		molecules_where_S_exists = &(it_inv_molecules->second);
	} else return; //this subject DOES NOT exist as an object in any triple


	vector< pair<const KEY_ID, Molecule>*  > elems;
	if (tripleTmpls.size() > 0) {

		diplo::TypesM.AddElement(KeyManager::GetType( s ), s );
		diplo::TypesM.AddElement(KeyManager::GetType( o ), o );

		unordered_set< KEY_ID > * el;
		size_t t = -1;

		for (multimap<TYPE_ID, size_t>::iterator it = tripleTmpls.begin(); it !=  tripleTmpls.end(); it ++) { //scan all templates where TRIPLE can exist

			if (t != (size_t)it->first) {
				t = it->first;
				el = diplo::TypesM.GetElementsOfType( t  ); // get list of molecules where triple can be in contexst of the above template
				elems.clear();

				for (set < pair<const KEY_ID, Molecule>* >::iterator it2 = molecules_where_S_exists->begin(); it2 != molecules_where_S_exists->end(); it2++) {
					//check if in the molecule SUBJECT exists as an OBJECT, if YES, that is what we want!!
					if ( el->find( (*it2)->first ) != el->end() ) {
						elems.push_back( *it2 );
					}
				}





			}



			for (vector< pair<const KEY_ID, Molecule>*  >::iterator it2 = elems.begin(); it2 != elems.end(); it2++) {

				(*it2)->second.FindEntityAndAddNext(it->second, o, s, prov, scope); //it has to be added there, molecules are preselected so it is just a question of finding right place to insert the triple
				//so then we can add also to the inverted index, that THE OBJECT (o) in THE MOLECULE pointer by *it2
				map <KEY_ID, set< pair<const KEY_ID, Molecule>* > >::iterator it_inv_molecules = diplo::M.inv_molecules.find(o);

				if (it_inv_molecules == diplo::M.inv_molecules.end() ) {
					set< pair<const KEY_ID, Molecule>* > tt;
					tt.insert( *it2 ) ;
					inv_molecules.insert( pair<KEY_ID, set< pair<const KEY_ID, Molecule>* > > (o, tt ) );
				} else {
					it_inv_molecules->second.insert( *it2 ) ;
				}


			}



		}
	}
}



void Molecules::Display() {

	for (unordered_map<KEY_ID, Molecule >::iterator it = molecules.begin(); it != molecules.end(); it++) {
			cout << it->first << "\t" << diplo::KM.Get(it->first )<< endl;
			it->second.Display();
	}

	cout << endl << "Number of molecules: " << molecules.size() << endl;
}

size_t Molecules::nbMolecules() {
return molecules.size();
}

Molecule *Molecules::Get(KEY_ID s)
{
	unordered_map<KEY_ID, Molecule>::iterator i;
//	cerr << "molecules.size(): "<< molecules.size() << endl;
	i = molecules.find(s);
	if (i != molecules.end())
		return &(i->second);
	return NULL;
}
pair<const KEY_ID, Molecule>* Molecules::GetPair(KEY_ID s)
{
	unordered_map<KEY_ID, Molecule>::iterator i;
//	cerr << "molecules.size(): "<< molecules.size() << endl;
	i = molecules.find(s);
	if (i != molecules.end())
		return &(*i);
	return NULL;
}

set<pair<const KEY_ID, Molecule> *>* Molecules::GetMoleculesFromInvertedIdx(KEY_ID o) {
	map <KEY_ID, set< pair<const KEY_ID, Molecule>* > >::iterator it = inv_molecules.find(o);
	if (it != inv_molecules.end())
		return &(it->second);
	else
		return NULL;
}

bool Molecules::IsItMyObject(KEY_ID s) {
	if ( molecules.find(s) != molecules.end() ) return true;
	return false;
}

Molecules::~Molecules() {
	// TODO Auto-generated destructor stub
}



bool Molecule::CheckIfEntityExists(unordered_set<size_t> *tripleTemplates, KEY_ID o, unsigned short int scope, unordered_set<KEY_ID>* prov) {
//FIXME prov
  
    bool ret = false;
    if (tripleTemplates->empty()) {
	  diplo::elementsChecked++;
//         for (vector< pair<KEY_ID, vector<pair<size_t,Entity> > >>::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
// 		  
//             if ( Molecules::provenanceTriger(it_p->first) ) diplo::ProvFilterCH = true;
//            
//         }
        return ret;
	}
    if (scope == 0) {
//		for (unordered_map<KEY_ID, multimap<size_t, Entity> >::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
        for (vector< pair<KEY_ID, vector<pair<size_t,Entity> > >>::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
            if ( Molecules::provenanceTriger(it_p->first) ) {
                diplo::ProvFilterCH = true;
                for (unordered_set<size_t>::iterator tripleTmpl =
                            tripleTemplates->begin();
                        tripleTmpl != tripleTemplates->end(); tripleTmpl++) {
//				pair<multimap<size_t, Entity>::iterator, multimap<size_t, Entity>::iterator> it = it_p->second.equal_range(*tripleTmpl);
//				for (multimap<size_t, Entity>::iterator i = it.first; i != it.second; i++) {
                    for (vector<pair<size_t, Entity> >::iterator i = it_p->second.begin(); i != it_p->second.end(); i++) {
					  diplo::elementsChecked++;
                        if (i->first == *tripleTmpl) {
                            if (i->second.obj == o or o == -1) {
//				cout << "\t------> " << i->first << " == " << tripleTmpl << "\t" << i->second.obj << " == " << o << endl;
                                ret = true;
                                prov->insert(it_p->first);
                            }
                        }
                    }
                }
            } else diplo::elementsChecked++;
        }
    } else {
        for (vector< pair<KEY_ID, vector<pair<size_t,Entity> > >>::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
            for (vector<pair<size_t,Entity> >::iterator it_next = it_p->second.begin(); it_next != it_p->second.end(); it_next++) {
                return it_next->second.CheckIfEntityExists(tripleTemplates, o, scope - 1, prov);
            }
        }
    }

    return ret;
}

void Molecule::GetObjects(size_t tripleTmpl, set<KEY_ID>* result)
{
//FIXME make it for scopes!!
//FIXME prov
	for (vector< pair<KEY_ID, vector<pair<size_t,Entity> > >>::iterator it_p =
			next.begin(); it_p != next.end(); it_p++) {
//		pair<multimap<size_t, Entity>::iterator,
//				multimap<size_t, Entity>::iterator> i =
//				it_p->second.equal_range(tripleTmpl);
//		for (vector<pair<size_t,Entity>::iterator it = i.first; it != i.second;
//				it++)
		if (Molecules::provenanceTriger(it_p->first)) {
			diplo::ProvFilterCH = true;
			for (vector<pair<size_t, Entity> >::iterator it =
					it_p->second.begin(); it != it_p->second.end(); it++) {
				result->insert(it->second.obj);
			}
		}
	}
}

void Molecule::Display() {
	//FIXME make it for scopes!!
	//FIXME prov
	for (vector< pair<KEY_ID, vector<pair<size_t,Entity> > >>::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
		cout << "-" << it_p->first << "\t" << diplo::KM.Get(it_p->first) << " [prov] " << endl;
		for (vector<pair<size_t,Entity> >::iterator it_next = it_p->second.begin(); it_next != it_p->second.end(); it_next++) {
			cout << "--" << it_next->first << "\t" << diplo::KM.Get(it_next->second.obj) << endl;

			it_next->second.Display();
		}
	}
}

void Molecule::GetEntity(unordered_set<size_t>* tripleTemplates, KEY_ID s, unsigned short int scope, unordered_set<KEY_ID>* result, unordered_set<KEY_ID>* prov) {
	//FIXME prov

	if (scope == 0) {
		for (unordered_set<size_t>::iterator templ = tripleTemplates->begin(); templ != tripleTemplates->end(); templ++) {
			for (vector< pair<KEY_ID, vector<pair<size_t,Entity> > >>::iterator it_p = next.begin(); it_p != next.end(); it_p++) {
				if (Molecules::provenanceTriger(it_p->first)) {
					diplo::ProvFilterCH = true;
					for (vector<pair<size_t, Entity> >::iterator i =
							it_p->second.begin(); i != it_p->second.end();
							i++) {
					  diplo::elementsRetrieved++;
						if (i->first == *templ) {
//				pair<multimap<size_t, Entity>::iterator, multimap<size_t, Entity>::iterator> it = it_p->second.equal_range(*templ);
//						for (multimap<size_t, Entity>::iterator i = it.first;
//								i != it.second; i++) {
							result->insert(i->second.obj);
							prov->insert(it_p->first);
//				cout << "\t------> " << diplo::KM.Get (i->second.obj ) << endl;
//						}
						}
					}
				} else 	diplo::elementsRetrieved++;
			}
		}
	} else {
		for (vector< pair<KEY_ID, vector<pair<size_t,Entity> > >>::iterator it_p =
				next.begin(); it_p != next.end(); it_p++) {
			for (vector<pair<size_t,Entity> >::iterator it_next = it_p->second.begin();
					it_next != it_p->second.end(); it_next++) {
				it_next->second.GetEntity(tripleTemplates, s, scope - 1, result, prov);
			}
		}
	}
}

void Molecule::GetAllEntities(unsigned short int scope, unordered_multimap<size_t, KEY_ID>* result, KEY_ID s) {
	//FIXME prov
	if (scope == 0) {
		for (vector< pair<KEY_ID, vector<pair<size_t,Entity> > >>::iterator it_p =
				next.begin(); it_p != next.end(); it_p++) {
			if (Molecules::provenanceTriger(it_p->first)) {
				diplo::ProvFilterCH = true;
				for (vector<pair<size_t, Entity> >::iterator it_next =
						it_p->second.begin(); it_next != it_p->second.end();
						it_next++) {
					result->insert(
							pair<size_t, KEY_ID>(it_next->first,
									it_next->second.obj));
//				cout << "\t------> " << diplo::KM.Get (i->second.obj ) << endl;
				}
			}
		}
	} else {
		for (vector< pair<KEY_ID, vector<pair<size_t,Entity> > >>::iterator it_p =
				next.begin(); it_p != next.end(); it_p++) {
			for (vector<pair<size_t,Entity> >::iterator it_next =
					it_p->second.begin(); it_next != it_p->second.end();
					it_next++) {
				it_next->second.GetAllEntities(scope - 1, result, s);
			}
		}
	}

}

Molecule::~Molecule() {
}

void Molecule::AddTriple(size_t tripleTmpl, KEY_ID o, KEY_ID prov) {
	//FIXME prov
//	next.insert( pair<size_t, Entity> ( tripleTmpl, Entity(o) ) );

for (vector< pair<KEY_ID, vector<pair<size_t,Entity> >>>::iterator i = next.begin(); i != next.end(); i++) {
	if (  i->first== prov) {
		i->second.reserve(i->second.capacity()+1);
		i->second.push_back(pair<size_t, Entity>(tripleTmpl, Entity(o)));
		return;
	}
}


vector<pair<size_t,Entity> > tmp;
	tmp.push_back(pair<size_t, Entity>(tripleTmpl, Entity(o)));
	next.reserve(next.capacity()+1);
	next.push_back( pair< KEY_ID, vector<pair<size_t,Entity> > > (prov,tmp) );

//	unordered_map< KEY_ID, multimap<size_t,Entity> >::iterator i = next.find(prov);
//	if (i != next.end())
//		i->second.insert(pair<size_t, Entity>(tripleTmpl, Entity(o)));
//	else {
//		multimap<size_t,Entity> tmp;
//		tmp.insert(pair<size_t, Entity>(tripleTmpl, Entity(o)));
//		next.insert( pair< KEY_ID, multimap<size_t,Entity> > (prov,tmp) );
//	}
	return;

}

Molecule::Molecule() {
}

void Molecule::FindEntityAndAddNext(size_t tripleTmpl, KEY_ID o, KEY_ID s, KEY_ID prov, unsigned short int scope) {
//	if (scope == 1 ) {
//		cerr << s << "\t" << diplo::KM.Get(s) << endl;
//	}

	//FIXME prov
	for (vector< pair<KEY_ID, vector<pair<size_t,Entity> > >>::iterator it_p =
			next.begin(); it_p != next.end(); it_p++) {
		for (vector<pair<size_t,Entity> >::iterator it_next = it_p->second.begin();
				it_next != it_p->second.end(); it_next++) {
			it_next->second.FindEntityAndAddNext(tripleTmpl, o, s, prov, scope,
					this);
		}
	}
}

bool Molecules::provenanceTriger(KEY_ID oid) {
	if (diplo::ProvTrigerON == false) return true;
	bool excludeInclude = false;
	/* true - exclude those form the list
	 * false - include only those from the list
	 */
//	cerr << "provenanceTriger -> ProvUris.size(): " << diplo::ProvUris.size() << "\t" << oid << "\t--" << diplo::KM.Get(oid) << "-- " << endl;
	if (diplo::ProvUris.find(oid) == diplo::ProvUris.end())
		return excludeInclude;
	else {
		return !excludeInclude;
	}
}

void Molecules::MaterializeMoleculesForProvenance(Molecules &M2) {
	diplo::ProvIdx.clear();
	for (unordered_map<KEY_ID, Molecule >::iterator it = molecules.begin(); it != molecules.end(); it++) {
		it->second.MaterializeMoleculesForProvenance(it->first, M2);
	}
}

void Molecule::MaterializeMoleculesForProvenance(KEY_ID molec, Molecules &M2) {
	for (vector<pair<KEY_ID, vector<pair<size_t, Entity> > >>::iterator it = next.begin(); it != next.end(); it++) {
		if (Molecules::provenanceTriger(it->first)) {
			for (vector<pair<size_t, Entity> >::iterator it_next = it->second.begin(); it_next != it->second.end(); it_next++) {
				M2.ReconstructMolecule(molec, it_next->first, it_next->second.obj, it->first);
			}
		}
	}
}
