/*
 * API.cpp
 *
 *  Created on: Jul 28, 2013
 *      Author: marcin
 */

#include "API.h"

namespace queries {

API::API() {
    // TODO Auto-generated constructor stub

}

API::~API() {
    // TODO Auto-generated destructor stub
}

void API::TriplePatern(vector<TripleIDs>& constrains, vector<TripleIDs>& projections, vector<vector<unordered_set<KEY_ID>>> & resultsmaster, vector<vector<unordered_set<KEY_ID>>> & provmaster) {
    set<pair<const KEY_ID, Molecule>*> molecules;
    set<pair<const KEY_ID, Molecule>*>* Molecules_with_O = NULL;
//	cerr << "constrains[0].subject: " << constrains[0].subject << endl;
    if (constrains.size() > 0) {
        if (constrains[0].subject != 0) {
            pair<const KEY_ID, Molecule>* m = diplo::M.GetPair(constrains[0].subject);
            if (m) {
                molecules.insert(m);
                Molecules_with_O = &molecules;
//									cerr << "ok\t" << m->first << endl;
            }
        } else if (constrains[0].object != -1) {
            Molecules_with_O = diplo::M.GetMoleculesFromInvertedIdx(constrains[0].object);
        }
    } else {
        pair<const KEY_ID, Molecule>* m = diplo::M.GetPair(projections[0].subject);
        if (m) {
            molecules.insert(m);
            Molecules_with_O = &molecules;
//					cerr << "ok\t" << m->first << endl;
        }
    }

//
//	set<pair<const KEY_ID, Molecule>*>* Molecules_with_O = diplo::M.GetMoleculesFromInvertedIdx(constrains[0].object);


    if (Molecules_with_O) { // do we have any of this

        for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_O->begin(); it != Molecules_with_O->end(); it++) {

//             if (ProvTrigerON) {
//                 bool getit = false;
//                 for (auto p : ProvUris ) {
//                     auto ms = ProvIdx.find(p);
//                     if (ms == ProvIdx.end()) continue;
// 
//                     if (ms->second.find((*it)->first) != ms->second.end()) {
//                         getit = true;
//                         break;
//                     }
//                 }
//                 if (!getit) continue;
//             }
		  
            if (ProvTrigerON and !ProvMolecules.empty())
                if (ProvMolecules.find((*it)->first) == ProvMolecules.end())
                    continue;
		  
		  
		  
            //cerr << "molecules: " << diplo::KM.Get((*it)->first) << endl;
            diplo::ProvFilterCH = false;
            diplo::moleculesCounter++;
            unordered_set<size_t> tripleTemplates;
            vector<unordered_set<KEY_ID>> results;
            vector<unordered_set<KEY_ID> > prov;

            results.resize(projections.size()+1);
            prov.resize( constrains.size()+projections.size() );
            bool cont = false;
            for (size_t i = 0; i < constrains.size(); i++) {
                tripleTemplates.clear();
                TYPE_ID tt;
                if (constrains[i].object == -1) tt = -1;
                else tt = diplo::KM.GetType(constrains[i].object);

                diplo::TM.GetTemplates(diplo::KM.GetType((*it)->first), constrains[i].predicate, tt, 0, &tripleTemplates);

                (*it)->second.CheckIfEntityExists(&tripleTemplates, constrains[i].object, 0, &prov[i]); // FIXME get prov
                if (prov[i].size() <= 0 and constrains[i].prov == 0) {
//					cerr << "OK2 " << tripleTemplates.size() << endl;
                    cont = true;
                    break;
                }
            }
            if (cont)
                goto end;

            for (size_t i = 0; i < projections.size(); i++) {
                tripleTemplates.clear();
                diplo::TM.GetTemplates(diplo::KM.GetType((*it)->first), projections[i].predicate, -1, 0, &tripleTemplates);
                (*it)->second.GetEntity(&tripleTemplates, 0, 0, &results[i], &prov[i + constrains.size()]);  // FIXME get prov
//				cerr << "OK2 " << (*it)->first << "\t" << projections[i].predicate << "\t" << tripleTemplates.size() << endl;
                if (results[i].size() <= 0 and projections[i].prov == 0) {
                    cont = true;
                    break;
                }
            }
             if (cont)
                 goto end;



//			copy(results.begin(), results.end(), back_inserter(resultsmaster));
//			copy(prov.begin(), prov.end(), back_inserter(provmaster));
            resultsmaster.push_back(results);
            provmaster.push_back(prov);

            //	cout << "a-->\t" << (*it)->first << "\t" << diplo::KM.Get((*it)->first) << endl;

//			if ((*it)->first == 2097152001)
//				(*it)->second.Display();
end:
            if (diplo::ProvFilterCH) {
                diplo::ProvFilterCounter++;
                diplo::ProvFilterCH = false;
            }
        }
    }

}

