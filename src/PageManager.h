/*
 * PageManager.h
 *
 *  Created on: 04-03-2013
 *      Author: marcin
 */

#ifndef PAGEMANAGER_H_
#define PAGEMANAGER_H_

#include	"diplodocus.h"

class PageManager {
	fstream file;
	size_t page_size;
	vector <char *> pages;

public:
	PageManager(string f);
	virtual ~PageManager();
	size_t GetPageSize(){return page_size;}
	size_t NewPage();
	char* GetPage(unsigned int no);
	void Store();
	void ReadAll();
	void ReadPage(unsigned int no);
};

#endif /* PAGEMANAGER_H_ */
