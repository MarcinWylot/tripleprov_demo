/*
 * LUBMnetwork.cpp
 *
 *  Created on: 28-03-2013
 *      Author: marcin
 */

#include "LUBMnetwork.h"

namespace queries {

LUBM_network::LUBM_network() {
	// TODO Auto-generated constructor stub

}

void LUBM_network::q01_m() {
	string q = "q01";
	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu/GraduateCourse0>");
	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");

	TYPE_ID t = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#GraduateStudent>");

	size_t s = q.size() + sizeof(o) + sizeof(p) + sizeof(t);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((KEY_ID *) data_tmp) = o;
	data_tmp = data_tmp + sizeof(o);
	*((KEY_ID *) data_tmp) = p;
	data_tmp = data_tmp + sizeof(p);
	*((TYPE_ID *) data_tmp) = t;
	data_tmp = data_tmp + sizeof(t);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(KEY_ID), i);

	}

	for (vector<KEY_ID>::iterator j = results.begin(); j != results.end(); j++)
		diplo::KM.Get(*j);


	PRINT_NO_RESULTS
}

void LUBM_network::q01_n(char *data) {
	/*  SELECT ?X
	 *  ?X a :GraduateStudent .
	 *	?X :takesCourse <http://www.Department0.University0.edu/GraduateCourse0> . <<<< ------- THE CONSTRAIN
	 */

	//THE ALGORITHM
	//find all molecules where GraduateCourse0 is an OBJECT
	//check if it is of type GraduateStudent (don't forget about COMPOSITE TYPES)
	//check each found molecule for THE CONSTRAIN
	vector<KEY_ID> results;

	KEY_ID o = *((KEY_ID *) data);
	data = data + sizeof(o);
	KEY_ID p = *((KEY_ID *) data);
	data = data + sizeof(p);
	TYPE_ID t = *((TYPE_ID *) data);
	data = data + sizeof(t);

	set<TYPE_ID> types = diplo::TH.GetCompositesForTypes(t);

	set<pair<const KEY_ID, Molecule>*>* Molecules_with_GraduateCourse0;
	Molecules_with_GraduateCourse0 = diplo::M.GetMoleculesFromInvertedIdx(o);
	if (Molecules_with_GraduateCourse0) { // do we have any of this
		for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_GraduateCourse0->begin(); it != Molecules_with_GraduateCourse0->end(); it++) {
			set<TYPE_ID>::iterator type = types.find(diplo::KM.GetType((*it)->first));
			if (type != types.end()) {
				size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, p, diplo::KM.GetType(o), 0);

				if ((*it)->second.CheckIfEntityExists(tripleTempl, o, 0)) {
					results.push_back((*it)->first);
				}
			}
		}
	}

	size_t s = results.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) results.data(), results.size() * sizeof(KEY_ID));

}

void LUBM_network::q02s1_m() {
	string q = "q02s1";
	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");
	TYPE_ID University = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#University>");

	TYPE_ID GraduateStudent = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#GraduateStudent>");

	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");

	KEY_ID undergraduateDegreeFrom = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#undergraduateDegreeFrom>");
	KEY_ID memberOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");

	size_t s = q.size() + sizeof(Department) + sizeof(University) + sizeof(GraduateStudent) + sizeof(subOrganizationOf) + sizeof(undergraduateDegreeFrom) + sizeof(memberOf);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = Department;
	data_tmp = data_tmp + sizeof(Department);
	*((TYPE_ID *) data_tmp) = University;
	data_tmp = data_tmp + sizeof(University);
	*((TYPE_ID *) data_tmp) = GraduateStudent;
	data_tmp = data_tmp + sizeof(GraduateStudent);
	*((KEY_ID *) data_tmp) = subOrganizationOf;
	data_tmp = data_tmp + sizeof(subOrganizationOf);
	*((KEY_ID *) data_tmp) = undergraduateDegreeFrom;
	data_tmp = data_tmp + sizeof(undergraduateDegreeFrom);
	*((KEY_ID *) data_tmp) = memberOf;
	data_tmp = data_tmp + sizeof(memberOf);

	unordered_map<KEY_ID, KEY_ID> DepartmentUniversity;
	vector<pair<KEY_ID, KEY_ID> > vDepartmentUniversity;
	vector<pair<KEY_ID, KEY_ID> > StudentUniversity;
	vector<pair<KEY_ID, KEY_ID>> StudentDepartment;
	vector<val4> results;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {

		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val4), i);

		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = vDepartmentUniversity.size();
		vDepartmentUniversity.resize(n + s);
		diplo::SRV.Receive((char *) (vDepartmentUniversity.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = StudentUniversity.size();
		StudentUniversity.resize(n + s);
		diplo::SRV.Receive((char *) (StudentUniversity.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = StudentDepartment.size();
		StudentDepartment.resize(n + s);
		diplo::SRV.Receive((char *) (StudentDepartment.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

	}

	copy(vDepartmentUniversity.begin(), vDepartmentUniversity.end(), inserter(DepartmentUniversity, DepartmentUniversity.begin()));




	//do Join!!!!
	for (size_t i = 0; i < StudentDepartment.size(); i++) {
		if (StudentDepartment[i].first != StudentUniversity[i].first) {
			cerr << "THIS IS NOT GOOD: " << diplo::KM.Get(StudentDepartment[i].first) << "\t" << diplo::KM.Get(StudentDepartment[i].first) << endl;
			break;
		}
		if (DepartmentUniversity.find(StudentDepartment[i].second)->second == StudentUniversity[i].second) {
			val4 x;
			x.v1 = StudentDepartment[i].first;
			x.v2 = StudentDepartment[i].second;
			x.v3 = StudentUniversity[i].second;
			results.push_back(x);
		}

	}

	for (vector<val4>::iterator j = results.begin(); j != results.end(); j++) {
//		cerr << diplo::KM.Get(j->v1) << "\t" << diplo::KM.Get(j->v2) << "\t" << diplo::KM.Get(j->v3) << endl;
		diplo::KM.Get(j->v1);
		diplo::KM.Get(j->v2);
		diplo::KM.Get(j->v3);

	}
	PRINT_NO_RESULTS
}

void LUBM_network::q02s1_n(char *data) {
	/*
	 * SELECT ?X ?Y ?Z

	 ?Z a :Department .
	 ?Y a :University .
	 ?Z :subOrganizationOf ?Y . <----- FIRST CONST

	 ?X a :GraduateStudent .
	 ?X :undergraduateDegreeFrom ?Y . <----- SECOND CONST
	 ?X :memberOf ?Z . <----- THIRD CONST
	 *
	 */

	//THE ALGORITM
	/*
	 * get list of Departments
	 * get list of University
	 * intersect in considaration of FIRST CONST
	 *
	 *
	 * get list of GraduateStudent
	 * intersect in considaration of SECOND & THIRD CONST (try to do it at the same time)
	 */

	TYPE_ID Department = *((TYPE_ID *) data);
	data = data + sizeof(Department);
	TYPE_ID University = *((TYPE_ID *) data);
	data = data + sizeof(University);

	TYPE_ID GraduateStudent = *((TYPE_ID *) data);
	data = data + sizeof(GraduateStudent);

	KEY_ID subOrganizationOf = *((KEY_ID *) data);
	data = data + sizeof(subOrganizationOf);

	KEY_ID undergraduateDegreeFrom = *((KEY_ID *) data);
	data = data + sizeof(undergraduateDegreeFrom);
	KEY_ID memberOf = *((KEY_ID *) data);

	set<TYPE_ID> typesDepartment = diplo::TH.GetCompositesForTypes(Department);
	set<TYPE_ID> typesUniversity = diplo::TH.GetCompositesForTypes(University);

	set<TYPE_ID> typesGraduateStudent = diplo::TH.GetCompositesForTypes(GraduateStudent);

	vector<unordered_set<KEY_ID>*> elsUniversity;
	for (set<TYPE_ID>::iterator it2 = typesUniversity.begin(); it2 != typesUniversity.end(); it2++) {
		unordered_set<KEY_ID>* el;
		el = diplo::TypesM.GetElementsOfType(*it2);
		elsUniversity.push_back(el);
	}

//first Part

	set<pair<const KEY_ID, Molecule>*>* Molecules_with_uni;

	unordered_map<KEY_ID, KEY_ID> DepartmentUniversity;
	vector<pair<KEY_ID, KEY_ID> > vDepartmentUniversity;

	for (vector<unordered_set<KEY_ID>*>::iterator uni_type = elsUniversity.begin(); uni_type != elsUniversity.end(); uni_type++) {
		for (unordered_set<KEY_ID>::iterator uni = (*uni_type)->begin(); uni != (*uni_type)->end(); uni++) {
			Molecules_with_uni = diplo::M.GetMoleculesFromInvertedIdx(*uni);
			if (Molecules_with_uni == NULL)
				continue;
			set<pair<const KEY_ID, Molecule>*>::iterator it;
			for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_uni->begin(); it != Molecules_with_uni->end(); it++) {
				set<TYPE_ID>::iterator type = typesDepartment.find(diplo::KM.GetType((*it)->first));
				if (type != typesDepartment.end()) {
					size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, subOrganizationOf, diplo::KM.GetType(*uni), 0);
					if ((*it)->second.CheckIfEntityExists(tripleTempl, *uni, 0)) {

						DepartmentUniversity.insert(pair<KEY_ID, KEY_ID>((*it)->first, *uni));
						vDepartmentUniversity.push_back(pair<KEY_ID, KEY_ID>((*it)->first, *uni));

					}
				}
			}
		}
	}

	val4 x;
	vector<val4> ready;

	vector<pair<KEY_ID, KEY_ID> > StudentUniversity;
	vector<pair<KEY_ID, KEY_ID>> StudentDepartment;

	for (set<TYPE_ID>::iterator it = typesGraduateStudent.begin(); it != typesGraduateStudent.end(); it++) {
		unordered_set<KEY_ID>* GraduateStudents;
		GraduateStudents = diplo::TypesM.GetElementsOfType(*it); // get all GraduateStudents

		unordered_set<size_t> templatesGraduateStudentUniversity;
		unordered_set<size_t> templatesGraduateStudentDepartment;

		for (set<TYPE_ID>::iterator it2 = typesUniversity.begin(); it2 != typesUniversity.end(); it2++) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, undergraduateDegreeFrom, *it2, 0);
			templatesGraduateStudentUniversity.insert(tripleTempl);
		}
		for (set<TYPE_ID>::iterator it2 = typesDepartment.begin(); it2 != typesDepartment.end(); it2++) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, memberOf, *it2, 0);
			templatesGraduateStudentDepartment.insert(tripleTempl);
		}

		for (unordered_set<KEY_ID>::iterator student = GraduateStudents->begin(); student != GraduateStudents->end(); student++) {
			unordered_set<KEY_ID> Universities;
			unordered_set<KEY_ID> Departments;
			Molecule *m = diplo::M.Get(*student);
			if (m != NULL) {

				m->GetEntity(&templatesGraduateStudentUniversity, *student, 0, &Universities);
				m->GetEntity(&templatesGraduateStudentDepartment, *student, 0, &Departments);

				for (unordered_set<KEY_ID>::iterator i = Departments.begin(); i != Departments.end(); i++) {
					unordered_map<KEY_ID, KEY_ID>::iterator dep = DepartmentUniversity.find(*i);
					if (dep != DepartmentUniversity.end()) {
						unordered_set<KEY_ID>::iterator uni = Universities.find(dep->second);
						if (uni != Universities.end()) {
							x.v1 = *student;
							x.v2 = dep->first;
							x.v3 = dep->second;
							ready.push_back(x);
						} else {
							StudentUniversity.push_back(pair<KEY_ID, KEY_ID>(*student, *(Universities.begin())));
							StudentDepartment.push_back(pair<KEY_ID, KEY_ID>(*student, *(Departments.begin())));
						}
					} else {
						StudentUniversity.push_back(pair<KEY_ID, KEY_ID>(*student, *(Universities.begin())));
						StudentDepartment.push_back(pair<KEY_ID, KEY_ID>(*student, *(Departments.begin())));
					}
				}
			}
		}
	}

	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(val4));

	s = vDepartmentUniversity.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vDepartmentUniversity.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

	s = StudentUniversity.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) StudentUniversity.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

	s = StudentDepartment.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) StudentDepartment.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

}