void API::DrisplayResults(vector<vector<unordered_set<KEY_ID>>>& results) {
//     return;
    if (results.empty()) return;
    cout << endl;
    for (vector<vector<unordered_set<KEY_ID>>> ::iterator it_p = results.begin(); it_p != results.end(); it_p++) {
        for (vector<unordered_set<KEY_ID>>::iterator it1 = it_p->begin(); it1 != it_p->end(); it1++) {
//			cout << "size() -> " << it1->size() << endl;
            for (unordered_set<KEY_ID>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
                cout << diplo::KM.Get(*it2) << endl;
            }
        }
        cout << endl;
    }
    cout << endl;
}

void API::DrisplayProvenance(vector<vector<unordered_set<KEY_ID> > >& prov) {
//     return;
    unordered_set<KEY_ID> uniqProv;
    cout << endl;
    for (vector<vector<unordered_set<KEY_ID>>> ::iterator it_p = prov.begin(); it_p != prov.end(); it_p++) {
        cout << "[ ";
        for (vector<unordered_set<KEY_ID>>::iterator it = it_p->begin(); it != it_p->end(); it++) {
            cout << "( ";
            for (unordered_set<KEY_ID>::iterator it2 = it->begin(); it2 != it->end(); it2++) {
                uniqProv.insert(*it2);
                cout << diplo::KM.Get(*it2) << "[" << *it2 << "]" << " + ";
            }
//			cout << *it2 << "+";
            cout << " ) x ";
        }
        cout << " ] + " << endl;
    }
    cout << endl;


//	for (unordered_set<KEY_ID>::iterator it2 = uniqProv.begin(); it2 != uniqProv.end(); it2++) {
//		cout << "<http://mwylot.net/> <http://www.w3.org/ns/prov#wasAssociatedWith> " << diplo::KM.Get(*it2) << " <http://mwylot.net/> ." << endl;
//	}
}


bool API::PostFiltering(vector<vector<unordered_set<KEY_ID> > >& prov, vector<vector<unordered_set<KEY_ID>>>& results) {
 return true ;
    bool removed = false;
    vector<size_t> ids;
    for (size_t b = 0; b < prov.size(); b++) {
        bool include2 = true;
        for (size_t c = 0; c < prov[b].size(); c++) {
            bool include1 = false;
            for (unordered_set<KEY_ID>::iterator it2 = prov[b][c].begin(); it2 != prov[b][c].end(); it2++) {
                if (ProvUris.find((*it2)) != ProvUris.end()) {
                    include1 = true;
                }
//                 if (*it2 == 14465449852929 )  include1 = false;
// 				 if (*it2 == 103079215108 )  include1 = false;
            }
            if (!include1) {
                include2 = false;
            }
        }
        if (!include2) {
            ids.push_back(b);
        }
    }
    for (auto c : ids) {
        results.erase(results.begin() + c);
        prov.erase(prov.begin() + c);
        removed = true;
    }
    return removed;
}


void API::GetSubjects(KEY_ID o, KEY_ID p, unordered_multimap<KEY_ID,KEY_ID>& results) {
    unordered_set<KEY_ID> prov;
    unordered_set<size_t> tripleTemplates;
    set<pair<const KEY_ID, Molecule>*>* Molecules_with_O = diplo::M.GetMoleculesFromInvertedIdx(o);
    if (Molecules_with_O) { // do we have any of this
        for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_O->begin(); it != Molecules_with_O->end(); it++) {
		  
//             if (ProvTrigerON) {
//                 bool getit = false;
//                 for (auto p : ProvUris ) {
//                     auto ms = ProvIdx.find(p);
//                     if (ms == ProvIdx.end()) continue;
// 
//                     if (ms->second.find((*it)->first) != ms->second.end()) {
//                         getit = true;
//                         break;
//                     }
//                 }
//                 if (!getit) continue;
//             }
// 		  
            if (ProvTrigerON and !ProvMolecules.empty())
                if (ProvMolecules.find((*it)->first) == ProvMolecules.end())
                    continue;
		
            diplo::ProvFilterCH = false;
			diplo::moleculesCounter++;
            tripleTemplates.clear();
            prov.clear();
            diplo::TM.GetTemplates(diplo::KM.GetType((*it)->first), p, diplo::KM.GetType(o), 0, &tripleTemplates);
            if ( (*it)->second.CheckIfEntityExists(&tripleTemplates, o, 0, &prov) ) {  // FIXME get prov
//				if ( prov.size() != 1 ) cerr << "ups " << prov.size() << endl;
                for (unordered_set<KEY_ID>::iterator it_p = prov.begin(); it_p != prov.end(); it_p++) {
                    results.insert(pair<KEY_ID,KEY_ID>( (*it)->first, *prov.begin() ));
                }
//				return;
            }
            if (diplo::ProvFilterCH) {
                diplo::ProvFilterCounter++;
                diplo::ProvFilterCH = false;
            }
        }
    }

}

