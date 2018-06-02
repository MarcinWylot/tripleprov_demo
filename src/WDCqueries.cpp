/*
 * WDCqueries.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: marcin
 */

#include "WDCqueries.h"

namespace queries {

WDCqueries::WDCqueries() {
	// TODO Auto-generated constructor stub

}

WDCqueries::~WDCqueries() {
	// TODO Auto-generated destructor stub
}

void WDCqueries::q01() {
	/*
	 }
	 select ?a ?title ?desc
where {
?a ?j "http://www.fortis-angelo-srl.com".
?a <http://opengraphprotocol.org/schema/site_name> "PagineBianche" .
?a <http://opengraphprotocol.org/schema/title> ?title.
?a <http://opengraphprotocol.org/schema/description> ?desc. }

//prov
 SELECT ?s WHERE {
?s <http://opengraphprotocol.org/schema/type> "company"
 }

	 */

//	cerr << "ProvUris.size(): " << ProvUris.size() << "\t" << KM.Get("<http://www.fortis-angelo-srl.com/>") << endl;
//	for (unordered_set<KEY_ID>::iterator it = ProvUris.begin(); it != ProvUris.end(); it++) cerr << *it << "\t--" << KM.Get(*it) << "--"<< endl;


	KEY_ID fortis_angelo = diplo::KM.Get("\"http://www.fortis-angelo-srl.com\"");

	KEY_ID site_name = diplo::KM.Get(
			"<http://opengraphprotocol.org/schema/site_name>");
	KEY_ID PagineBianche = diplo::KM.Get("\"PagineBianche\"");

	KEY_ID title = diplo::KM.Get("<http://opengraphprotocol.org/schema/title>");
	KEY_ID description = diplo::KM.Get(
			"<http://opengraphprotocol.org/schema/description>");

	vector < TripleIDs > constrains;
	constrains.push_back(TripleIDs(0, -1, fortis_angelo, 0));
	constrains.push_back(TripleIDs(0, site_name, PagineBianche, 0));

	vector < TripleIDs > projections;
	projections.push_back(TripleIDs(0, title, 0, 0));
	projections.push_back(TripleIDs(0, description, 0, 0));

	vector < vector < unordered_set<KEY_ID> >> results;
	vector < vector < unordered_set<KEY_ID> >> prov;

	API::TriplePatern(constrains, projections, results, prov);

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

void WDCqueries::q02() {
	/*
select ?b ?p ?bn
where {
?a ?j "http://fortyagency.com/35-under-35" .
?a <http://ogp.me/ns#image> ?b .
?a <http://ogp.me/ns#description> ?p .
?a <http://ogp.me/ns#title> ?bn.

//prov
SELECT ?s WHERE {
?s  <http://www.facebook.com/2008/fbmladmins> "500386069"   .
?s <http://ogp.me/ns#url> "http://fortyagency.com/35-under-35"
}
		 */



	KEY_ID fortyagency = diplo::KM.Get("\"http://fortyagency.com/35-under-35\"");

	KEY_ID image = diplo::KM.Get(
			"<http://ogp.me/ns#image>");
	KEY_ID description = diplo::KM.Get(
			"<http://ogp.me/ns#description>");
	KEY_ID title = diplo::KM.Get("<http://ogp.me/ns#title>");

	vector<TripleIDs> constrains;
	constrains.push_back(TripleIDs(0, -1, fortyagency, 0));

	vector<TripleIDs> projections;
	projections.push_back(TripleIDs(0, image, 0, 0));
	projections.push_back(TripleIDs(0, description, 0, 0));
	projections.push_back(TripleIDs(0, title, 0, 0));

	vector<vector<unordered_set<KEY_ID>>> results;
	vector<vector<unordered_set<KEY_ID>>> prov;

	API::TriplePatern(constrains, projections, results, prov);

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

void WDCqueries::q03() {
	/*
	 * select ?t ?lat ?long
where  {
?a <http://opengraphprotocol.org/schema/country-name> "United States" .
?a <http://opengraphprotocol.org/schema/region> "Maine" .
?a <http://opengraphprotocol.org/schema/title> ?t .
?a <http://opengraphprotocol.org/schema/latitude> ?lat.
?a <http://opengraphprotocol.org/schema/longitude> ?long.
}

//prov
SELECT ?s WHERE {
?s <http://opengraphprotocol.org/schema/email> ""
?s <http://opengraphprotocol.org/schema/type> "Hotel"
?s <http://opengraphprotocol.org/schema/country-name> "United States"
}
	 */

	KEY_ID country_name = diplo::KM.Get("<http://opengraphprotocol.org/schema/country-name>");
	KEY_ID UnitedStates = diplo::KM.Get("\"United States\"");

	KEY_ID region = diplo::KM.Get("<http://opengraphprotocol.org/schema/region>");
	KEY_ID Maine = diplo::KM.Get("\"Maine\"");


	KEY_ID title = diplo::KM.Get("<http://opengraphprotocol.org/schema/title>");
	KEY_ID lat = diplo::KM.Get("<http://opengraphprotocol.org/schema/latitude>");
	KEY_ID llong = diplo::KM.Get("<http://opengraphprotocol.org/schema/longitude>");

	vector<TripleIDs> constrains;
	constrains.push_back(TripleIDs(0, region, Maine, 0));
	constrains.push_back(TripleIDs(0, country_name, UnitedStates, 0));

	vector<TripleIDs> projections;
	projections.push_back(TripleIDs(0, title, 0, 0));
	projections.push_back(TripleIDs(0, lat, 0, 0));
	projections.push_back(TripleIDs(0, llong, 0, 0));

	vector<vector<unordered_set<KEY_ID>>> results;
	vector<vector<unordered_set<KEY_ID>>> prov;

	API::TriplePatern(constrains, projections, results, prov);

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

void WDCqueries::q04() {
	/*
	select ?c ?l ?page ?desc
	where {
	?p <http://xmlns.com/foaf/0.1/page> <http://www.france-artifices.com/lot-de-12-lampions-tricolores-16-cm,fr,4,L12LT16.cfm> .
	?p <http://purl.org/goodrelations/v1#hasBusinessFunction> ?l  .
	?c ?j ?l.
	?c <http://purl.org/goodrelations/v1#hasStockKeepingUnit>       "DUB073"^^<http://www.w3.org/2001/XMLSchema#string> .
	?c <http://purl.org/goodrelations/v1#description> ?desc .
	?c <http://xmlns.com/foaf/0.1/page> ?page
	}

//prov
select ?s WHERE {
?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://rdfs.org/sioc/types#BoardPost>
 }

	*/


	vector<vector<vector<unordered_set<KEY_ID>>>> results;
	vector<vector<vector<unordered_set<KEY_ID>>>> prov;

	KEY_ID france_artifices = diplo::KM.Get("<http://www.france-artifices.com/lot-de-12-lampions-tricolores-16-cm,fr,4,L12LT16.cfm>");
	KEY_ID page = diplo::KM.Get("<http://xmlns.com/foaf/0.1/page>");

	KEY_ID hasBusinessFunction = diplo::KM.Get("<http://purl.org/goodrelations/v1#hasBusinessFunction>");

	KEY_ID hasStockKeepingUnit = diplo::KM.Get("<http://purl.org/goodrelations/v1#hasStockKeepingUnit>");
	KEY_ID Unit = diplo::KM.Get("\"DUB073\"^^<http://www.w3.org/2001/XMLSchema#string>");

	KEY_ID description = diplo::KM.Get("<http://purl.org/goodrelations/v1#description>");

	{
		vector<TripleIDs> constrains;
		constrains.push_back(TripleIDs(0, page, france_artifices, 0));

		vector<TripleIDs> projections;
		projections.push_back(TripleIDs(0, hasBusinessFunction, 0, 0));

		vector<vector<unordered_set<KEY_ID>>> results_local;
		vector<vector<unordered_set<KEY_ID>>> prov_local;

		API::TriplePatern(constrains, projections, results_local, prov_local);
		results.push_back(results_local);
		prov.push_back(prov_local);
	}
	
#ifdef SHOW_STATS
    size_t is, im, imf;
#endif
	
	
	{
		vector<TripleIDs> constrains;
		for (vector<vector<vector<unordered_set<KEY_ID>>>> ::iterator it_p1 = results.begin(); it_p1 != results.end(); it_p1++) {
			for (vector<vector<unordered_set<KEY_ID>>> ::iterator it_p = it_p1->begin(); it_p != it_p1->end(); it_p++) {
				for (vector<unordered_set<KEY_ID>>::iterator it1 = it_p->begin(); it1 != it_p->end(); it1++) {
					for (unordered_set<KEY_ID>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
						constrains.push_back(TripleIDs(0, -1, *it2, 0));
					}
				}
			}
		}

		constrains.push_back(TripleIDs(0, hasStockKeepingUnit, Unit, 0));

#ifdef SHOW_STATS
        is=constrains.size();
        im = diplo::moleculesCounter;
        imf = diplo::ProvFilterCounter;
#endif
		
		
		vector<TripleIDs> projections;
		projections.push_back(TripleIDs(0, description, 0, 0));
		projections.push_back(TripleIDs(0, page, 0, 0));

		vector<vector<unordered_set<KEY_ID>>> results_local;
		vector<vector<unordered_set<KEY_ID>>> prov_local;

		API::TriplePatern(constrains, projections, results_local, prov_local);
		results.push_back(results_local);
		prov.push_back(prov_local);

	}

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

void WDCqueries::q05() {
	/*
	 * select distinct ?d
where {
?c <http://rdfs.org/sioc/ns#has_container>      <http://www.foundationdrupal7.com/forums/announcements> .
?b <http://xmlns.com/foaf/0.1/name> "Anonymous"@EN

?a <http://rdfs.org/sioc/ns#reply_of>  ?c .
?a <http://rdfs.org/sioc/ns#has_creator> ?b .

?a <http://purl.org/dc/terms/date> ?d .


}

//prov
select ?s WHERE {
?a <http://rdfs.org/sioc/ns#has_creator> _:node178qc1e8nx57171426
?a <http://rdfs.org/sioc/ns#has_container> ?s

?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://rdfs.org/sioc/ns#Forum>
?s <http://www.w3.org/2004/02/skos/core#prefLabel> "Announcements"@en

 }
	 *
	 */



	KEY_ID has_container = diplo::KM.Get("<http://rdfs.org/sioc/ns#has_container>");
	KEY_ID announcements = diplo::KM.Get("<http://www.foundationdrupal7.com/forums/announcements>");

	KEY_ID name = diplo::KM.Get("<http://xmlns.com/foaf/0.1/name>");
	KEY_ID Anonymous = diplo::KM.Get("\"Anonymous\"@en");

	KEY_ID reply_of = diplo::KM.Get("<http://rdfs.org/sioc/ns#reply_of>");
	KEY_ID has_creator = diplo::KM.Get("<http://rdfs.org/sioc/ns#has_creator>");
	KEY_ID date = diplo::KM.Get("<http://purl.org/dc/terms/date>");

	unordered_multimap<KEY_ID, KEY_ID> results_ANN;
	unordered_multimap<KEY_ID, KEY_ID> results_ANO;

	API::GetSubjects(announcements, has_container, results_ANN);
	API::GetSubjects(Anonymous, name, results_ANO);


	vector<vector<vector<unordered_set<KEY_ID>>> > results;
	vector<vector<vector<unordered_set<KEY_ID>>> > prov;

	vector<diplo::TripleIDs> projections;
	projections.push_back(TripleIDs(0, date, 0, 0));

	unordered_set<KEY_ID> pr;
#ifdef SHOW_STATS
    size_t is=results_ANN.size()*results_ANO.size();
    size_t im = diplo::moleculesCounter;
    size_t imf = diplo::ProvFilterCounter;
    size_t rs=0;
#endif


	for (unordered_multimap<KEY_ID, KEY_ID>::iterator it = results_ANO.begin();
			it != results_ANO.end();) {
		pr.clear();
//		cerr << "1" << endl;
		KEY_ID varANO = it->first;
		do {
			pr.insert(it->second);
			it++;
		} while (it != results_ANO.end() and varANO == it->first);
		vector<vector<unordered_set<KEY_ID>>> prov_localANO( {vector<unordered_set<KEY_ID>>( {pr})});
//		cerr << "2" << endl;
		for (unordered_multimap<KEY_ID, KEY_ID>::iterator it2 = results_ANN.begin(); it2 != results_ANN.end();) {
			pr.clear();
//			cerr << "3" << endl;
			KEY_ID varANN = it2->first;
			do {
				pr.insert(it2->second);
				it2++;
			} while (it2 != results_ANN.end() and varANN == it2->first);
//			cerr << "4" << endl;
			vector<vector<unordered_set<KEY_ID>>> prov_localANN( {vector<unordered_set<KEY_ID>>( {pr})});

			vector<TripleIDs> constrains;
			constrains.push_back(TripleIDs(0, reply_of, varANN, 0));
			constrains.push_back(TripleIDs(0, has_creator, varANO, 0));
			vector<vector<unordered_set<KEY_ID>>> results_local;
			vector<vector<unordered_set<KEY_ID>>> prov_local;

			API::TriplePatern(constrains, projections, results_local,prov_local);

//			cerr << "varANN: " << diplo::KM.Get(varANN) << "\tvarANO: " << diplo::KM.Get(varANO) << "\tprov_local.size(): " << prov_local.size()
//					<<  "\tresults_local.size(): " <<results_local.size() << endl;

			if (results_local.size() > 0) {
				prov.push_back(prov_localANO);
				prov.push_back(prov_localANN);
				prov.push_back(prov_local);
				results.push_back(results_local);
			}

		}
	}

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
    for (auto prov1 : prov) for (auto it_p : prov1) for (auto it : it_p ) for (auto it2 : it ) ps.insert(it2);
    for (auto it_r : results) for (auto it_p : it_r) for (auto it : it_p ) rs+=it.size();
    PrintStats2
#endif
    ProvOut
}

void WDCqueries::q06() {
	/*
		 * SELECT DISTINCT ?s WHERE {
{ ?s  <http://ogp.me/ns#site_name> "fosiki" . } UNION
{ ?s <http://ogp.me/ns#site_name> "Fotogard" . }}

//prov
SELECT ?s WHERE {
?s <http://ogp.me/ns#url> "http://www.fotogard.com/velika-krada-u-las-vegasu" .
UNION
?s <http://www.facebook.com/2008/fbmlapp_id> "274726392563373" .
UNION
?s <http://ogp.me/ns#url> "http://www.fotogard.com/milijun-dolara-za-ulozenih-800"
}
		 */



	KEY_ID Fotolog = diplo::KM.Get("\"fosiki\"");
	KEY_ID Fotogard = diplo::KM.Get("\"Fotogard\"");

	KEY_ID name = diplo::KM.Get("<http://ogp.me/ns#site_name>");


	vector<vector<unordered_set<KEY_ID>>> results;
	vector<vector<unordered_set<KEY_ID>>> prov;

	results.resize(2);
	results[0].resize(1);
	results[1].resize(1);

	prov.resize(2);
	API::GetSubjects(Fotolog, name, results[0][0], prov[0]);
	API::GetSubjects(Fotogard, name, results[1][0], prov[1]);

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

void WDCqueries::q07() {
	/*
	 * select ?t ?lat ?long
	 where  {
	 ?a <http://opengraphprotocol.org/schema/region> "Maine"  .
	 OPTTIONAL { ?a <http://opengraphprotocol.org/schema/country-name> "United States" } .
	 ?a <http://opengraphprotocol.org/schema/title> ?t .
	 ?a <http://opengraphprotocol.org/schema/latitude> ?lat.
	 ?a <http://opengraphprotocol.org/schema/longitude> ?long.
	 }

SELECT ?s WHERE {
?s <http://opengraphprotocol.org/schema/email> ""
?s <http://opengraphprotocol.org/schema/type> "Hotel"
?s <http://opengraphprotocol.org/schema/country-name> "United States"
}

	 */

	KEY_ID country_name = diplo::KM.Get(
			"<http://opengraphprotocol.org/schema/country-name>");
	KEY_ID UnitedStates = diplo::KM.Get("\"United States\"");

	KEY_ID region = diplo::KM.Get(
			"<http://opengraphprotocol.org/schema/region>");
	KEY_ID Maine = diplo::KM.Get("\"Maine\"");

	KEY_ID title = diplo::KM.Get("<http://opengraphprotocol.org/schema/title>");
	KEY_ID lat = diplo::KM.Get(
			"<http://opengraphprotocol.org/schema/latitude>");
	KEY_ID llong = diplo::KM.Get(
			"<http://opengraphprotocol.org/schema/longitude>");

	vector<TripleIDs> constrains;
	constrains.push_back(TripleIDs(0, region, Maine, 0));
	constrains.push_back(TripleIDs(0, country_name, UnitedStates, 1));

	vector<TripleIDs> projections;
	projections.push_back(TripleIDs(0, title, 0, 0));
	projections.push_back(TripleIDs(0, lat, 0, 0));
	projections.push_back(TripleIDs(0, llong, 0, 0));

	vector<vector<unordered_set<KEY_ID>>> results;
	vector<vector<unordered_set<KEY_ID>>> prov;

	API::TriplePatern(constrains, projections, results, prov);

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

void WDCqueries::runthemall() {
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
	cout << "---------------------q07---------------------" << endl;
	diplo::moleculesCounter = 0;
	diplo::ProvFilterCounter = 0; diplo::ProvFilterCH = false;diplo::elementsChecked = 0; diplo::elementsRetrieved = 0;
	q07();
}

void WDCqueries::benchmark() {
	vector<vector<double> > times4exel;
	times4exel.resize(7);
	double time;
	for (int i = 0; i < 30; i++) {
		cout << "--------------------- ROUND: " << i << "---------------------"
				<< endl;
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

		diplo::stopwatch_start();
		q07();
		time = diplo::stopwatch_get();
		times4exel[6].push_back(time);
		cout << "q07: " << time << endl;
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

void WDCqueries::ProvQuery() {
/*
SELECT ?s WHERE {
	{?s <http://opengraphprotocol.org/schema/type> "company"}
	UNION
	{?s  <http://www.facebook.com/2008/fbmladmins> "500386069"   .
	?s <http://ogp.me/ns#url> "http://fortyagency.com/35-under-35"}
	UNION
	{
	?s <http://opengraphprotocol.org/schema/email> ""
	?s <http://opengraphprotocol.org/schema/type> "Hotel"
	?s <http://opengraphprotocol.org/schema/country-name> "United States"
	}
	UNION {}
	UNION
	{?a <http://rdfs.org/sioc/ns#has_creator> _:node178qc1e8nx57171426
	?a <http://rdfs.org/sioc/ns#has_container> ?s

	?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://rdfs.org/sioc/ns#Forum>
	?s <http://www.w3.org/2004/02/skos/core#prefLabel> "Announcements"@en
	} UNION
	{
		?s <http://ogp.me/ns#url> "http://www.fotogard.com/velika-krada-u-las-vegasu" .
	UNION
	?s <http://www.facebook.com/2008/fbmlapp_id> "274726392563373" .
	UNION
	?s <http://ogp.me/ns#url> "http://www.fotogard.com/milijun-dolara-za-ulozenih-800"
	} UNION
	{
		?a <http://www.w3.org/ns/prov#SoftwareAgent> <imagination> .
		?a <http://www.w3.org/ns/prov#activity> <Activity1123> .
		?a <http://www.w3.org/ns/prov#atLocation> <http://dbpedia.org/resource/Fribourg> .
		?a <http://www.w3.org/ns/prov#generated> ?s .
		?a <http://www.w3.org/ns/prov#wasAssociatedWith> ?s .
	}

 }

 */

	{ //q01
		KEY_ID p = diplo::KM.Get("<http://opengraphprotocol.org/schema/type>");
		KEY_ID o = diplo::KM.Get("\"company\"");
		vector<unordered_set<KEY_ID>> prov;
		API::GetSubjects(o, p, ProvUris, prov);

	}

	{ //q02
		KEY_ID p = diplo::KM.Get("<http://www.facebook.com/2008/fbmladmins>");
		KEY_ID o = diplo::KM.Get("\"500386069\"");
		vector<unordered_set<KEY_ID>> prov;
		unordered_set<KEY_ID> res1;
		unordered_set<KEY_ID> res2;
		API::GetSubjects(o, p, res1, prov);

		p = diplo::KM.Get("<http://ogp.me/ns#url>");
		o = diplo::KM.Get("\"http://fortyagency.com/35-under-35\"");
		API::GetSubjects(o, p, res2, prov);

		unordered_set_intersection(res1, res2, ProvUris);

	}


	{ //q03
		KEY_ID p = diplo::KM.Get("<http://opengraphprotocol.org/schema/email>");
		KEY_ID o = diplo::KM.Get("\"\"");
		vector<unordered_set<KEY_ID>> prov;
		unordered_set<KEY_ID> res1;
		unordered_set<KEY_ID> res2;
		unordered_set<KEY_ID> res3;
		unordered_set<KEY_ID> res4;
		API::GetSubjects(o, p, res1, prov);

		p = diplo::KM.Get("<http://opengraphprotocol.org/schema/type>");
		o = diplo::KM.Get("\"Hotel\"");
		API::GetSubjects(o, p, res2, prov);

		p = diplo::KM.Get("<http://opengraphprotocol.org/schema/country-name>");
		o = diplo::KM.Get("\"United States\"");
		API::GetSubjects(o, p, res3, prov);

		unordered_set_intersection(res1, res2, res4);
		unordered_set_intersection(res3, res4, ProvUris);

		//		cerr << "4res.size(): " << ProvUris.size() << endl;
		//		for (unordered_set<KEY_ID>::iterator it = ProvUris.begin(); it != ProvUris.end(); it++) cerr << KM.Get(*it) << endl;

	}


	{//q04
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

	{ //q05
		KEY_ID has_creator = diplo::KM.Get("<http://rdfs.org/sioc/ns#has_creator>");
		KEY_ID node = diplo::KM.Get("_:node178qc1e8nx57171426");
		KEY_ID has_container = diplo::KM.Get("<http://rdfs.org/sioc/ns#has_container>");

		KEY_ID tForum = KM.GetSeq(diplo::KM.Get("<http://rdfs.org/sioc/ns#Forum>"));
		KEY_ID prefLabel = diplo::KM.Get("<http://www.w3.org/2004/02/skos/core#prefLabel>");
		KEY_ID Announcements = diplo::KM.Get("\"Announcements\"@en");

		vector<unordered_set<KEY_ID>> prov;
		unordered_set<KEY_ID> resS;
		API::GetSubjects(Announcements, prefLabel, resS, prov);

		vector<TripleIDs> constrains;
		constrains.push_back(TripleIDs(0, has_creator, node, 0));

		vector<TripleIDs> projections;
		projections.push_back(TripleIDs(0, has_container, 0, 0));

		vector<vector<unordered_set<KEY_ID>>> resS1;
		vector<vector<unordered_set<KEY_ID>>> prov1;
		API::TriplePatern(constrains, projections, resS1, prov1);

		for (vector<vector<unordered_set<KEY_ID>>> ::iterator it_p = resS1.begin(); it_p != resS1.end(); it_p++) {
			for (vector<unordered_set<KEY_ID>>::iterator it1 = it_p->begin(); it1 != it_p->end(); it1++) {
				for (unordered_set<KEY_ID>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
					if ( KM.GetType(*it2) == tForum and resS.find(*it2) != resS.end() ) {
						ProvUris.insert(*it2);
					}
				}
			}
		}
	}


	{ //q06
		KEY_ID p = diplo::KM.Get("<http://ogp.me/ns#url>");
		KEY_ID o = diplo::KM.Get("\"http://www.fotogard.com/velika-krada-u-las-vegasu\"");
		vector<unordered_set<KEY_ID>> prov;
		API::GetSubjects(o, p, ProvUris, prov);

		p = diplo::KM.Get("<http://www.facebook.com/2008/fbmlapp_id>");
		o = diplo::KM.Get("\"274726392563373\"");
		API::GetSubjects(o, p, ProvUris, prov);

		p = diplo::KM.Get("<http://ogp.me/ns#url>");
		o = diplo::KM.Get("\"http://www.fotogard.com/milijun-dolara-za-ulozenih-800\"");
		API::GetSubjects(o, p, ProvUris, prov);

	}
	{//q07 TODO

	}

//	cerr << "ProvUris.size(): " << ProvUris.size() << endl;
//	for (unordered_set<KEY_ID>::iterator it = ProvUris.begin(); it != ProvUris.end(); it++) cerr << KM.Get(*it) << endl;
}

} /* namespace queries */
