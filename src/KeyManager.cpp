/*
 * KeyManager.cpp
 *
 *  Created on: 12-07-2012
 *      Author: marcin
 */

#include "KeyManager.h"
#include 	"TypesHierarchy.h"

Leaf::Leaf(KEY_ID _id) {
	id = _id;
//	tid = NULL;
}

Leaf::~Leaf() {
//	free(tid);
}

//KEY_ID Node::leafIDautoincrement = 10;
class Node * Node::Add(char *part1, const char *part2) {
	size_t n;
	class Node *uri_id_tmp = NULL;
	unsigned short c = 0;
	class Node **nx, **newnext;

	class Leaf *l = new class Leaf(0);

	if (strcmp(part1,part2) == 0) {
		if (leaf == NULL) this->leaf = l;
//		cerr << "----part1 part2" << *part1 << "\t" << *part2 << endl;
		return this; //alreadu exists in the index
	}




	if (*part1 != '\0') {
//		if (strcmp ("Evaluation", this->uri) == 0)
//			cerr << "1 uri_id_tmp != NULL: " << this->uri << "\t" << part1 << "\t" << part2 << "\t" << this->prev->uri << endl;
		//the new one, takes all children of THIS and become child node for THIS
		//where we have part of URI which is not common
		uri_id_tmp = new class Node();
		if (next) {
			nx = this->next;
			while (*nx != NULL) {
				(*nx)->prev = uri_id_tmp;
				nx++;
			}
			uri_id_tmp->next = this->next;
		}
		uri_id_tmp->leaf = this->leaf;
		this->leaf = NULL;
		uri_id_tmp->prev = this;
		n = strlen(part1) + 1;
		uri_id_tmp->uri = (char *) DiploMalloc(n);
		memcpy((void *) uri_id_tmp->uri, (const void *) part1, n);

		//THIS keeps common part of URI, and becomes parent for the previews one
		*part1 = '\0';
		n = strlen(this->uri) + 1;
		char *uri_tmp = (char *) DiploMalloc(n+1);
		memcpy((void *) uri_tmp, (const void *) this->uri, n);
		free(this->uri);
		this->uri = uri_tmp;
		c = 1; //this is new node, nexts list is empty

//		if (strcmp ("Evaluation", this->uri) == 0)
//					cerr << "2 uri_id_tmp != NULL: " << this->uri << "\t" << uri_id_tmp->uri << "\t" << uri_id_tmp->prev->uri << "\t" << this->prev->uri << endl;

	} else {
		if (next) {
			nx = this->next;
			while (*nx != NULL) {
				nx++;
				c++;
			}
		}

	}

	//we are creating a new list of children
	newnext = (class Node **) calloc(c + 2, sizeof(class Node **));
	newnext[c + 1] = NULL;

	//we are increasing its size, and copying all elements from the old one
	if (uri_id_tmp) {
		newnext[c - 1] = uri_id_tmp;
	} else {
		memcpy((void **) newnext, (const void **) this->next,
				sizeof(class Node **) * c);
		free(this->next);
	}
	this->next = newnext;


//	if (strcmp ("Evaluation", this->uri) == 0)
//							cerr << "3a uri_id_tmp != NULL: " << this->uri << "\t" << this->prev->uri << endl;
//	if (uri_id_tmp == NULL)
//		cerr << "uri_id_tmp == NULL: " << this->uri << "\t" << part2 << endl;

	//adding new element (part of URI which was not common) as a new node
	uri_id_tmp = new class Node();
	n = strlen(part2) + 1;
	uri_id_tmp->uri = (char *) DiploMalloc(n);
	memcpy((void *) uri_id_tmp->uri, (const void *) part2, n);
	uri_id_tmp->prev = this;
	uri_id_tmp->leaf = l;

//	if (strcmp ("Evaluation", this->uri) == 0)
//						cerr << "3 uri_id_tmp != NULL: " << this->uri << "\t" << uri_id_tmp->uri << "\t" << uri_id_tmp->prev->uri << "\t" << this->prev->uri << endl;

//	if (prev) { cerr << "0: " << prev->uri << endl;
//		if (strcmp("owl-ontologies.com/Ontology1324312315.owl",prev->uri) == 0)
//			cerr<< "-------------------> UPSSS"<<endl<<endl;
//	}
//	cerr << "1: " << this->uri << endl;
//	cerr << "2: " << uri_id_tmp->uri << endl;