void LUBM_network::q02s2_n(char *data) {
	/*
	 * SELECT ?X ?Y ?Z

	 ?Z a :Department .
	 ?Y a :University .
	 ?Z :subOrganizationOf ?Y . <----- FIRST CONST

	 ?X a :GraduateStudent .
	 ?X :undergraduateDegreeFrom ?Y . <----- SECOND CONST
	 ?X :memberOf ?Z . <----- THIRD CONST
	 *
	 */

	//THE ALGORITM FOR Scope 2
	/*
	 *
	 * get list of GraduateStudent
	 * 	for each student get undergraduateDegreeFrom and memberOf
	 *  	check FIRST CONST for undergraduateDegreeFrom and memberOf
	 */
//	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");
//	TYPE_ID University = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#University>");
//
//	TYPE_ID GraduateStudent = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#GraduateStudent>");
//
//	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
//
//	KEY_ID undergraduateDegreeFrom = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#undergraduateDegreeFrom>");
//	KEY_ID memberOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");

	TYPE_ID Department = *((TYPE_ID *) data);
	data = data + sizeof(Department);
	TYPE_ID University = *((TYPE_ID *) data);
	data = data + sizeof(University);

	TYPE_ID GraduateStudent = *((TYPE_ID *) data);
	data = data + sizeof(GraduateStudent);

	KEY_ID subOrganizationOf = *((KEY_ID *) data);
	data = data + sizeof(subOrganizationOf);

	KEY_ID undergraduateDegreeFrom = *((KEY_ID *) data);
	data = data + sizeof(undergraduateDegreeFrom);
	KEY_ID memberOf = *((KEY_ID *) data);

	set<TYPE_ID> typesDepartment = diplo::TH.GetCompositesForTypes(Department);
	set<TYPE_ID> typesUniversity = diplo::TH.GetCompositesForTypes(University);

	set<TYPE_ID> typesGraduateStudent = diplo::TH.GetCompositesForTypes(GraduateStudent);

	val4 x;
	vector<val4> ready;

	for (set<TYPE_ID>::iterator it = typesGraduateStudent.begin(); it != typesGraduateStudent.end(); it++) {
		unordered_set<KEY_ID>* GraduateStudents;
		GraduateStudents = diplo::TypesM.GetElementsOfType(*it); // get all GraduateStudents

		unordered_set<size_t> templatesGraduateStudentUniversity;
		unordered_set<size_t> templatesGraduateStudentDepartment;

		for (set<TYPE_ID>::iterator it2 = typesUniversity.begin(); it2 != typesUniversity.end(); it2++) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, undergraduateDegreeFrom, *it2, 0);
			templatesGraduateStudentUniversity.insert(tripleTempl);
		}
		for (set<TYPE_ID>::iterator it2 = typesDepartment.begin(); it2 != typesDepartment.end(); it2++) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, memberOf, *it2, 0);
			templatesGraduateStudentDepartment.insert(tripleTempl);
		}

		unordered_set<size_t> templatesDepartmentUniversity;
		for (set<TYPE_ID>::iterator dep = typesDepartment.begin(); dep != typesDepartment.end(); dep++) {
			for (set<TYPE_ID>::iterator uni = typesUniversity.begin(); uni != typesUniversity.end(); uni++) {
				size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *dep, subOrganizationOf, *uni, 1);
				if (tripleTempl == -1) //continue;
					ERROR_OUT
						// this should not happen
				templatesDepartmentUniversity.insert(tripleTempl);
			}
		}

		for (unordered_set<KEY_ID>::iterator student = GraduateStudents->begin(); student != GraduateStudents->end(); student++) {
			unordered_set<KEY_ID> Universities;
			unordered_set<KEY_ID> Departments;
			unordered_set<KEY_ID> UniversitiesForDepartement;
			Molecule *m = diplo::M.Get(*student);
			if (m != NULL) {

				m->GetEntity(&templatesGraduateStudentUniversity, *student, 0, &Universities);
				m->GetEntity(&templatesGraduateStudentDepartment, *student, 0, &Departments);

				m->GetEntity(&templatesDepartmentUniversity, *Departments.begin(), 1, &UniversitiesForDepartement);

				if (*Universities.begin() == *UniversitiesForDepartement.begin()) {
					x.v1 = *student;
					x.v2 = *Departments.begin();
					x.v3 = *Universities.begin();
					ready.push_back(x);
				}
			}
		}
	}

//	cout << "q2s2 (before sending): " << diplo::stopwatch_get() << endl;
	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(val4));

}

void LUBM_network::q02s2_m() {
	string q = "q02s2";
	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");
	TYPE_ID University = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#University>");

	TYPE_ID GraduateStudent = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#GraduateStudent>");

	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");

	KEY_ID undergraduateDegreeFrom = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#undergraduateDegreeFrom>");
	KEY_ID memberOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");

	size_t s = q.size() + sizeof(Department) + sizeof(University) + sizeof(GraduateStudent) + sizeof(subOrganizationOf) + sizeof(undergraduateDegreeFrom) + sizeof(memberOf);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = Department;
	data_tmp = data_tmp + sizeof(Department);
	*((TYPE_ID *) data_tmp) = University;
	data_tmp = data_tmp + sizeof(University);
	*((TYPE_ID *) data_tmp) = GraduateStudent;
	data_tmp = data_tmp + sizeof(GraduateStudent);
	*((KEY_ID *) data_tmp) = subOrganizationOf;
	data_tmp = data_tmp + sizeof(subOrganizationOf);
	*((KEY_ID *) data_tmp) = undergraduateDegreeFrom;
	data_tmp = data_tmp + sizeof(undergraduateDegreeFrom);
	*((KEY_ID *) data_tmp) = memberOf;
	data_tmp = data_tmp + sizeof(memberOf);

	unordered_map<KEY_ID, KEY_ID> DepartmentUniversity;
	vector<pair<KEY_ID, KEY_ID> > vDepartmentUniversity;
	vector<pair<KEY_ID, KEY_ID> > StudentUniversity;
	vector<pair<KEY_ID, KEY_ID>> StudentDepartment;
	vector<val4> results;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {

		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val4), i);

	}

//	cout << "q2s2 (before KM): " << diplo::stopwatch_get() << endl;
	for (vector<val4>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->v1);
		diplo::KM.Get(j->v2);
		diplo::KM.Get(j->v3);
	}
//	for (vector<val4>::iterator j = results.begin(); j != results.end(); j++)
//		cout << diplo::KM.Get(j->v1) << "\t" << diplo::KM.Get(j->v2) << "\t" << diplo::KM.Get(j->v3) << endl;

	PRINT_NO_RESULTS
}

void LUBM_network::q03_n(char *data) {
	/*
	 * SELECT ?X
	 * ?X a :Publication .
	 * ?X :publicationAuthor <http://www.Department0.University0.edu/AssistantProfessor0> .   <---- THE CONSTRAIN
	 *
	 */

	/* THE ALGORITM
	 *
	 * get molecules where AssistantProfessor0 is an object
	 * check if a molecule is of type Publication
	 * 	if YES then check THE CONSTRAIN
	 * 		if correct then we want it
	 */

//	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu/AssistantProfessor0>");
//	TYPE_ID Publication = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Publication>");
//	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#publicationAuthor>");
	KEY_ID o = *((KEY_ID *) data);
	data = data + sizeof(o);
	KEY_ID p = *((KEY_ID *) data);
	data = data + sizeof(p);
	TYPE_ID t = *((TYPE_ID *) data);
	data = data + sizeof(t);

	vector<KEY_ID> results;

	set<TYPE_ID> types = diplo::TH.GetCompositesForTypes(t);
	set<pair<const KEY_ID, Molecule>*>* Molecules_with_AssistantProfessor0;
	Molecules_with_AssistantProfessor0 = diplo::M.GetMoleculesFromInvertedIdx(o);
	if (Molecules_with_AssistantProfessor0) { // do we have any of this
		for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_AssistantProfessor0->begin(); it != Molecules_with_AssistantProfessor0->end(); it++) {

			set<TYPE_ID>::iterator type = types.find(diplo::KM.GetType((*it)->first));
			if (type != types.end()) {
				size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, p, diplo::KM.GetType(o), 0);

				if ((*it)->second.CheckIfEntityExists(tripleTempl, o, 0)) {
					results.push_back((*it)->first);
//					cout << diplo::KM.Get( (*it)->first ) << endl;
				}
			}

		}
	}

	size_t s = results.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) results.data(), results.size() * sizeof(KEY_ID));

}

void LUBM_network::q03_m() {
	string q = "q03";

	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu/AssistantProfessor0>");

	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#publicationAuthor>");
	TYPE_ID t = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Publication>");

	size_t s = q.size() + sizeof(o) + sizeof(p) + sizeof(t);

	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((KEY_ID *) data_tmp) = o;
	data_tmp = data_tmp + sizeof(o);
	*((KEY_ID *) data_tmp) = p;
	data_tmp = data_tmp + sizeof(p);
	*((TYPE_ID *) data_tmp) = t;
	data_tmp = data_tmp + sizeof(t);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(KEY_ID), i);

	}

//	cout << "q1 (before KM): " << diplo::stopwatch_get() << endl;
	for (vector<KEY_ID>::iterator j = results.begin(); j != results.end(); j++)
		diplo::KM.Get(*j);
//			cout << diplo::KM.Get(*j) << endl;

	PRINT_NO_RESULTS
}

void LUBM_network::q04_n(char *data) {
	/*
	 * SELECT ?X ?Y1 ?Y2 ?Y3
	 * WHERE
	 *   {
	 *     ?t rdfs:subClassOf :Professor .
	 *     ?X a ?t .
	 *     ?X :worksFor <http://www.Department0.University0.edu> .  <---- THE CONSTRAIN
	 *     ?X :name ?Y1 .
	 *     ?X :emailAddress ?Y2 .
	 *     ?X :telephone ?Y3 .
	 *   }
	 *
	 */

	/* THE ALGORITM
	 *
	 * get molecules where <http://www.Department0.University0.edu> is an object
	 * check if a molecule is of type Professor
	 * 	if YES then check THE CONSTRAIN
	 * 		if correct then we want it
	 * 			for each of them get data from the molecule
	 */

	KEY_ID o = *((KEY_ID *) data);
	data = data + sizeof(o);
	KEY_ID p = *((KEY_ID *) data);
	data = data + sizeof(p);
	TYPE_ID t = *((TYPE_ID *) data);
	data = data + sizeof(t);
	KEY_ID p1 = *((KEY_ID *) data);
	data = data + sizeof(p1);
	KEY_ID p2 = *((KEY_ID *) data);
	data = data + sizeof(p2);
	KEY_ID p3 = *((KEY_ID *) data);
	data = data + sizeof(p3);

	vector<val4> results;
	val4 x;

	set<TYPE_ID> types = diplo::TH.GetHierarchyForType(t);
	set<pair<const KEY_ID, Molecule>*>* Molecules_with_o;
	Molecules_with_o = diplo::M.GetMoleculesFromInvertedIdx(o);
	if (Molecules_with_o) { // do we have any of this
		for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_o->begin(); it != Molecules_with_o->end(); it++) {

			set<TYPE_ID>::iterator type = types.find(diplo::KM.GetType((*it)->first));
			if (type != types.end()) {
				size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, p, diplo::KM.GetType(o), 0);

				if ((*it)->second.CheckIfEntityExists(tripleTempl, o, 0)) {
					x.v1 = (*it)->first;

					{
						unordered_set<size_t> tripleTemplates;
						unordered_set<KEY_ID> property_v;
						size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, p1, 0, 0);
						tripleTemplates.insert(tripleTempl);
						(*it)->second.GetEntity(&tripleTemplates, (*it)->first, 0, &property_v);
						x.v2 = *(property_v.begin());
					}
					{
						unordered_set<size_t> tripleTemplates;
						unordered_set<KEY_ID> property_v;
						size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, p2, 0, 0);
						tripleTemplates.insert(tripleTempl);
						(*it)->second.GetEntity(&tripleTemplates, (*it)->first, 0, &property_v);
						x.v3 = *(property_v.begin());
					}
					{
						unordered_set<size_t> tripleTemplates;
						unordered_set<KEY_ID> property_v;
						size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, p3, 0, 0);
						tripleTemplates.insert(tripleTempl);
						(*it)->second.GetEntity(&tripleTemplates, (*it)->first, 0, &property_v);
						x.v4 = *(property_v.begin());
					}

					results.push_back(x);
//					cout << diplo::KM.Get( x.v1 ) << "\t" << diplo::KM.Get( x.v2 ) << "\t" << diplo::KM.Get( x.v3 ) << "\t" << diplo::KM.Get( x.v4 ) << "\t" << endl;
				}
			}

		}
	}

	size_t s = results.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) results.data(), results.size() * sizeof(val4));

}

void LUBM_network::q04_m() {
	string q = "q04";

	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu>");

	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#worksFor>");
	TYPE_ID t = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Professor>");

	KEY_ID p1 = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#name>");
	KEY_ID p2 = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#emailAddress>");
	KEY_ID p3 = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#telephone>");

	size_t s = q.size() + sizeof(o) + sizeof(p) + sizeof(t) + sizeof(p1) + sizeof(p2) + sizeof(p3);

	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((KEY_ID *) data_tmp) = o;
	data_tmp = data_tmp + sizeof(o);
	*((KEY_ID *) data_tmp) = p;
	data_tmp = data_tmp + sizeof(p);
	*((TYPE_ID *) data_tmp) = t;
	data_tmp = data_tmp + sizeof(t);
	*((KEY_ID *) data_tmp) = p1;
	data_tmp = data_tmp + sizeof(p1);
	*((KEY_ID *) data_tmp) = p2;
	data_tmp = data_tmp + sizeof(p2);
	*((KEY_ID *) data_tmp) = p3;
	data_tmp = data_tmp + sizeof(p3);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<val4> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val4), i);

	}

	for (vector<val4>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->v1);
		diplo::KM.Get(j->v2);
		diplo::KM.Get(j->v3);
		diplo::KM.Get(j->v4);
