/*
 * Conductor.cpp
 *
 *  Created on: 19-11-2012
 *      Author: marcin
 */

#include "Conductor.h"

#include <algorithm>

namespace diplo {
class KeyManager KM;
class TypesHierarchy TH;
class TemplateManager TM;
class Molecules M;
class Partitioner P;
TypesManager TypesM;
Server SRV;
Client CLI;
timeval stopwatch;
}

class TripleTemplateLeafs {
public:
	Leaf* subject;
	Leaf* predicate;
	Leaf* object;
	Leaf* prov;
};

//class TripleIDs {
//public:
//	KEY_ID subject;
//	KEY_ID predicate;
//	KEY_ID object;
//	KEY_ID prov;
//};

using namespace diplo;

void diplo::usage(const char * msg) {
		return;
	{
		cout << endl << endl << endl << "REPORt " << msg << endl << endl;
		string line;
		ifstream myfile("/proc/self/status");
		if (myfile.is_open()) {
			while (myfile.good()) {
				getline(myfile, line);
				cout << line << endl;
			}
			myfile.close();
		}
	}
}

Conductor::Conductor() {
	// TODO Auto-generated constructor stub

}

void Conductor::LoadData() {

	DIR* dir = opendir(srcDir.c_str());
	vector < string > files;
	string file;
	struct dirent* pos = 0;

	while ((pos = readdir(dir))) {
		file = pos->d_name;
		if (file != "." and file != ".." and file.size() >= 1 and file[0] != '.' and ( file.compare(file.size() - 3, 3, ".nt") == 0 or file.compare(file.size() - 3, 3, ".nq") ==0
// 	or              file.compare(file.size() - 8, 8, "-urified") == 0
// 	 or              file.compare(file.size() - 8, 8, ".nq.sort") == 0
	) )
			files.push_back(srcDir + "/" + file);
	}

sort (files.begin(), files.end());


	ifstream f;
	string line;
	vector < string > triple;

	/////////////////////////////
	/////////////////////////////
	//1st pass
	/////////////////////////////////
	////////////////////////////////
	//we create add TYPES to KM and we create Types Hierarchy
	cout << "first pass" << endl;
	{
		set < string > types;
		multimap < string, string > subclesses;
		multimap<string, string>::iterator it_subclesses;
		set<string>::iterator it_types;

		cout << "KM.Add" << endl;
		for (vector<string>::iterator it = files.begin(); it != files.end(); it++) {
			file = *it;
			f.open(file.c_str());
//int i = 0;
			while (getline(f, line)) {
				triple.clear();
				tokenize(line, " ", triple);
//				cout << line << endl;
//				for (vector < string >::iterator i = triple.begin(); i != triple.end(); i++) {
//					cout << "---> " << *i << endl;
//				}
//				cout << "------------------------------------------------------------------------" << endl;
//				triple.clear();
////				i++; if (i>10) return;
//				continue;


				if (triple[1] == "<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>") {
		if (triple[2] == "<http://dbpedia.org/resource/Scotland>") continue; //FIXME, this is a bad hack!!!

					KM.Add(triple[2].c_str());

					types.insert(triple[2]);

				} else if (triple[1] == "<http://www.w3.org/2000/01/rdf-schema#subClassOf>") {
		if (triple[0] == "<http://dbpedia.org/resource/Scotland>" or triple[2] == "<http://dbpedia.org/resource/Scotland>") continue; //FIXME, this is a bad hack!!!
					KM.Add(triple[0].c_str());
					KM.Add(triple[2].c_str());

					types.insert(triple[0]);
					types.insert(triple[2]);
					subclesses.insert(pair<string, string> (triple[0], triple[2]));
				}
			}
			f.close();
		}
//		return;
		KM.MakeInverseIndexTypes();

//		for (set < string >::iterator i = types.begin(); i != types.end(); i++) {
//			cout << "---> " << *i << endl;
//		}

		cout << "TH.Add" << endl;
		TH.SetNublerOfTypes(types.size());
		for (it_types = types.begin(); it_types != types.end(); it_types++) {
//			cout << "---> " << it_types->c_str() << "\t" << KM.GetSeq(it_types->c_str()) << endl;
			TH.Add(KM.GetSeq(it_types->c_str()));
		}
		cout << "TH.AddRelation" << endl;
		for (it_subclesses = subclesses.begin(); it_subclesses != subclesses.end(); it_subclesses++) {
			TH.AddRelation(KM.GetSeq(it_subclesses->first.c_str()), KM.GetSeq(it_subclesses->second.c_str()));
		}

	}

	//	usage("after 1st pass");


	unordered_map<Leaf*,unordered_map<Leaf*,int>> lin;
			unordered_map<Leaf*,unordered_map<Leaf*,int>>::iterator lin_it;
			unordered_map<Leaf*,int>::iterator ell_it;
	/////////////////////////////
	/////////////////////////////
	//2nd pass
	/////////////////////////////////
	////////////////////////////////
	//we add other values to the KM, group them by types and we assignd IDs to URIs
	cout << "second pass" << endl;
	{

		//



		multimap<Leaf*, TYPE_ID> tt;
		multimap<Leaf*, TYPE_ID>::iterator it_tt;
		Leaf* leaf;

		TripleTemplateLeafs TripleTemplateLeafsTmp;
		vector<TripleTemplateLeafs> tripleTemplatesLeafs;
		vector<TripleTemplateLeafs>::iterator it_tripleTemplatesLeafs;


		cout << "KM.Add, tripleTemplatesLeafs.push_back" << endl;
		for (vector<string>::iterator it = files.begin(); it != files.end(); it++) {
			file = *it;
			f.open(file.c_str());

			//add URI to the KM, and store somehow triple templates to create molecu;e templates afterwords
			while (getline(f, line)) {
				triple.clear();
				tokenize(line, " ", triple);
//				cerr << line << endl;
//				cerr << triple[0] << " -- " << triple[1] << " -- " << triple[3] << " -- " << triple[3] << " -- " << endl;
				if (triple[1] == "<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>") {
					leaf = KM.Add(triple[0].c_str());
					//here store somehow types for objects
					tt.insert(pair<Leaf*, TYPE_ID> (leaf, KM.GetSeq(triple[2].c_str())));

				} else if (triple[1] != "<http://www.w3.org/2000/01/rdf-schema#subClassOf>") {

//					if (triple[3] == "<http://mwylot.net/>") cerr << "got it" << endl;

					TripleTemplateLeafsTmp.subject = KM.Add(triple[0].c_str());
					TripleTemplateLeafsTmp.predicate = KM.Add(triple[1].c_str());
					TripleTemplateLeafsTmp.object = KM.Add(triple[2].c_str());
					TripleTemplateLeafsTmp.prov = KM.Add(triple[3].c_str());


//					Leaf* ff = TripleTemplateLeafsTmp.predicate;
//					lin_it = lin.find(TripleTemplateLeafsTmp.prov);
//					if(lin_it == lin.end()) {
//						unordered_map<Leaf*,int> x;
//						x.insert(pair<Leaf*,int>( ff, 1 ) );
//						lin.insert(
//								pair< Leaf*,unordered_map<Leaf*,int> >(TripleTemplateLeafsTmp.prov, x)
//								);
//					} else {
//						ell_it = lin_it->second.find( ff );
//						if (ell_it == lin_it->second.end() ) {
//							lin_it->second.insert(pair<Leaf*,int>( ff, 1 ) );
//						} else {
//							ell_it->second++;
//						}
//					}



//					if(triple[2] == "\"Paul Boateng\"") {
//						cerr << line << endl;
//						cerr << "--"<<triple[0] << "--\t--" << triple[1] << "--\t--" << triple[2] << "--\t--" << triple[3] << "--"<< endl << endl;
//					}

					if (diplo::onlyPartition == true) //otherwise it isn't needed
						tripleTemplatesLeafs.push_back(TripleTemplateLeafsTmp);

				}

//				triple.clear();
			}

			f.close();
		}

		cout << "KM.MakeInverseIndex" << endl;
		KM.MakeInverseIndex(&tt);


//		for (lin_it = lin.begin(); lin_it != lin.end(); lin_it++ ) {
//			cout << (*(lin_it->first)).id << endl;
//			for (ell_it = lin_it->second.begin(); ell_it != lin_it->second.end(); ell_it++ ) {
//				cout << "\t" << (*(ell_it->first)).id << "\t" << ell_it->second << endl;
//			}
//		}
//		exit(1);

if (diplo::onlyPartition == false) return;

		TripleIDs tripleIDsTmp;
		vector<TripleIDs> tripleIDs;
		vector<TripleIDs>::iterator it_tripleIDs;
		for (it_tripleTemplatesLeafs = tripleTemplatesLeafs.begin(); it_tripleTemplatesLeafs != tripleTemplatesLeafs.end(); it_tripleTemplatesLeafs++) {
			tripleIDsTmp.subject = it_tripleTemplatesLeafs->subject->id;
			tripleIDsTmp.predicate = it_tripleTemplatesLeafs->predicate->id;
			tripleIDsTmp.object = it_tripleTemplatesLeafs->object->id;
			tripleIDsTmp.prov = it_tripleTemplatesLeafs->prov->id;

			tripleIDs.push_back(tripleIDsTmp);
		}


		//create Molecule templates
		TripleTemplate tripleTemplatesTypesTmp;
		set<TripleTemplate> tripleTemplatesTypes;
		set<TripleTemplate>::iterator it_tripleTemplatesTypes;

		for (it_tripleTemplatesLeafs = tripleTemplatesLeafs.begin(); it_tripleTemplatesLeafs != tripleTemplatesLeafs.end(); it_tripleTemplatesLeafs++) {
			tripleTemplatesTypesTmp.subjectType = KM.GetType(it_tripleTemplatesLeafs->subject->id);
			tripleTemplatesTypesTmp.predicateID = it_tripleTemplatesLeafs->predicate->id;
			tripleTemplatesTypesTmp.objectType = KM.GetType(it_tripleTemplatesLeafs->object->id);

			tripleTemplatesTypes.insert(tripleTemplatesTypesTmp);

		}

		cout << "TM.AddTemplate" << endl;
		for (it_tripleTemplatesTypes = tripleTemplatesTypes.begin(); it_tripleTemplatesTypes != tripleTemplatesTypes.end(); it_tripleTemplatesTypes++) {
			TM.AddTemplate(*it_tripleTemplatesTypes);
		}


		cout << "TM.AddTripleInScope up to minScope: " << diplo::minScope << endl;
		for (int scope = 1; scope <= diplo::minScope; scope++) {
			for (it_tripleTemplatesTypes = tripleTemplatesTypes.begin(); it_tripleTemplatesTypes != tripleTemplatesTypes.end(); it_tripleTemplatesTypes++) {
				TM.AddTriple(*it_tripleTemplatesTypes,scope);
			}
		}


		//read config for Molecule Templates
		string uri, prefix;
		f.open(moleculeconffile.c_str());
		vector < string > pathStrings;
		vector < vector<TYPE_ID> > pathIDs;
		vector<TYPE_ID> *it_pathIDs;
		getline(f, prefix);

		while (getline(f, line)) {
			if (line[0] == '#') continue;
			tokenize2(line, " ", pathStrings);
			pathIDs.push_back(vector<TYPE_ID> ());
			it_pathIDs = &pathIDs[pathIDs.size() - 1];
			for (vector<string>::iterator it = pathStrings.begin(); it != pathStrings.end(); it++) {
				uri = "<" + prefix + *it + ">";
				it_pathIDs->push_back(KM.GetSeq(uri.c_str()));
			}
			pathStrings.clear();

		}

		set<TYPE_ID> subclasses;

		unsigned short int scope;
		TYPE_ID root;
		cout << "TM.AddTripleInScope upto mxaScope (from conf file): "<< diplo::maxScope << endl;
		for (vector<vector<TYPE_ID> >::iterator it = pathIDs.begin(); it != pathIDs.end(); it++) {
			root = * (it->begin()) ;
			set<TYPE_ID> types = diplo::TH.GetHierarchyForType( root );
			for (set<TYPE_ID>::iterator compositType = types.begin(); compositType != types.end(); compositType++) {
				vector<TYPE_ID>::iterator it2 = it->begin();
				root = *compositType;
				it2++;
				scope = 1;
				for (; it2 != it->end(); it2++) {
					for (it_tripleTemplatesTypes = tripleTemplatesTypes.begin(); it_tripleTemplatesTypes != tripleTemplatesTypes.end(); it_tripleTemplatesTypes++) {
						subclasses = TH.GetHierarchyForType(*it2);
						set<TYPE_ID>::iterator it3;
						for (it3 = subclasses.begin(); it3 != subclasses.end(); it3++) {

							if (it_tripleTemplatesTypes->subjectType == *it3) {
								TM.AddTripleInScope(*it_tripleTemplatesTypes, root, scope);
							}
						}

					}
					scope++;
				}
			}
		}

//store triples
	cout << "storing triples" << endl;
		fstream file;
		size_t s = tripleIDs.size();
		try {
			file.open(diplo::dbDir+"triples", ios::binary | ios::out);
			file.write( (char*)  &s, sizeof(s));
			file.write( (char*)  tripleIDs.data(), s * sizeof(TripleIDs));
			file.close();

		} catch (fstream::failure &e) {
			cerr << "Exception writing file";
		}

		{
			unordered_set<KEY_ID> molecules_tmp;
			cout << "P.PartitionMolecules" << endl;
			//create Molecules, and add scole 0 for everything
			for (it_tripleIDs = tripleIDs.begin(); it_tripleIDs != tripleIDs.end(); it_tripleIDs++) {
				if (KM.GetType(it_tripleIDs->subject) > 1) { //FIXME!!! there will be an issue TYPE 2 is GENERIC
					molecules_tmp.insert(it_tripleIDs->subject);
				}
			}
			cout << "all molecules on master: " << molecules_tmp.size() << endl;
			P.PartitionMolecules(&molecules_tmp);
			P.SendMolecules();
		}


		TM.Store();
		TH.Store();

usage("END");
//for a moment we don't need it, not on a masted node
//return;
		cout << "M.CreateMolecule-----" << endl;
		//for statistics

//		KEY_ID x = diplo::KM.Get ( "<http://mwylot.net/>" );
//		cerr << x << endl;
		//create Molecules, and add scole 0 for everything

		for (it_tripleTemplatesLeafs = tripleTemplatesLeafs.begin(); it_tripleTemplatesLeafs != tripleTemplatesLeafs.end(); it_tripleTemplatesLeafs++) {
//			if (x == it_tripleTemplatesLeafs->prov->id  ) cout << "\tXXXXXXXX------> " << diplo::KM.Get (it_tripleTemplatesLeafs->prov->id ) << endl; //MW
			if (KM.GetType(it_tripleTemplatesLeafs->subject->id) >= 2) {
				M.CreateMolecule(it_tripleTemplatesLeafs->subject->id, it_tripleTemplatesLeafs->predicate->id, it_tripleTemplatesLeafs->object->id, it_tripleTemplatesLeafs->prov->id);
			}

			//here we do provenance index

//			if (it_tripleTemplatesLeafs->prov->id == 999517642299539457) {
//				cout << "\tXXXXXXXX------> " << diplo::KM.Get (it_tripleTemplatesLeafs->subject->id )
//								<< "\t" << diplo::KM.Get (it_tripleTemplatesLeafs->predicate->id )
//								<< "\t" << diplo::KM.Get (it_tripleTemplatesLeafs->object->id )
//								<< "\t" << diplo::KM.Get (it_tripleTemplatesLeafs->prov->id )
//						<< endl; //MW
//			}

			auto provIT = ProvIdx.find(it_tripleTemplatesLeafs->prov->id);
			if (provIT == ProvIdx.end()) {
				ProvIdx.insert( { { it_tripleTemplatesLeafs->prov->id, { it_tripleTemplatesLeafs->subject->id } } });
			} else {
				provIT->second.insert(it_tripleTemplatesLeafs->subject->id);
			}

			//for statistics, remove for benchmarking
			auto it_contextTriples = contextTriples.find(it_tripleTemplatesLeafs->prov->id);
			if (it_contextTriples == contextTriples.end()) {
				contextTriples.insert( { { it_tripleTemplatesLeafs->prov->id, { 1 } } });
			} else {

				it_contextTriples->second++;
			}

		}



		//add edges, further scopes from 1..n
		for (int i = 1; i <= diplo::maxScope; i++) {
			cout << "M.AddTriple in scope: " << i << endl;
			for (it_tripleTemplatesLeafs = tripleTemplatesLeafs.begin(); it_tripleTemplatesLeafs != tripleTemplatesLeafs.end(); it_tripleTemplatesLeafs++) {
				M.AddTriple(it_tripleTemplatesLeafs->subject->id, it_tripleTemplatesLeafs->predicate->id, it_tripleTemplatesLeafs->object->id, it_tripleTemplatesLeafs->prov->id, i);

			}
		}
	}

}

