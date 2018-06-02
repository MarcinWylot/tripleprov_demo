/*
 * diplodocus.h
 *
 *  Created on: 12-07-2012
 *      Author: marcin
 */

#ifndef DIPLODOCUS_H_
#define DIPLODOCUS_H_

//common includes
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <map>
#include <time.h>
#include <unistd.h>


#include <sys/time.h>

#include <unordered_set>

#include <dirent.h>


using namespace std;

//functions
void *DiploMalloc(size_t size);
void ExtractTripleFromLine(const string& str, vector<string>& tokens);
void tokenize(const string& str,const string& delimiters, vector<string>& tokens);
void tokenize2(const string& str,const string& delimiters, vector<string>& tokens);
void getStats();

long double getDuble(const string& str);




/// proper types deffinitions
typedef unsigned int TYPE_ID;

typedef	unsigned long int KEY_ID;
#define MASK 32
#define BASE 64

//typedef	unsigned int KEY_ID;
//#define MASK 8
//#define BASE 32

//#define IFEXITERROR                                                     if(Constante::ExitIfError){exit(0);};
//#define PRINT_ERROR                                                     false
#define ERROR_OUT {cout << endl << "ERROR in " << __FILE__ << " at line " << __LINE__ << endl;}

//#define PRINT_NO_RESULTS {cout << endl << "results.size(): " << results.size() << endl;}
#define PRINT_NO_RESULTS {}

//#define PRINT_CLIENT_TIME {data[3] = '\0'; cout << data << "\t" << diplo::stopwatch_get() << endl;}
#define PRINT_CLIENT_TIME {}


namespace queries {

class val4 {
public:
	KEY_ID v1;
	KEY_ID v2;
	KEY_ID v3;
	KEY_ID v4;
};

class val5 {
public:
	KEY_ID v1;
	KEY_ID v2;
	KEY_ID v3;
	KEY_ID v4;
	KEY_ID v5;
};

}


namespace diplo {
extern string server_adr;
extern string server_port;
extern unsigned myID;
extern unsigned nbOfClients;
extern void usage(const char * msg);
extern bool onlyPartition;
extern int maxScope;
extern int minScope;
extern void stopwatch_start();
extern double stopwatch_get();
extern string memory_usage();
extern timeval stopwatch;
extern string moleculeconffile;
extern string dbDir;
extern string srcDir;
extern int pause_int;
extern size_t network_buf_size;
extern int PartitionerRange;
extern bool ProvTrigerON;
extern int moleculesCounter;
extern int ProvFilterCounter;
extern int ProvFilterCH;
extern int resCounterCounter;
extern int elementsChecked;
extern int elementsRetrieved;
extern ofstream queriesStart;
extern ofstream benchmarkResults;
extern ofstream provOutput;
extern string statsDir;
extern string file_q;
extern string file_p;
class TripleIDs {
public:
	KEY_ID subject;
	KEY_ID predicate;
	KEY_ID object;
	KEY_ID prov;
	TripleIDs() {};
	TripleIDs(KEY_ID s,KEY_ID p,KEY_ID o,KEY_ID pr) {subject = s;predicate=p; object=o;prov=pr;}
};

}

template<typename In>
void unordered_set_intersection(In &in1, In &in2, In &out) {
	//out.clear();
	if (in2.size() < in1.size()) {
		unordered_set_intersection(in2, in1, out);
		return;
	}
	for (typename In::const_iterator it = in1.begin(); it != in1.end(); it++) {
		if (in2.find(*it) != in2.end())
			out.insert(*it);
	}
}

#endif /* DIPLODOCUS_H_ */