//		cout << diplo::KM.Get( j->v1 ) << "\t" << diplo::KM.Get( j->v2 ) << "\t" << diplo::KM.Get( j->v3 ) << "\t" << diplo::KM.Get( j->v4 ) << "\t" << endl;
	}

	PRINT_NO_RESULTS
}

void LUBM_network::q05_n(char* data) {
	/*
	 * SELECT ?X
	 WHERE
	 { ?t rdfs:subClassOf :Person .
	 ?X a ?t .
	 ?X :memberOf <http://www.Department0.University0.edu> .
	 }
	 *
	 */

	/* THE ALGORITHM
	 *
	 * like q1 and q3
	 *
	 */
	KEY_ID o = *((KEY_ID *) data);
	data = data + sizeof(o);
	KEY_ID p = *((KEY_ID *) data);
	data = data + sizeof(p);
	TYPE_ID t = *((TYPE_ID *) data);
	data = data + sizeof(t);

	vector<KEY_ID> results;

	set<TYPE_ID> types = diplo::TH.GetHierarchyForType(t);
	set<pair<const KEY_ID, Molecule>*>* Molecules_with_o;
	Molecules_with_o = diplo::M.GetMoleculesFromInvertedIdx(o);
	if (Molecules_with_o) { // do we have any of this
		for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_o->begin(); it != Molecules_with_o->end(); it++) {

			set<TYPE_ID>::iterator type = types.find(diplo::KM.GetType((*it)->first));
			if (type != types.end()) {
				size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, p, diplo::KM.GetType(o), 0);

				if ((*it)->second.CheckIfEntityExists(tripleTempl, o, 0)) {
					results.push_back((*it)->first);
//					cout << diplo::KM.Get( (*it)->first ) << endl;
				}
			}

		}
	}

	size_t s = results.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) results.data(), results.size() * sizeof(KEY_ID));
}

void LUBM_network::q05_m() {
	string q = "q05";

	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu>");

	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");
	TYPE_ID t = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Person>");

	size_t s = q.size() + sizeof(o) + sizeof(p) + sizeof(t);

	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((KEY_ID *) data_tmp) = o;
	data_tmp = data_tmp + sizeof(o);
	*((KEY_ID *) data_tmp) = p;
	data_tmp = data_tmp + sizeof(p);
	*((TYPE_ID *) data_tmp) = t;
	data_tmp = data_tmp + sizeof(t);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(KEY_ID), i);

	}

//	cout << "q1 (before KM): " << diplo::stopwatch_get() << endl;
	for (vector<KEY_ID>::iterator j = results.begin(); j != results.end(); j++)
		diplo::KM.Get(*j);
//			cout << diplo::KM.Get(*j) << endl;
	PRINT_NO_RESULTS
}

void LUBM_network::q06_n(char* data) {
	/*
	 * SELECT ?X
	 WHERE
	 {
	 { ?t rdfs:subClassOf :Student .
	 ?X a ?t .
	 }
	 UNION
	 {
	 ?X a :Student .
	 }
	 }
	 *
	 */

	/* THE ALGORITHM
	 *
	 * just get all elements of type Student
	 *
	 */
	TYPE_ID t = *((TYPE_ID *) data);
	data = data + sizeof(t);

	vector<KEY_ID> results;

	set<TYPE_ID> types = diplo::TH.GetHierarchyForType(t);

	for (set<TYPE_ID>::iterator it = types.begin(); it != types.end(); it++) {
		unordered_set<KEY_ID>* elems;
		elems = diplo::TypesM.GetElementsOfType(*it); // get all Students
		for (unordered_set<KEY_ID>::iterator it2 = elems->begin(); it2 != elems->end(); it2++) {
			if (diplo::M.IsItMyObject(*it2))
				results.push_back(*it2);
//			cout << diplo::KM.Get(*it2) << endl;
		}

	}

	size_t s = results.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) results.data(), results.size() * sizeof(KEY_ID));
}

void LUBM_network::q06_m() {
	string q = "q06";

	TYPE_ID t = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");

	size_t s = q.size() + sizeof(t);

	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = t;
	data_tmp = data_tmp + sizeof(t);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(KEY_ID), i);

	}

	for (vector<KEY_ID>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(*j);
//		cout << diplo::KM.Get(*j) << endl;

	}
	PRINT_NO_RESULTS
}

void LUBM_network::q07_n(char* data) {
	/*
	 * SELECT ?X ?Y
	 WHERE
	 {
	 {
	 ?t2 rdfs:subClassOf :Course .
	 ?Y a ?t2 .
	 }
	 UNION
	 {
	 ?Y a :Course .
	 }

	 {
	 ?t rdfs:subClassOf :Student .
	 ?X a ?t .
	 }
	 UNION
	 {
	 ?X a :Student .
	 }

	 <http://www.Department0.University0.edu/AssociateProfessor0> :teacherOf ?Y .
	 ?X :takesCourse ?Y .

	 }
	 *
	 */

	/*
	 * get all Courses given by AssociateProfessor0
	 *
	 * get list of Student with values for [takesCourse]
	 * 	-> check if Course is on the list above
	 * 		if yes add on to the result
	 * 		if not add pair<Sudent,course> to do join on the Master
	 *
	 */

	TYPE_ID Student = *((TYPE_ID *) data);
	data = data + sizeof(Student);
	TYPE_ID Course = *((TYPE_ID *) data);
	data = data + sizeof(Course);

	KEY_ID AssociateProfessor0 = *((KEY_ID *) data);
	data = data + sizeof(AssociateProfessor0);
	KEY_ID takesCourse = *((KEY_ID *) data);
	data = data + sizeof(takesCourse);
	KEY_ID teacherOf = *((KEY_ID *) data);
	data = data + sizeof(teacherOf);

//	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
//	TYPE_ID Course = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Course>");
//
//	KEY_ID AssociateProfessor0 = diplo::KM.Get("<http://www.Department0.University0.edu/AssociateProfessor0>");
//	KEY_ID takesCourse =diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");
//	KEY_ID teacherOf =diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#teacherOf>");

	set<TYPE_ID> typesStudent = diplo::TH.GetHierarchyForType(Student);
	set<TYPE_ID> typesCourse = diplo::TH.GetHierarchyForType(Course);


	unordered_set<KEY_ID> Courses;
	vector<KEY_ID> vCourses;
	Molecule *m = diplo::M.Get(AssociateProfessor0);

	multimap<TYPE_ID, size_t> templatesStudentCourse;

	if (m != NULL) {
		unordered_set<size_t> templatesAssociateProfessorCourse;
		TYPE_ID AssociateProfessor = diplo::KM.GetType(AssociateProfessor0);
		for (set<TYPE_ID>::iterator it2 = typesCourse.begin(); it2 != typesCourse.end(); it2++) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate(AssociateProfessor, AssociateProfessor, teacherOf, *it2, 0);
			templatesAssociateProfessorCourse.insert(tripleTempl);
		}
		m->GetEntity(&templatesAssociateProfessorCourse, AssociateProfessor0, 0, &Courses);
		if (!Courses.empty())
			copy(Courses.begin(), Courses.end(), inserter(vCourses, vCourses.begin()));
	}


	//send it to the Master
	size_t s = vCourses.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vCourses.data(), s * sizeof(KEY_ID));



	diplo::CLI.Receive((char*) &s, sizeof(s));
	vCourses.resize(s);
	diplo::CLI.Receive((char *) (vCourses.data()), s * sizeof(KEY_ID));


	for (set<TYPE_ID>::iterator it2 = typesCourse.begin(); it2 != typesCourse.end(); it2++) {
		for (set<TYPE_ID>::iterator student = typesStudent.begin(); student != typesStudent.end(); student++) {
			diplo::TM.GetTemplates(*student, takesCourse, *it2, 0, &templatesStudentCourse);
		}
	}

	//do join here!!
	pair<KEY_ID,KEY_ID> x;
	vector< pair<KEY_ID,KEY_ID> > ready;
	for (vector<KEY_ID>::iterator c = vCourses.begin(); c!= vCourses.end(); c++ ) {
		set<pair<const KEY_ID, Molecule>*>* Molecules_with_Course = diplo::M.GetMoleculesFromInvertedIdx(*c);
		if (Molecules_with_Course) {
			for (set<pair<const KEY_ID, Molecule>*>::iterator i = Molecules_with_Course->begin(); i != Molecules_with_Course->end(); i++) {
				multimap<TYPE_ID, size_t>::iterator type = templatesStudentCourse.find(diplo::KM.GetType((*i)->first));
				if (type != templatesStudentCourse.end()) {
					if ( (*i)->second.CheckIfEntityExists(type->second, *c, 0) ) {
						x.first = (*i)->first;
						x.second = *c;
						ready.push_back(x);
					}
				}
			}
		}
	}

	s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(pair<KEY_ID,KEY_ID>));

}

void LUBM_network::q07_m() {
	string q = "q07";
	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
	TYPE_ID Course = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Course>");

	KEY_ID AssociateProfessor0 = diplo::KM.Get("<http://www.Department0.University0.edu/AssociateProfessor0>");
	KEY_ID takesCourse = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");
	KEY_ID teacherOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#teacherOf>");

	size_t s = q.size() + sizeof(Student) + sizeof(Course) + sizeof(AssociateProfessor0) + sizeof(takesCourse) + sizeof(teacherOf);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = Student;
	data_tmp = data_tmp + sizeof(Student);
	*((TYPE_ID *) data_tmp) = Course;
	data_tmp = data_tmp + sizeof(Course);

	*((KEY_ID *) data_tmp) = AssociateProfessor0;
	data_tmp = data_tmp + sizeof(AssociateProfessor0);
	*((KEY_ID *) data_tmp) = takesCourse;
	data_tmp = data_tmp + sizeof(takesCourse);
	*((KEY_ID *) data_tmp) = teacherOf;
	data_tmp = data_tmp + sizeof(teacherOf);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> vCourses;


	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = vCourses.size();
		vCourses.resize(n + s);
		diplo::SRV.Receive((char *) (vCourses.data() + n), s * sizeof(KEY_ID), i);
	}

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = vCourses.size();
		diplo::SRV.Send((char*) &s, sizeof(s), i);
		diplo::SRV.Send((char *) (vCourses.data()), s * sizeof(KEY_ID), i);
	}


	vector<pair<KEY_ID, KEY_ID>> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(pair<KEY_ID, KEY_ID>), i);
	}

	for (vector<pair<KEY_ID, KEY_ID>>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->first);
		diplo::KM.Get(j->second);
//		cerr << diplo::KM.Get(j->first) << "\t" << diplo::KM.Get(j->second) << endl;
	}

	PRINT_NO_RESULTS

}

