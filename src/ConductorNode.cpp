/*
 * ConductorNode.cpp
 *
 *  Created on: 22-03-2013
 *      Author: marcin
 */

#include "ConductorNode.h"

using namespace diplo;

//class TripleIDs {
//public:
//	KEY_ID subject;
//	KEY_ID predicate;
//	KEY_ID object;
//	KEY_ID prov;
//};

ConductorNode::ConductorNode() {
	// TODO Auto-generated constructor stub

}

void ConductorNode::LoadData() {
	//store triples
//	class TripleIDs tripleIDsTmp;
	vector<TripleIDs> tripleIDs;
	vector<TripleIDs>::iterator it_tripleIDs;

	fstream file;

//	size_t cnt = 1981056;
	size_t s;
//	tripleIDs.resize(cnt/sizeof(TripleIDs));
//	TripleIDs* p = tripleIDs.data();
//	cout << "cnt: " << cnt << "\t cnt/sizeof(TripleIDs): " << cnt/sizeof(TripleIDs) <<  endl << endl;
	try {
		file.open(diplo::dbDir+"triples", ios::binary | ios::in);
		file.read( (char*)  &s, sizeof(s));
		tripleIDs.resize(s);


		file.read((char*) tripleIDs.data(), s * sizeof(TripleIDs));
		file.close();

	} catch (fstream::failure &e) {
		cerr << "Exception reading file";
	}


	readMyMolecules();
	TH.Restore();
	TM.Restore();



	cout << "M.CreateMoleculeXXXXXXX" << endl;

	//for statistics
	unordered_map<KEY_ID,int> contextTriples;

	//create Molecules, and add scole 0 for everything
	for (it_tripleIDs = tripleIDs.begin(); it_tripleIDs != tripleIDs.end(); it_tripleIDs++) {
//		if (KeyManager::GetType(it_tripleIDs->subject) > 2 and
		if (myMolecules.find(it_tripleIDs->subject) != myMolecules.end() ) {
			TypesM.AddElement(KeyManager::GetType( it_tripleIDs->subject), it_tripleIDs->subject);
			TypesM.AddElement(KeyManager::GetType( it_tripleIDs->object), it_tripleIDs->object);
			M.CreateMolecule(it_tripleIDs->subject, it_tripleIDs->predicate, it_tripleIDs->object, it_tripleIDs->prov);
		}

		//here we do provenance index

		auto provIT = ProvIdx.find(it_tripleIDs->prov);
		if ( provIT == ProvIdx.end()) {
			ProvIdx.insert({{it_tripleIDs->prov,{it_tripleIDs->subject} }});
		} else {
			provIT->second.insert(it_tripleIDs->subject);
		}

		//for statistics, remove for benchmarking
		auto it_contextTriples = contextTriples.find(it_tripleIDs->prov);
		if (it_contextTriples == contextTriples.end())
			contextTriples.insert( { { it_tripleIDs->prov, { 1 } } });
		else it_contextTriples->second++;


	}


//		usage("before scope");
	//add edges, further scopes from 1..n
//		cout << "M.AddTriple" << endl;
	for (int i = 1; i <= diplo::maxScope; i++) {
		cout << "M.AddTriple in scope: " << i << endl;
		for (it_tripleIDs = tripleIDs.begin(); it_tripleIDs != tripleIDs.end(); it_tripleIDs++) {
			M.AddTriple(it_tripleIDs->subject, it_tripleIDs->predicate, it_tripleIDs->object, it_tripleIDs->prov, i);

		}
			cout << "----> scope: " << i << endl;
			usage("after scope");
	}
//	usage("loaded data");
}

void ConductorNode::readMyMolecules() {
	vector<KEY_ID> myMolecules_tmp;
	size_t size = 0;
	fstream file;
	try {
		file.open(diplo::dbDir+"node" + to_string(diplo::myID), ios::in | ios::out | ios::app | ios::binary | ios::out);
		file.read((char*) &size, sizeof(size));
		myMolecules_tmp.resize(size);
		file.read((char*) myMolecules_tmp.data(), size*sizeof(KEY_ID));
		file.close();

	} catch (fstream::failure &e) {
		cerr << "Exception reading file";
	}

	copy(myMolecules_tmp.begin(), myMolecules_tmp.end(), inserter(myMolecules,myMolecules.begin() ) );

	cout << "Number of molecules for this node: " << myMolecules.size() << endl;


//	for (unordered_set<KEY_ID>::iterator i = myMolecules.begin(); i != myMolecules.end(); i++) {
//		cout << *i << endl;
//	}
}