	this->next[c] = uri_id_tmp;
	return uri_id_tmp;
}

void Node::FindAdd(const char* part, bool *done, class Node ** newNode) {
	if (*done)
		return;
	class Node **nx;
	char *tmp1;
	const char *tmp2;

	tmp1 = uri;
	tmp2 = part;


	//first character is different, we are not interested by this node
	if (*tmp1 != *tmp2)
		return;


//
//	if ( strcmp(tmp1,tmp2) == 0) {
//		*newNode = Add(tmp1, tmp2);
//		*done = true;
//		return;
//	}


//	cerr << "----->1 tmp1 & tmp2: " << tmp1 << "\t" << tmp2 << endl;
	while (*tmp1 && *tmp2) {
//		cerr << "----->1aaa tmp1 & tmp2: " << tmp1 << "\t" << tmp2 << endl;
		if (*tmp1 == *tmp2) {
			tmp1++;
			tmp2++;
		} else {
//			cerr << "----->2 tmp1 & tmp2: " << tmp1 << "\t" << tmp2 << endl ;
			*newNode = Add(tmp1, tmp2);
			*done = true;
//			if (*newNode == NULL) {cerr << "-----> 1 uposss" << done << endl ; }
			return;
		}
	}

//	cerr << "part: " << part << endl;

	if (*tmp2 != '\0') {
//		cerr << "----->1 tmp1 & tmp2: " << tmp1 << "\t" << tmp2 << endl;
		if (next) {
//			cerr << "----->2 tmp1 & tmp2: " << tmp1 << "\t" << tmp2 << endl;
			nx = next;
			while (*nx != NULL) {
				(*nx)->FindAdd(tmp2, done, newNode);
				if (*done) {
					return;
				}
				nx++;
			}
		}

//		cerr << "----->1 tmp3 & tmp2: " << tmp1 << "\t" << tmp2 << endl;
		*newNode = Add(tmp1, tmp2);
		//	if (*newNode == NULL) {cerr << "-----> 3 uposss" << done << endl ; }
		*done = true;
	} else {
//		cerr << "----->3 tmp1 & tmp2: " << uri  << "\t" << part << endl ;
		//if (leaf == NULL) {
//			cerr << "1" << endl;
		*newNode = Add(uri, part);
		*done = true;
//		} else {
//			cerr << "2" << endl;
//			*newNode = this;
//			*done = true;
//		}
	}

}

void Node::Display() {
	static int i = 0;
	static string spaces;

	cout << spaces << uri;
	if (leaf) {
		cout << " ( " << leaf->id << " ) ";
		cout << "\t\t" << ++i;
	}
	cout << endl;

	if (next == NULL)
		return;


	class Node **nx;
	nx = next;
	while (*nx != NULL) {
		spaces.append("-");
		(*nx)->Display();
		spaces = spaces.substr(0, spaces.length() - 1);
		nx++;
	}
}

Node::Node() {
	uri = NULL;
	next = NULL;
	prev = NULL;
	leaf = NULL;
}

KEY_ID KeyManager::GetId(KEY_ID seq, TYPE_ID tid) {
	KEY_ID tmp;
	tmp = (KEY_ID)tid << (BASE-MASK);
	return seq | tmp;
}


void Node::MakeInverseIndexTypes(vector<class Types>* types) {

	if (leaf) {
		leaf->id = KeyManager::GetId((KEY_ID) ((*types)[1].elements.size()) +3, 1);
//		class IdUri * id = new class IdUri(this);
		(*types)[1].elements.insert(pair<KEY_ID, class Node *>(leaf->id, this) );
		//return;
	}

	if (next == NULL)
		return;

	class Node **nx;
	nx = next;
	while (*nx != NULL) {
		(*nx)->MakeInverseIndexTypes(types);
		nx++;
	}

}