void LUBM_network::q08s1_n(char* data) {
	/*
	 *
	 *
	 * SELECT ?X ?Y ?Z
	 WHERE
	 {
	 {
	 ?t rdfs:subClassOf :Student .
	 ?X a ?t .
	 }
	 UNION
	 {
	 ?X a :Student .
	 }

	 ?Y :subOrganizationOf <http://www.University0.edu> .
	 ?Y a :Department .


	 ?X :memberOf ?Y . < ----- CONDITION
	 ?X :emailAddress ?Z .
	 }
	 *
	 */

	/*
	 * get list of molecules where <http://www.University0.edu> is in context of [Department --> subOrganizationOf]
	 *
	 * get list of Student with values for [memberOf] and [emailAddress]
	 * 	-> if value for memberOf is a in Department list it's a result
	 * 	-> else intersect on the Master
	 *
	 */

	TYPE_ID Student = *((TYPE_ID *) data);
	data = data + sizeof(Student);

	KEY_ID subOrganizationOf = *((KEY_ID *) data);
	data = data + sizeof(subOrganizationOf);

	KEY_ID University = *((KEY_ID *) data);
	data = data + sizeof(University);

	TYPE_ID Department = *((TYPE_ID *) data);
	data = data + sizeof(Department);

	KEY_ID memberOf = *((KEY_ID *) data);
	data = data + sizeof(memberOf);
	KEY_ID emailAddress = *((KEY_ID *) data);

//	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
//	KEY_ID subOrganizationOf =diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
//	KEY_ID University = diplo::KM.Get("<http://www.University0.edu>");
//	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");
//	KEY_ID memberOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");
//	KEY_ID emailAddress = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#emailAddress>");

	set<TYPE_ID> typesDepartment = diplo::TH.GetCompositesForTypes(Department);

	set<TYPE_ID> typesStudent = diplo::TH.GetHierarchyForType(Student);

//first Part
	set<pair<const KEY_ID, Molecule>*>* Molecules_with_uni;
	unordered_set<KEY_ID> Departments;
	vector<KEY_ID> vDepartments;
	Molecules_with_uni = diplo::M.GetMoleculesFromInvertedIdx(University);
//	if (Molecules_with_uni == NULL)
//		ERROR_OUT

	size_t tripleTempl = diplo::TM.GetTripleTemplate(Department, Department, subOrganizationOf, diplo::KM.GetType(University), 0);
	set<pair<const KEY_ID, Molecule>*>::iterator it;
	if (Molecules_with_uni != NULL) {
		for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_uni->begin(); it != Molecules_with_uni->end(); it++) {
			if (Department == diplo::KM.GetType((*it)->first)) {
				if ((*it)->second.CheckIfEntityExists(tripleTempl, University, 0)) {
					vDepartments.push_back((*it)->first);
//				cerr << diplo::KM.Get( (*it)->first ) << endl;
				}
			}
		}
	}

	size_t s = vDepartments.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vDepartments.data(), s * sizeof(KEY_ID));

	diplo::CLI.Receive((char*) &s, sizeof(s));
	vDepartments.resize(s);
	diplo::CLI.Receive((char *) (vDepartments.data()), s * sizeof(KEY_ID));

	copy(vDepartments.begin(), vDepartments.end(), inserter(Departments, Departments.begin()));



	val4 x;
	vector<val4> ready;

	vector<pair<KEY_ID, pair<KEY_ID, KEY_ID> >> DepartmentStudent;

	for (set<TYPE_ID>::iterator it = typesStudent.begin(); it != typesStudent.end(); it++) {
		unordered_set<KEY_ID>* Students;
		Students = diplo::TypesM.GetElementsOfType(*it); // get all Students

		unordered_set<size_t> templatesStudentEMail;
		unordered_set<size_t> templatesStudentDepartment;

		{
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, emailAddress, 0, 0);
			templatesStudentEMail.insert(tripleTempl);
		}

		for (set<TYPE_ID>::iterator it2 = typesDepartment.begin(); it2 != typesDepartment.end(); it2++) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, memberOf, *it2, 0);
			templatesStudentDepartment.insert(tripleTempl);
		}

		for (unordered_set<KEY_ID>::iterator student = Students->begin(); student != Students->end(); student++) {
			unordered_set<KEY_ID> EMails;
			unordered_set<KEY_ID> Departments_4Student;
			Molecule *m = diplo::M.Get(*student);
			if (m != NULL) {

				m->GetEntity(&templatesStudentDepartment, *student, 0, &Departments_4Student);
				m->GetEntity(&templatesStudentEMail, *student, 0, &EMails);
				for (unordered_set<KEY_ID>::iterator i = Departments_4Student.begin(); i != Departments_4Student.end(); i++) {
					unordered_set<KEY_ID>::iterator dep = Departments.find(*i);
					if (dep != Departments.end() and !EMails.empty()) {
						x.v1 = *student;
						x.v2 = *dep;
						x.v3 = *(EMails.begin());
						ready.push_back(x);
//							cerr << diplo::KM.Get( x.v1 ) << "\t" << diplo::KM.Get( x.v2 ) << "\t" << diplo::KM.Get( x.v3 ) << endl;
					}
				}
			}
		}
	}

	s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(val4));

}

void LUBM_network::q08s1_m() {
	string q = "q08s1";

	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
	KEY_ID University = diplo::KM.Get("<http://www.University0.edu>");
	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");
	KEY_ID memberOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");
	KEY_ID emailAddress = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#emailAddress>");

	size_t s = q.size() + sizeof(Student) + sizeof(subOrganizationOf) + sizeof(University) + sizeof(Department) + sizeof(memberOf) + sizeof(emailAddress);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());

	data_tmp = data_tmp + q.size();

	*((TYPE_ID *) data_tmp) = Student;
	data_tmp = data_tmp + sizeof(Student);
	*((KEY_ID *) data_tmp) = subOrganizationOf;
	data_tmp = data_tmp + sizeof(subOrganizationOf);
	*((KEY_ID *) data_tmp) = University;
	data_tmp = data_tmp + sizeof(University);
	*((TYPE_ID *) data_tmp) = Department;
	data_tmp = data_tmp + sizeof(Department);
	*((KEY_ID *) data_tmp) = memberOf;
	data_tmp = data_tmp + sizeof(memberOf);
	*((KEY_ID *) data_tmp) = emailAddress;
	data_tmp = data_tmp + sizeof(emailAddress);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> vDepartments;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = vDepartments.size();
		vDepartments.resize(n + s);
		diplo::SRV.Receive((char *) (vDepartments.data() + n), s * sizeof(KEY_ID), i);
	}

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = vDepartments.size();
		diplo::SRV.Send((char*) &s, sizeof(s), i);
		diplo::SRV.Send((char *) (vDepartments.data()), s * sizeof(KEY_ID), i);
	}


	vector<val4> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {

		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val4), i);

	}

	for (vector<val4>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->v1);
		diplo::KM.Get(j->v2);
		diplo::KM.Get(j->v3);
//		cout << diplo::KM.Get( j->v1 ) << "\t" << diplo::KM.Get( j->v2 ) << "\t" << diplo::KM.Get( j->v3 ) << "\t" << endl;
	}
	PRINT_NO_RESULTS

}

void LUBM_network::q08s2_n(char* data) {

	/*
	 *
	 *
	 * SELECT ?X ?Y ?Z
	 WHERE
	 {
	 {
	 ?t rdfs:subClassOf :Student .
	 ?X a ?t .
	 }
	 UNION
	 {
	 ?X a :Student .
	 }

	 ?Y :subOrganizationOf <http://www.University0.edu> .
	 ?Y a :Department .


	 ?X :memberOf ?Y . < ----- CONDITION
	 ?X :emailAddress ?Z .
	 }
	 *
	 */


	/*
	 * get list of Student, for each student get [memberOf]
	 * 	-> check if the object of [memberOf] is [subOrganizationOf] <http://www.University0.edu>
	 * 		if yes get [emailAddress] for Student and add to ready
	 */

//	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
//	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
//	KEY_ID University = diplo::KM.Get("<http://www.University0.edu>");
//	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");
//	KEY_ID memberOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");
//	KEY_ID emailAddress = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#emailAddress>");

	TYPE_ID Student = *((TYPE_ID *) data);
	data = data + sizeof(Student);

	KEY_ID subOrganizationOf = *((KEY_ID *) data);
	data = data + sizeof(subOrganizationOf);

	KEY_ID University = *((KEY_ID *) data);
	data = data + sizeof(University);

	TYPE_ID Department = *((TYPE_ID *) data);
	data = data + sizeof(Department);

	KEY_ID memberOf = *((KEY_ID *) data);
	data = data + sizeof(memberOf);
	KEY_ID emailAddress = *((KEY_ID *) data);

	set<TYPE_ID> typesDepartment = diplo::TH.GetCompositesForTypes(Department);
	set<TYPE_ID> typesStudent = diplo::TH.GetHierarchyForType(Student);

//first Part
	set<pair<const KEY_ID, Molecule>*>* Molecules_with_uni;
	unordered_set<KEY_ID> Departments;
	vector<KEY_ID> vDepartments;
	Molecules_with_uni = diplo::M.GetMoleculesFromInvertedIdx(University);
//	if (Molecules_with_uni == NULL)
//		ERROR_OUT

	unordered_set<size_t> templatesStudentEMail;
	unordered_set<size_t> templatesStudentDepartment;

	multimap<TYPE_ID, size_t> templatesStudentDepartmentUniversity;


	for (set<TYPE_ID>::iterator it = typesStudent.begin(); it != typesStudent.end(); it++) {
		size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, emailAddress, 0, 0);
		templatesStudentEMail.insert(tripleTempl);

		for (set<TYPE_ID>::iterator it2 = typesDepartment.begin(); it2 != typesDepartment.end(); it2++) {
			tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, memberOf, *it2, 0);
			templatesStudentDepartment.insert(tripleTempl);

			diplo::TM.GetTemplates(*it2, subOrganizationOf, diplo::KM.GetType(University), 1, &templatesStudentDepartmentUniversity);
		}
	}

	val4 x;
	vector<val4> ready;
	set<pair<const KEY_ID, Molecule>*>::iterator it;
	if (Molecules_with_uni != NULL) {
		for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_uni->begin(); it != Molecules_with_uni->end(); it++) {
			if (typesStudent.find(diplo::KM.GetType((*it)->first)) == typesStudent.end())
				continue;
			multimap<TYPE_ID, size_t>::iterator type = templatesStudentDepartmentUniversity.find(diplo::KM.GetType((*it)->first));
			if (type != templatesStudentDepartmentUniversity.end()) {
				if ((*it)->second.CheckIfEntityExists(type->second, University, 1)) {
					unordered_set<KEY_ID> EMails;
					unordered_set<KEY_ID> Departments_4Student;
					(*it)->second.GetEntity(&templatesStudentEMail, (*it)->first, 0, &EMails);
					(*it)->second.GetEntity(&templatesStudentDepartment, (*it)->first, 0, &Departments_4Student);

					if (!Departments_4Student.empty() and !EMails.empty()) {
						x.v1 = (*it)->first;
						x.v2 = *(Departments_4Student.begin());
						x.v3 = *(EMails.begin());
						ready.push_back(x);
					}
				}
			}
		}
	}
	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(val4));
}

void LUBM_network::q08s2_m() {
	string q = "q08s2";

	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
	KEY_ID University = diplo::KM.Get("<http://www.University0.edu>");
	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");
	KEY_ID memberOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");
	KEY_ID emailAddress = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#emailAddress>");

	size_t s = q.size() + sizeof(Student) + sizeof(subOrganizationOf) + sizeof(University) + sizeof(Department) + sizeof(memberOf) + sizeof(emailAddress);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());

	data_tmp = data_tmp + q.size();

	*((TYPE_ID *) data_tmp) = Student;
	data_tmp = data_tmp + sizeof(Student);
	*((KEY_ID *) data_tmp) = subOrganizationOf;
	data_tmp = data_tmp + sizeof(subOrganizationOf);
	*((KEY_ID *) data_tmp) = University;
	data_tmp = data_tmp + sizeof(University);
	*((TYPE_ID *) data_tmp) = Department;
	data_tmp = data_tmp + sizeof(Department);
	*((KEY_ID *) data_tmp) = memberOf;
	data_tmp = data_tmp + sizeof(memberOf);
	*((KEY_ID *) data_tmp) = emailAddress;
	data_tmp = data_tmp + sizeof(emailAddress);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> vDepartments;
	vector<pair<KEY_ID, pair<KEY_ID, KEY_ID> >> vDepartmentStudent;
	vector<val4> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {

		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val4), i);
	}
	for (vector<val4>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->v1);
		diplo::KM.Get(j->v2);
		diplo::KM.Get(j->v3);
//		cout << diplo::KM.Get( j->v1 ) << "\t" << diplo::KM.Get( j->v2 ) << "\t" << diplo::KM.Get( j->v3 ) << "\t" << endl;
	}
	PRINT_NO_RESULTS
}