void Conductor::HandleQueriesLUBMnetwork() {
	diplo::SRV.Start();
//	queries::LUBM_network lubm;

//	diplo::TM.Display();
//	diplo::TH.Display();

//	diplo::stopwatch_start();
//	lubm.q02s1_m();
//	cout << "q02s1_m: " << diplo::stopwatch_get() << endl;
//	diplo::stopwatch_start();
//	lubm.q02s2_m();
//	cout << "q02s2_m: " << diplo::stopwatch_get() << endl;
//	return;

//
//	vector< vector<double> > times4exel;
//	times4exel.resize(17);
//	double time;
//
//	for (int i = 0; i < 11; i++) {
//		cout << "--------------------- ROUND: " << i << "---------------------" << endl;
//		diplo::stopwatch_start();
//		lubm.q01_m();
//		time = diplo::stopwatch_get();
//		times4exel[0].push_back(time);
//		cout << "q01: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q02s1_m();
//		time = diplo::stopwatch_get();
//		times4exel[1].push_back(time);
//		cout << "q02s1: " << time << endl;
//		sleep(diplo::pause_int);
//
//		if (diplo::maxScope == 1) {
//			diplo::stopwatch_start();
//			lubm.q02s2_m();
//			time = diplo::stopwatch_get();
////				times4exel[0].push_back(time);
//			cout << "q02s2: " << time << endl;
//			sleep(diplo::pause_int);
//		}
//
//		diplo::stopwatch_start();
//		lubm.q03_m();
//		time = diplo::stopwatch_get();
//		times4exel[2].push_back(time);
//		cout << "q03: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q04_m();
//		time = diplo::stopwatch_get();
//		times4exel[3].push_back(time);
//		cout << "q04: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q05_m();
//		time = diplo::stopwatch_get();
//		times4exel[4].push_back(time);
//		cout << "q05: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q06_m();
//		time = diplo::stopwatch_get();
//		times4exel[5].push_back(time);
//		cout << "q06: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q07_m();
//		time = diplo::stopwatch_get();
//		times4exel[6].push_back(time);
//		cout << "q07: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q08s1_m();
//		time = diplo::stopwatch_get();
//		times4exel[7].push_back(time);
//		cout << "q08s1: " << time << endl;
//		sleep(diplo::pause_int);
//
//		if (diplo::maxScope == 1) {
//			diplo::stopwatch_start();
//			lubm.q08s2_m();
//			time = diplo::stopwatch_get();
////				times4exel[0].push_back(time);
//			cout << "q08s2: " << time << endl;
//			sleep(diplo::pause_int);
//		}
//
//		diplo::stopwatch_start();
//		lubm.q09s1_m();
//		time = diplo::stopwatch_get();
//		times4exel[8].push_back(time);
//		cout << "q09s1: " << time << endl;
//		sleep(diplo::pause_int);
//
//		if (diplo::maxScope == 1) {
//			diplo::stopwatch_start();
//			lubm.q09s2_m();
//			time = diplo::stopwatch_get();
////				times4exel[0].push_back(time);
//			cout << "q09s2: " << time << endl;
//			sleep(diplo::pause_int);
//		}
//		diplo::stopwatch_start();
//		lubm.q10_m();
//		time = diplo::stopwatch_get();
//		times4exel[9].push_back(time);
//		cout << "q10: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q11s1_m();
//		time = diplo::stopwatch_get();
//		times4exel[10].push_back(time);
//		cout << "q11s1: " << time << endl;
//		sleep(diplo::pause_int);
//
//		if (diplo::maxScope == 1) {
//			diplo::stopwatch_start();
//			lubm.q11s2_m();
//			time = diplo::stopwatch_get();
////				times4exel[0].push_back(time);
//			cout << "q11s2: " << time << endl;
//			sleep(diplo::pause_int);
//		}
//
//		diplo::stopwatch_start();
//		lubm.q12_m();
//		time = diplo::stopwatch_get();
//		times4exel[11].push_back(time);
//		cout << "q12: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q13_m();
//		time = diplo::stopwatch_get();
//		times4exel[12].push_back(time);
//		cout << "q13: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q14_m();
//		time = diplo::stopwatch_get();
//		times4exel[13].push_back(time);
//		cout << "q14: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q15_m();
//		time = diplo::stopwatch_get();
//		times4exel[14].push_back(time);
//		cout << "q15: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		lubm.q16s1_m();
//		time = diplo::stopwatch_get();
//		times4exel[15].push_back(time);
//		cout << "q16s1: " << time << endl;
//		sleep(diplo::pause_int);
//
//		if (diplo::maxScope == 1) {
//			diplo::stopwatch_start();
//			lubm.q16s2_m();
//			time = diplo::stopwatch_get();
////				times4exel[0].push_back(time);
//			cout << "q16s2: " << time << endl;
//			sleep(diplo::pause_int);
//		}
//
//		diplo::stopwatch_start();
//		lubm.q17_m();
//		time = diplo::stopwatch_get();
//		times4exel[16].push_back(time);
//		cout << "q17: " << time << endl;
//		sleep(diplo::pause_int);
//	}
//
//
//	/////printing for exel
//	{
//		cout << endl << endl;
//		for (vector<vector<double> >::iterator q = times4exel.begin(); q != times4exel.end(); q++) {
//			for (size_t r = 0; r < q->size(); r++) {
//				string n = to_string( (*q)[r] );
//				replace(n.begin(), n.end(),'.',',');
//				cout << n << "\t";
//			}
//			cout << endl;
//		}
//	}

}

