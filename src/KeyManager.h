/*
 * KeyManager.h
 *
 *  Created on: 12-07-2012
 *      Author: marcin
 */

#ifndef KEYMANAGER_H_
#define KEYMANAGER_H_

#include	"diplodocus.h"
#include	"TemplateManager.h"


class Leaf {
public:
	KEY_ID id; //key_id (combined type & seq)
//	class Node *node; //pointer to the parent Node ???
	//KEY_ID id** // multiplr id for one element if there is more than one type of certain an dthen type is in each ID
//	TYPE_ID	**tid; //terminated by NULL
//	size_t len;
	Leaf(KEY_ID _id);
	~Leaf();
};

class Node {
public:
//	static KEY_ID leafIDautoincrement;

	char* uri; //terminated by NULL
//	size_t len; //TODO cumulated size of string
	class Node **next; //many children, terminated by NULL
	class Node *prev; //only one parent
	class Leaf *leaf; //NULL in no leaf, otherwise leaf

	class Node * Add(char *part1, const char *part2);
	static TYPE_ID GetType(KEY_ID id);
	KEY_ID Get(const char* _uri);
	string GetUri();
	void FindAdd(const char *part2, bool *done, class Node ** newNode);
	void MakeInverseIndexTypes(vector< class Types >* types);
	void MakeInverseIndex(vector<class Types>* types, multimap<Leaf*, TYPE_ID>* data);
	void Display();
	Node();
	~Node();
};



class Types {
public:
//	KEY_ID counter;
	map< KEY_ID, class Node * > elements; //FIXME!! waste of memory, eliminate set/vector etc....
	unordered_set< KEY_ID > elements2;

	void Display();
	KEY_ID GetNextSeq();
	Types();
	~Types();
};

class KeyManager {
	class Node **uri_id; //head
	vector< class Types > types; /*
									0 - unclassified URI
									1 - type
									2 - unclassified (same as 0)
								*/

//	class IdUri **id_uri;
public:
	Leaf* Add(const char * _uri);
	void Display();
	void MakeInverseIndexTypes();
	void MakeInverseIndex(multimap<Leaf*, TYPE_ID>* data);
	string Get(KEY_ID id);
	KEY_ID Get(const char* _uri);
	static TYPE_ID GetType(KEY_ID id);
	TYPE_ID GetType(const char* _uri);
	size_t GetNbOfTypes();
	KEY_ID GetSeq(KEY_ID id);
	static KEY_ID GetId(KEY_ID seq, TYPE_ID tid);
	KEY_ID GetSeq(const char* _uri);
	map< KEY_ID, class Node * > * GetElementsOfType( TYPE_ID _type);
	KeyManager();
	~KeyManager();
};

namespace diplo {
	extern class KeyManager KM;
}

#endif /* KEYMANAGER_H_ */