void LUBM_network::q09s1_n(char* data) {
	/*
	 *
	 * SELECT ?X ?Y ?Z
	 WHERE
	 {
	 {
	 ?t rdfs:subClassOf :Student .
	 ?X a ?t .
	 }
	 UNION
	 {
	 ?X a :Student .
	 }

	 ?t3 rdfs:subClassOf :Faculty .
	 {?Y a ?t3.}
	 UNION
	 {?t4 rdfs:subClassOf ?t3.
	 ?Y a ?t4.}


	 {
	 ?t2 rdfs:subClassOf :Course .
	 ?Z a ?t2 .
	 }
	 UNION
	 {
	 ?Z a :Course .
	 }


	 ?X :advisor ?Y . <--- COND 1ST
	 ?X :takesCourse ?Z .<--- COND 2RD
	 ?Y :teacherOf ?Z .  <--- COND 3RD
	 }
	 *
	 */

	/*
	 * get molecules where Course appears
	 * 	if the molecule is of type Faculty check {COND 3RD}
	 * 		save it
	 *
	 * get list of Student with values for [advisor] and [takesCourse]
	 * 	-> check if Course and Faculty if in the list above {COND 1ST & 2RD}
	 *
	 */

	TYPE_ID Student = *((TYPE_ID *) data);
	data = data + sizeof(Student);
	TYPE_ID Faculty = *((TYPE_ID *) data);
	data = data + sizeof(Faculty);
	TYPE_ID Course = *((TYPE_ID *) data);
	data = data + sizeof(Course);

	KEY_ID advisor = *((KEY_ID *) data);
	data = data + sizeof(advisor);
	KEY_ID takesCourse = *((KEY_ID *) data);
	data = data + sizeof(takesCourse);
	KEY_ID teacherOf = *((KEY_ID *) data);
	data = data + sizeof(teacherOf);

//
//	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
//	TYPE_ID Faculty = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Faculty>");
//	TYPE_ID Course = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Course>");
//
//	KEY_ID advisor =diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#advisor>");
//	KEY_ID takesCourse =diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");
//	KEY_ID teacherOf =diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#teacherOf>");

	set<TYPE_ID> typesStudent = diplo::TH.GetHierarchyForType(Student);
	set<TYPE_ID> typesFaculty = diplo::TH.GetHierarchyForType(Faculty);
	set<TYPE_ID> typesCourse = diplo::TH.GetHierarchyForType(Course);

//first Part
	vector<unordered_set<KEY_ID>*> elsCourse;
	for (set<TYPE_ID>::iterator it2 = typesCourse.begin(); it2 != typesCourse.end(); it2++) {
		unordered_set<KEY_ID>* el;
		el = diplo::TypesM.GetElementsOfType(*it2);
		elsCourse.push_back(el);
	}

	unordered_map<KEY_ID, KEY_ID> CourseFaculty;
	vector<pair<KEY_ID, KEY_ID> > vCourseFaculty;
	set<pair<const KEY_ID, Molecule>*>* Molecules_with_course;
	for (vector<unordered_set<KEY_ID>*>::iterator course_type = elsCourse.begin(); course_type != elsCourse.end(); course_type++) {
		for (unordered_set<KEY_ID>::iterator course = (*course_type)->begin(); course != (*course_type)->end(); course++) {
			Molecules_with_course = diplo::M.GetMoleculesFromInvertedIdx(*course);
			if (Molecules_with_course == NULL)
				continue;

			for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_course->begin(); it != Molecules_with_course->end(); it++) {
				set<TYPE_ID>::iterator type = typesFaculty.find(diplo::KM.GetType((*it)->first));
				if (type != typesFaculty.end()) {

					size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, teacherOf, diplo::KM.GetType(*course), 0);
					if ((*it)->second.CheckIfEntityExists(tripleTempl, *course, 0)) {
						CourseFaculty.insert(pair<KEY_ID, KEY_ID>(*course, (*it)->first));
						vCourseFaculty.push_back(pair<KEY_ID, KEY_ID>(*course, (*it)->first));
//						cerr << diplo::KM.Get( (*it)->first ) << "\t" << diplo::KM.Get( *course )<< endl;
					}
				}
			}

		}
	}

	val4 x;
	vector<val4> ready;
	vector<pair<KEY_ID, KEY_ID> > StudentFaculty;
	vector<pair<KEY_ID, KEY_ID> > StudentCourse;
	for (set<TYPE_ID>::iterator it = typesStudent.begin(); it != typesStudent.end(); it++) {
		unordered_set<KEY_ID>* Students;
		Students = diplo::TypesM.GetElementsOfType(*it); // get all Students

		unordered_set<size_t> templatesStudentFaculty;
		unordered_set<size_t> templatesStudentCourse;

		for (set<TYPE_ID>::iterator it2 = typesFaculty.begin(); it2 != typesFaculty.end(); it2++) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, advisor, *it2, 0);
			templatesStudentFaculty.insert(tripleTempl);
		}
		for (set<TYPE_ID>::iterator it2 = typesCourse.begin(); it2 != typesCourse.end(); it2++) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, takesCourse, *it2, 0);
			templatesStudentCourse.insert(tripleTempl);
		}

		for (unordered_set<KEY_ID>::iterator student = Students->begin(); student != Students->end(); student++) {
			unordered_set<KEY_ID> Faculties;
			unordered_set<KEY_ID> Courses;
			Molecule *m = diplo::M.Get(*student);
			if (m != NULL) {

				m->GetEntity(&templatesStudentFaculty, *student, 0, &Faculties);
				m->GetEntity(&templatesStudentCourse, *student, 0, &Courses);
				if (Faculties.empty() or Courses.empty())
					continue;
				for (unordered_set<KEY_ID>::iterator i = Courses.begin(); i != Courses.end(); i++) {
					unordered_map<KEY_ID, KEY_ID>::iterator faculty = CourseFaculty.find(*i);
					if (faculty != CourseFaculty.end()) {
						unordered_set<KEY_ID>::iterator f = Faculties.find(faculty->second);
						if (f != Faculties.end()) {
							x.v1 = *student;
							x.v2 = faculty->second;
							x.v3 = *i;
//							cerr << diplo::KM.Get(x.v1) << "\t" << diplo::KM.Get(x.v2) << "\t"<< diplo::KM.Get(x.v3) << "\t" << endl;
							ready.push_back(x);
						} else {
							StudentFaculty.push_back(pair<KEY_ID, KEY_ID>(*student, *(Faculties.begin())));
							StudentCourse.push_back(pair<KEY_ID, KEY_ID>(*student, *i));
						}
					} else {
						StudentFaculty.push_back(pair<KEY_ID, KEY_ID>(*student, *(Faculties.begin())));
						StudentCourse.push_back(pair<KEY_ID, KEY_ID>(*student, *i));
					}
				}
			}
		}
	}

	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(val4));

	s = vCourseFaculty.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vCourseFaculty.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

	s = StudentFaculty.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) StudentFaculty.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

	s = StudentCourse.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) StudentCourse.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));
}

void LUBM_network::q09s1_m() {
	string q = "q09s1";
	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
	TYPE_ID Faculty = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Faculty>");
	TYPE_ID Course = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Course>");

	KEY_ID advisor = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#advisor>");
	KEY_ID takesCourse = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");
	KEY_ID teacherOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#teacherOf>");

	size_t s = q.size() + sizeof(Student) + sizeof(Faculty) + sizeof(Course) + sizeof(advisor) + sizeof(takesCourse) + sizeof(teacherOf);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = Student;
	data_tmp = data_tmp + sizeof(Student);
	*((TYPE_ID *) data_tmp) = Faculty;
	data_tmp = data_tmp + sizeof(Faculty);
	*((TYPE_ID *) data_tmp) = Course;
	data_tmp = data_tmp + sizeof(Course);

	*((KEY_ID *) data_tmp) = advisor;
	data_tmp = data_tmp + sizeof(advisor);
	*((KEY_ID *) data_tmp) = takesCourse;
	data_tmp = data_tmp + sizeof(takesCourse);
	*((KEY_ID *) data_tmp) = teacherOf;
	data_tmp = data_tmp + sizeof(teacherOf);

	unordered_map<KEY_ID, KEY_ID> CourseFaculty;
	vector<pair<KEY_ID, KEY_ID> > vCourseFaculty;
	vector<pair<KEY_ID, KEY_ID> > StudentFaculty;
	vector<pair<KEY_ID, KEY_ID> > StudentCourse;
	vector<val4> results;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {

		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val4), i);

		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = vCourseFaculty.size();
		vCourseFaculty.resize(n + s);
		diplo::SRV.Receive((char *) (vCourseFaculty.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = StudentFaculty.size();
		StudentFaculty.resize(n + s);
		diplo::SRV.Receive((char *) (StudentFaculty.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = StudentCourse.size();
		StudentCourse.resize(n + s);
		diplo::SRV.Receive((char *) (StudentCourse.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

	}

	copy(vCourseFaculty.begin(), vCourseFaculty.end(), inserter(CourseFaculty, CourseFaculty.begin()));
	//do Join!!!!
	for (size_t i = 0; i < StudentCourse.size(); i++) {
		if (StudentCourse[i].first != StudentFaculty[i].first) {
			cerr << "THIS IS NOT GOOD: " << diplo::KM.Get(StudentCourse[i].first) << "\t" << diplo::KM.Get(StudentFaculty[i].first) << endl;
			break;
		}
		if (CourseFaculty.find(StudentCourse[i].second)->second == StudentFaculty[i].second) {
			val4 x;
			x.v1 = StudentCourse[i].first;
			x.v2 = StudentFaculty[i].second;
			x.v3 = StudentCourse[i].second;
			results.push_back(x);
		}

	}

	for (vector<val4>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->v1);
		diplo::KM.Get(j->v2);
		diplo::KM.Get(j->v3);
//		cerr << diplo::KM.Get(j->v1) << "\t" << diplo::KM.Get(j->v2) << "\t"<< diplo::KM.Get(j->v3) << "\t" << endl;
	}
	PRINT_NO_RESULTS
}

void LUBM_network::q09s2_n(char* data) {
	/*
	 *
	 * SELECT ?X ?Y ?Z
	 WHERE
	 {
	 {
	 ?t rdfs:subClassOf :Student .
	 ?X a ?t .
	 }
	 UNION
	 {
	 ?X a :Student .
	 }

	 ?t3 rdfs:subClassOf :Faculty .
	 {?Y a ?t3.}
	 UNION
	 {?t4 rdfs:subClassOf ?t3.
	 ?Y a ?t4.}


	 {
	 ?t2 rdfs:subClassOf :Course .
	 ?Z a ?t2 .
	 }
	 UNION
	 {
	 ?Z a :Course .
	 }


	 ?X :advisor ?Y . <--- COND 1ST
	 ?X :takesCourse ?Z .<--- COND 2RD
	 ?Y :teacherOf ?Z .  <--- COND 3RD
	 }
	 *
	 */

	/*
	 * get list of Student with values for [advisor] and [takesCourse]
	 * 	-> if there are some entities for [advisor] and [takesCourse]
	 * 	-> check if The Faculty is [teacherOf] one of course the student takes {COND 3RD}
	 *
	 */

	TYPE_ID Student = *((TYPE_ID *) data);
	data = data + sizeof(Student);
	TYPE_ID Faculty = *((TYPE_ID *) data);
	data = data + sizeof(Faculty);
	TYPE_ID Course = *((TYPE_ID *) data);
	data = data + sizeof(Course);

	KEY_ID advisor = *((KEY_ID *) data);
	data = data + sizeof(advisor);
	KEY_ID takesCourse = *((KEY_ID *) data);
	data = data + sizeof(takesCourse);
	KEY_ID teacherOf = *((KEY_ID *) data);
	data = data + sizeof(teacherOf);

//	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
//	TYPE_ID Faculty = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Faculty>");
//	TYPE_ID Course = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Course>");
//
//	KEY_ID advisor =diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#advisor>");
//	KEY_ID takesCourse =diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");
//	KEY_ID teacherOf =diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#teacherOf>");

	set<TYPE_ID> typesStudent = diplo::TH.GetHierarchyForType(Student);
	set<TYPE_ID> typesFaculty = diplo::TH.GetHierarchyForType(Faculty);
	set<TYPE_ID> typesCourse = diplo::TH.GetHierarchyForType(Course);

	vector<unordered_set<KEY_ID>*> elsCourse;
	for (set<TYPE_ID>::iterator it2 = typesCourse.begin(); it2 != typesCourse.end(); it2++) {
		unordered_set<KEY_ID>* el;
		el = diplo::TypesM.GetElementsOfType(*it2);
		elsCourse.push_back(el);
	}

	val4 x;
	vector<val4> ready;
	vector<pair<KEY_ID, KEY_ID> > StudentFaculty;
	vector<pair<KEY_ID, KEY_ID> > StudentCourse;

	for (set<TYPE_ID>::iterator it = typesStudent.begin(); it != typesStudent.end(); it++) {
		unordered_set<KEY_ID>* Students;
		Students = diplo::TypesM.GetElementsOfType(*it); // get all Students

		unordered_set<size_t> templatesStudentFaculty;
		unordered_set<size_t> templatesStudentCourse;
		unordered_set<size_t> templatesFacultyCourse;

		for (set<TYPE_ID>::iterator it2 = typesFaculty.begin(); it2 != typesFaculty.end(); it2++) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, advisor, *it2, 0);
			templatesStudentFaculty.insert(tripleTempl);
		}
		for (set<TYPE_ID>::iterator it2 = typesCourse.begin(); it2 != typesCourse.end(); it2++) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, takesCourse, *it2, 0);
			templatesStudentCourse.insert(tripleTempl);

			for (set<TYPE_ID>::iterator it3 = typesFaculty.begin(); it3 != typesFaculty.end(); it3++) {
				tripleTempl = diplo::TM.GetTripleTemplate(*it, *it3, teacherOf, *it2, 1);
				templatesFacultyCourse.insert(tripleTempl);
			}
		}

		for (unordered_set<KEY_ID>::iterator student = Students->begin(); student != Students->end(); student++) {
			unordered_set<KEY_ID> Faculties;
			unordered_set<KEY_ID> Courses;
			unordered_set<KEY_ID> Courses4Faculty;
			Molecule *m = diplo::M.Get(*student);
			if (m != NULL) {

				m->GetEntity(&templatesStudentFaculty, *student, 0, &Faculties);
				if (Faculties.empty())
					continue;

				m->GetEntity(&templatesStudentCourse, *student, 0, &Courses);
				if (Courses.empty())
					continue;

				m->GetEntity(&templatesFacultyCourse, *(Faculties.begin()), 1, &Courses4Faculty);

				for (unordered_set<KEY_ID>::iterator i = Courses.begin(); i != Courses.end(); i++) {
					unordered_set<KEY_ID>::iterator faculty = Courses4Faculty.find(*i);
					if (faculty != Courses4Faculty.end()) {
						x.v1 = *student;
						x.v2 = *faculty;
						x.v3 = *i;
//						cerr << diplo::KM.Get(x.v1) << "\t" << diplo::KM.Get(x.v2) << "\t" << diplo::KM.Get(x.v3) << "\t" << endl;
						ready.push_back(x);
					}
				}
			}
		}
	}

	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(val4));

}

