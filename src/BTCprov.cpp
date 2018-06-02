/*
 * BTCprov.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: marcin
 */

#include "BTCprov.h"

namespace queries {

	
BTCprov::BTCprov() {
//	provOutput.open(diplo::statsDir+"provOutput");
    // TODO Auto-generated constructor stub

}

BTCprov::~BTCprov() {
    // TODO Auto-generated destructor stub
}


void BTCprov::mixer() {
}


void BTCprov::TPDemo() {

	if (!diplo::file_p.empty()) {
		vector<TripleIDs> constrains;
		vector<TripleIDs> projections;
		vector<TripleIDs> *ptr = &constrains;
		vector<vector<unordered_set<KEY_ID>>> results;
		vector<vector<unordered_set<KEY_ID>>> prov;
		
		ifstream f(diplo::file_p);
		string line;
		
		while (getline(f, line)) {
			// 		cout << line << endl;
			if (line == "XXX") {ptr = &projections; continue;}
			vector < string > triple;
			tokenize(line, " ", triple);
			
			KEY_ID s = diplo::KM.Get(triple[0].c_str()); //s = s == 0 ? -1 : s;
			KEY_ID p = diplo::KM.Get(triple[1].c_str());
			KEY_ID o = diplo::KM.Get(triple[2].c_str());
			
			ptr->push_back(TripleIDs(s,p,o,0));
		}
		
		API::TriplePatern(constrains, projections, results, prov);
		
		for (vector<vector<unordered_set<KEY_ID>>> ::iterator it_p = results.begin(); it_p != results.end(); it_p++)
			for (vector<unordered_set<KEY_ID>>::iterator it1 = it_p->begin(); it1 != it_p->end(); it1++)
				for (unordered_set<KEY_ID>::iterator it2 = it1->begin(); it2 != it1->end(); it2++)
					ProvUris.insert(*it2);
		
	diplo::ProvTrigerON = true;	
	}
	

	vector<TripleIDs> constrains;
	vector<TripleIDs> projections;
	vector<TripleIDs> *ptr = &constrains;
	vector<vector<unordered_set<KEY_ID>>> results;
	vector<vector<unordered_set<KEY_ID>>> prov;
	
	ifstream f(diplo::file_q);
	string line;
	
	while (getline(f, line)) {
// 		cout << line << endl;
		if (line == "XXX") {ptr = &projections; continue;}
		vector < string > triple;
		tokenize(line, " ", triple);
		
		KEY_ID s = diplo::KM.Get(triple[0].c_str()); //s = s == 0 ? -1 : s;
		KEY_ID p = diplo::KM.Get(triple[1].c_str());
		KEY_ID o = diplo::KM.Get(triple[2].c_str());
		
		ptr->push_back(TripleIDs(s,p,o,0));
	}
	
	API::TriplePatern(constrains, projections, results, prov);
	
	
	cout << endl << "\033[1;31m" ;
	for (int i = 0; i < results.size(); i++) {
		for (int k = 0; k < results[i].size(); k++) {
			for (auto it : results[i][k]) {
				
				for (int m = k+1; m < results[i].size(); m++) {
					for (auto it2 : results[i][m]) {
						cout << diplo::KM.Get(it) << "\t" << diplo::KM.Get(it2) << endl;
					}
				}
				
				
				
			}
		}
		cout << endl;
	}
	
	cout << "\033[0m" << endl << "\033[1;34m";
	
	for (vector<vector<unordered_set<KEY_ID>>> ::iterator it_p = prov.begin(); it_p != prov.end(); it_p++) {
		cout << "[ ";
		for (vector<unordered_set<KEY_ID>>::iterator it = it_p->begin(); it != it_p->end(); it++) {
			cout << "( ";
			for (unordered_set<KEY_ID>::iterator it2 = it->begin(); it2 != it->end(); it2++) {
// 				cout << diplo::KM.Get(*it2) << "[" << *it2 << "]" << " + ";
				cout << diplo::KM.Get(*it2) <<  " + ";
			}
			//			cout << *it2 << "+";
			cout << " ) x " << endl;
		}
		cout << " ] + " << endl << endl;
	}
	cout << "\033[0m" << endl;
}


void BTCprov::q01() {
  

    /*
     * select ?lat ?long
    where {
    ?a [] "Eiffel Tower".
    ?a <http://www.geonames.org/ontology#inCountry> <http://www.geonames.org/countries/#FR>.
    ?a <http://www.w3.org/2003/01/geo/wgs84_pos#lat> ?lat.
    ?a <http://www.w3.org/2003/01/geo/wgs84_pos#long> ?long. }

    //Prov query
    select ?s where {
    ?s <http://www.geonames.org/ontology#wikipediaArticle> <http://af.wikipedia.org/wiki/Eiffel-toring> .
    }
     */


    KEY_ID EiffelTower = diplo::KM.Get("\"Eiffel Tower\"");

    KEY_ID inCountry = diplo::KM.Get("<http://www.geonames.org/ontology#inCountry>");
    KEY_ID fr = diplo::KM.Get("<http://www.geonames.org/countries/#FR>");

    KEY_ID lat = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#lat>");
    KEY_ID llong = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#long>");


    vector<TripleIDs> constrains;
    KEY_ID name = diplo::KM.Get("<http://www.geonames.org/ontology#name>");
    constrains.push_back(TripleIDs(0,-1,EiffelTower,0));
    constrains.push_back(TripleIDs(0,inCountry,fr,0));

    vector<TripleIDs> projections;
    projections.push_back(TripleIDs(0,lat,0,0));
    projections.push_back(TripleIDs(0,llong,0,0));


    vector<vector<unordered_set<KEY_ID>>> results;
    vector<vector<unordered_set<KEY_ID>>> prov;

    API::TriplePatern(constrains, projections, results, prov);

	
	 API::PostFiltering(prov, results);
	 
	 
#ifdef DISPLAY_RESULTS
    API::DrisplayResults(results);
    API::DrisplayProvenance(prov);
#endif

#ifdef SHOW_STATS
    unordered_set<KEY_ID> ps;
    size_t rs=0;
    for (auto it_p : prov) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    for (auto it_p : results) for (auto it : it_p ) rs+=it.size();
    PrintStats
#endif
    ProvOut
}

void BTCprov::q02() {
    /*
    select ?b ?p ?bn
    where {
    ?a [] "Tim Berners-Lee".
    ?a <http://dbpedia.org/property/dateOfBirth> ?b.
    ?a <http://dbpedia.org/property/placeOfBirth> ?p.
    ?a <http://dbpedia.org/property/name> ?bn. }


    //Prov query
    select ?s where {
    ?s <http://dbpedia.org/property/website> <http://www.w3.org/> .
    }
    <http://dbpedia.org/resource/Tim_Berners-Lee>
    */


    KEY_ID tbl = diplo::KM.Get("\"Tim Berners-Lee\"");

    KEY_ID dateOfBirth = diplo::KM.Get("<http://dbpedia.org/property/dateOfBirth>");
    KEY_ID placeOfBirth = diplo::KM.Get("<http://dbpedia.org/property/placeOfBirth>");
    KEY_ID name = diplo::KM.Get("<http://dbpedia.org/property/name>");


    vector<TripleIDs> constrains;
    constrains.push_back(TripleIDs(0,-1,tbl,0));

    vector<TripleIDs> projections;
    projections.push_back(TripleIDs(0,dateOfBirth,0,0));
    projections.push_back(TripleIDs(0,placeOfBirth,0,0));
    projections.push_back(TripleIDs(0,name,0,0));


    vector<vector<unordered_set<KEY_ID>>> results;
    vector<vector<unordered_set<KEY_ID>>> prov;

    API::TriplePatern(constrains, projections, results, prov);
	
	 API::PostFiltering(prov, results);

#ifdef DISPLAY_RESULTS
    API::DrisplayResults(results);
    API::DrisplayProvenance(prov);
#endif

#ifdef SHOW_STATS
    unordered_set<KEY_ID> ps;
    size_t rs=0;
    for (auto it_p : prov) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    for (auto it_p : results) for (auto it : it_p ) rs+=it.size();
    PrintStats
#endif
    ProvOut
}

void BTCprov::q03() {
    /*
    select ?t ?lat ?long
    where {
    ?a <http://dbpedia.org/property/wikilink> <http://dbpedia.org/resource/List_of_World_Heritage_Sites_in_Europe>.
    ?a <http://dbpedia.org/property/wikilink> <http://dbpedia.org/resource/Middle_Ages> .
    ?a <http://dbpedia.org/property/title> ?t.
    ?a <http://www.w3.org/2003/01/geo/wgs84_pos#lat> ?lat.
    ?a <http://www.w3.org/2003/01/geo/wgs84_pos#long> ?long.
     }

    //Prov query
    select ?s where {
    ?a <http://www.w3.org/ns/prov#SoftwareAgent> <imagination> .
    ?a <http://www.w3.org/ns/prov#activity> <Activity1123> .
    ?a <http://www.w3.org/ns/prov#atLocation> <http://dbpedia.org/resource/Fribourg> .

    ?a <http://www.w3.org/ns/prov#generated> ?s .
    ?a <http://www.w3.org/ns/prov#wasAssociatedWith> ?s .

    }
    */


    KEY_ID List = diplo::KM.Get("<http://dbpedia.org/resource/List_of_World_Heritage_Sites_in_Europe>");
    KEY_ID Middle_Ages = diplo::KM.Get("<http://dbpedia.org/resource/Middle_Ages>");

    KEY_ID wikilink = diplo::KM.Get("<http://dbpedia.org/property/wikilink>");

    KEY_ID title = diplo::KM.Get("<http://dbpedia.org/property/title>");
    KEY_ID lat = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#lat>");
    KEY_ID llong = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#long>");

    vector<TripleIDs> constrains;
    constrains.push_back(TripleIDs(0, wikilink, List, 0));
    constrains.push_back(TripleIDs(0, wikilink, Middle_Ages, 0));

    vector<TripleIDs> projections;
    projections.push_back(TripleIDs(0, title, 0, 0));
    projections.push_back(TripleIDs(0, lat, 0, 0));
    projections.push_back(TripleIDs(0, llong, 0, 0));

    vector<vector<unordered_set<KEY_ID>>> results;
    vector<vector<unordered_set<KEY_ID>>> prov;

    API::TriplePatern(constrains, projections, results, prov);

	
	
   API::PostFiltering(prov, results);

	
#ifdef DISPLAY_RESULTS
    API::DrisplayResults(results);
    API::DrisplayProvenance(prov);
#endif

#ifdef SHOW_STATS
    unordered_set<KEY_ID> ps;
    size_t rs=0;
    for (auto it_p : prov) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    for (auto it_p : results) for (auto it : it_p ) rs+=it.size();
    PrintStats
#endif

    ProvOut
}

void BTCprov::q04() {
    /*
    select *
    where {
    ?p <http://dbpedia.org/property/name> "Krebs, Emil".
    ?p <http://dbpedia.org/property/deathPlace> ?l.

    ?c [] ?l.
    ?c <http://www.geonames.org/ontology#featureClass> <http://www.geonames.org/ontology#P>.
    ?c <http://www.geonames.org/ontology#inCountry> <http://www.geonames.org/countries/#DE>.
    ?c <http://www.w3.org/2003/01/geo/wgs84_pos#lat> ?lat.
    ?c <http://www.w3.org/2003/01/geo/wgs84_pos#long> ?long.
    }


    //Prov query
    select ?s where {
    ?a <http://dbpedia.org/property/reference> <http://www.swr.de/wiesoweshalbwarum/archiv/2005/03/10/beitrag1.html> .
    ?a <http://dbpedia.org/property/sterbedatum> <http://dbpedia.org/resource/31._M??rz> .
    ?a <http://dbpedia.org/property/wikilink> <http://dbpedia.org/resource/sinologist> .
    }
     */





    vector<vector<vector<unordered_set<KEY_ID>>>> results;
    vector<vector<vector<unordered_set<KEY_ID>>>> prov;

    KEY_ID Krebs = diplo::KM.Get("\"Krebs, Emil\"");
    KEY_ID name = diplo::KM.Get("<http://dbpedia.org/property/name>");

    KEY_ID deathPlace = diplo::KM.Get("<http://dbpedia.org/property/deathPlace>");

    KEY_ID featureClass = diplo::KM.Get("<http://www.geonames.org/ontology#featureClass>");
    KEY_ID P = diplo::KM.Get("<http://www.geonames.org/ontology#P>");
    KEY_ID inCountry = diplo::KM.Get("<http://www.geonames.org/ontology#inCountry>");
    KEY_ID DE = diplo::KM.Get("<http://www.geonames.org/countries/#DE>");

    KEY_ID lat = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#lat>");
    KEY_ID llong = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#long>");

    vector<vector<unordered_set<KEY_ID>>> results_local1;
    vector<vector<unordered_set<KEY_ID>>> prov_local1;

    {
        vector<TripleIDs> constrains;
        constrains.push_back(TripleIDs(0, name, Krebs, 0));

        vector<TripleIDs> projections;
        projections.push_back(TripleIDs(0, deathPlace, 0, 0));



        API::TriplePatern(constrains, projections, results_local1, prov_local1);


    }
#ifdef SHOW_STATS
    size_t is, im, imf;
#endif
    KEY_ID name2 = diplo::KM.Get("<http://www.geonames.org/ontology#name>");
    {
        vector<TripleIDs> constrains;

        for (vector<vector<unordered_set<KEY_ID>>> ::iterator it_p = results_local1.begin(); it_p != results_local1.end(); it_p++) {
            for (vector<unordered_set<KEY_ID>>::iterator it1 = it_p->begin(); it1 != it_p->end(); it1++) {
                for (unordered_set<KEY_ID>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
                    constrains.push_back(TripleIDs(0, -1, *it2, 0));
                }
            }
        }



        if (constrains.size() == 0 )
            constrains.push_back(TripleIDs(0, 0, 0, 0));
#ifdef SHOW_STATS
        is=constrains.size();
        im = diplo::moleculesCounter;
        imf = diplo::ProvFilterCounter;
#endif

        constrains.push_back(TripleIDs(0, featureClass, P, 0));
        constrains.push_back(TripleIDs(0, inCountry, DE, 0));


        vector<TripleIDs> projections;
        projections.push_back(TripleIDs(0, lat, 0, 0));
        projections.push_back(TripleIDs(0, llong, 0, 0));

        vector<vector<unordered_set<KEY_ID>>> results_local;
        vector<vector<unordered_set<KEY_ID>>> prov_local;

        API::TriplePatern(constrains, projections, results_local, prov_local);

        if (prov_local.size() > 0) {
            results.push_back(results_local1);
            prov.push_back(prov_local1);

            results.push_back(results_local);
            prov.push_back(prov_local);
        }

    }

    for (size_t a = 0; a < prov.size(); a++)
        API::PostFiltering(prov[a], results[a]);

    
#ifdef DISPLAY_RESULTS
    for (vector<vector<vector<unordered_set<KEY_ID>>>> ::iterator it_p1 = results.begin(); it_p1 != results.end(); it_p1++) {
        API::DrisplayResults(*it_p1);
    }

    for (vector<vector<vector<unordered_set<KEY_ID>>>> ::iterator it_p1 = prov.begin(); it_p1 != prov.end(); it_p1++) {
        cout << "{";
        API::DrisplayProvenance(*it_p1);
        cout << "} X ";
    }
    cout << endl;
#endif

#ifdef SHOW_STATS
    unordered_set<KEY_ID> ps;
    size_t rs=0;
    for (auto prov1 : prov) for (auto it_p : prov1) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    for (auto it_r : results) for (auto it_p : it_r) for (auto it : it_p ) rs+=it.size();
    PrintStats2
#endif
    ProvOut

}

/*
 * I've removed those lines form data source,
 * something what is supposed to be molecule is as a type and this cause an issue
 * <http://dbpedia.org/resource/Barack_Obama> <http://www.w3.org/2000/01/rdf-schema#subClassOf> <http://dbpedia.org/resource/American_politicians> <http://dbpedia.org/resource/American_politicians> .
 * <http://dbpedia.org/resource/Barack_Obama_background_whisper_campaign_and_media_controversy> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://dbpedia.org/resource/Barack_Obama> <http://dbpedia.org/resource/Barack_Obama_background_whisper_campaign_and_media_controversy> .
 *
 */
void BTCprov::q05() {
    /*
     select distinct ?l ?long ?lat
     where {
     ?a [] "Barack Obama".
     ?a <http://dbpedia.org/property/placeOfBirth> ?l.
     ?l <http://www.w3.org/2003/01/geo/wgs84 pos#lat> ?lat.
     ?l <http://www.w3.org/2003/01/geo/wgs84 pos#long> ?long. }


    //Prov query
    select ?s where {
    ?a <http://www.w3.org/ns/prov#SoftwareAgent> <imagination> .
    ?a <http://www.w3.org/ns/prov#activity> <Activity1123> .
    ?a <http://www.w3.org/ns/prov#atLocation> <http://dbpedia.org/resource/Fribourg> .

    ?a <http://www.w3.org/ns/prov#generated> ?s .
    ?a <http://www.w3.org/ns/prov#wasAssociatedWith> ?s .

    }
    */




    vector<vector<vector<unordered_set<KEY_ID>>> > results;
    vector<vector<vector<unordered_set<KEY_ID>>>> prov;
    vector<pair<vector<vector<unordered_set<KEY_ID>>>, vector<vector<unordered_set<KEY_ID>>>> > intermediate_res;

    KEY_ID Obama = diplo::KM.Get("\"Barack Obama\"");

    KEY_ID placeOfBirth = diplo::KM.Get("<http://dbpedia.org/property/placeOfBirth>");

    KEY_ID lat = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#lat>");
    KEY_ID llong = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#long>");
#ifdef SHOW_STATS
    size_t is, im, imf;
#endif

//  diplo::stopwatch_start();
    {

        KEY_ID name = diplo::KM.Get("<http://dbpedia.org/property/name>");
        vector<TripleIDs> constrains;
        constrains.push_back(TripleIDs(0, -1, Obama, 0));

        vector<TripleIDs> projections;
        projections.push_back(TripleIDs(0, placeOfBirth, 0, 0));

        vector<vector<unordered_set<KEY_ID>>> results_local;
        vector<vector<unordered_set<KEY_ID>>> prov_local;

        API::TriplePatern(constrains, projections, results_local, prov_local);

#ifdef SHOW_STATS
        is=0;
        im = diplo::moleculesCounter;
        imf = diplo::ProvFilterCounter;
        for (auto it_p : results_local) for (auto it : it_p ) is+=it.size();
#endif
        intermediate_res.push_back( pair<vector<vector<unordered_set<KEY_ID>>>, vector<vector<unordered_set<KEY_ID>>>>(results_local,prov_local) );

    }

// cout << "q05_1: " << diplo::stopwatch_get() << endl;

    {

        for (vector<pair<vector<vector<unordered_set<KEY_ID>>>, vector<vector<unordered_set<KEY_ID>>>> > ::iterator it_p1 = intermediate_res.begin(); it_p1 != intermediate_res.end(); it_p1++) {
            for (vector<vector<unordered_set<KEY_ID>>> ::iterator it_p = it_p1->first.begin(); it_p != it_p1->first.end(); it_p++) {
                for (vector<unordered_set<KEY_ID>>::iterator it1 = it_p->begin(); it1 != it_p->end(); it1++) {
                    for (unordered_set<KEY_ID>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {

//						cerr << diplo::KM.Get(*it2) << endl;
                        vector<TripleIDs> constrains;
                        constrains.push_back(TripleIDs(*it2, lat, -1, 0));
                        constrains.push_back(TripleIDs(*it2, llong, -1, 0));

                        vector<TripleIDs> projections;
                        projections.push_back(TripleIDs(*it2, lat, 0, 0));
                        projections.push_back(TripleIDs(*it2, llong, 0, 0));

                        vector<vector<unordered_set<KEY_ID>>> results_local;
                        vector<vector<unordered_set<KEY_ID>>> prov_local;

                        API::TriplePatern(constrains, projections, results_local, prov_local);
// 						cout << "q05_2: " << diplo::stopwatch_get() << endl;

                        if(results_local.size() > 0) {
                            results_local[0].insert(results_local[0].begin(), unordered_set<KEY_ID>( {*it2}) );
                            results.push_back(results_local);

                            prov.push_back(it_p1->second);
                            prov.push_back(prov_local);
// 							cout << "q05_3: " << diplo::stopwatch_get() << endl;

                        }
                    }
                }
            }
        }

    }

    for (size_t a = 0; a < prov.size(); a++)
        API::PostFiltering(prov[a], results[a]);
    
#ifdef DISPLAY_RESULTS
    for (vector<vector<vector<unordered_set<KEY_ID>>>> ::iterator it_p1 = results.begin(); it_p1 != results.end(); it_p1++) {
        API::DrisplayResults(*it_p1);
    }

    for (vector<vector<vector<unordered_set<KEY_ID>>>> ::iterator it_p1 = prov.begin(); it_p1 != prov.end(); it_p1++) {
        cout << "{";
        API::DrisplayProvenance(*it_p1);
        cout << "} X ";
    }
    cout << endl;
#endif
#ifdef SHOW_STATS
    unordered_set<KEY_ID> ps;
    size_t rs=0;
    for (auto prov1 : prov) for (auto it_p : prov1) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    for (auto it_r : results) for (auto it_p : it_r) for (auto it : it_p ) rs+=it.size();
    PrintStats2
#endif
    ProvOut
}

void BTCprov::q06() {
    /*
     select distinct ?d
     where {
     ?c <http://dbpedia.org/property/profession> <http://dbpedia.org/resource/Veterinarian> .
     ?b <http://www.geonames.org/ontology#inCountry> <http://www.geonames.org/countries/#US>.

     ?a <http://dbpedia.org/property/senators> ?c.
     ?a <http://www.w3.org/2002/07/owl#sameAs> ?b.

     ?a <http://dbpedia.org/property/name> ?d.


     }

    //prov
    SELECT ?s WHERE {
    ?s <http://www.w3.org/ns/prov#wasGeneratedBy> <http://mwylot.net/> .
    UNION
    ?s <http://www.w3.org/ns/prov#wasGeneratedBy> <http://pg.net/> .
    UNION
    ?s <http://www.w3.org/ns/prov#wasGeneratedBy> <http://pcm.net/> .
    }
     */


    KEY_ID profession = diplo::KM.Get("<http://dbpedia.org/property/profession>");
    KEY_ID Veterinarian = diplo::KM.Get("<http://dbpedia.org/resource/Veterinarian>");

    KEY_ID inCountry = diplo::KM.Get("<http://www.geonames.org/ontology#inCountry>");
    KEY_ID US = diplo::KM.Get("<http://www.geonames.org/countries/#US>");

    KEY_ID senators = diplo::KM.Get("<http://dbpedia.org/property/senators>");
    KEY_ID sameAs = diplo::KM.Get("<http://www.w3.org/2002/07/owl#sameAs>");
    KEY_ID name = diplo::KM.Get("<http://dbpedia.org/property/name>");

    unordered_multimap<KEY_ID, KEY_ID> results_US;
    unordered_multimap<KEY_ID, KEY_ID> results_Vet;
//  diplo::stopwatch_start();
    API::GetSubjects(US, inCountry, results_US);
// cout << "q06_1: " << diplo::stopwatch_get() << endl;
    API::GetSubjects(Veterinarian, profession, results_Vet);
// cout << "q06_2: " << diplo::stopwatch_get() << endl;

// // 	diplo::moleculesCounter = 0;diplo::ProvFilterCounter = 0; diplo::elementsChecked = 0; diplo::elementsRetrieved = 0;

    vector<vector<unordered_set<KEY_ID>>> ss;
//	PRINT_STATS("q06_a", results_Vet, ss)

    vector<vector<vector<unordered_set<KEY_ID>>> > results;
    vector<vector<vector<unordered_set<KEY_ID>>> > prov;

    vector<diplo::TripleIDs> projections;
    projections.push_back(TripleIDs(0, name, 0, 0));

    unordered_set<KEY_ID> pr;
#ifdef SHOW_STATS
    size_t is=results_US.size()*results_Vet.size();
    size_t im = diplo::moleculesCounter;
    size_t imf = diplo::ProvFilterCounter;
    size_t rs=0;
#endif
    for (unordered_multimap<KEY_ID, KEY_ID>::iterator it = results_Vet.begin(); it != results_Vet.end();) {
        set<pair<const KEY_ID, Molecule>*>* mV = diplo::M.GetMoleculesFromInvertedIdx(it->first);

        pr.clear();
        KEY_ID varVet = it->first;
        do {
            pr.insert(it->second);
            it++;
        } while (it != results_Vet.end() and varVet == it->first);
        vector<vector<unordered_set<KEY_ID>>> prov_localVet( {vector<unordered_set<KEY_ID>>( {pr})});


        if (mV == NULL) continue;
        unordered_set<KEY_ID> mV2;
        for(auto vet : *mV) {
            mV2.insert (vet->first);
        }


// cout << "q06_3: " << diplo::stopwatch_get() << endl;
        for (unordered_multimap<KEY_ID, KEY_ID>::iterator it2 = results_US.begin(); it2 != results_US.end();) {
            set<pair<const KEY_ID, Molecule>*>* mU = diplo::M.GetMoleculesFromInvertedIdx(it2->first);
            pr.clear();
            KEY_ID varUS = it2->first;
            do {
                pr.insert(it2->second);
                it2++;
            } while (it2 != results_US.end() and varUS == it2->first);
            vector<vector<unordered_set<KEY_ID>>> prov_localUS( {vector<unordered_set<KEY_ID>>( {pr})});


            if (mU == NULL) continue;

            for(auto u : *mU) {
                if ( mV2.find(u->first) != mV2.end()) {

                    vector<TripleIDs> constrains;
                    constrains.push_back(TripleIDs(u->first, senators, varVet, 0));
                    constrains.push_back(TripleIDs(u->first, sameAs, varUS, 0));
                    vector<vector<unordered_set<KEY_ID>>> results_local;
                    vector<vector<unordered_set<KEY_ID>>> prov_local;

                    API::TriplePatern(constrains, projections, results_local, prov_local);
// cout << "q06_4: " << diplo::stopwatch_get() << endl;
                    if (results_local.size() > 0) {
                        prov.push_back(prov_localVet);
                        prov.push_back(prov_localUS);
                        prov.push_back(prov_local);
                        results.push_back(results_local);
// 				cout << "q06_5: " << diplo::stopwatch_get() << endl;
                    }
                }
            }
        }
    }
    
        for (size_t a = 0; a < prov.size(); a++)
        API::PostFiltering(prov[a], results[a]);
    
#ifdef DISPLAY_RESULTS
    for (vector<vector<vector<unordered_set<KEY_ID>>> > ::iterator it_p1 = results.begin(); it_p1 != results.end(); it_p1++) {
        API::DrisplayResults(*it_p1);
    }

    for (vector<vector<vector<unordered_set<KEY_ID>>> > ::iterator it_p1 = prov.begin(); it_p1 != prov.end(); it_p1++) {
        cout << "{";
        API::DrisplayProvenance(*it_p1);
        cout << "} X ";
    }
    cout << endl;
#endif
#ifdef SHOW_STATS
    unordered_set<KEY_ID> ps;
    for (auto prov1 : prov) for (auto it_p : prov1) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    for (auto it_r : results) for (auto it_p : it_r) for (auto it : it_p ) rs+=it.size();
    PrintStats2
#endif
    ProvOut


}

/*
 * added
 <http://dbpedia.org/resource/Diane_Cilento> <http://dbpedia.org/property/placeOfBirth> <http://dbpedia.org/resource/Scotland> <http://mwylot.net> .
 <http://dbpedia.org/resource/Hilton_McRae> <http://dbpedia.org/property/placeOfBirth> <http://dbpedia.org/resource/Scotland> <http://mwylot.net> .
 *
 */
void BTCprov::q07() {
    /*
     Q7:
     select distinct ?a ?b ?lat ?long
     where {

     ?a <http://dbpedia.org/property/wikilink> <http://dbpedia.org/resource/actor>.
     ?a <http://dbpedia.org/property/spouse> ?b.
     ?a <http://dbpedia.org/property/placeOfBirth> ?c.
     ?b <http://dbpedia.org/property/placeOfBirth> ?c.
     ?b <http://dbpedia.org/property/wikilink> dbpediares:actor.

     ?c <http://www.w3.org/2002/07/owl#sameAs> ?c2.

     ?c2 <http://www.w3.org/2003/01/geo/wgs84 pos#lat> ?lat.
     ?c2 <http://www.w3.org/2003/01/geo/wgs84 pos#long> ?long. }

    //prov
    SELECT ?s WHERE {
    ?s <http://www.w3.org/ns/prov#invalidated> <true> .
    UNION
    {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://dbpedia.org/resource/Australian_stage_actors>}
    }
     */


    KEY_ID actor = diplo::KM.Get("<http://dbpedia.org/resource/actor>");
    KEY_ID spouse = diplo::KM.Get("<http://dbpedia.org/property/spouse>");
    KEY_ID placeOfBirth = diplo::KM.Get("<http://dbpedia.org/property/placeOfBirth>");
    KEY_ID wikilink = diplo::KM.Get("<http://dbpedia.org/property/wikilink>");
    KEY_ID sameAs = diplo::KM.Get("<http://www.w3.org/2002/07/owl#sameAs>");
    KEY_ID lat = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#lat>");
    KEY_ID llong = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#long>");

    unordered_multimap<KEY_ID, pair<KEY_ID,KEY_ID> > place_spouses;

    unordered_map<KEY_ID, unordered_map<KEY_ID, unordered_set<KEY_ID>> > place_spouses2;

    vector<vector<vector<unordered_set<KEY_ID> > > > prov_all;


#ifdef SHOW_STATS
    size_t is=0;
    size_t im = 0;
    size_t rs=0;
    size_t imf = 0;
    size_t last_p = 0;

#endif
// diplo::stopwatch_start();


    {
        vector<vector<unordered_set<KEY_ID> > > prov;
        set<pair<const KEY_ID, Molecule>*>* Molecules_with_O = diplo::M.GetMoleculesFromInvertedIdx(actor);

        for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_O->begin(); it != Molecules_with_O->end(); it++) {
            if (ProvTrigerON and !ProvMolecules.empty())
                if (ProvMolecules.find((*it)->first) == ProvMolecules.end())
                    continue;

            unordered_set<size_t> tripleTemplates;
            unordered_set<KEY_ID> results_spouse;
            unordered_set<KEY_ID> results_placeOfBirth;
            unordered_set<KEY_ID> prov_A1;
#ifdef SHOW_STATS
            im++;
            ProvFilterCH = false;
#endif
            tripleTemplates.clear();
            diplo::TM.GetTemplates(diplo::KM.GetType((*it)->first), wikilink, diplo::KM.GetType(actor), 0, &tripleTemplates);

            if ((*it)->second.CheckIfEntityExists(&tripleTemplates, actor, 0, &prov_A1) ) { // FIXME get prov
#ifdef SHOW_STATS
                diplo::ProvFilterCounter++;
                ProvFilterCH = false;
#endif
                unordered_set<KEY_ID> prov_spouse;
                tripleTemplates.clear();
                diplo::TM.GetTemplates(diplo::KM.GetType((*it)->first), spouse, -1, 0, &tripleTemplates);
                (*it)->second.GetEntity(&tripleTemplates, 0, 0, &results_spouse, &prov_spouse);  // FIXME get prov

// cout << "q06_2: " << diplo::stopwatch_get() << endl;
                for (unordered_set<KEY_ID>::iterator it_spouse = results_spouse.begin(); it_spouse != results_spouse.end(); it_spouse++) {
                    if (ProvTrigerON and !ProvMolecules.empty())
                        if (ProvMolecules.find(*it_spouse) == ProvMolecules.end())
                            continue;
                    Molecule *m = diplo::M.Get(*it_spouse);
                    if (m) {
#ifdef SHOW_STATS
                        im++;
#endif

                        unordered_set<KEY_ID> prov_A2;
                        tripleTemplates.clear();
                        diplo::TM.GetTemplates(diplo::KM.GetType(*it_spouse), wikilink, diplo::KM.GetType(actor), 0, &tripleTemplates);
                        if (m->CheckIfEntityExists(&tripleTemplates, actor, 0, &prov_A2)) {
#ifdef SHOW_STATS
                            diplo::ProvFilterCounter++;
                            ProvFilterCH = false;
#endif
                            unordered_set<KEY_ID> prov_placeOfBirth;
                            tripleTemplates.clear();
                            diplo::TM.GetTemplates(diplo::KM.GetType((*it)->first), placeOfBirth, -1, 0, &tripleTemplates);
                            (*it)->second.GetEntity(&tripleTemplates, 0, 0, &results_placeOfBirth, &prov_placeOfBirth);  // FIXME get prov
// cout << "q06_3: " << diplo::stopwatch_get() << endl;
                            for (unordered_set<KEY_ID>::iterator it_results_placeOfBirth = results_placeOfBirth.begin(); it_results_placeOfBirth != results_placeOfBirth.end();
                                    it_results_placeOfBirth++) {

                                unordered_set<KEY_ID> prov_placeOfBirth2;
                                tripleTemplates.clear();
                                diplo::TM.GetTemplates(diplo::KM.GetType(*it_spouse), placeOfBirth, diplo::KM.GetType(*it_results_placeOfBirth), 0, &tripleTemplates);
                                if (m->CheckIfEntityExists(&tripleTemplates, *it_results_placeOfBirth, 0, &prov_placeOfBirth2)) {
                                    vector<unordered_set<KEY_ID> > provl;
                                    pair<KEY_ID, KEY_ID> spouses((*it)->first, *it_spouse);
                                    place_spouses.insert(pair<KEY_ID, pair<KEY_ID, KEY_ID> >(*it_results_placeOfBirth, spouses));
                                    provl.push_back(prov_A1);
                                    provl.push_back(prov_spouse);
                                    provl.push_back(prov_A2);
                                    provl.push_back(prov_placeOfBirth);
                                    provl.push_back(prov_placeOfBirth2);
                                    prov.push_back(provl);

//							cout << "\t" << diplo::KM.Get((*it)->first) <<  "\t" << diplo::KM.Get(*it_spouse) <<  "\t" << diplo::KM.Get(*it_results_placeOfBirth) << endl;
                                }
// 								cout << "q06_4: " << diplo::stopwatch_get() << endl;
                            }
                        } else if (diplo::ProvFilterCH) {
                            diplo::ProvFilterCounter++;
                            ProvFilterCH = false;
                        }


                    }
                }
            } else if (diplo::ProvFilterCH) {
                diplo::ProvFilterCounter++;
                ProvFilterCH = false;
            }
        }
        prov_all.push_back(prov);
    }
    //cerr << "1: " << diplo::stopwatch_get();
#ifdef SHOW_STATS
    imf = ProvFilterCounter;
#endif

// cout << "q07_XX: " << diplo::stopwatch_get() << endl;

    {
        vector<vector<unordered_set<KEY_ID> > > prov;
        for (unordered_multimap<KEY_ID, pair<KEY_ID, KEY_ID> >::iterator it = place_spouses.begin(); it != place_spouses.end(); it++) {
//
            unordered_set<KEY_ID> results_sameAs;
            unordered_set<KEY_ID> prov_sameAs;
            API::GetObjects(it->first, sameAs, results_sameAs, prov_sameAs);

            for (unordered_set<KEY_ID>::iterator it2 = results_sameAs.begin(); it2 != results_sameAs.end(); it2++) {
//			cout << "\t" << diplo::KM.Get( *it2 ) << endl;
//		}
                vector<vector<unordered_set<KEY_ID>>> results_geo;
                vector<vector<unordered_set<KEY_ID>>> provmaster_geo;
                vector<TripleIDs> constrains;
                vector<TripleIDs> projections;
                projections.push_back(TripleIDs(*it2, lat, 0, 0));
                projections.push_back(TripleIDs(*it2, llong, 0, 0));
                API::TriplePatern(constrains, projections, results_geo, provmaster_geo);
#ifdef SHOW_STATS
                is++;
#endif
//	cout << "\t" << diplo::KM.Get( *it2 ) << "\tprovmaster_geo.size(): " << provmaster_geo.size() << endl;
                if ( !results_geo.empty() ) {
                    vector<unordered_set<KEY_ID> > provl;
                    provmaster_geo[0].push_back(prov_sameAs);
                    prov.push_back(provmaster_geo[0]);
#ifdef DISPLAY_RESULTS
                    cout << diplo::KM.Get( it->first ) << "\t" << diplo::KM.Get(it->second.first) << "\t" << diplo::KM.Get(it->second.first) << endl;
                    API::DrisplayResults(results_geo);
#endif
#ifdef SHOW_STATS
                    rs+=3;
                    for (auto it_p : results_geo) for (auto it : it_p ) rs+=it.size();
#endif
                }
            }
        }
        prov_all.push_back(prov);
    }
    //cerr << "1: " << diplo::stopwatch_get();
// 	cout << "q06_2: " << diplo::stopwatch_get() << endl;

#ifdef DISPLAY_RESULTS
    for (vector<vector<vector<unordered_set<KEY_ID>>> >::iterator it_p1 = prov_all.begin(); it_p1 != prov_all.end(); it_p1++) {
        cout << "{";
        API::DrisplayProvenance(*it_p1);
        cout << "} X ";
    }
    cout << endl;
#endif

#ifdef SHOW_STATS
    unordered_set<KEY_ID> ps;
    for (auto prov1 : prov_all) for (auto it_p : prov1) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    moleculesCounter = moleculesCounter + im;
    PrintStats2
#endif
    ProvOut
}

void BTCprov::q08() {
    /*
     select ?a ?y
     where {

     ?b <http://purl.org/dc/elements/1.1/subject> "Blackwater".
     ?b [] ?n.

     ?a a <http://dbpedia.org/class/yago/Politician110451263>.

     ?a <http://xmlns.com/foaf/0.1/name> ?n.
     ?a <http://dbpedia.org/property/years> ?y.


      }

    //prov
    SELECT ?s WHERE {
    <http://dbpedia.org/resource/Stephen_Timms> <http://dbpedia.org/property/successor> ?s .
    <http://dbpedia.org/resource/Stephen_Timms> <http://dbpedia.org/property/wikilink> ?s .
    }
     */

    vector<vector<unordered_set<KEY_ID>>> resultsBlackwater;
    vector<vector<unordered_set<KEY_ID>>> provBlackwater;

    vector<vector<vector<unordered_set<KEY_ID>>>> results;
    vector<vector<vector<unordered_set<KEY_ID>>>> prov;

    KEY_ID subject = diplo::KM.Get("<http://purl.org/dc/elements/1.1/subject>");
    KEY_ID Blackwater = diplo::KM.Get("\"Blackwater\"");

    {
        vector<TripleIDs> constrains;
        constrains.push_back(TripleIDs(0, subject, Blackwater, 0));

        vector<TripleIDs> projections;
        projections.push_back(TripleIDs(0, -1, 0, 0));

        API::TriplePatern(constrains, projections, resultsBlackwater,
                          provBlackwater);


    }

#ifdef SHOW_STATS
    size_t is=0;
    size_t rs=0;
    size_t im = diplo::moleculesCounter;
    size_t imf = diplo::ProvFilterCounter;
#endif
//	API::DrisplayResults(resultsBlackwater);
//	API::DrisplayProvenance(provBlackwater);


    TYPE_ID Politician110451263_type_sq = diplo::KM.GetSeq(diplo::KM.Get("<http://dbpedia.org/class/yago/Politician110451263>"));
    Politician110451263_type_sq = diplo::KM.GetType(diplo::KM.Get("<http://dbpedia.org/resource/Paul_Boateng>")); //very nasty hack!!

    map<KEY_ID, class Node *> * molecules = diplo::KM.GetElementsOfType(Politician110451263_type_sq);

    KEY_ID years = diplo::KM.Get("<http://dbpedia.org/property/years>");
    KEY_ID name = diplo::KM.Get("<http://xmlns.com/foaf/0.1/name>");

//	cerr << "years " << years << "\t" <<  "name " << name << "\t" << molecules->size() << endl;


//for (map<KEY_ID, class Node *>::iterator it = molecules->begin(); it != molecules->end(); it++) {
    for (size_t it_p = 0; it_p < resultsBlackwater.size(); it_p++) {
        for (size_t it1 = 0; it1 < resultsBlackwater[it_p].size(); it1++) {
            for (unordered_set<KEY_ID>::iterator it2 = resultsBlackwater[it_p][it1].begin(); it2 != resultsBlackwater[it_p][it1].end(); it2++) {


                set<pair<const KEY_ID, Molecule>*>* m = diplo::M.GetMoleculesFromInvertedIdx(*it2);
                for (auto it : *m) {

                    vector<vector<unordered_set<KEY_ID>>> results_local;
                    vector<vector<unordered_set<KEY_ID>>> prov_local;


                    vector<TripleIDs> constrains;
                    constrains.push_back(TripleIDs(it->first, name, *it2, 0));

                    vector<TripleIDs> projections;
                    projections.push_back(TripleIDs(0, years, 0, 0));

                    API::TriplePatern(constrains, projections, results_local,prov_local);
#ifdef SHOW_STATS
                    is++;
#endif
// cout << "Time: " << diplo::stopwatch_get() << endl;
// cout << rs << "\t" << moleculesCounter << "\t" << diplo::ProvFilterCounter << "\t" << 0 << "\t" << im << "\t" << imf << "\t" << is << "\t" <<  diplo::elementsChecked << "\t" <<  diplo::elementsRetrieved <<  endl;

//if (diplo::KM.Get("<http://dbpedia.org/resource/Paul_Boateng>")== it->first) {
//cerr << "results_local.size(): " << results_local.size() <<endl;
//cerr << "\t " << KM.Get(it->first) << "\t " << KM.Get(*it2) <<endl;
//}
					
					API::PostFiltering(prov_local, results_local);
                    if (!results_local.empty() and KM.GetType(it->first) == Politician110451263_type_sq) {
					  
					  
                        prov.push_back(provBlackwater);
                        prov.push_back(prov_local);
#ifdef DISPLAY_RESULTS
                        API::DrisplayResults(results_local);
#endif
#ifdef SHOW_STATS
                        for (auto it_p : results_local) for (auto it : it_p ) rs+=it.size();
#endif
                    }

                }
            }
        }
    }
    //}
    
    
    
#ifdef DISPLAY_RESULTS
    for (vector<vector<vector<unordered_set<KEY_ID>>> > ::iterator it_p1 = prov.begin(); it_p1 != prov.end(); it_p1++) {
        cout << "{";
        API::DrisplayProvenance(*it_p1);
        cout << "} X ";
    }
    cout << endl;
#endif

#ifdef SHOW_STATS
    unordered_set<KEY_ID> ps;
    for (auto prov1 : prov) for (auto it_p : prov1) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    PrintStats2
#endif
    ProvOut
}

void BTCprov::q09() {
    /*
     * SELECT DISTINCT ?s WHERE {
     * { ?s <http://xmlns.com/foaf/0.1/name> "Samuel" . } UNION
     * { ?s <http://xmlns.com/foaf/0.1/name> "Elisabeth" . }}
     *
     *
    //Prov query
    select ?s where {
    ?s <http://dbpedia.org/ontology/musicBy> <http://dbpedia.org/resource/Sylvester_Levay> .
    }
     */

    KEY_ID Samuel = diplo::KM.Get("\"Samuel\"");
    KEY_ID Elisabeth = diplo::KM.Get("\"Elisabeth\"");

    KEY_ID name = diplo::KM.Get("<http://xmlns.com/foaf/0.1/name>");
//
//	unordered_multimap<KEY_ID, diplo::TripleIDs> results_S;
//	unordered_multimap<KEY_ID, diplo::TripleIDs> results_E;

    vector<vector<unordered_set<KEY_ID>>> results;
    vector<vector<unordered_set<KEY_ID>>> prov;

    results.resize(2);
    results[0].resize(1);
    results[1].resize(1);

    prov.resize(2);

    API::GetSubjects(Samuel, name, results[0][0], prov[0]);
    API::GetSubjects(Elisabeth, name, results[1][0], prov[1]);

	API::PostFiltering(prov, results);
	
#ifdef DISPLAY_RESULTS
    API::DrisplayResults(results);
    API::DrisplayProvenance(prov);
#endif


#ifdef SHOW_STATS
    unordered_set<KEY_ID> ps;
    size_t rs=0;
    for (auto it_p : prov) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    for (auto it_p : results) for (auto it : it_p ) rs+=it.size();
    PrintStats
#endif

    ProvOut


}

void BTCprov::q10() {
    /* like q03
     select ?t ?lat ?long
     where {
     ?a <http://dbpedia.org/property/wikilink> <http://dbpedia.org/resource/List_of_World_Heritage_Sites_in_Europe>.
     OPTTIONAL { ?a <http://dbpedia.org/property/wikilink> <http://dbpedia.org/resource/Middle_Ages> } .
     ?a <http://dbpedia.org/property/title> ?t.
     ?a <http://www.w3.org/2003/01/geo/wgs84_pos#lat> ?lat.
     ?a <http://www.w3.org/2003/01/geo/wgs84_pos#long> ?long.
     }

    //Prov query
    select ?s where {
    ?a <http://www.w3.org/ns/prov#SoftwareAgent> <imagination> .
    ?a <http://www.w3.org/ns/prov#activity> <Activity1123> .
    ?a <http://www.w3.org/ns/prov#atLocation> <http://dbpedia.org/resource/Fribourg> .

    ?a <http://www.w3.org/ns/prov#generated> ?s .
    ?a <http://www.w3.org/ns/prov#wasAssociatedWith> ?s .

    }
    */

    KEY_ID List = diplo::KM.Get("<http://dbpedia.org/resource/List_of_World_Heritage_Sites_in_Europe>");
    KEY_ID Middle_Ages = diplo::KM.Get("<http://dbpedia.org/resource/Middle_Ages>");

    KEY_ID wikilink = diplo::KM.Get("<http://dbpedia.org/property/wikilink>");

    KEY_ID title = diplo::KM.Get("<http://dbpedia.org/property/title>");
    KEY_ID lat = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#lat>");
    KEY_ID llong = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#long>");

    vector<TripleIDs> constrains;
    constrains.push_back(TripleIDs(0, wikilink, List, 0));
    constrains.push_back(TripleIDs(0, wikilink, Middle_Ages, 1));

    vector<TripleIDs> projections;
    projections.push_back(TripleIDs(0, title, 0, 0));
    projections.push_back(TripleIDs(0, lat, 0, 0));
    projections.push_back(TripleIDs(0, llong, 0, 0));

    vector<vector<unordered_set<KEY_ID>>> results;
    vector<vector<unordered_set<KEY_ID>>> prov;

    API::TriplePatern(constrains, projections, results, prov);
	
	API::PostFiltering(prov, results);
#ifdef DISPLAY_RESULTS
    API::DrisplayResults(results);
    API::DrisplayProvenance(prov);
#endif
#ifdef SHOW_STATS
    unordered_set<KEY_ID> ps;
    size_t rs=0;
    for (auto it_p : prov) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    for (auto it_p : results) for (auto it : it_p ) rs+=it.size();
    PrintStats
#endif
    ProvOut
}

void BTCprov::runthemall() {
#ifdef SHOW_STATS
    queriesStart << "#r\t#m\t#mf\t#prov\t#im\t#imf\t#i\t#ec\t#er" <<endl;
#endif
    cout << "---------------------q01---------------------" << endl;
    diplo::moleculesCounter = 0;
    diplo::ProvFilterCounter = 0;
    diplo::ProvFilterCH = false;
    diplo::elementsChecked = 0;
    diplo::elementsRetrieved = 0;
    q01();
    cout << "---------------------q02---------------------" << endl;
    diplo::moleculesCounter = 0;
    diplo::ProvFilterCounter = 0;
    diplo::ProvFilterCH = false;
    diplo::elementsChecked = 0;
    diplo::elementsRetrieved = 0;
    q02();
    cout << "---------------------q03---------------------" << endl;
    diplo::moleculesCounter = 0;
    diplo::ProvFilterCounter = 0;
    diplo::ProvFilterCH = false;
    diplo::elementsChecked = 0;
    diplo::elementsRetrieved = 0;
    q03();
    cout << "---------------------q04---------------------" << endl;
    diplo::moleculesCounter = 0;
    diplo::ProvFilterCounter = 0;
    diplo::ProvFilterCH = false;
    diplo::elementsChecked = 0;
    diplo::elementsRetrieved = 0;
    q04();
    cout << "---------------------q05---------------------" << endl;
    diplo::moleculesCounter = 0;
    diplo::ProvFilterCounter = 0;
    diplo::ProvFilterCH = false;
    diplo::elementsChecked = 0;
    diplo::elementsRetrieved = 0;
    q05();
    cout << "---------------------q06---------------------" << endl;
    diplo::moleculesCounter = 0;
    diplo::ProvFilterCounter = 0;
    diplo::ProvFilterCH = false;
    diplo::elementsChecked = 0;
    diplo::elementsRetrieved = 0;
    q06();
// 	cout << "---------------------q07---------------------" << endl;
// 	diplo::moleculesCounter = 0;
// 	diplo::ProvFilterCounter = 0; diplo::ProvFilterCH = false;diplo::elementsChecked = 0; diplo::elementsRetrieved = 0;
// 	q07();
    cout << "---------------------q08---------------------" << endl;
    diplo::moleculesCounter = 0;
    diplo::ProvFilterCounter = 0;
    diplo::ProvFilterCH = false;
    diplo::elementsChecked = 0;
    diplo::elementsRetrieved = 0;
    q08();
    cout << "---------------------q09---------------------" << endl;
    diplo::moleculesCounter = 0;
    diplo::ProvFilterCounter = 0;
    diplo::ProvFilterCH = false;
    diplo::elementsChecked = 0;
    diplo::elementsRetrieved = 0;
    q09();
    cout << "---------------------q10---------------------" << endl;
    diplo::moleculesCounter = 0;
    diplo::ProvFilterCounter = 0;
    diplo::ProvFilterCH = false;
    diplo::elementsChecked = 0;
    diplo::elementsRetrieved = 0;
    q10();
}

void BTCprov::benchmark() {
    vector<vector<double> > times4exel;
    times4exel.resize(10);
    double time;
    for (int i = 0; i < 10; i++) {
        cout << "--------------------- ROUND: " << i << "---------------------" << endl;
        diplo::stopwatch_start();
        q01();
        time = diplo::stopwatch_get();
        times4exel[0].push_back(time);
        cout << "q01: " << time << endl;
        sleep(diplo::pause_int);

        diplo::stopwatch_start();
        q02();
        time = diplo::stopwatch_get();
        times4exel[1].push_back(time);
        cout << "q02: " << time << endl;
        sleep(diplo::pause_int);

        diplo::stopwatch_start();
        q03();
        time = diplo::stopwatch_get();
        times4exel[2].push_back(time);
        cout << "q03: " << time << endl;
        sleep(diplo::pause_int);

        diplo::stopwatch_start();
        q04();
        time = diplo::stopwatch_get();
        times4exel[3].push_back(time);
        cout << "q04: " << time << endl;
        sleep(diplo::pause_int);

        diplo::stopwatch_start();
        q05();
        time = diplo::stopwatch_get();
        times4exel[4].push_back(time);
        cout << "q05: " << time << endl;
        sleep(diplo::pause_int);

        diplo::stopwatch_start();
        q06();
        time = diplo::stopwatch_get();
        times4exel[5].push_back(time);
        cout << "q06: " << time << endl;
        sleep(diplo::pause_int);

// 		diplo::stopwatch_start();
// 		q07();
// 		time = diplo::stopwatch_get();
// 		times4exel[6].push_back(time);
// 		cout << "q07: " << time << endl;
// 		sleep(diplo::pause_int);

        diplo::stopwatch_start();
        q08();
        time = diplo::stopwatch_get();
        times4exel[7].push_back(time);
        cout << "q08: " << time << endl;
        sleep(diplo::pause_int);

        diplo::stopwatch_start();
        q09();
        time = diplo::stopwatch_get();
        times4exel[8].push_back(time);
        cout << "q09: " << time << endl;
        sleep(diplo::pause_int);

        diplo::stopwatch_start();
        q10();
        time = diplo::stopwatch_get();
        times4exel[9].push_back(time);
        cout << "q10: " << time << endl;
        sleep(diplo::pause_int);
    }


    /////printing for exel
    {
        for (vector<vector<double> >::iterator q = times4exel.begin();
                q != times4exel.end(); q++) {
            for (size_t r = 0; r < q->size(); r++) {
                string n = to_string((*q)[r]);
                //			std::replace(n.begin(), n.end(), '.', ',');
                benchmarkResults << n << "\t";
            }
            if (!q->empty())
                benchmarkResults << endl;
        }
    }
}

void BTCprov::ProvQuery() {
    /*
    select ?s where {
    	{?s <http://www.geonames.org/ontology#wikipediaArticle> <http://af.wikipedia.org/wiki/Eiffel-toring> .} UNION
    	{?s <http://dbpedia.org/property/website> <http://www.w3.org/> .} UNION
    	{?s <http://xmlns.com/foaf/0.1/primaryTopic> <http://dbpedia.org/resource/Tim_Berners-Lee> .} UNION
    	{
    		?a <http://www.w3.org/ns/prov#SoftwareAgent> <imagination> .
    		?a <http://www.w3.org/ns/prov#activity> <Activity1123> .
    		?a <http://www.w3.org/ns/prov#atLocation> <http://dbpedia.org/resource/Fribourg> .
    		?a <http://www.w3.org/ns/prov#generated> ?s .
    		?a <http://www.w3.org/ns/prov#wasAssociatedWith> ?s .
    	} UNION
    	{
    		?a <http://dbpedia.org/property/reference> <http://www.swr.de/wiesoweshalbwarum/archiv/2005/03/10/beitrag1.html> .
    		?a <http://dbpedia.org/property/sterbedatum> <http://dbpedia.org/resource/31._M??rz> .
    		?a <http://dbpedia.org/property/wikilink> <http://dbpedia.org/resource/sinologist> .
    	} UNION
    	{?s <http://www.w3.org/ns/prov#wasGeneratedBy> <http://mwylot.net/> . } UNION
    	{?s <http://www.w3.org/2002/07/owl#sameAs> <http://sws.geonames.org/2643576/> . } UNION
    	{?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://dbpedia.org/class/yago/LivingPeople> . }
    }
    */
    {   //q01
        KEY_ID p = diplo::KM.Get("<http://www.geonames.org/ontology#wikipediaArticle>");
        KEY_ID o = diplo::KM.Get("<http://af.wikipedia.org/wiki/Eiffel-toring>");

        vector<unordered_set<KEY_ID>> prov;

        API::GetSubjects(o, p, ProvUris, prov);

    }

    {   //q02
        KEY_ID p = diplo::KM.Get("<http://dbpedia.org/property/website>");
        KEY_ID o = diplo::KM.Get("<http://www.w3.org/>");
        vector<unordered_set<KEY_ID>> prov;
        API::GetSubjects(o, p, ProvUris, prov);

        //added
        p = diplo::KM.Get("<http://xmlns.com/foaf/0.1/primaryTopic>");
        o = diplo::KM.Get("<http://dbpedia.org/resource/Tim_Berners-Lee>");
        API::GetSubjects(o, p, ProvUris, prov);

    }

    {   //q03
        KEY_ID SoftwareAgent = diplo::KM.Get("<http://www.w3.org/ns/prov#SoftwareAgent>");
        KEY_ID imagination = diplo::KM.Get("<imagination>");
        KEY_ID Activity1123 = diplo::KM.Get("<Activity1123>");
        KEY_ID Fribourg = diplo::KM.Get("<http://dbpedia.org/resource/Fribourg>");
        KEY_ID activity = diplo::KM.Get("<http://www.w3.org/ns/prov#activity>");
        KEY_ID atLocation = diplo::KM.Get("<http://www.w3.org/ns/prov#atLocation>");

        KEY_ID generated = diplo::KM.Get("<http://www.w3.org/ns/prov#generated>");
        KEY_ID wasAssociatedWith = diplo::KM.Get("<http://www.w3.org/ns/prov#wasAssociatedWith>");

        vector<TripleIDs> constrains;
        constrains.push_back(TripleIDs(0, SoftwareAgent, imagination, 0));
        constrains.push_back(TripleIDs(0, activity, Activity1123, 0));
        constrains.push_back(TripleIDs(0, atLocation, Fribourg, 0));

        vector<TripleIDs> projections;
        projections.push_back(TripleIDs(0, generated, 0, 0));
        projections.push_back(TripleIDs(0, wasAssociatedWith, 0, 0));

        vector<vector<unordered_set<KEY_ID>>> results;
        vector<vector<unordered_set<KEY_ID>>> prov;

        API::TriplePatern(constrains, projections, results, prov);

        for (vector<vector<unordered_set<KEY_ID>>> ::iterator it_p = results.begin(); it_p != results.end(); it_p++)
            for (vector<unordered_set<KEY_ID>>::iterator it1 = it_p->begin(); it1 != it_p->end(); it1++)
                for (unordered_set<KEY_ID>::iterator it2 = it1->begin(); it2 != it1->end(); it2++)
                    ProvUris.insert(*it2);


        //added
        KEY_ID p = diplo::KM.Get("<http://xmlns.com/foaf/0.1/homepage>");
        KEY_ID o = diplo::KM.Get("<http://whc.unesco.org/en/list/348>");
        vector<unordered_set<KEY_ID>> prov1;
        API::GetSubjects(o, p, ProvUris, prov1);

        p = diplo::KM.Get("<http://www.w3.org/2004/02/skos/core#subject>");
        o = diplo::KM.Get("<http://dbpedia.org/resource/Category:FIFA_World_Cup_stadiums>");
        API::GetSubjects(o, p, ProvUris, prov1);

    }

    {   //q04

        vector<unordered_set<KEY_ID>> results;

        {
            KEY_ID p = diplo::KM.Get("<http://dbpedia.org/property/reference>");
            KEY_ID o = diplo::KM.Get("<http://www.swr.de/wiesoweshalbwarum/archiv/2005/03/10/beitrag1.html>");
            vector<unordered_set<KEY_ID>> prov;
            unordered_set<KEY_ID> result;
            API::GetSubjects(o, p, result, prov);
            results.push_back(result);
        }
        {
            KEY_ID p = diplo::KM.Get("<http://dbpedia.org/property/sterbedatum>");
            KEY_ID o = diplo::KM.Get("<http://dbpedia.org/resource/31._M??rz>");
            vector<unordered_set<KEY_ID>> prov;
            unordered_set<KEY_ID> result;
            API::GetSubjects(o, p, result, prov);
            results.push_back(result);
        }
        {
            KEY_ID p = diplo::KM.Get("<http://dbpedia.org/property/wikilink>");
            KEY_ID o = diplo::KM.Get("<http://dbpedia.org/resource/sinologist>");
            vector<unordered_set<KEY_ID>> prov;
            unordered_set<KEY_ID> result;
            API::GetSubjects(o, p, result, prov);
            results.push_back(result);
        }

        //ProvUris
        results.push_back(unordered_set<KEY_ID>());

        unordered_set_intersection(results[0], results[1], results[3]);
        unordered_set_intersection(results[2], results[3], ProvUris);


        KEY_ID p = diplo::KM.Get("<http://www.geonames.org/ontology#inCountry>");
        KEY_ID o = diplo::KM.Get("<http://www.geonames.org/countries/#DE>");
        vector<unordered_set<KEY_ID>> prov;

        API::GetSubjects(o, p, ProvUris, prov);
    }


    {   //q05 done with previous qieries

    }

    {   //q06
        KEY_ID p = diplo::KM.Get("<http://www.w3.org/ns/prov#wasGeneratedBy>");
        KEY_ID o = diplo::KM.Get("<http://mwylot.net/>");
        vector<unordered_set<KEY_ID>> prov;

        API::GetSubjects(o, p, ProvUris, prov);

    }
    {   //q07


        {
            TYPE_ID t = diplo::KM.GetSeq(diplo::KM.Get("<http://dbpedia.org/class/yago/LivingPeople>"));

            map<KEY_ID, class Node *> * molecules = diplo::KM.GetElementsOfType(t);
            for (map<KEY_ID, class Node *>::iterator it = molecules->begin(); it != molecules->end(); it++) {
                ProvUris.insert(it->first);
            }
        }

        {

            KEY_ID p = diplo::KM.Get("<http://www.w3.org/2002/07/owl#sameAs>");
            KEY_ID o = diplo::KM.Get("<http://sws.geonames.org/2643576/>");
            vector<unordered_set<KEY_ID>> prov;

            API::GetSubjects(o, p, ProvUris, prov);
        }
    }

    {   //q08

    }

    {   //q09 done with previous

    }

    {   //q10 done with previous

    }

// 	for (auto p : ProvUris ) {
// 		auto ms = ProvIdx.find(p);
// 		if (ms != ProvIdx.end())
// 			ProvMolecules.insert(ms->second.begin(),ms->second.end());
// 	}

    cout << "ProvUris.size(): " << ProvUris.size() << endl;
// 	cout << "ProvIdx.size(): " << ProvIdx.size() << endl;
// // 	cout << "ProvMolecules.size(): " << ProvMolecules.size() << endl;


//for (unordered_set<KEY_ID>::iterator it = ProvUris.begin(); it != ProvUris.end(); it++) cout << "Prov: ----> " << KM.Get(*it) << endl;
}

} /* namespace queries */