void ConductorNode::HandleQueries() {
//	diplo::CLI.Start();
//	queries::LUBM_network lubm;
//	queries::DBP_network dbp;
//
//	char *data = (char *)malloc(diplo::network_buf_size);
//	while(1) {
//		bzero(data,diplo::network_buf_size);
//		diplo::CLI.Receive(data, diplo::network_buf_size);
//		diplo::stopwatch_start();
//		if (strncmp(data,"q01",3) == 0 ) {
//			lubm.q01_n(data+3);
//		} else if (strncmp(data,"q02s1",5) == 0 ) {
//			lubm.q02s1_n(data+5);
//		} else if (strncmp(data,"q02s2",5) == 0 ) {
//			lubm.q02s2_n(data+5);
//		} else if (strncmp(data,"q03",3) == 0 ) {
//				lubm.q03_n(data+3);
//		} else if (strncmp(data,"q04",3) == 0 ) {
//			lubm.q04_n(data+3);
//		} else if (strncmp(data,"q05",3) == 0 ) {
//			lubm.q05_n(data+3);
//		} else if (strncmp(data,"q06",3) == 0 ) {
//			lubm.q06_n(data+3);
//		} else if (strncmp(data,"q07",3) == 0 ) {
//			lubm.q07_n(data+3);
//		} else if (strncmp(data,"q08s1",5) == 0 ) {
//			lubm.q08s1_n(data+5);
//		} else if (strncmp(data,"q08s2",5) == 0 ) {
//			lubm.q08s2_n(data+5);
//		} else if (strncmp(data,"q09s1",5) == 0 ) {
//			lubm.q09s1_n(data+5);
//		} else if (strncmp(data,"q09s2",5) == 0 ) {
//			lubm.q09s2_n(data+5);
//		} else if (strncmp(data,"q10",3) == 0 ) {
//			lubm.q10_n(data+3);
//		} else if (strncmp(data,"q11s1",5) == 0 ) {
//			lubm.q11s1_n(data+5);
//		} else if (strncmp(data,"q11s2",5) == 0 ) {
//			lubm.q11s2_n(data+5);
//		} else if (strncmp(data,"q12",3) == 0 ) {
//			lubm.q12_n(data+3);
//		} else if (strncmp(data,"q13",3) == 0 ) {
//			lubm.q13_n(data+3);
//		} else if (strncmp(data,"q14",3) == 0 ) {
//			lubm.q14_n(data+3);
//		} else if (strncmp(data,"q15",3) == 0 ) {
//			lubm.q15_n(data+3);
//		} else if (strncmp(data,"q16s1",5) == 0 ) {
//			lubm.q16s1_n(data+5);
//		} else if (strncmp(data,"q16s2",5) == 0 ) {
//			lubm.q16s2_n(data+5);
//		} else if (strncmp(data,"q17",3) == 0 ) {
//			lubm.q17_n(data+3);
//		} else if (strncmp(data, "Dq01", 4) == 0) {
//			dbp.q01_n(data + 4);
//		} else if (strncmp(data, "Dq02", 4) == 0) {
//			dbp.q02_n(data + 4);
//		} else if (strncmp(data, "Dq03", 4) == 0) {
//			dbp.q03_n(data + 4);
//		} else if (strncmp(data, "Dq04", 4) == 0) {
//			dbp.q04_n(data + 4);
//		} else if (strncmp(data, "Dq05", 4) == 0) {
//			dbp.q05_n(data + 4);
//		}
//
//		PRINT_CLIENT_TIME
////		return;
//	}
//	free (data);

}

ConductorNode::~ConductorNode() {
	// TODO Auto-generated destructor stub
}