void LUBM_network::q09s2_m() {
	string q = "q09s2";
	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
	TYPE_ID Faculty = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Faculty>");
	TYPE_ID Course = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Course>");

	KEY_ID advisor = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#advisor>");
	KEY_ID takesCourse = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");
	KEY_ID teacherOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#teacherOf>");

	size_t s = q.size() + sizeof(Student) + sizeof(Faculty) + sizeof(Course) + sizeof(advisor) + sizeof(takesCourse) + sizeof(teacherOf);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = Student;
	data_tmp = data_tmp + sizeof(Student);
	*((TYPE_ID *) data_tmp) = Faculty;
	data_tmp = data_tmp + sizeof(Faculty);
	*((TYPE_ID *) data_tmp) = Course;
	data_tmp = data_tmp + sizeof(Course);

	*((KEY_ID *) data_tmp) = advisor;
	data_tmp = data_tmp + sizeof(advisor);
	*((KEY_ID *) data_tmp) = takesCourse;
	data_tmp = data_tmp + sizeof(takesCourse);
	*((KEY_ID *) data_tmp) = teacherOf;
	data_tmp = data_tmp + sizeof(teacherOf);

	vector<val4> results;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {

		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val4), i);
	}

	for (vector<val4>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->v1);
		diplo::KM.Get(j->v2);
		diplo::KM.Get(j->v3);
//		cerr << diplo::KM.Get(j->v1) << "\t" << diplo::KM.Get(j->v2) << "\t"<< diplo::KM.Get(j->v3) << "\t" << endl;
	}
	PRINT_NO_RESULTS
}

void LUBM_network::q10_n(char* data) {
	/*
	 * SELECT ?X
	 WHERE
	 {
	 ?t rdfs:subClassOf :Person .
	 ?X a ?t .

	 ?X :takesCourse <http://www.Department0.University0.edu/GraduateCourse0> .
	 }
	 *
	 */


	//THE ALGORITHM
	//find all molecules where GraduateCourse0 is an OBJECT
	//check if it is of type GraduateStudent (don't forget about COMPOSITE TYPES)
	//check each found molecule for THE CONSTRAIN
	vector<KEY_ID> results;

	KEY_ID o = *((KEY_ID *) data);
	data = data + sizeof(o);
	KEY_ID p = *((KEY_ID *) data);
	data = data + sizeof(p);
	TYPE_ID t = *((TYPE_ID *) data);
	data = data + sizeof(t);

	set<TYPE_ID> types = diplo::TH.GetHierarchyForType(t);

	set<pair<const KEY_ID, Molecule>*>* Molecules_with_GraduateCourse0;
	Molecules_with_GraduateCourse0 = diplo::M.GetMoleculesFromInvertedIdx(o);
	if (Molecules_with_GraduateCourse0) { // do we have any of this
		for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_GraduateCourse0->begin(); it != Molecules_with_GraduateCourse0->end(); it++) {
			set<TYPE_ID>::iterator type = types.find(diplo::KM.GetType((*it)->first));
			if (type != types.end()) {
				size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, p, diplo::KM.GetType(o), 0);

				if ((*it)->second.CheckIfEntityExists(tripleTempl, o, 0)) {
					results.push_back((*it)->first);
				}
			}
		}
	}

	size_t s = results.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) results.data(), results.size() * sizeof(KEY_ID));
}

void LUBM_network::q10_m() {
	string q = "q10";
	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu/GraduateCourse0>");
	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");

	TYPE_ID t = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Person>");

	size_t s = q.size() + sizeof(o) + sizeof(p) + sizeof(t);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((KEY_ID *) data_tmp) = o;
	data_tmp = data_tmp + sizeof(o);
	*((KEY_ID *) data_tmp) = p;
	data_tmp = data_tmp + sizeof(p);
	*((TYPE_ID *) data_tmp) = t;
	data_tmp = data_tmp + sizeof(t);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(KEY_ID), i);

	}

	for (vector<KEY_ID>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(*j);
//		cerr << diplo::KM.Get(*j) << endl;
	}
	PRINT_NO_RESULTS
}

void LUBM_network::q11s1_n(char* data) {
	/*
	 * SELECT ?X
	 WHERE
	 {
	 ?X a :ResearchGroup .
	 ?X :subOrganizationOf ?Y . <---- CONSTRAIN
	 ?Y :subOrganizationOf <http://www.University0.edu> .
	 }
	 *
	 */

	/*
	 * get list of [ResearchGroup] with values for [subOrganizationOf]
	 *
	 * get list of molecules where <http://www.University0.edu> is an object
	 *
	 * intersect the lists above and check [CONSTRAIN]
	 */

//	TYPE_ID ResearchGroup = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#ResearchGroup>");
//
//	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
//	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");

	TYPE_ID ResearchGroup = *((TYPE_ID *) data);
	data = data + sizeof(ResearchGroup);

	KEY_ID subOrganizationOf = *((KEY_ID *) data);
	data = data + sizeof(subOrganizationOf);

	KEY_ID University0 = *((KEY_ID *) data);
	data = data + sizeof(University0);

	set<TYPE_ID> typesResearchGroup = diplo::TH.GetHierarchyForType(ResearchGroup);

	set<pair<const KEY_ID, Molecule>*>* Molecules_with_uni;
	Molecules_with_uni = diplo::M.GetMoleculesFromInvertedIdx(University0);
//	if (Molecules_with_uni == NULL)
//		ERROR_OUT

	unordered_set<KEY_ID> Object4University;
	vector<KEY_ID> vObject4University;
	vector<KEY_ID> ready;
	vector< pair<KEY_ID,KEY_ID> > vObjectResearchGroup;
	TYPE_ID University = diplo::KM.GetType(University0);
	if (Molecules_with_uni != NULL) {
		for (set<pair<const KEY_ID, Molecule>*>::iterator i = Molecules_with_uni->begin(); i != Molecules_with_uni->end(); i++) {
			unordered_set<size_t> templatesObject_subOrganizationOf_University0;
			TYPE_ID t = diplo::KM.GetType((*i)->first);

			size_t tripleTempl = diplo::TM.GetTripleTemplate(t, t, subOrganizationOf, University, 0);
			if ((*i)->second.CheckIfEntityExists(tripleTempl, University0, 0)) {
				Object4University.insert((*i)->first);
				vObject4University.push_back((*i)->first);
			}

		}
	}

	for (set<TYPE_ID>::iterator it = typesResearchGroup.begin(); it != typesResearchGroup.end(); it++) {
		unordered_set<KEY_ID>* ResearchGroups;
		ResearchGroups = diplo::TypesM.GetElementsOfType(*it); // get all ResearchGroups

		unordered_set<size_t> templatesResearchGroup_subOrganizationOf;

		{
			size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, subOrganizationOf, 0, 0);
			templatesResearchGroup_subOrganizationOf.insert(tripleTempl);
		}

		for (unordered_set<KEY_ID>::iterator rg = ResearchGroups->begin(); rg != ResearchGroups->end(); rg++) {
			unordered_set<KEY_ID> objects;
			Molecule *m = diplo::M.Get(*rg);
			if (m != NULL) {
				m->GetEntity(&templatesResearchGroup_subOrganizationOf, *rg, 0, &objects);
				unordered_set<KEY_ID>::iterator forUni = Object4University.find( *(objects.begin()) );
				if (forUni != Object4University.end()) {
					ready.push_back(*rg);
				} else {
					vObjectResearchGroup.push_back( pair<KEY_ID,KEY_ID> (*(objects.begin()), *rg) );
				}
			}
		}

	}

	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(KEY_ID));

	s = vObject4University.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vObject4University.data(), s * sizeof( KEY_ID ));

	s = vObjectResearchGroup.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vObjectResearchGroup.data(), s * sizeof( pair<KEY_ID, KEY_ID> ));


}

void LUBM_network::q11s1_m() {
	string q = "q11s1";
	TYPE_ID ResearchGroup = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#ResearchGroup>");

	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");

	size_t s = q.size() + sizeof(ResearchGroup) + sizeof(subOrganizationOf) + sizeof(University0);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = ResearchGroup;
	data_tmp = data_tmp + sizeof(ResearchGroup);

	*((KEY_ID *) data_tmp) = subOrganizationOf;
	data_tmp = data_tmp + sizeof(subOrganizationOf);
	*((KEY_ID *) data_tmp) = University0;
	data_tmp = data_tmp + sizeof(University0);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> vObject4University;
	vector<KEY_ID> results;
	vector< pair<KEY_ID,KEY_ID> > vObjectResearchGroup;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(KEY_ID), i);

		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = vObject4University.size();
		vObject4University.resize(n + s);
		diplo::SRV.Receive((char *) (vObject4University.data() + n), s * sizeof( KEY_ID ), i);

		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = vObjectResearchGroup.size();
		vObjectResearchGroup.resize(n + s);
		diplo::SRV.Receive((char *) (vObjectResearchGroup.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);
	}

	unordered_multimap<KEY_ID, KEY_ID> ObjectResearchGroup;
	copy(vObjectResearchGroup.begin(), vObjectResearchGroup.end(), inserter(ObjectResearchGroup, ObjectResearchGroup.begin()));

	pair<unordered_multimap<KEY_ID, KEY_ID>::iterator, unordered_multimap<KEY_ID, KEY_ID>::iterator> it;
	for (vector<KEY_ID>::iterator c = vObject4University.begin(); c != vObject4University.end(); c++) {
		it = ObjectResearchGroup.equal_range(*c);
		for (unordered_multimap<KEY_ID, KEY_ID>::iterator it2 = it.first; it2 != it.second; it2++) {
			results.push_back(it2->second);
		}
	}

	for (vector<KEY_ID>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(*j);
//		cerr << diplo::KM.Get(*j) << endl;
	}
	PRINT_NO_RESULTS

}

void LUBM_network::q11s2_n(char* data) {
	/*
	 * SELECT ?X
	 WHERE
	 {
	 ?X a :ResearchGroup .
	 ?X :subOrganizationOf ?Y .
	 ?Y :subOrganizationOf <http://www.University0.edu> .
	 }
	 *
	 */


//	TYPE_ID ResearchGroup = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#ResearchGroup>");
//
//	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
//	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");

	TYPE_ID ResearchGroup = *((TYPE_ID *) data);
	data = data + sizeof(ResearchGroup);

	KEY_ID subOrganizationOf = *((KEY_ID *) data);
	data = data + sizeof(subOrganizationOf);

	KEY_ID University0 = *((KEY_ID *) data);
	data = data + sizeof(University0);

	set<TYPE_ID> typesResearchGroup = diplo::TH.GetHierarchyForType(ResearchGroup);

	vector<KEY_ID> ready;
	TYPE_ID University = diplo::KM.GetType(University0);

	for (set<TYPE_ID>::iterator it = typesResearchGroup.begin(); it != typesResearchGroup.end(); it++) {
		unordered_set<KEY_ID>* ResearchGroups;
		ResearchGroups = diplo::TypesM.GetElementsOfType(*it); // get all ResearchGroups

		unordered_set<size_t> templatesResearchGroup_subOrganizationOf;


		size_t tripleTempl = diplo::TM.GetTripleTemplate(*it, *it, subOrganizationOf, 0, 0);
		templatesResearchGroup_subOrganizationOf.insert(tripleTempl);

		unordered_set<size_t> templatesObject_subOrganizationOf_University0;
		tripleTempl = diplo::TM.GetTripleTemplate(*it, 0, subOrganizationOf, University, 1);
		templatesObject_subOrganizationOf_University0.insert(tripleTempl);

		for (unordered_set<KEY_ID>::iterator rg = ResearchGroups->begin(); rg != ResearchGroups->end(); rg++) {
			unordered_set<KEY_ID> objects;
			Molecule *m = diplo::M.Get(*rg);
			if (m != NULL) {
				m->GetEntity(&templatesResearchGroup_subOrganizationOf, *rg, 0, &objects);
				for (unordered_set<KEY_ID>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
					unordered_set<KEY_ID> objects2;

					m->GetEntity(&templatesObject_subOrganizationOf_University0, *obj, 1, &objects2);
					if (objects2.size() == 1 and *(objects2.begin()) == University0)
						ready.push_back(*rg);

				}
			}
		}

	}

	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(KEY_ID));
}

void LUBM_network::q11s2_m() {
	string q = "q11s2";
	TYPE_ID ResearchGroup = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#ResearchGroup>");

	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");

	size_t s = q.size() + sizeof(ResearchGroup) + sizeof(subOrganizationOf) + sizeof(University0);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = ResearchGroup;
	data_tmp = data_tmp + sizeof(ResearchGroup);

	*((KEY_ID *) data_tmp) = subOrganizationOf;
	data_tmp = data_tmp + sizeof(subOrganizationOf);
	*((KEY_ID *) data_tmp) = University0;
	data_tmp = data_tmp + sizeof(University0);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(KEY_ID), i);

	}

	for (vector<KEY_ID>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(*j);
//		cerr << diplo::KM.Get(*j) << endl;
	}
	PRINT_NO_RESULTS
}