void Node::MakeInverseIndex(vector<class Types>* types, multimap<Leaf*, TYPE_ID>* data) {
	//cerr << spaces << uri; //FIXME!!!
	if (leaf)
		if (leaf->id == 0) {
//			cerr << "TU trzeba popracowac: " << uri << endl;

			multimap<Leaf*, TYPE_ID>::iterator it;
			pair<multimap<Leaf*, TYPE_ID>::iterator,multimap<Leaf*, TYPE_ID>::iterator> ret;
			TYPE_ID t = 0; //by default unclassified
			ret = data->equal_range(leaf);
//			for (it=ret.first; it!=ret.second; ++it) {
//				if (t == 2) t = it->second; //if default then don't check Subclasses, means unclassified
//				else t = diplo::TH.GiveSubClass(t, it->second); //here we have something, lest check what it new one is not more specific
//			}

			set <TYPE_ID> tmptypes;
			set <TYPE_ID>::iterator it_types;
			for (it = ret.first; it != ret.second; ++it) {
				t=0;
				if (tmptypes.empty())
					tmptypes.insert(it->second); //if default then don't check Subclasses, means unclassified
				else {
					for ( it_types = tmptypes.begin(); it_types != tmptypes.end(); it_types++) {
						//check also what is in types vector
						t = diplo::TH.GiveSubClass(*it_types, it->second); //here we have something, lest check what it new one is not more specific


						if (t == it->second) { //we found younger, so let's replace
							tmptypes.erase(*it_types);
							tmptypes.insert(it->second);
							break;
						}
					}

					//here is none of them was found as subclass i can decide to create composite
					if (t == 0) {
						tmptypes.insert(it->second);
					}
				}
			}

			if (tmptypes.size() == 0) t = 2 ;//unclassified
			else if (tmptypes.size() == 1) t = *(tmptypes.begin());
			else {
				//check if this type already exists
//				cout << "-----------------------------------------" << endl;
				t = diplo::TH.AddCompisite(&tmptypes);

				if (t > types->size()-1) { //means that we've just created a new type
					types->resize( t +1 );
				}



			}



			leaf->id = KeyManager::GetId((KEY_ID) ((*types)[t].elements.size()+1), t);
			(*types)[t].elements.insert(pair<KEY_ID, class Node *>(leaf->id, this) );


		}

	if (next == NULL)
		return;

	class Node **nx;
	nx = next;
	while (*nx != NULL) {
		(*nx)->MakeInverseIndex(types, data);
		nx++;
	}

}


string Node::GetUri() {
	class Node *p;
	p = prev;
	string s;
//	if (strcmp ("Evaluation", uri) == 0)
//		cerr << "1 GetUri: " << this->uri << "\t" << this->prev->uri << "\t" << this->next[0]->uri << endl;

//	Display();
//	cerr << uri << endl << endl ;
	if(p == NULL) {return uri;}
	s = p->GetUri() + uri;
//	cerr << s << "\t" << uri <<  endl ;
	return s;
}


Node::~Node() {

	if (next) {
		class Node **nx;
		nx = next;
		while (*nx != NULL) {
			delete *nx;
			nx++;
		}
		free(next);
		next = NULL;
	}
	free(uri);
	delete leaf;

}

////void IdUri::Add(unsigned long int id, class UriId* uri) {
////}
//
//IdUri::IdUri(class Node *_uri) {
//	uri = _uri;
//	id = uri->leaf->id;
//}
//
//void IdUri::Display() {
//	uri->Display();
////	cerr << "-----> " <<  uri->uri << endl ; //" ( " << uri->leaf->id << " ) " << endl;
//}
//
//IdUri::~IdUri() {
//}

//int operator==(const IdUri& left, const IdUri& right) {
//	return left.id == right.id;
//}