void Conductor::HandleQueriesDBPnetwork() {
//	diplo::SRV.Start();
//	queries::LUBM_network lubm; //FIXME why without this line it breaks????????
//
//	queries::DBP_network dbp;
//
//	vector< vector<double> > times4exel;
//	times4exel.resize(5);
//	double time;
//
//	for (int i = 0; i < 11; i++) {
//		cout << "--------------------- ROUND: " << i << "---------------------" << endl;
//		diplo::stopwatch_start();
//		dbp.q01_m();
//		time = diplo::stopwatch_get();
//		times4exel[0].push_back(time);
//		cout << "q01: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		dbp.q02_m();
//		time = diplo::stopwatch_get();
//		times4exel[1].push_back(time);
//		cout << "q02: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		dbp.q03_m();
//		time = diplo::stopwatch_get();
//		times4exel[2].push_back(time);
//		cout << "q03: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		dbp.q04_m();
//		time = diplo::stopwatch_get();
//		times4exel[3].push_back(time);
//		cout << "q04: " << time << endl;
//		sleep(diplo::pause_int);
//
//		diplo::stopwatch_start();
//		dbp.q05_m();
//		time = diplo::stopwatch_get();
//		times4exel[4].push_back(time);
//		cout << "q05: " << time << endl;
//		sleep(diplo::pause_int);
//	}
//
//
//	/////printing for exel
//	{
//		cout << endl << endl;
//		for (vector<vector<double> >::iterator q = times4exel.begin(); q != times4exel.end(); q++) {
//			for (size_t r = 0; r < q->size(); r++) {
//				string n = to_string((*q)[r]);
//				replace(n.begin(), n.end(), '.', ',');
//				cout << n << "\t";
//			}
//			cout << endl;
//		}
//	}

}

Conductor::~Conductor() {
	// TODO Auto-generated destructor stub
}