void LUBM_network::q12_n(char* data) {
	/*
	 * SELECT ?X ?Y
	 WHERE
	 { ?Y :subOrganizationOf <http://www.University0.edu> .
	 ?Y a :Department .
	 ?X :worksFor ?Y . <---- COND 2
	 ?X :headOf ?Y . <--- COND 1
	 }
	 *
	 */

	/* THE ALGORITHM
	 *
	 * get list of all Departments for [University0]
	 *
	 * get all Departments, for each
	 * 	find molecules where it appears,
	 * 		check  COND 1 if OK
	 * 			check COND 2
	 * 				if OK check if if exist on the list of above Departments for University0,
	 * 					if YES -> ready
	 * 					if NO -> let server work on it
	 *
	 *
	 */

//	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");
//	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
//
//	KEY_ID worksFor = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#worksFor>");
//	KEY_ID headOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#headOf>");
//
//	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");


	KEY_ID University0 = *((KEY_ID *) data);
	data = data + sizeof(University0);
	KEY_ID subOrganizationOf = *((KEY_ID *) data);
	data = data + sizeof(subOrganizationOf);

	KEY_ID worksFor = *((KEY_ID *) data);
	data = data + sizeof(worksFor);
	KEY_ID headOf = *((KEY_ID *) data);
	data = data + sizeof(headOf);

	TYPE_ID Department = *((TYPE_ID *) data);
	data = data + sizeof(Department);

	vector< pair<KEY_ID,KEY_ID> > ready;

	multimap<TYPE_ID, size_t> templatesheadOfDepartment;
	diplo::TM.GetTemplates(-1, headOf, Department, 0, &templatesheadOfDepartment);

	multimap<TYPE_ID, size_t> templatesworksForDepartment;
	diplo::TM.GetTemplates(-1, worksFor, Department, 0, &templatesworksForDepartment);

	unordered_set<KEY_ID> departmentsUniversity0;
	vector<KEY_ID> vdepartmentsUniversity0;
	vector< pair<KEY_ID,KEY_ID> > vObjheadOfDep;

	set<pair<const KEY_ID, Molecule>*>* Molecules_with_University0 = diplo::M.GetMoleculesFromInvertedIdx(University0);
	if (Molecules_with_University0) {
		for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_University0->begin(); it != Molecules_with_University0->end(); it++) {
			if (diplo::KM.GetType((*it)->first) == Department) {
				departmentsUniversity0.insert((*it)->first);
				vdepartmentsUniversity0.push_back((*it)->first);
//				cout << diplo::KM.Get((*it)->first) << endl;
			}
		}
	}

	unordered_set<KEY_ID>* departments = diplo::TypesM.GetElementsOfType(Department); // get all Departments

	for (unordered_set<KEY_ID>::iterator dep = departments->begin(); dep != departments->end(); dep++) {
		set<pair<const KEY_ID, Molecule>*>* Molecules_with_dep = diplo::M.GetMoleculesFromInvertedIdx(*dep);
		if (Molecules_with_dep) {
			for (set<pair<const KEY_ID, Molecule>*>::iterator o = Molecules_with_dep->begin(); o != Molecules_with_dep->end(); o++) {
				multimap<TYPE_ID, size_t>::iterator type = templatesheadOfDepartment.find(diplo::KM.GetType((*o)->first));
				if (type != templatesheadOfDepartment.end()) {
					Molecule *m = diplo::M.Get( (*o)->first );
					if (m != NULL) {
						if (m->CheckIfEntityExists(type->second, *dep, 0)) {
							multimap<TYPE_ID, size_t>::iterator type2 = templatesworksForDepartment.find(diplo::KM.GetType((*o)->first));
							if (type2 != templatesworksForDepartment.end()) {
								if (m->CheckIfEntityExists(type2->second, *dep, 0)) {


									if (departmentsUniversity0.find(*dep) != departmentsUniversity0.end()) { //ready
//										cout << "1: " << diplo::KM.Get((*o)->first) << "\t" << diplo::KM.Get( *dep ) << endl;
										ready.push_back(pair<KEY_ID, KEY_ID>((*o)->first, *dep));
									} else { //pass it to the server to do join
//										cout << "2: " << diplo::KM.Get((*o)->first) << "\t" << diplo::KM.Get( *dep ) << endl;
										vObjheadOfDep.push_back( pair<KEY_ID, KEY_ID>( *dep, (*o)->first ));
									}
								}


							}
						}
					}
				}
			}
		}
	}

//	cout << "ready.size(): " << ready.size() << endl;
	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(pair<KEY_ID,KEY_ID>));

	s = vdepartmentsUniversity0.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vdepartmentsUniversity0.data(), s * sizeof( KEY_ID ));

	s = vObjheadOfDep.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vObjheadOfDep.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

}

void LUBM_network::q12_m() {
	string q = "q12";
	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");
	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");

	KEY_ID worksFor = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#worksFor>");
	KEY_ID headOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#headOf>");

	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");

	size_t s = q.size() + sizeof(University0) + sizeof(subOrganizationOf) + sizeof(worksFor) + sizeof(headOf) + sizeof(Department);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();

	*((KEY_ID *) data_tmp) = University0;
	data_tmp = data_tmp + sizeof(University0);
	*((KEY_ID *) data_tmp) = subOrganizationOf;
	data_tmp = data_tmp + sizeof(subOrganizationOf);


	*((KEY_ID *) data_tmp) = worksFor;
	data_tmp = data_tmp + sizeof(worksFor);
	*((KEY_ID *) data_tmp) = headOf;
	data_tmp = data_tmp + sizeof(headOf);

	*((TYPE_ID *) data_tmp) = Department;
	data_tmp = data_tmp + sizeof(Department);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> vdepartmentsUniversity0;
	vector< pair<KEY_ID,KEY_ID> > vObjheadOfDep;
	unordered_map< KEY_ID,KEY_ID > ObjheadOfDep;

	vector<pair<KEY_ID, KEY_ID> > results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(pair<KEY_ID, KEY_ID>), i);

		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = vdepartmentsUniversity0.size();
		vdepartmentsUniversity0.resize(n + s);
		diplo::SRV.Receive((char *) (vdepartmentsUniversity0.data() + n), s * sizeof( KEY_ID ), i);

		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = vObjheadOfDep.size();
		vObjheadOfDep.resize(n + s);
		diplo::SRV.Receive((char *) (vObjheadOfDep.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

	}

	copy(vObjheadOfDep.begin(), vObjheadOfDep.end(), inserter(ObjheadOfDep, ObjheadOfDep.begin()));


	for (vector<KEY_ID>::iterator j = vdepartmentsUniversity0.begin(); j != vdepartmentsUniversity0.end(); j++) {
		unordered_map< KEY_ID,KEY_ID >::iterator it = ObjheadOfDep.find(*j);
		if( it != ObjheadOfDep.end() ) {
			results.push_back(pair<KEY_ID, KEY_ID>( it->second, it->first));
		}
	}

	for (vector<pair<KEY_ID, KEY_ID>>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->first);
		diplo::KM.Get(j->second);
//		cout << diplo::KM.Get(j->first) << "\t" << diplo::KM.Get(j->second) << endl;
	}

	PRINT_NO_RESULTS
}

void LUBM_network::q13_n(char* data) {
	/*
	 * SELECT ?X
	 WHERE
	 {
	 :hasAlumnus owl:inverseOf ?c .
	 ?a ?b ?c .
	 ?X ?a <http://www.University0.edu> .
	 }
	 *
	 */

////that is a bit tricky, we should implement some OWL schema manager to avoid that kind of tricks
//	KEY_ID p1 = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#doctoralDegreeFrom>");
//	KEY_ID p2 = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#mastersDegreeFrom>");
//	KEY_ID p3 = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#undergraduateDegreeFrom>");
//////
//	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");

	KEY_ID p1 = *((KEY_ID *) data);
	data = data + sizeof(p1);
	KEY_ID p2 = *((KEY_ID *) data);
	data = data + sizeof(p2);
	KEY_ID p3 = *((KEY_ID *) data);
	data = data + sizeof(p3);

	KEY_ID University0 = *((KEY_ID *) data);
	data = data + sizeof(University0);

//	diplo::onlyPartition = true;

//
//	cerr << "#############################################################################################################################################################"
//			<< endl << endl << endl << endl;
	vector< KEY_ID > ready;

	multimap<TYPE_ID, size_t> templatesPUniversity;
	diplo::TM.GetTemplates(-1, p1, diplo::KM.GetType ( University0 ), 0, &templatesPUniversity);
//	cerr << "p1: " << templatesPUniversity.size() << endl;
	diplo::TM.GetTemplates(-1, p2, diplo::KM.GetType ( University0 ), 0, &templatesPUniversity);
//	cerr << "p2: " << templatesPUniversity.size() << endl;
	diplo::TM.GetTemplates(-1, p3, diplo::KM.GetType ( University0 ), 0, &templatesPUniversity);
//	cerr << "p3: " << templatesPUniversity.size() << endl;

//	for (multimap<TYPE_ID, size_t>::iterator type = templatesPUniversity.begin(); type != templatesPUniversity.end(); type++) {
//		cerr << type->first << "\t" << type->second << endl;
//
//	}

//
//	Molecule *m3 = diplo::M.Get( 14073748835536021 );
//	if (m3)
//		m3->CheckIfEntityExists(3, University0, 0);
//	else
//		cerr << "no m3" << endl;
//	return;

//	cerr << endl << endl;

	set<pair<const KEY_ID, Molecule>*>* Molecules_with_University0 = diplo::M.GetMoleculesFromInvertedIdx(University0);
	if (Molecules_with_University0) {
		for (set<pair<const KEY_ID, Molecule>*>::iterator i = Molecules_with_University0->begin(); i != Molecules_with_University0->end(); i++) {
			pair< multimap<TYPE_ID, size_t>::iterator, multimap<TYPE_ID, size_t>::iterator>  types = templatesPUniversity.equal_range(diplo::KM.GetType((*i)->first));
			Molecule *m = NULL;
			for (multimap<TYPE_ID, size_t>::iterator type = types.first; type != types.second; type++) {
				if (m == NULL ) m = diplo::M.Get((*i)->first);
				if (m != NULL) {
//					cerr << "before CheckIfEntityExists: " <<  type->first << "\t" << type->second << "\t" << (*i)->first << endl;
					if (m->CheckIfEntityExists(type->second, University0, 0)) {

						ready.push_back( (*i)->first );
					}
				}
			}
		}
	}

	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof( KEY_ID ));
}

void LUBM_network::q13_m() {
	string q = "q13";

//that is a bit tricky, we should implement some OWL schema manager to avoid that kind of tricks
	KEY_ID p1 = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#doctoralDegreeFrom>");
	KEY_ID p2 = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#mastersDegreeFrom>");
	KEY_ID p3 = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#undergraduateDegreeFrom>");
////

	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");


	size_t s = q.size() + sizeof(p1) + sizeof(p2) + sizeof(p3) + sizeof(University0);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((KEY_ID *) data_tmp) = p1;
	data_tmp = data_tmp + sizeof(p1);
	*((KEY_ID *) data_tmp) = p2;
	data_tmp = data_tmp + sizeof(p2);
	*((KEY_ID *) data_tmp) = p3;
	data_tmp = data_tmp + sizeof(p3);
	*((KEY_ID *) data_tmp) = University0;
	data_tmp = data_tmp + sizeof(University0);


	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(KEY_ID), i);

	}

	for (vector<KEY_ID>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(*j);
//		cout << diplo::KM.Get( *j ) << endl;
	}
	PRINT_NO_RESULTS
}

void LUBM_network::q14_n(char* data) {
	/*
	 * SELECT ?X
	 WHERE
	 { ?X a :UndergraduateStudent .
	 }
	 *
	 */


	TYPE_ID UndergraduateStudent = *((TYPE_ID *) data);
	data = data + sizeof(UndergraduateStudent);

	vector<KEY_ID> ready;;
	set<TYPE_ID> typesGraduateStudent = diplo::TH.GetCompositesForTypes(UndergraduateStudent);

	vector<unordered_set<KEY_ID>*> elsUndergraduateStudent;
	for (set<TYPE_ID>::iterator it2 = typesGraduateStudent.begin(); it2 != typesGraduateStudent.end(); it2++) {
		unordered_set<KEY_ID>* el = diplo::TypesM.GetElementsOfType(*it2);
		for (unordered_set<KEY_ID>::iterator i = el->begin(); i != el->end(); i++) {
			if (diplo::M.IsItMyObject(*i))
				ready.push_back(*i);
		}
	}


	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(KEY_ID));
}

void LUBM_network::q14_m() {
	string q = "q14";

	TYPE_ID t = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#UndergraduateStudent>");

	size_t s = q.size() + sizeof(t);

	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = t;
	data_tmp = data_tmp + sizeof(t);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
//		cerr << i << "\ts: " << s << "\tn: " << n << "\t" << sizeof(s)<< endl;
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(KEY_ID), i);

	}

	for (vector<KEY_ID>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(*j);
//		cout << diplo::KM.Get(*j) << endl;

	}

	PRINT_NO_RESULTS

}