void API::GetSubjects(KEY_ID o, KEY_ID p, unordered_set<KEY_ID> &results, vector<unordered_set<KEY_ID>> &prov) {
    unordered_set<KEY_ID> prov_local;
    unordered_set<size_t> tripleTemplates;
    set<pair<const KEY_ID, Molecule>*>* Molecules_with_O = diplo::M.GetMoleculesFromInvertedIdx(o);
//	cerr << "Molecules_with_O->size(): " <<Molecules_with_O->size() << endl;
    if (Molecules_with_O) { // do we have any of this
//		cerr << "Molecules_with_O->size(); " << Molecules_with_O->size() << endl;
        for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_O->begin(); it != Molecules_with_O->end(); it++) {


//             if (ProvTrigerON) {
//                 bool getit = false;
//                 for (auto p : ProvUris ) {
//                     auto ms = ProvIdx.find(p);
//                     if (ms == ProvIdx.end()) continue;
// 
//                     if (ms->second.find((*it)->first) != ms->second.end()) {
//                         getit = true;
//                         break;
//                     }
//                 }
//                 if (!getit) continue;
//             }
                
	              if (ProvTrigerON and !ProvMolecules.empty())
                if (ProvMolecules.find((*it)->first) == ProvMolecules.end())
                    continue;
				
				
				
			diplo::moleculesCounter++;
            diplo::ProvFilterCH = false;
//			cerr << "A" << endl;
            tripleTemplates.clear();
//			prov.clear();
            diplo::TM.GetTemplates(diplo::KM.GetType((*it)->first), p, diplo::KM.GetType(o), 0, &tripleTemplates);
//			cerr << "B" << endl;
            if ( (*it)->second.CheckIfEntityExists(&tripleTemplates, o, 0, &prov_local) ) {  // FIXME get prov
//				cerr << "INSERT" << endl;

                results.insert((*it)->first);
                prov.push_back(prov_local);
//				if ( prov.size() != 1 ) cerr << "ups " << prov.size() << endl;
//				for (unordered_set<diplo::TripleIDs>::iterator it_p = prov.begin(); it_p != prov.end(); it_p++) {
//					results.insert(pair<KEY_ID,diplo::TripleIDs>( (*it)->first, *it_p));
//				}
//				return;
            }
            if (diplo::ProvFilterCH) {
                diplo::ProvFilterCounter++;
                diplo::ProvFilterCH = false;
            }
        }
    }

}

void API::GetObjects(KEY_ID s, KEY_ID p, unordered_set<KEY_ID> &results, unordered_set<KEY_ID> &prov) {
    Molecule *m = diplo::M.Get(s);
    unordered_set<size_t> tripleTemplates;
    if (m) {
        diplo::TM.GetTemplates(diplo::KM.GetType(s), p, -1, 0, &tripleTemplates);
//		unordered_set<KEY_ID> result4Geo;
//		unordered_set<diplo::TripleIDs> prov;
        m->GetEntity(&tripleTemplates,s,0,&results, &prov);
//		results.insert(unordered_multimap<KEY_ID, diplo::TripleIDs>());
//		cout << "city:\t" << diplo::KM.Get(s) << "\t" << diplo::KM.GetType(s);
//		cerr << "\tOK\t" << results.size() << endl;
//		for (unordered_set<KEY_ID>::iterator it = results.begin(); it != results.end(); it++) {
//			cout << "\t\t---> " << diplo::KM.Get(*it) << endl;
//		}
    }
}



} /* namespace queries */