Leaf* KeyManager::Add(const char* _uri) {
	class Node *uri_id_tmp, *newNode;
	class Node **uri_id_list;
	newNode = NULL;

	if (uri_id == NULL) { //first element on the list
		//uri->id index (tree)
		uri_id_tmp = new Node();
		uri_id_tmp->uri = (char *) DiploMalloc(strlen(_uri) + 1);
		uri_id_tmp->leaf = new class Leaf(0);
		strcpy(uri_id_tmp->uri, _uri);
		uri_id = (class Node **) calloc(2, sizeof(class Node **));
		uri_id[0] = uri_id_tmp;
		uri_id[1] = NULL;
		newNode = uri_id_tmp;

	} else { //following elements
		bool done = false;
		unsigned int c = 0;
		uri_id_list = uri_id;
		while (*uri_id_list != NULL) {
			(*uri_id_list)->FindAdd(_uri, &done, &newNode);
			uri_id_list++;
			c++;

//			cerr << "-----1 " << _uri << " ---- " << done << endl;
			if (done) {
//				if (newNode == NULL) {cerr << "-----> FUCk1" <<_uri  << endl ; }
//				if (newNode->leaf == NULL) {cerr << "-----> FUCk2 " <<_uri << endl ; }
				break;
			}
//			cerr << "* ";
		}

//		cerr << "-" << *_uri << endl;
//		if (newNode == NULL) {cerr << "-----> FUCk" << done << endl ; }
		if (!done) {
//			cerr << "-- " << _uri << endl;
			uri_id_tmp = new class Node();
			uri_id_tmp->uri = (char *) DiploMalloc(strlen(_uri) + 1);
			strcpy(uri_id_tmp->uri, _uri);
			uri_id_tmp->leaf = new class Leaf(0);

			uri_id_list = (class Node **) calloc(c + 2, sizeof(class Node **));
			memcpy((class Node **) uri_id_list, (class Node **) uri_id,
					sizeof(class Node **) * c);
			free(uri_id);
			uri_id_list[c] = uri_id_tmp;
			uri_id_list[c + 1] = NULL;
			uri_id = uri_id_list;

			newNode = uri_id_tmp;
		}

//		cerr << "-----2 " << _uri << endl;
	}

	return newNode->leaf;
}

void KeyManager::MakeInverseIndexTypes() {

	class Node **uri_id_list;

	if (uri_id) {
		uri_id_list = uri_id;
		while (*uri_id_list != NULL) {
			(*uri_id_list)->MakeInverseIndexTypes(&types);
			uri_id_list++;
//		c++;
			//			cerr << "* ";
		}
	}
	types.resize( types[1].elements.size() +3 );
//
//
//	// a to do MakeInverseIndex classy Node
//	if (next && *tmp2 != '\0') {
//		nx = next;
//		while (*nx != NULL) {
//			(*nx)->FindAdd(tmp2, done, newNode);
//			if (*done) {
////				if (*newNode == NULL) {cerr << "-----> 2 uposss" << done << endl ; }
//				return;
//			}
//			nx++;
//		}
//	}
//
//
//	static string spaces;
//
//	cerr << spaces << uri;
//	if (leaf)
//		cerr << " ( " << leaf->id << " ) ";
//	cerr << endl;
//
//	if (next == NULL)
//		return;
//
//	class Node **nx;
//	nx = next;
//	while (*nx != NULL) {
//		spaces.append("-");
//		(*nx)->Display();
//		spaces = spaces.substr(0, spaces.length() - 1);
//		nx++;
//	}
//
//
//	if (newNode != NULL) {
////		cerr << "2---> " << newNode->uri << endl;
//		class IdUri * id = new class IdUri(newNode);
//		types[_tid].elements.insert(id);
//		newNode->SetType((KEY_ID) (types[_tid].elements.size() + 1), _tid); //we start id from 2 because 0 and 1 are taken by "unclassified" and "type of type"
//
////		id->Display();
////		newNode->Display();
////		cerr << "--- " << _tid << ": "<< types[_tid].elements.size() << endl;
//	}
}

void KeyManager::MakeInverseIndex(multimap<Leaf*, TYPE_ID>* data) {

	class Node **uri_id_list;
	if (uri_id != NULL) {
		uri_id_list = uri_id;
		while (*uri_id_list != NULL) {
			(*uri_id_list)->MakeInverseIndex(&types, data);
			uri_id_list++;
		}
	}
}

string KeyManager::Get(KEY_ID id) {
	TYPE_ID t = GetType(id);
	map<KEY_ID, class Node * >::iterator it;


	it = types[t].elements.find(id);

	if(it == types[t].elements.end()) {
		cerr << "KM can't find ID: " << id << endl;
		string n = "{ " + to_string( id ) + " } ";
		return n;
	}

	return it->second->GetUri();
}