void LUBM_network::q15_n(char* data) {
	/*
	 * select ?Y (count(?X) as ?ns) where {
	 {
	 ?t rdfs:subClassOf :Student .
	 ?X a ?t .
	 }
	 UNION
	 {
	 ?X a :Student .
	 }

	 ?t3 rdfs:subClassOf :Faculty .
	 {?Y a ?t3.}
	 UNION
	 {?t4 rdfs:subClassOf ?t3.
	 ?Y a ?t4.}



	 ?X :advisor ?Y .
	 }
	 group by ?Y
	 *
	 */

//	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
//	TYPE_ID Faculty = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Faculty>");
//
//	KEY_ID advisor =diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#advisor>");


	TYPE_ID Student = *((TYPE_ID *) data);
	data = data + sizeof(Student);
	TYPE_ID Faculty = *((TYPE_ID *) data);
	data = data + sizeof(Faculty);

	KEY_ID advisor = *((KEY_ID *) data);
	data = data + sizeof(advisor);


	set<TYPE_ID> typesStudent = diplo::TH.GetHierarchyForType(Student);
	set<TYPE_ID> typesFaculty = diplo::TH.GetHierarchyForType(Faculty);

	unordered_map<KEY_ID, unsigned> ready;
	unordered_map<KEY_ID, unsigned>::iterator el;
	for (set<TYPE_ID>::iterator Tfaculty = typesFaculty.begin(); Tfaculty != typesFaculty.end(); Tfaculty++) {
		unordered_set<KEY_ID>* faculties = diplo::TypesM.GetElementsOfType(*Tfaculty);
		for (unordered_set<KEY_ID>::iterator faculty = faculties->begin(); faculty != faculties->end(); faculty++) {
			set<pair<const KEY_ID, Molecule>*>* Molecules_with_faculty = diplo::M.GetMoleculesFromInvertedIdx(*faculty);
			if (Molecules_with_faculty == NULL)
				continue;
			for (set<pair<const KEY_ID, Molecule>*>::iterator s = Molecules_with_faculty->begin(); s != Molecules_with_faculty->end(); s++) {
				set<TYPE_ID>::iterator type = typesStudent.find(diplo::KM.GetType( (*s)->first) );
				if (type != typesStudent.end()) {
					size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, advisor, diplo::KM.GetType(*faculty), 0);
					if ( (*s)->second.CheckIfEntityExists(tripleTempl, *faculty, 0) ) {
						el = ready.insert( el, pair<KEY_ID, unsigned>(*faculty,0) );
						el->second++;
					}
				}
			}
		}
	}
	vector< pair<KEY_ID, unsigned> >vready;
	copy(ready.begin(), ready.end(), inserter(vready, vready.begin()));

	size_t s = vready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vready.data(), s * sizeof( pair<KEY_ID, unsigned> ));
}

void LUBM_network::q15_m() {
	string q = "q15";
	TYPE_ID Student = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>");
	TYPE_ID Faculty = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Faculty>");

	KEY_ID advisor = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#advisor>");

	size_t s = q.size() + sizeof(Student) + sizeof(Faculty) + sizeof(advisor);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = Student;
	data_tmp = data_tmp + sizeof(Student);
	*((TYPE_ID *) data_tmp) = Faculty;
	data_tmp = data_tmp + sizeof(Faculty);

	*((KEY_ID *) data_tmp) = advisor;
	data_tmp = data_tmp + sizeof(advisor);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector< pair<KEY_ID, unsigned> > vresults;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = vresults.size();
		vresults.resize(n + s);
		diplo::SRV.Receive((char *) (vresults.data() + n), s * sizeof( pair<KEY_ID, unsigned> ), i);
	}


	unordered_map<KEY_ID, unsigned> results;
	unordered_map<KEY_ID, unsigned>::iterator el;
	for (vector<pair<KEY_ID, unsigned> >::iterator i = vresults.begin(); i != vresults.end(); i++) {
		el = results.insert( el, pair<KEY_ID, unsigned>(i->first,i->second) );
		el->second += i->second;
	}

	for (unordered_map<KEY_ID, unsigned>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->first);
//		cerr << diplo::KM.Get(j->first) << "\t" << j->second << endl;
	}
	PRINT_NO_RESULTS

}

void LUBM_network::q16s1_n(char* data) {
	/*
	 * select ?a ?b ?c ?d ?e where {
	 ?a :emailAddress 'UndergraduateStudent0@Department0.University0.edu' .
	 ?a ?b ?c .
	 ?c ?d ?e .
	 }
	 */

	/*
	 * get molecules where 'UndergraduateStudent0@Department0.University0.edu' is as an object
	 *
	 * for each check if it is [emailAddress]
	 * 	if yes get all from this molecule
	 * 		for each element try to get it's molecule and all predicates for this one
	 * 	if cannot prepare molecule to sent to the Mastes
	 */

//	KEY_ID v = diplo::KM.Get("\"UndergraduateStudent0@Department0.University0.edu\"");

	KEY_ID v = *((KEY_ID *) data);
	data = data + sizeof(v);

	set<pair<const KEY_ID, Molecule>*>* Molecules_with_v;
	Molecules_with_v = diplo::M.GetMoleculesFromInvertedIdx(v);

	val4 x;
	val5 y;
	vector<val4> s1;
	vector<val5> s2;

	if (Molecules_with_v != NULL) {
		for (set<pair<const KEY_ID, Molecule>*>::iterator i = Molecules_with_v->begin(); i != Molecules_with_v->end(); i++) {
			unordered_multimap<size_t, KEY_ID> mol;
			(*i)->second.GetAllEntities(0, &mol,0);
			x.v1 = (*i)->first;
			for (unordered_multimap<size_t, KEY_ID>::iterator j = mol.begin(); j != mol.end(); j++) {
				x.v2 = diplo::TM.GetPredicate(diplo::KM.GetType((*i)->first), j->first, 0);
				x.v3 = j->second;
				s1.push_back(x);
			}
		}
	}

	size_t s = s1.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) s1.data(), s * sizeof(val4));

	diplo::CLI.Receive((char*) &s, sizeof(s));
	s1.resize(s);
	diplo::CLI.Receive((char *) (s1.data()), s * sizeof(val4));

	for (vector<val4>::iterator j = s1.begin(); j != s1.end(); j++) {
		Molecule *m = diplo::M.Get(j->v3);
		if (m != NULL) {
			unordered_multimap<size_t, KEY_ID> mol;
			m->GetAllEntities(0, &mol,0);
			y.v1 = j->v1;
			y.v2 = j->v2;
			y.v3 = j->v3;
			for (unordered_multimap<size_t, KEY_ID>::iterator i = mol.begin(); i != mol.end(); i++) {
				y.v4 = diplo::TM.GetPredicate(diplo::KM.GetType(j->v3), i->first, 0);
				y.v5 = i->second;
				s2.push_back(y);
			}
		}
	}

	s = s2.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) s2.data(), s * sizeof(val5));
}

void LUBM_network::q16s1_m() {
	string q = "q16s1";
	KEY_ID v = diplo::KM.Get("\"UndergraduateStudent0@Department0.University0.edu\"");

	size_t s = q.size() + sizeof(v);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((KEY_ID *) data_tmp) = v;
	data_tmp = data_tmp + sizeof(v);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector <val4> s1;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = s1.size();
		s1.resize(n + s);
		diplo::SRV.Receive((char *) (s1.data() + n), s * sizeof(val4), i);
	}

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = s1.size();
		diplo::SRV.Send((char*) &s, sizeof(s), i);
		diplo::SRV.Send((char *) (s1.data()), s * sizeof(val4), i);
	}


	vector<val5> results;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val5), i);
	}

	for (vector<val5>::iterator i = results.begin(); i!=results.end(); i++) {
		diplo::KM.Get( i->v1 );
		diplo::KM.Get( i->v2 );
		diplo::KM.Get( i->v3 );
		diplo::KM.Get( i->v4 );
		diplo::KM.Get( i->v5 );

//		cerr << diplo::KM.Get(  i->v1  ) << "\t" << diplo::KM.Get(  i->v2  )<< "\t" << diplo::KM.Get( i->v3 ) << "\t"  << diplo::KM.Get(  i->v4  )<< "\t" << diplo::KM.Get( i->v5 ) << endl;
	}
	PRINT_NO_RESULTS
}

void LUBM_network::q16s2_n(char* data) {
	/*
	 * select ?a ?b ?c ?d ?e where {
	 ?a :emailAddress 'UndergraduateStudent0@Department0.University0.edu' .
	 ?a ?b ?c .
	 ?c ?d ?e .
	 }
	 */

	/*
	 * get molecules where 'UndergraduateStudent0@Department0.University0.edu' is as an object
	 *
	 * for each check if it is [emailAddress]
	 * 	if yes get all from this molecule
	 * 		for each element try to get it's molecule and all predicates for this one
	 * 	if cannot prepare molecule to sent to the Mastes
	 */

//	KEY_ID v = diplo::KM.Get("\"UndergraduateStudent0@Department0.University0.edu\"");

	KEY_ID v = *((KEY_ID *) data);
	data = data + sizeof(v);

	set<pair<const KEY_ID, Molecule>*>* Molecules_with_v;
	Molecules_with_v = diplo::M.GetMoleculesFromInvertedIdx(v);

	val5 x;
	vector<val5> ready;

	if (Molecules_with_v != NULL) {
		for (set<pair<const KEY_ID, Molecule>*>::iterator i = Molecules_with_v->begin(); i != Molecules_with_v->end(); i++) {
			unordered_multimap<size_t, KEY_ID> mol;
			(*i)->second.GetAllEntities(0, &mol,0);
			x.v1 = (*i)->first;
			for (unordered_multimap<size_t, KEY_ID>::iterator j = mol.begin(); j != mol.end(); j++) {
				x.v2 = diplo::TM.GetPredicate(diplo::KM.GetType((*i)->first), j->first, 0);
				x.v3 = j->second;
				unordered_multimap<size_t, KEY_ID> mol2;
				(*i)->second.GetAllEntities(1, &mol2, j->second);
				for (unordered_multimap<size_t, KEY_ID>::iterator u = mol2.begin(); u != mol2.end(); u++) {
					x.v4 = diplo::TM.GetPredicate(diplo::KM.GetType( (*i)->first ), u->first, 1);
					x.v5 = u->second;
					ready.push_back(x);
				}
			}

		}
	}

	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(val5));
}

void LUBM_network::q16s2_m() {
	string q = "q16s2";
	KEY_ID v = diplo::KM.Get("\"UndergraduateStudent0@Department0.University0.edu\"");

	size_t s = q.size() + sizeof(v);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((KEY_ID *) data_tmp) = v;
	data_tmp = data_tmp + sizeof(v);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<val5> results;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val5), i);
	}

	for (vector<val5>::iterator i = results.begin(); i!=results.end(); i++) {
		diplo::KM.Get( i->v1 );
		diplo::KM.Get( i->v2 );
		diplo::KM.Get( i->v3 );
		diplo::KM.Get( i->v4 );
		diplo::KM.Get( i->v5 );

//		cerr << diplo::KM.Get(  i->v1  ) << "\t" << diplo::KM.Get(  i->v2  )<< "\t" << diplo::KM.Get( i->v3 ) << "\t"  << diplo::KM.Get(  i->v4  )<< "\t" << diplo::KM.Get( i->v5 ) << endl;
	}
	PRINT_NO_RESULTS
}

void LUBM_network::q17_n(char* data) {
	/*
	 select * where
	 {
	 ?x a :GraduateStudent .
	 ?x :name ?y .
	 }
	 */

	TYPE_ID GraduateStudent = *((TYPE_ID *) data);
	data = data + sizeof(GraduateStudent);
	KEY_ID p1 = *((KEY_ID *) data);

	pair<KEY_ID, KEY_ID> x;
	vector<pair<KEY_ID, KEY_ID>> ready;

	set<TYPE_ID> typesGraduateStudent = diplo::TH.GetCompositesForTypes(GraduateStudent);

	vector<unordered_set<KEY_ID>*> elsUndergraduateStudent;
	for (set<TYPE_ID>::iterator it2 = typesGraduateStudent.begin(); it2 != typesGraduateStudent.end(); it2++) {
		unordered_set<KEY_ID>* el = diplo::TypesM.GetElementsOfType(*it2);

		unordered_set<size_t> tripleTemplates;
		unordered_set<KEY_ID> property_v;
		size_t tripleTempl = diplo::TM.GetTripleTemplate(*it2, *it2, p1, 0, 0);
		tripleTemplates.insert(tripleTempl);

		for (unordered_set<KEY_ID>::iterator i = el->begin(); i != el->end(); i++) {
			Molecule *m = diplo::M.Get(*i);
			if (m) {
				m->GetEntity(&tripleTemplates, *i, 0, &property_v);
				x.second = *(property_v.begin());
				x.first = *i;
				ready.push_back(x);
			}
		}
	}

	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

}

void LUBM_network::q17_m() {
	string q = "q17";
	TYPE_ID GraduateStudent = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#GraduateStudent>");
	KEY_ID p1 = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#name>");

	size_t s = q.size() + sizeof(GraduateStudent) + sizeof(p1);

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((TYPE_ID *) data_tmp) = GraduateStudent;
	data_tmp = data_tmp + sizeof(GraduateStudent);
	*((KEY_ID *) data_tmp) = p1;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<pair<KEY_ID, KEY_ID>> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

	}

	for (vector<pair<KEY_ID, KEY_ID>>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->first);
		diplo::KM.Get(j->second);
//		cout << diplo::KM.Get( j->first ) << "\t" << diplo::KM.Get( j->second ) << "\t" << endl;
	}
	PRINT_NO_RESULTS
}

LUBM_network::~LUBM_network() {
	// TODO Auto-generated destructor stub
}

} /* namespace queries */
