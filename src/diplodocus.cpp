//============================================================================
// Name        : diplodocus.cpp
// Author      : marcin
// Version     : 2.0
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include	"diplodocus.h"
#include	"Conductor.h"
//#include	"LUBM.h"
#include	"LUBMnetwork.h"
#include	"DBPnetwork.h"
#include	"PageManager.h"
#include	"ConductorNode.h"
#include	"BTCprov.h"
#include	"WDCqueries.h"

#include <algorithm>

using namespace diplo;
namespace diplo {
string server_adr;
string server_port;
unsigned myID;
unsigned nbOfClients;
bool onlyPartition;
int maxScope;
int minScope;
string moleculeconffile;
string dbDir;
string srcDir;
int pause_int;
int moleculesCounter;
int ProvFilterCounter;
int ProvFilterCH;
int resCounterCounter;
size_t network_buf_size;
int PartitionerRange;
ofstream queriesStart;
ofstream benchmarkResults;
ofstream provOutput;
int elementsChecked;
int elementsRetrieved;
unordered_map<KEY_ID,int> contextTriples;
unordered_set<KEY_ID> ProvUris;
unordered_set<KEY_ID> ProvMolecules;
unordered_map<KEY_ID, unordered_set<KEY_ID>> ProvIdx; //context value -> molecules
bool ProvTrigerON;
string statsDir;
string file_q;
string file_p;
}


#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int main(int argc, char *argv[]) {
    char hostname[128];
    bzero(hostname,128);
    gethostname(hostname, 128);

    string fn = argv[0];
    diplo::server_port = "9999";
    diplo::onlyPartition = false;
    diplo::nbOfClients = 1;
    diplo::moleculeconffile = "";
    diplo::dbDir = "";
    diplo::pause_int = 0;
    diplo::maxScope = 0;
    diplo::minScope = 0;
    diplo::network_buf_size = 56;
    diplo::PartitionerRange = 1;
    diplo::ProvTrigerON = false;
    statsDir="btc/";



     diplo::srcDir = "../dataset/";
				
				if (argc >= 2) diplo::file_q = argv[1];
				if (argc == 3) diplo::file_p = argv[2];
				
				
    diplo::stopwatch_start();
    diplo::onlyPartition = true;
    Conductor dipl;
    dipl.LoadData();



    cout << "Loading Time: " << diplo::stopwatch_get() << endl;
    cout << "Memory: " << diplo::memory_usage() << endl;

    queries::BTCprov q;

		if (!diplo::file_q.empty()) { 
			q.TPDemo();	
			return 0;
		}

}


void diplo::stopwatch_start() {
    gettimeofday(&diplo::stopwatch,0);
}
double diplo::stopwatch_get() {
    timeval tmp;
    gettimeofday(&tmp,0);
    return (tmp.tv_sec - diplo::stopwatch.tv_sec) + (tmp.tv_usec - diplo::stopwatch.tv_usec)/1000000.0;
}

string diplo::memory_usage() {
    string l;
    ifstream my("/proc/self/status");
    if (my.is_open()) {
        while (getline(my, l)) {
            if (l.find("VmSize:") != string::npos) {
                return l;
            }
        }
        my.close();
    }
    return "no data";

}


void getStats() {
    cout << "writing stats" << endl;
    ofstream contextTriplesstats(statsDir+"contextTriples.stats");
    for (auto i : contextTriples) {
        contextTriplesstats << i.first << "\t" << i.second << endl;
    }

    ofstream ProvIdxstats(statsDir+"ProvIdx.stats");
    for (auto i : ProvIdx) {
        ProvIdxstats << i.first << "\t" << i.second.size() << endl;
    }

    ofstream Moleculesstats(statsDir+"Molecules.stats");
    for (auto i : M.molecules) {
        Moleculesstats << i.first << "\t" << i.second.next.size() << endl;
    }
}
