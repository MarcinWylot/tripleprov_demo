/*
 * LUBM.cpp
 *
 *  Created on: 19-11-2012
 *      Author: marcin
 */

#include "LUBM.h"

namespace queries {

LUBM::LUBM() {
	// TODO Auto-generated constructor stub

}

LUBM::~LUBM() {
	// TODO Auto-generated destructor stub
}


void LUBM::q01() {
	/*  SELECT ?X
	 *  ?X a :GraduateStudent .
     *	?X :takesCourse <http://www.Department0.University0.edu/GraduateCourse0> . <<<< ------- THE CONSTRAIN
	 */


	//THE ALGORITHM
	//find all molecules where GraduateCourse0 is an OBJECT
	//check if it is of type GraduateStudent (don't forget about COMPOSITE TYPES)
	//check each found molecule for THE CONSTRAIN

	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu/GraduateCourse0>");
	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");


	TYPE_ID t = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#GraduateStudent>");
	set<TYPE_ID> types = diplo::TH.GetCompositesForTypes(t);

	set< pair<const KEY_ID, Molecule>* >* Molecules_with_GraduateCourse0;
	Molecules_with_GraduateCourse0 = diplo::M.GetMoleculesFromInvertedIdx(o);
	set< pair<const KEY_ID, Molecule>* >::iterator it;
	for (set< pair<const KEY_ID, Molecule>* >::iterator it = Molecules_with_GraduateCourse0->begin(); it != Molecules_with_GraduateCourse0->end(); it++) {
		set<TYPE_ID>::iterator type = types.find( diplo::KM.GetType ((*it)->first) );
		if ( type != types.end() ) {
			size_t tripleTempl = diplo::TM.GetTripleTemplate( *type , *type, p, diplo::KM.GetType(o), 0);

			if ( (*it)->second.CheckIfEntityExists( tripleTempl, o, 0 ) )
				cout << diplo::KM.Get( (*it)->first ) << endl;
		}
	}

}

void LUBM::q02() {
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


	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");
	TYPE_ID University = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#University>");

	TYPE_ID GraduateStudent = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#GraduateStudent>"); //FIXME

	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");

	set<TYPE_ID> typesDepartment = diplo::TH.GetCompositesForTypes( Department );
	set<TYPE_ID> typesUniversity = diplo::TH.GetCompositesForTypes( University );

	set<TYPE_ID> typesGraduateStudent = diplo::TH.GetCompositesForTypes( GraduateStudent );


	vector< map<KEY_ID, class Node *> * > elsUniversity;
	for (set<TYPE_ID>::iterator it2 = typesUniversity.begin(); it2 != typesUniversity.end(); it2++) {
		map<KEY_ID, class Node *> * el;
		el = diplo::KM.GetElementsOfType(*it2); // get all Universities
		elsUniversity.push_back( el );
	}

//first Part

	typedef pair< KEY_ID, set<pair<const KEY_ID, Molecule>*>* > T_ID_Molecules_with;
	typedef multimap<T_ID_Molecules_with, T_ID_Molecules_with> T_molecules_uni_dep;
	T_molecules_uni_dep molecules_uni_dep;
	set<pair<const KEY_ID, Molecule>*>* Molecules_with_uni;
	set<pair<const KEY_ID, Molecule>*>* Molecules_with_dep;

	T_ID_Molecules_with ID_Molecules_with_uni;
	T_ID_Molecules_with ID_Molecules_with_dep;

	for (vector<map<KEY_ID, class Node *> *>::iterator uni_type = elsUniversity.begin(); uni_type != elsUniversity.end(); uni_type++) {
		for (map<KEY_ID, class Node *>::iterator uni = (*uni_type)->begin(); uni != (*uni_type)->end(); uni++) {
			Molecules_with_uni = diplo::M.GetMoleculesFromInvertedIdx(uni->first);
			set<pair<const KEY_ID, Molecule>*>::iterator it;
			for (set<pair<const KEY_ID, Molecule>*>::iterator it = Molecules_with_uni->begin(); it != Molecules_with_uni->end(); it++) {
				set<TYPE_ID>::iterator type = typesDepartment.find(diplo::KM.GetType((*it)->first));
				if (type != typesDepartment.end()) {
					size_t tripleTempl = diplo::TM.GetTripleTemplate(*type, *type, subOrganizationOf, diplo::KM.GetType(uni->first), 0);
					if ((*it)->second.CheckIfEntityExists(tripleTempl, uni->first, 0)) {

//						cout << diplo::KM.Get((*it)->first) << "\t"<< diplo::KM.Get(uni->first) << endl;
						Molecules_with_dep = diplo::M.GetMoleculesFromInvertedIdx((*it)->first);
						ID_Molecules_with_uni.first = uni->first;
						ID_Molecules_with_uni.second = Molecules_with_uni;

						ID_Molecules_with_dep.first = (*it)->first;
						ID_Molecules_with_dep.second = Molecules_with_dep;

						molecules_uni_dep.insert( pair<T_ID_Molecules_with, T_ID_Molecules_with>(ID_Molecules_with_uni, ID_Molecules_with_dep));
					}
				}
			}
		}
	}


//second part
	KEY_ID undergraduateDegreeFrom = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#undergraduateDegreeFrom>");
	KEY_ID memberOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");


	KEY_ID uni = 0;
	for (T_molecules_uni_dep::iterator it = molecules_uni_dep.begin(); it != molecules_uni_dep.end(); it++ ) {
		if (uni != it->first.first) {
			uni = it->first.first;
			Molecules_with_uni = it->first.second;
		}
		Molecules_with_dep =  it->second.second;
		for (set<pair<const KEY_ID, Molecule>*>::iterator it_mol_dep = Molecules_with_dep->begin() ; it_mol_dep != Molecules_with_dep->end(); it_mol_dep++ ) {
			if (Molecules_with_uni->find(*it_mol_dep) != Molecules_with_uni->end() ) {
				set<TYPE_ID>::iterator type = typesGraduateStudent.find(diplo::KM.GetType((*it_mol_dep)->first));
				if (type != typesGraduateStudent.end()) {
					size_t tripleTempl_undergraduateDegreeFrom = diplo::TM.GetTripleTemplate(*type, *type, undergraduateDegreeFrom, diplo::KM.GetType(it->first.first), 0);
					if (( *it_mol_dep)->second.CheckIfEntityExists(tripleTempl_undergraduateDegreeFrom, it->first.first, 0)) { ///<----- SECOND CONSTRAIN
						size_t tripleTempl_memberOf = diplo::TM.GetTripleTemplate(*type, *type, memberOf, diplo::KM.GetType(it->second.first), 0);
						if (( *it_mol_dep)->second.CheckIfEntityExists(tripleTempl_memberOf, it->second.first, 0)) { ///<----- THIRD CONSTRAIN
							cout << diplo::KM.Get( it->first.first ) << "\t"<< diplo::KM.Get( it->second.first ) << "\t"<< diplo::KM.Get( (*it_mol_dep)->first ) << endl;
						}
					}
				}
			}
		}

	}


}
//
//void LUBM::q03() {
//	/*
//	 * SELECT ?X
//	 * ?X a :Publication .
//     * ?X :publicationAuthor <http://www.Department0.University0.edu/AssistantProfessor0> .   <---- THE CONSTRAIN
//	 *
//	 */
//
//	//THE ALGORITM
//	/*
//	 * get molecules where AssistantProfessor0 is an object
//	 * check if a molecule is of type Publication
//	 * 	if YES then check THE CONSTRAIN
//	 * 		if correct then we want it
//	 */
//	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu/AssistantProfessor0>");
//	pair < multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator, multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator > it_inv_molecules;
//	it_inv_molecules = diplo::M.inv_molecules.equal_range(o);
//
//	TYPE_ID Publication = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Publication>");
//	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#publicationAuthor>");
//
//	size_t tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType( Publication ), Publication, p, diplo::KM.GetType(o), 0);
//
//	for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it = it_inv_molecules.first; it != it_inv_molecules.second; it++) {
//		if ( diplo::KM.GetType( it->second->first ) == Publication and
//				it->second->second.CheckIfEntityExists(tripleTempl, it->first, 0) ) {
//			cout << diplo::KM.Get( it->second->first ) << endl;
//		}
//	}
//
//
//
//
//}
//
//void LUBM::q04() {
//	/*
//	SELECT ?X ?Y1 ?Y2 ?Y3
//	    ?t rdfs:subClassOf :Professor .
//	    ?X a ?t .
//	    ?X :worksFor <http://www.Department0.University0.edu> .
//
//	    ?X :name ?Y1 .
//	    ?X :emailAddress ?Y2 .
//	    ?X :telephone ?Y3 .
//	 */
//	set<TYPE_ID> ProfessorAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Professor>") );
//
////	cerr << ProfessorAndSubClasses.size() << endl;
//
//	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu>");
//	pair < multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator, multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator > it_inv_molecules;
//	it_inv_molecules = diplo::M.inv_molecules.equal_range(o);
//
//	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#worksFor>");
//	map<TYPE_ID,size_t> ProfessorAndSubClassesTmpl;
//
//	KEY_ID name = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#name>");
//	map<TYPE_ID,size_t> ProfessorAndSubClassesTmpl_name;
//
//	KEY_ID emailAddress = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#emailAddress>");
//	map<TYPE_ID,size_t> ProfessorAndSubClassesTmpl_emailAddress;
//
//	KEY_ID telephone = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#telephone>");
//	map<TYPE_ID,size_t> ProfessorAndSubClassesTmpl_telephone;
//
//	for ( set<TYPE_ID>::iterator it = ProfessorAndSubClasses.begin(); it != ProfessorAndSubClasses.end(); it++) {
////		cerr << "----> " << *it << "\t" << diplo::TM.GetTemplateForType( *it ) << endl;
//		if (diplo::TM.GetTemplateForType( *it ) == (size_t)-1 ) continue;
//
//		size_t tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType( *it ), *it, p, diplo::KM.GetType(o), 0);
//		ProfessorAndSubClassesTmpl.insert( pair<TYPE_ID,size_t>(*it,tripleTempl) );
//
//		tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType( *it ), *it, name, 2, 0); //literars are now "unclasifies", that's why Object Type is 2
////		cerr << "1: " << tripleTempl << endl;
//		ProfessorAndSubClassesTmpl_name.insert( pair<TYPE_ID,size_t>(*it,tripleTempl) );
//
//		tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType( *it ), *it, emailAddress, 2, 0); //literars are now "unclasifies", that's why Object Type is 2
////		cerr << "2: " << tripleTempl << endl;
//		ProfessorAndSubClassesTmpl_emailAddress.insert( pair<TYPE_ID,size_t>(*it,tripleTempl) );
//
//		tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType( *it ), *it, telephone, 2, 0); //literars are now "unclasifies", that's why Object Type is 2
////		cerr << "3: " << tripleTempl << endl;
//		ProfessorAndSubClassesTmpl_telephone.insert( pair<TYPE_ID,size_t>(*it,tripleTempl) );
//
//	}
//
//
//
//
//
//	set<KEY_ID> names;
//	set<KEY_ID> emailAddresss;
//	set<KEY_ID> telephones;
//	for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it = it_inv_molecules.first; it != it_inv_molecules.second; it++) {
//		map<TYPE_ID,size_t>::iterator it2 = ProfessorAndSubClassesTmpl.find( diplo::KM.GetType( it->second->first ) );
//
//		map<TYPE_ID,size_t>::iterator it_name = ProfessorAndSubClassesTmpl_name.find( diplo::KM.GetType( it->second->first ) );
//		map<TYPE_ID,size_t>::iterator it_emailAddress = ProfessorAndSubClassesTmpl_emailAddress.find( diplo::KM.GetType( it->second->first ) );
//		map<TYPE_ID,size_t>::iterator it_telephone = ProfessorAndSubClassesTmpl_telephone.find( diplo::KM.GetType( it->second->first ) );
////
//
//		if (it2 != ProfessorAndSubClassesTmpl.end() )
//			if ( it->second->second.CheckIfEntityExists(it2->second, it->first, 0) ) {
//				cout << diplo::KM.Get( it->first ) << "\t" << diplo::KM.Get( it->second->first );
//
//				it->second->second.GetObjects(it_name->second, &names);
//				cout << "\t" << diplo::KM.Get( *(names.begin()) );
//
//				it->second->second.GetObjects(it_emailAddress->second, &emailAddresss);
//				cout << "\t" << diplo::KM.Get( *(emailAddresss.begin()) );
//
//				it->second->second.GetObjects(it_telephone->second, &telephones);
//				cout << "\t" << diplo::KM.Get( *(telephones.begin()) );
//
//				cout << endl;
//			}
//
//
//	}
//
//}
//
//void LUBM::q05() {
//	/*
//	 * SELECT ?X
//		?t rdfs:subClassOf :Person .
//    	?X a ?t .
//    	?X :memberOf <http://www.Department0.University0.edu> .
//	 *
//	 */
//
//	set<TYPE_ID> PersonAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Person>") );
//	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu>");
//	pair < multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator, multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator > it_inv_molecules;
//	it_inv_molecules = diplo::M.inv_molecules.equal_range(o);
//
//
//	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");
//	map<TYPE_ID,size_t> PersonAndSubClassesTmpl;
//
//
//	for ( set<TYPE_ID>::iterator it = PersonAndSubClasses.begin(); it != PersonAndSubClasses.end(); it++) {
//		if (diplo::TM.GetTemplateForType( *it ) == (size_t)-1 ) continue;
//
//		size_t tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType( *it ), *it, p, diplo::KM.GetType(o), 0);
//		PersonAndSubClassesTmpl.insert( pair<TYPE_ID,size_t>(*it,tripleTempl) );
//	}
//
//
//	for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it = it_inv_molecules.first; it != it_inv_molecules.second; it++) {
//			map<TYPE_ID,size_t>::iterator it2 = PersonAndSubClassesTmpl.find( diplo::KM.GetType( it->second->first ) );
//
//			if (it2 != PersonAndSubClassesTmpl.end() ) {
//				if ( it->second->second.CheckIfEntityExists(it2->second, it->first, 0) ) {
//					cout << diplo::KM.Get( it->second->first ) << endl;
//				}
//			}
//	}
//
//}
//
//void LUBM::q06() {
//
//	/*
//	 * SELECT ?X
//	 {
//	 	 { ?t rdfs:subClassOf :Student .
//	 	 ?X a ?t .
//	 }
//	 UNION
//	 {
//	 	 ?X a :Student .
//	 }
//	 *
//	 */
//
//	set<TYPE_ID> StudentAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>") );
//
//	for (set<TYPE_ID>::iterator it = StudentAndSubClasses.begin(); it != StudentAndSubClasses.end(); it++) {
//		if (diplo::TM.GetTemplateForType(*it) == (size_t) -1)
//			continue;
//
//		map<KEY_ID, class Node *> * el;
//		el = diplo::KM.GetElementsOfType(*it); // get list of molecules where triple can be in contexst of the above template
//
//		for (map<KEY_ID, class Node *>::iterator it2 = el->begin(); it2 != el->end(); it2++) {
//			cout << diplo::KM.Get( it2->first ) << endl;
//		}
//	}
//}
//
//void LUBM::q07() {
//	/*
//	 * SELECT ?X ?Y
//	 *
//	{
//	 	 ?t2 rdfs:subClassOf :Course .
//	 	 ?Y a ?t2 .
//	 	 }
//	 	 UNION
//	 	 {
//	 	 ?Y a :Course .
//	 	 }
//	 <http://www.Department0.University0.edu/AssociateProfessor0> :teacherOf ?Y .
//	 {
//	 	 ?t rdfs:subClassOf :Student .
//	 	 ?X a ?t .
//	 }
//	  UNION
//	 {
//	 	 ?X a :Student .
//	 }
//
//	 ?X :takesCourse ?Y . <----- THE CONSTRAIN
//
//	 *
//	 */
//
//	//THR ALGORITHM
//	/*
//	 * get all courses given by AssociateProfessor0 (from the molecule)
//	 * for each course
//	 * 	get list of molecules where each course apears as an OBJECT
//	 * 		check if molecule is of type STUDENT (or related)
//	 * 		if yes, check THE CONSTRAIN
//	 */
//
//
//	set<TYPE_ID> CourseAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Course>") );
//	set<TYPE_ID> StudentAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>") );
//
//	KEY_ID teacherOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#teacherOf>");
//	KEY_ID takesCourse = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");
//
//
//	KEY_ID AssociateProfessor0 = diplo::KM.Get("<http://www.Department0.University0.edu/AssociateProfessor0>");
//	size_t tmpl = diplo::TM.GetTemplateForType( diplo::KM.GetType(AssociateProfessor0) );
//
//	vector<size_t > CourseAndSubClasses_tripleTempl_teacherOf ;
//	for (set<TYPE_ID>::iterator it = CourseAndSubClasses.begin(); it != CourseAndSubClasses.end(); it++) {
//		size_t tripleTempl = diplo::TM.GetTripleTemplate(tmpl, diplo::KM.GetType(AssociateProfessor0), teacherOf, *it, 0);
//		CourseAndSubClasses_tripleTempl_teacherOf.push_back ( tripleTempl );
//	}
//
//	map<TYPE_ID,size_t> CourseAndSubClasses_tripleTempl_takesCourse;
//	for (set<TYPE_ID>::iterator it = CourseAndSubClasses.begin(); it != CourseAndSubClasses.end(); it++) {
//		for (set<TYPE_ID>::iterator it2 = StudentAndSubClasses.begin(); it2 != StudentAndSubClasses.end(); it2++) {
//			if (diplo::TM.GetTemplateForType( *it2 ) == (size_t)-1 ) continue;
//			size_t tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType( *it2 ), *it2, takesCourse, *it, 0);
//			if ( tripleTempl == (size_t)-1 ) continue;
//			CourseAndSubClasses_tripleTempl_takesCourse.insert( pair<TYPE_ID,size_t>(*it,tripleTempl) );
//		}
//	}
//
//
//	set<KEY_ID> coursesGivenBy;
//	Molecule* AssociateProfessor0_Molecule = diplo::M.Get(AssociateProfessor0);
//	if ( AssociateProfessor0_Molecule ) {
//		for (vector<size_t >:: iterator it = CourseAndSubClasses_tripleTempl_teacherOf.begin(); it != CourseAndSubClasses_tripleTempl_teacherOf.end(); it++) {
//			AssociateProfessor0_Molecule->GetObjects(*it, &coursesGivenBy);
//		}
//	}
//
//
//	pair < multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator, multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator > it_inv_molecules;
//
//	for(set<KEY_ID>::iterator it = coursesGivenBy.begin(); it != coursesGivenBy.end(); it++ ) {
//
//		map<TYPE_ID, size_t >::iterator j = CourseAndSubClasses_tripleTempl_takesCourse.find(diplo::KM.GetType( *it ));
//		if ( j != CourseAndSubClasses_tripleTempl_takesCourse.end() ) {
//			it_inv_molecules = diplo::M.inv_molecules.equal_range(*it);
//			for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it2 = it_inv_molecules.first; it2 != it_inv_molecules.second; it2++) {
//				set<TYPE_ID>::iterator i = StudentAndSubClasses.find( diplo::KM.GetType(it2->second->first) );
//				if ( i != StudentAndSubClasses.end() ) {
//						if ( it2->second->second.CheckIfEntityExists(j->second, *it, 0) ) {
//							cout << diplo::KM.Get( it2->second->first ) << "\t" << diplo::KM.Get( *it )<< endl;
//						}
//
//
//
//				}
//			}
//		}
//	}
//
//
//}
//
//void LUBM::q08() {
//
//	/*
//	 * SELECT ?X ?Y ?Z
//  	 {
//       {
//		?t rdfs:subClassOf :Student .
//		?X a ?t .
//	 }
//	UNION
//	{
//		?X a :Student .
//	}
//
//    	?Y :subOrganizationOf <http://www.University0.edu> .
//    	?Y a :Department .
//
//    	?X :memberOf ?Y .
//
//
//    	?X :emailAddress ?Z .
//  	  }
//	 *
//	 */
//
//	//THE ALGORITHM
//	/*
//	 * get all molecyles where <http://www.University0.edu> is as an object
//	 * 	check each of above elements if it is Department
//	 * 	check is the relation is subOrganizationOf
//	 *
//	 * for each Dep get molecules where it apears as an object
//	 * 	check if each of those elements is of tyle Student
//	 * 	check if the relation between Dep and the element is memberOf
//	 * 		if YES, get e-mail adres from the molecule
//	 */
//
//	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");
//	pair < multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator, multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator > it_inv_molecules;
//	it_inv_molecules = diplo::M.inv_molecules.equal_range( University0 );
//
//	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
//
//	vector <KEY_ID> departements;
//	set<TYPE_ID> DepartmentAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>") );
//	map<TYPE_ID, size_t> DepartmentAndSubClassesTmpl;
//	for (set<TYPE_ID>::iterator it = DepartmentAndSubClasses.begin(); it != DepartmentAndSubClasses.end(); it++) {
//		if (diplo::TM.GetTemplateForType(*it) == (size_t) -1)
//			continue;
//
//		size_t tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType(*it), *it, subOrganizationOf, diplo::KM.GetType( University0 ), 0);
//		DepartmentAndSubClassesTmpl.insert(pair<TYPE_ID, size_t> (*it, tripleTempl));
//	}
//
//
//
//	for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it = it_inv_molecules.first; it != it_inv_molecules.second; it++) {
//		map<TYPE_ID, size_t>::iterator it2 = DepartmentAndSubClassesTmpl.find(diplo::KM.GetType(it->second->first));
//
//		if (it2 != DepartmentAndSubClassesTmpl.end()) {
//			if (it->second->second.CheckIfEntityExists(it2->second, it->first, 0)) {
//				departements.push_back( it->second->first ); //// <<<<< here we have departements!
//			}
//		}
//	}
//
//	//get all students
//	set<TYPE_ID> StudentAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>") );
//	map<TYPE_ID, size_t> StudentAndSubClassesTmpl;
//	map<TYPE_ID, size_t> StudentAndSubClassesTmpl_emailAddress;
//	KEY_ID memberOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>");
//	KEY_ID emailAddress = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#emailAddress>");
//	for (set<TYPE_ID>::iterator it = StudentAndSubClasses.begin(); it != StudentAndSubClasses.end(); it++) {
//		if (diplo::TM.GetTemplateForType(*it) == (size_t) -1)
//			continue;
//
//		for (set<TYPE_ID>::iterator i = DepartmentAndSubClasses.begin(); i != DepartmentAndSubClasses.end(); i++) {
//			size_t tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType(*it), *it, memberOf, *i, 0);
//			StudentAndSubClassesTmpl.insert(pair<TYPE_ID, size_t> (*it, tripleTempl));
//
//			tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType( *it ), *it, emailAddress, 2, 0); //literars are now "unclasifies", that's why Object Type is 2
//			StudentAndSubClassesTmpl_emailAddress.insert( pair<TYPE_ID,size_t>(*it,tripleTempl) );
//
//		}
//	}
//
//
//	map < pair<const KEY_ID, Molecule>*, KEY_ID > studentsDeps;
//	for ( vector <KEY_ID>::iterator i = departements.begin(); i != departements.end(); i++ ) {
//		it_inv_molecules = diplo::M.inv_molecules.equal_range( *i ); //get all molecules where Department is an abject
//		for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it = it_inv_molecules.first; it != it_inv_molecules.second; it++) {
//
//			map<TYPE_ID, size_t>::iterator it2 = StudentAndSubClassesTmpl.find( diplo::KM.GetType(it->second->first) );
//			if (it2 != StudentAndSubClassesTmpl.end()) { //and check is it is of type Student
//				if (it->second->second.CheckIfEntityExists(it2->second, it->first, 0)) {
//					studentsDeps.insert( pair < pair<const KEY_ID, Molecule>*, KEY_ID > (it->second, *i ) ); //make output UNIQ
//				}
//			}
//
//		}
//	}
//
//
//	map<TYPE_ID,size_t>::iterator it_emailAddress;
//	for ( map < pair<const KEY_ID, Molecule>*, KEY_ID >::iterator i = studentsDeps.begin(); i != studentsDeps.end(); i++ ) {
//
//		it_emailAddress = StudentAndSubClassesTmpl_emailAddress.find( diplo::KM.GetType( i->first->first ) );
//		set<KEY_ID> emailAddresss;
//		i->first->second.GetObjects(it_emailAddress->second, &emailAddresss);
//
//		cout << diplo::KM.Get ( i->second ) << "\t" << diplo::KM.Get ( i->first->first ) << "\t" << diplo::KM.Get( *(emailAddresss.begin()) ) << endl;
//	}
//}
//
//void LUBM::q09() {
//	/*
//	 * SELECT ?X ?Y ?Z
//  {
//	{
//		?t rdfs:subClassOf :Student .
//		?X a ?t .
//	}
//	UNION
//	{
//		?X a :Student .
//	}
//
//	?t3 rdfs:subClassOf :Faculty .
//	{?Y a ?t3.}
//	UNION
//	{?t4 rdfs:subClassOf ?t3.
//	?Y a ?t4.}
//
//
//	{
//		?t2 rdfs:subClassOf :Course .
//		?Z a ?t2 .
//	}
//	UNION
//	{
//		?Z a :Course .
//	}
//
// ?X :takesCourse ?Z .
//?Y :teacherOf ?Z .
//
//  		?X :advisor ?Y . <<<< CONSTR
//
//
//
//  }
//	 *
//	 */
//
//	KEY_ID advisor = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#advisor>");
//	KEY_ID takesCourse = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");
//	KEY_ID teacherOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#teacherOf>");
//
//	set<TYPE_ID> CourseAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Course>") );
//
//	set<TYPE_ID> StudentAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Student>") );
//	map<TYPE_ID, size_t> StudentCourseTmpl;
//	for (set<TYPE_ID>::iterator it = StudentAndSubClasses.begin(); it != StudentAndSubClasses.end(); it++) {
//		if (diplo::TM.GetTemplateForType(*it) == (size_t) -1)
//			continue;
//
//		for (set<TYPE_ID>::iterator course = CourseAndSubClasses.begin(); course != CourseAndSubClasses.end(); course++) {
//			size_t tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType(*it), *it, takesCourse, *course, 0);
//			if ( tripleTempl  == (size_t) -1) continue;
//			StudentCourseTmpl.insert(pair<TYPE_ID, size_t> (*it, tripleTempl));
//		}
//
//	}
//
//	set<TYPE_ID> FacultyAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Faculty>") );
//	multimap<TYPE_ID, size_t> FacultyCourseTmpl;
//	for (set<TYPE_ID>::iterator it = FacultyAndSubClasses.begin(); it != FacultyAndSubClasses.end(); it++) {
//		if (diplo::TM.GetTemplateForType(*it) == (size_t) -1)
//			continue;
//		for (set<TYPE_ID>::iterator course = CourseAndSubClasses.begin(); course != CourseAndSubClasses.end(); course++) {
//			size_t tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType(*it), *it, teacherOf, *course, 0);
//			if ( tripleTempl  == (size_t) -1) continue;
//			FacultyCourseTmpl.insert(pair<TYPE_ID, size_t> (*it, tripleTempl));
//		}
//	}
//
//
//	map< pair<TYPE_ID, TYPE_ID> , size_t> FacultyStudentTmpl;
//	for (set<TYPE_ID>::iterator it = FacultyAndSubClasses.begin(); it != FacultyAndSubClasses.end(); it++) {
//		if (diplo::TM.GetTemplateForType(*it) == (size_t) -1)
//			continue;
//		for (set<TYPE_ID>::iterator student = StudentAndSubClasses.begin(); student != StudentAndSubClasses.end(); student++) {
//			if (diplo::TM.GetTemplateForType(*student) == (size_t) -1)
//				continue;
//			size_t tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType(*student), *student, advisor, *it, 0);
//			if (tripleTempl == (size_t) -1) continue;
//			FacultyStudentTmpl.insert( pair < pair<TYPE_ID, TYPE_ID> , size_t >( pair<TYPE_ID, TYPE_ID>(*it,*student) , tripleTempl));
//		}
//	}
//	multimap<KEY_ID, pair<const KEY_ID, Molecule>* > StudentCourse;
//	multimap<KEY_ID, pair<const KEY_ID, Molecule>* > FacultyCourse;
//
//	pair < multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator, multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator > it_inv_molecules_classes;
//	for (set<TYPE_ID>::iterator it = CourseAndSubClasses.begin(); it != CourseAndSubClasses.end(); it++) {
//		map<KEY_ID, class Node *> * elems = diplo::KM.GetElementsOfType(*it);
//		for (map<KEY_ID, class Node *>::iterator i = elems->begin(); i != elems->end(); i++) { //<<<< CLASSES
//			it_inv_molecules_classes = diplo::M.inv_molecules.equal_range( i->first ); //get all molecules where Class is an abject
//
//			for (multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator it_inv_class = it_inv_molecules_classes.first; it_inv_class != it_inv_molecules_classes.second; it_inv_class++) {
//				TYPE_ID it_inv_class_type = diplo::KM.GetType( it_inv_class->second->first );
//				map<TYPE_ID, size_t>::iterator it_Tmpl;
//
//
//				if (  (it_Tmpl = StudentCourseTmpl.find( it_inv_class_type ) ) != StudentCourseTmpl.end() //<<<<<this is STUDENT
//						and it_inv_class->second->second.CheckIfEntityExists(it_Tmpl->second, i->first, 0)
//						) {
//					StudentCourse.insert( pair<KEY_ID, pair<const KEY_ID, Molecule>* > (i->first, it_inv_class->second)  );
//
//				}
//				else if ( FacultyCourseTmpl.find( it_inv_class_type ) != FacultyCourseTmpl.end() ) {//<<this is Faculty
//					pair<multimap<TYPE_ID, size_t>::iterator, multimap<TYPE_ID, size_t>::iterator> p_FacultyCourseTmpl = FacultyCourseTmpl.equal_range( it_inv_class_type ) ;
//
//					for (multimap<TYPE_ID, size_t>::iterator j = p_FacultyCourseTmpl.first; j != p_FacultyCourseTmpl.second; j++) {
//						if( it_inv_class->second->second.CheckIfEntityExists(j->second, i->first, 0) )
//							FacultyCourse.insert( pair<KEY_ID, pair<const KEY_ID, Molecule>* > (i->first, it_inv_class->second)  );
//					}
//				}
//
//			}
//
//		}
//	}
//
//
//	map< pair<TYPE_ID, TYPE_ID> , size_t>::iterator i_FacultyStudentTmpl;
//	pair < multimap<KEY_ID, pair<const KEY_ID, Molecule>* >::iterator, multimap<KEY_ID, pair<const KEY_ID, Molecule>* >::iterator > it_StudentCourse;
//	for (multimap<KEY_ID, pair<const KEY_ID, Molecule>* >::iterator i = FacultyCourse.begin(); i != FacultyCourse.end(); i++ ) {
//		it_StudentCourse = StudentCourse.equal_range(i->first);
//
//		for (multimap<KEY_ID, pair<const KEY_ID, Molecule>* >::iterator i2 = it_StudentCourse.first; i2 != it_StudentCourse.second; i2++ ) {
//
//			i_FacultyStudentTmpl = FacultyStudentTmpl.find( pair<TYPE_ID, TYPE_ID>( diplo::KM.GetType(i->second->first), diplo::KM.GetType(i2->second->first)  ) );
//			if ( i_FacultyStudentTmpl != FacultyStudentTmpl.end() and
//				i2->second->second.CheckIfEntityExists(i_FacultyStudentTmpl->second, i->second->first, 0) ) { //check CONSTR3
//					cout << diplo::KM.Get( i2->second->first ) << "\t" << diplo::KM.Get( i->second->first ) << "\t" << diplo::KM.Get( i->first ) << endl; /// <<MAKE IT UNIQ
//			}
//		}
//	}
//
//}
//
//void LUBM::q10() {
//
//	/*
//	 * SELECT ?X
//		?t rdfs:subClassOf :Person .
//		?X a ?t .
//    	?X :takesCourse <http://www.Department0.University0.edu/GraduateCourse0> .
//	 *
//	 */
//
//
//	set<TYPE_ID> PersonAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Person>") );
//	KEY_ID o = diplo::KM.Get("<http://www.Department0.University0.edu/GraduateCourse0>");
//	pair < multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator, multimap <KEY_ID, pair<const KEY_ID, Molecule>* >::iterator > it_inv_molecules;
//	it_inv_molecules = diplo::M.inv_molecules.equal_range(o);
//
//
//	KEY_ID p = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>");
//	map<TYPE_ID,size_t> PersonAndSubClassesTmpl;
//
//
//	for ( set<TYPE_ID>::iterator it = PersonAndSubClasses.begin(); it != PersonAndSubClasses.end(); it++) {
//		if (diplo::TM.GetTemplateForType( *it ) == (size_t)-1 ) continue;
//
//		size_t tripleTempl = diplo::TM.GetTripleTemplate(diplo::TM.GetTemplateForType( *it ), *it, p, diplo::KM.GetType(o), 0);
//		PersonAndSubClassesTmpl.insert( pair<TYPE_ID,size_t>(*it,tripleTempl) );
//	}
//
//
//	for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it = it_inv_molecules.first; it != it_inv_molecules.second; it++) {
//			map<TYPE_ID,size_t>::iterator it2 = PersonAndSubClassesTmpl.find( diplo::KM.GetType( it->second->first ) );
//
//			if (it2 != PersonAndSubClassesTmpl.end() ) {
//				if ( it->second->second.CheckIfEntityExists(it2->second, it->first, 0) ) {
//					cout << diplo::KM.Get( it->second->first ) << endl;
//				}
//			}
//	}
//
//}
//
//void LUBM::q11() {
///*
// * SELECT ?X
//	?X a :ResearchGroup .
//	?X :subOrganizationOf ?Y . <<< CONST2
//	?Y :subOrganizationOf <http://www.University0.edu> . <<< CONST1
// */
//
//	//THE ALGORITHM
//	/*
//	 * get molecules where <http://www.University0.edu> is an Object
//	 * 	for each of x check CONST1
//	 * 		if OK, get all molecules whete x is an Object
//	 * 		for each y check CONST2 and is it is of Type ResearchGroup
//	 */
//
//	set<TYPE_ID> ResearchGroupAndSubClasses = diplo::TH.GetHierarchyForType ( diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#ResearchGroup>") );
//
//	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");
//	pair<multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator, multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator> it_inv_molecules;
//	it_inv_molecules = diplo::M.inv_molecules.equal_range( University0 );
//	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
//
//	multimap<TYPE_ID, size_t> tripleTmpls;
//	diplo::TM.GetTemplates(-1, subOrganizationOf, diplo::KM.GetType( University0 ), 0, &tripleTmpls);
//
//
//	pair< multimap<size_t, size_t>::iterator, multimap<size_t, size_t>::iterator > tripleTmpls_range;
//
//	for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it = it_inv_molecules.first; it != it_inv_molecules.second; it++) {
//		size_t tmpl = diplo::TM.GetTemplateForType( diplo::KM.GetType( it->second->first) );
//		if (tmpl == (size_t)-1 ) continue;
//		tripleTmpls_range = tripleTmpls.equal_range(tmpl);
//		for ( multimap<size_t, size_t>::iterator it2 = tripleTmpls_range.first; it2 != tripleTmpls_range.second; it2++ ) {
//			if (it->second->second.CheckIfEntityExists(it2->second, University0, 0)) { //<<<<<CONST1
//
//
//				pair<multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator, multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator> const1_range;
//				const1_range = diplo::M.inv_molecules.equal_range( it->second->first );
//				multimap<TYPE_ID, size_t> tripleTmpls_const1;
//				diplo::TM.GetTemplates(-1, subOrganizationOf, diplo::KM.GetType( it->second->first ), 0, &tripleTmpls_const1);
//				pair< multimap<size_t, size_t>::iterator, multimap<size_t, size_t>::iterator > tripleTmpls_range_const1;
//
//				for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator const1_range_it = const1_range.first; const1_range_it != const1_range.second; const1_range_it++) {
//					size_t tmpl_const1 = diplo::TM.GetTemplateForType( diplo::KM.GetType( const1_range_it->second->first) );
//					if (tmpl_const1 == (size_t)-1 ) continue;
//					tripleTmpls_range_const1 = tripleTmpls_const1.equal_range(tmpl_const1);
//					for (multimap<size_t, size_t>::iterator it2_const1 = tripleTmpls_range_const1.first; it2_const1 != tripleTmpls_range_const1.second; it2_const1++) {
//						if (ResearchGroupAndSubClasses.find(diplo::KM.GetType(const1_range_it->second->first)) != ResearchGroupAndSubClasses.end()) {
//							if (const1_range_it->second->second.CheckIfEntityExists(it2_const1->second, it->second->first, 0)) { //<<<<<CONST2
//								cout << diplo::KM.Get(const1_range_it->second->first) << endl;
//							}
//						}
//					}
//				}
//
//
//			}
//		}
//
//	}
//}
//
//void LUBM::q12() {
//	/*
//	 * SELECT ?X ?Y
//	 ?Y :subOrganizationOf <http://www.University0.edu> .
//	 ?Y a :Department .
//
//	 ?X :worksFor ?Y .
//	 ?X :headOf ?Y .
//	 *
//	 */
//
//	TYPE_ID Department = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department>");
////	set<TYPE_ID> DepartmentGroupAndSubClasses = diplo::TH.GetHierarchyForType ( Department );
//
//	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");
//	pair<multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator, multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator> it_inv_molecules;
//	it_inv_molecules = diplo::M.inv_molecules.equal_range(University0);
//	KEY_ID subOrganizationOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>");
//
//	multimap<TYPE_ID, size_t> tripleTmpls;
//	diplo::TM.GetTemplates(Department, subOrganizationOf, diplo::KM.GetType( University0 ), 0, &tripleTmpls);
//	pair< multimap<size_t, size_t>::iterator, multimap<size_t, size_t>::iterator > tripleTmpls_range;
//	pair< multimap<size_t, size_t>::iterator, multimap<size_t, size_t>::iterator > tripleTmpls_worksFor_range;
//	pair< multimap<size_t, size_t>::iterator, multimap<size_t, size_t>::iterator > tripleTmpls_headOf_range;
//
//
//	multimap<TYPE_ID, size_t> tripleTmpls_worksFor;
//	KEY_ID worksFor = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#worksFor>");
//	diplo::TM.GetTemplates(-1, worksFor, Department, 0, &tripleTmpls_worksFor);
//
//	multimap<TYPE_ID, size_t> tripleTmpls_headOf;
//	KEY_ID headOf = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#headOf>");
//	diplo::TM.GetTemplates(-1, headOf, Department, 0, &tripleTmpls_headOf);
//
//	pair<multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator, multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator> it_inv_molecules_Department;
//
//
//	for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it = it_inv_molecules.first; it != it_inv_molecules.second; it++) {
//		if( diplo::KM.GetType( it->second->first) == Department ) {
//			size_t tmpl = diplo::TM.GetTemplateForType( diplo::KM.GetType( it->second->first) );
//			if (tmpl == (size_t)-1 ) continue;
//			tripleTmpls_range = tripleTmpls.equal_range(tmpl);
//
//			for ( multimap<size_t, size_t>::iterator it2 = tripleTmpls_range.first; it2 != tripleTmpls_range.second; it2++ ) {
//				if (it->second->second.CheckIfEntityExists(it2->second, University0, 0)) {
//					it_inv_molecules_Department = diplo::M.inv_molecules.equal_range( it->second->first ); //<<<< molekuly gdzie wystepuje dany depatrament
//
//					for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it_it_inv_molecules_Department = it_inv_molecules_Department.first; it_it_inv_molecules_Department != it_inv_molecules_Department.second; it_it_inv_molecules_Department++) {
//						size_t tmpl_Department = diplo::TM.GetTemplateForType( diplo::KM.GetType( it_it_inv_molecules_Department->second->first) );
//						if (tmpl_Department == (size_t)-1 ) continue;
//						tripleTmpls_headOf_range = tripleTmpls_headOf.equal_range(tmpl_Department);
//
//						for ( multimap<size_t, size_t>::iterator it2_headOf = tripleTmpls_headOf_range.first; it2_headOf != tripleTmpls_headOf_range.second; it2_headOf++ ) {
//							if (it_it_inv_molecules_Department->second->second.CheckIfEntityExists(it2_headOf->second, it->second->first, 0)) { //<<<< check headOf
//
//								tripleTmpls_worksFor_range = tripleTmpls_worksFor.equal_range(tmpl_Department);
//								for ( multimap<size_t, size_t>::iterator it2_worksFor = tripleTmpls_worksFor_range.first; it2_worksFor != tripleTmpls_worksFor_range.second; it2_worksFor++ ) {
//									if (it_it_inv_molecules_Department->second->second.CheckIfEntityExists(it2_worksFor->second, it->second->first, 0)) { //<<<< check worksFor
//
//										cout <<  diplo::KM.Get(it->second->first)  << "\t" << diplo::KM.Get(it_it_inv_molecules_Department->second->first) << endl;
//
//									}
//								}
//							}
//						}
//
//					}
//
//				}
//			}
//
//		}
//	}
//
//}
//
//void LUBM::q13() {
//
//	/*
//	 * SELECT ?X
//	 :hasAlumnus owl:inverseOf ?c .
//	 ?a ?b ?c .
//	 ?X ?a <http://www.University0.edu> .
//	 *
//	 */
//
//	KEY_ID hasAlumnus = diplo::KM.Get("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#hasAlumnus>");
//	TYPE_ID hasAlumnus_type =  diplo::KM.GetType(hasAlumnus);
//	Molecule *mol = diplo::M.Get(hasAlumnus);
//	set<KEY_ID> subdegrees;
//	if (mol) {
//		multimap<TYPE_ID, size_t> tripleTmpls;
//		KEY_ID inverseOf = diplo::KM.Get("<http://www.w3.org/2002/07/owl#inverseOf>");
//		diplo::TM.GetTemplates(hasAlumnus_type, inverseOf, (TYPE_ID) -1, 0, &tripleTmpls);
//		size_t tripleTempl = tripleTmpls.begin()->second;
//		set<KEY_ID> degrees;
//		mol->GetObjects(tripleTempl, &degrees);
//		KEY_ID degree = *degrees.begin();
//
//		KEY_ID subPropertyOf = diplo::KM.Get("<http://www.w3.org/2000/01/rdf-schema#subPropertyOf>");
//		multimap<TYPE_ID, size_t> tripleTmpls2;
//		TYPE_ID degree_type = diplo::KM.GetType(degree);
//		diplo::TM.GetTemplates((TYPE_ID) -1, subPropertyOf, degree_type, 0, &tripleTmpls2);
//
//		pair<multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator, multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator> it_inv_molecules;
//		it_inv_molecules = diplo::M.inv_molecules.equal_range(degree);
//		for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it = it_inv_molecules.first; it != it_inv_molecules.second; it++) {
//			size_t t = tripleTmpls2.find(diplo::TM.GetTemplateForType(diplo::KM.GetType(it->second->first)))->second;
//
//			if (it->second->second.CheckIfEntityExists(t, degree, 0)) {
//				subdegrees.insert(it->second->first);
//			}
//		}
//
//	}
//
//
//
//
//
//
////	cerr << diplo::KM.Get( p ) << endl;
//
//	KEY_ID University0 = diplo::KM.Get("<http://www.University0.edu>");
//	pair<multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator, multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator> it_inv_molecules;
//	it_inv_molecules = diplo::M.inv_molecules.equal_range(University0);
//
//	multimap<TYPE_ID, size_t> tripleTmpls;
////	cerr <<"------------------" << endl;
////	cerr << diplo::KM.Get( p ) << "\t" << p << "\t" << diplo::KM.GetType(University0) <<  endl;
//
//	for ( set<KEY_ID>::iterator i = subdegrees.begin(); i !=  subdegrees.end(); i++ ) {
//			diplo::TM.GetTemplates( (TYPE_ID)-1, *i, diplo::KM.GetType(University0), 0, &tripleTmpls);
//	}
//	pair< multimap<size_t, size_t>::iterator, multimap<size_t, size_t>::iterator > tripleTmpls_range;
//
//
//	for (multimap<KEY_ID, pair<const KEY_ID, Molecule>*>::iterator it = it_inv_molecules.first; it != it_inv_molecules.second; it++) {
//		size_t tmpl = diplo::TM.GetTemplateForType( diplo::KM.GetType(it->second->first) );
//		if (tmpl == (size_t) -1)
//			continue;
//		tripleTmpls_range = tripleTmpls.equal_range(tmpl);
//
//		for (multimap<size_t, size_t>::iterator it2 = tripleTmpls_range.first; it2 != tripleTmpls_range.second; it2++) {
//			if (it->second->second.CheckIfEntityExists(it2->second, University0, 0)) {
//				cout <<  diplo::KM.Get(it->second->first) << endl;
//			}
//		}
//
//	}
//
//
//
//}
//
//void LUBM::q14() {
///*
// * 	SELECT ?X
//	?X a :UndergraduateStudent .
// *
// */
//	TYPE_ID UndergraduateStudent = diplo::KM.GetSeq("<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#UndergraduateStudent>");
//	map< KEY_ID, class Node * > * elems = diplo::KM.GetElementsOfType(UndergraduateStudent);
//	for (map< KEY_ID, class Node * >::iterator i = elems->begin(); i != elems->end(); i++) {
//		cout <<  diplo::KM.Get(i->first) << endl;
//	}
//
//}


}
