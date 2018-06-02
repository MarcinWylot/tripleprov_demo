/*
 * API.h
 *
 *  Created on: Jul 28, 2013
 *      Author: marcin
 */

#ifndef API_H_
#define API_H_

#include	"diplodocus.h"
#include	"KeyManager.h"
#include 	"TypesHierarchy.h"
#include	"TemplateManager.h"
#include	"Molecules.h"
using namespace diplo;
namespace queries {

//queriesStart
// #define SHOW_STATS
#define PrintStats {queriesStart << rs << "\t" << diplo::moleculesCounter << "\t" << diplo::ProvFilterCounter << "\t" << ps.size() << "\t0\t0\t0\t" << diplo::elementsChecked << "\t" <<  diplo::elementsRetrieved <<  endl;}
#define PrintStats2 {queriesStart << rs << "\t" << moleculesCounter << "\t" << diplo::ProvFilterCounter << "\t" << ps.size() << "\t" << im << "\t" << imf << "\t" << is << "\t" <<  diplo::elementsChecked << "\t" <<  diplo::elementsRetrieved <<  endl;}
//#define ProvOut {for (auto it : ps) provOutput << "<http://mwylot.net/> <http://www.w3.org/ns/prov#wasAssociatedWith> " << diplo::KM.Get(it) << " <http://mwylot.net/> ." << endl;}
#define ProvOut
#define DISPLAY_RESULTS

class API {
public:
	API();
	virtual ~API();
	static void TriplePatern(vector<TripleIDs> &constrains, vector<TripleIDs> &projections, vector<vector<unordered_set<KEY_ID>>> &resultsmaster, vector<vector<unordered_set<KEY_ID>>> &provmaster );
	static void GetSubjects(KEY_ID o, KEY_ID p, unordered_multimap<KEY_ID,KEY_ID>& results);
	static void GetSubjects(KEY_ID o, KEY_ID p, unordered_set<KEY_ID> &results, vector<unordered_set<KEY_ID>> &prov);
	static void GetObjects(KEY_ID s, KEY_ID p, unordered_set<KEY_ID> &results, unordered_set<KEY_ID> &prov);
	static bool PostFiltering(vector<vector<unordered_set<KEY_ID> > >& prov, vector<vector<unordered_set<KEY_ID>>>& results);
	static void DrisplayResults(vector<vector<unordered_set<KEY_ID>>> &results);
	static void DrisplayProvenance(vector<vector<unordered_set<KEY_ID>>> &prov);
};

} /* namespace queries */
#endif /* API_H_ */