KEY_ID Node::Get(const char* _uri) {
	class Node **nx;
	char *tmp1;
	const char *tmp2;
	KEY_ID id = 0;

	tmp1 = uri;
	tmp2 = _uri;

	if (*tmp1 != *tmp2)
		return 0;

	while (*tmp1 && *tmp2) {
		if (*tmp1 == *tmp2) {
			tmp1++;
			tmp2++;
		} else {
			return 0;
		}
	}

	if (*tmp1 == '\0' and *tmp2 == '\0' and leaf) {
		return leaf->id;
	} else if (next && *tmp2 != '\0') {
		nx = next;
		while (*nx != NULL) {
			id = (*nx)->Get(tmp2);
			if (id) return id;
			nx++;
		}
	}
	return id;
}
KEY_ID KeyManager::Get(const char* _uri) {
	class Node **uri_id_list;
	KEY_ID id = 0;
	uri_id_list = uri_id;
	while (*uri_id_list != NULL) {
		id = (*uri_id_list)->Get(_uri);
		if (id) return id;
		uri_id_list++;
	}
	return id;
}

KeyManager::KeyManager() {
	class Types tmp;
	uri_id = NULL;
	types.push_back(tmp); //no 0 - reserved as an error message
	types.push_back(tmp); //no 1 - reserved for types
	types.push_back(tmp); //no 2 reserved for unclassified
}

void KeyManager::Display() {

	class Node **uri_id_list;
	if (uri_id != NULL) {
		uri_id_list = uri_id;
		while (*uri_id_list != NULL) {
			(*uri_id_list)->Display();
			uri_id_list++;
		}
	}

//	set<class IdUri *>::iterator it;
//	map<KEY_ID, class Node * >::iterator it;
//	for (unsigned int i = 0; i < types.size(); i++) {
//		cerr << endl << endl << "------------------------------> Type ID: " << i << ", nubmer of elements: "
//				<< types[i].elements.size() << endl ;
//		for (it = types[i].elements.begin(); it != types[i].elements.end();
//				it++) {
//			cout << (*it)->uri->uri << "\t( "  << (*it)->id << " ) " <<  "\t( "  << GetSeq((*it)->id) << " ) " << endl;
//		}
//	}




//	class IdUri **id_uri_list;
//	if (id_uri != NULL) {
//		id_uri_list = id_uri;
//		while (*id_uri_list != NULL) {
//			(*id_uri_list)->Display();
//			id_uri_list++;
//		}
//	}
}

KEY_ID KeyManager::GetSeq(KEY_ID id) {
	id <<= MASK;
	return id >> MASK;
}

TYPE_ID KeyManager::GetType(KEY_ID id) {
	return id >> (BASE-MASK);
}

KEY_ID KeyManager::GetSeq(const char* _uri) {
	return GetSeq( Get(_uri) );
}

TYPE_ID KeyManager::GetType(const char* _uri) {
	return GetType( Get(_uri) );
}


map< KEY_ID, class Node * > * KeyManager::GetElementsOfType(TYPE_ID _type) {
	return &(types[_type].elements);
//	for ( map< KEY_ID, class Node * >::iterator it = types[_type].elements.begin(); it != types[_type].elements.end(); it++ ) {
//		elems->push_back(it->first);
//	}
}

size_t KeyManager::GetNbOfTypes() {
	return types.size();
}

KeyManager::~KeyManager() {
	if (uri_id == NULL ) return;
	class Node **uri_id_list;
	uri_id_list = uri_id;
	while (*uri_id_list != NULL) {
		delete *uri_id_list;
		*uri_id_list = NULL;
		uri_id_list++;
	}
	free(uri_id);
	uri_id = NULL;


//	map<KEY_ID, class Node * >::iterator it;
//	for (unsigned int i = 0; i < types.size(); i++)
//		for (it = types[i].elements.begin(); it != types[i].elements.end(); it++)
//			delete (*it)->second;

}

void Types::Display() {
}

Types::Types() {
//	counter = 0;

}

KEY_ID Types::GetNextSeq() {
//	return ++counter;
	return 0;
}

Types::~Types() {
}

