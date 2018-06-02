/*
 * PageManager.cpp
 *
 *  Created on: 04-03-2013
 *      Author: marcin
 */

#include "PageManager.h"

PageManager::PageManager(string f) {
	page_size = 4056;
	file.open(f, ios::in | ios::out | ios::app | ios::binary | ios::out);

}

PageManager::~PageManager() {
	// TODO Auto-generated destructor stub
}

size_t PageManager::NewPage() {
	char * x = (char *) malloc(page_size);
	bzero(x, page_size);
	pages.push_back(x);
	return pages.size() - 1;
}

char* PageManager::GetPage(unsigned int no) {
	if (no < pages.size())
		return pages[no];
	else
		return NULL;
}

void PageManager::Store() {

	for (vector<char *>::iterator i = pages.begin(); i != pages.end(); i++) {
		file.write(*i, page_size);
	}

}

void PageManager::ReadAll() {
	file.seekg(0, ios::beg);

	while (!file.eof()) {
		char *i = (char *) malloc(page_size);
		try {
			file.read(i, page_size);
		} catch (fstream::failure &e) {
			cerr << "Exception reading file";
		}

		pages.push_back(i);
	}
}

void PageManager::ReadPage(unsigned int no) {
	file.seekg(no*page_size);
	char *i = (char *) malloc(page_size);
	try {
		file.read(i, page_size);
	} catch (fstream::failure &e) {
		cerr << "Exception reading file";
	}

	pages.resize(no+1);
	pages[no] = i;

}
