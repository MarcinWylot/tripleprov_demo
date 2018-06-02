/*
 * Functions.cpp
 *
 *  Created on: 23-07-2012
 *      Author: marcin
 */

#include	"diplodocus.h"


void *DiploMalloc(size_t size) {
	void *t;
	t = malloc(size);
	bzero(t,size);
	return t;
}

void ExtractTripleFromLine(const string& str, vector<string>& tokens)
{

	string::size_type lastPos = str.find_first_of('<', 0);
	// find first "non-delimiter".
	string::size_type pos = str.find_first_of('>', lastPos);
	cerr << str.substr(lastPos, pos - lastPos + 1) << "---";

	lastPos = str.find_first_of('<', lastPos);
	pos = str.find_first_of('>', lastPos);
	cerr << str.substr(lastPos, pos - lastPos + 1) << endl;


//	while (string::npos != pos || string::npos != lastPos)
//	{
//    	// found a token, add it to the vector.
//    	tokens.push_back(str.substr(lastPos, pos - lastPos));
//
//    	// skip delimiters.  Note the "not_of"
//    	lastPos = str.find_first_not_of(delimiters, pos);
//
//    	// find next "non-delimiter"
//    	pos = str.find_first_of(delimiters, lastPos);
//	}
//
//	char *c, *c1 ;
//	static char cstr[2048];
//	cstr[2047] = '\0';
//	strncpy (cstr, line->c_str(), 2047);
//	c=cstr;
//
//	if ((c=strchr (c,'<'))) { c++; if((c1=strchr (c,'>'))) {*c1=0; *s=c; c = c1+1;}}
//	if ((c=strchr (c,'<'))) { c++; if((c1=strchr (c,'>'))) {*c1=0; *p=c; c = c1+1;}}
//
//
//	if ((c=strchr (c1+1,'<'))) { c++; c1=strchr (c,'>');}
//	else if ((c=strchr (c1+1,'"'))) { c++; c1=strchr (c,'"');}
//	if(c && c1) {*c1=0;*o=c;}
//
//
//	if ( s->empty() or p->empty() or o->empty()) cerr << "fuck" << endl;

}


long double getDuble(const string& str)
{
	string::size_type lastPos = str.find_first_not_of('"', 0);
	string::size_type pos = str.find_first_of('"', lastPos);
	return stold(str.substr(lastPos, pos - lastPos));
}

void tokenize(const string& str,const string& delimiters, vector<string>& tokens)
{
	//SUBJECT
	// skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// find first "non-delimiter".
	string::size_type pos = str.find_first_of(delimiters, lastPos);
	string s = str.substr(lastPos, pos - lastPos);
	tokens.push_back(s);


	//PREDICATE
	// skip delimiters.  Note the "not_of"
	lastPos = str.find_first_not_of(delimiters, pos);
	// find next "non-delimiter"
	pos = str.find_first_of(delimiters, lastPos);
	s = str.substr(lastPos, pos - lastPos);
	tokens.push_back(s);


	//PREDICATE
	// skip delimiters.  Note the "not_of"
	lastPos = str.find_first_not_of(delimiters, pos);


	pos = str.find_last_of(delimiters);
//	cerr << str.size() << "\t" << pos << "\t" << lastPos << endl;
	s = str.substr(lastPos, pos - lastPos);

	pos = s.find(" <", 1);
	string r = s.substr(0, pos);

	tokens.push_back(r);


	r = s.substr(pos+1, s.size());
	r.erase(r.find_last_not_of(" \t")+1);
	tokens.push_back(r);




	return;
}
void tokenize2(const string& str, const string& delimiters, vector<string>& tokens) {
	// skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// find first "non-delimiter".
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos) {
		// found a token, add it to the vector.
		string s = str.substr(lastPos, pos - lastPos);
		tokens.push_back(s);

		// skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

