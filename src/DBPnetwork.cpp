/*
 * DBPnetwork.cpp
 *
 *  Created on: 26-04-2013
 *      Author: marcin
 */

#include "DBPnetwork.h"

namespace queries {

DBP_network::DBP_network() {
	// TODO Auto-generated constructor stub

}

void DBP_network::q01_n(char* data) {
	/*
	 * SELECT ?p ?o WHERE {
	 <http://dbpedia.org/resource/Metropolitan_Museum_of_Art> ?p ?o
	 }
	 *
	 */
//	KEY_ID sbj = diplo::KM.Get("<http://dbpedia.org/resource/Metropolitan_Museum_of_Art>");
	KEY_ID sbj = *((KEY_ID *) data);

	Molecule *m = diplo::M.Get(sbj);
	vector<pair<KEY_ID, KEY_ID> > ready;
	pair<KEY_ID, KEY_ID> x;

	if (m != NULL) {
		unordered_multimap<size_t, KEY_ID> mol;
		m->GetAllEntities(0, &mol, 0);
		for (unordered_multimap<size_t, KEY_ID>::iterator j = mol.begin(); j != mol.end(); j++) {
			x.first = diplo::TM.GetPredicate(diplo::KM.GetType(sbj), j->first, 0);
			x.second = j->second;
			ready.push_back(x);
		}
	}

	size_t s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

}

void DBP_network::q01_m() {
	string q = "Dq01";
	KEY_ID sbj = diplo::KM.Get("<http://dbpedia.org/resource/Metropolitan_Museum_of_Art>");

	size_t s = q.size() + sizeof(sbj);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(sbj);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());
	data_tmp = data_tmp + q.size();
	*((KEY_ID *) data_tmp) = sbj;
	data_tmp = data_tmp + sizeof(sbj);


	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<pair<KEY_ID, KEY_ID>> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

	}

	for (vector<pair<KEY_ID, KEY_ID>>::iterator j = results.begin(); j != results.end(); j++) {
		diplo::KM.Get(j->first);
		diplo::KM.Get(j->second);
//		cout << diplo::KM.Get(j->first) << "\t" << diplo::KM.Get(j->second) << endl;
	}

	PRINT_NO_RESULTS
}

void DBP_network::q02_n(char* data) {
	/*
	 * PREFIX p: <http://dbpedia.org/property/>
	 SELECT ?film1 ?actor1 ?film2 ?actor2
	 WHERE 		{
	 ?film1 p:starring <http://dbpedia.org/resource/Kevin_Bacon> .
	 ?film1 p:starring ?actor1 .
	 ?film2 p:starring ?actor1 .
	 ?film2 p:starring ?actor2 .
	 }
	 *
	 */

//	KEY_ID v = diplo::KM.Get("<http://dbpedia.org/resource/Kevin_Bacon>");
//	KEY_ID p = diplo::KM.Get("<http://dbpedia.org/property/starring>");
	KEY_ID v = *((KEY_ID *) data);
	data = data + sizeof(v);
	KEY_ID p = *((KEY_ID *) data);
	data = data + sizeof(p);

	TYPE_ID vT = diplo::KM.GetType(v);

	val4 x;
	vector<val4> s1;
	vector<val4> ready;

	set<pair<const KEY_ID, Molecule>*>* Molecules_with_v;
	Molecules_with_v = diplo::M.GetMoleculesFromInvertedIdx(v);

	if (Molecules_with_v != NULL) {
		for (set<pair<const KEY_ID, Molecule>*>::iterator i = Molecules_with_v->begin(); i != Molecules_with_v->end(); i++) {
			TYPE_ID iT = diplo::KM.GetType((*i)->first);
			unordered_set<size_t> tripleTemplates;
			unordered_set<KEY_ID> property_v;
			size_t tripleTempl = diplo::TM.GetTripleTemplate(iT, iT, p, vT, 0);
			tripleTemplates.insert(tripleTempl);

			if (!(*i)->second.CheckIfEntityExists(tripleTempl, v, 0))
				continue;

			multimap<TYPE_ID, size_t> templates;
			diplo::TM.GetTemplates(iT, p, -1, 0, &templates);
			for (multimap<TYPE_ID, size_t>::iterator g = templates.begin(); g != templates.end(); g++) {
				tripleTemplates.insert(g->second);
			}

			(*i)->second.GetEntity(&tripleTemplates, 0, 0, &property_v);
			x.v1 = (*i)->first;
			for (unordered_set<KEY_ID>::iterator j = property_v.begin(); j != property_v.end(); j++) {
				x.v2 = *j;
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
		Molecules_with_v = diplo::M.GetMoleculesFromInvertedIdx(j->v2);
		if (Molecules_with_v != NULL) {
			x.v1 = j->v1;
			x.v2 = j->v2;
			for (set<pair<const KEY_ID, Molecule>*>::iterator i = Molecules_with_v->begin(); i != Molecules_with_v->end(); i++) {
				TYPE_ID iT = diplo::KM.GetType((*i)->first);
				unordered_set<size_t> tripleTemplates;
				unordered_set<KEY_ID> property_v;
				size_t tripleTempl = diplo::TM.GetTripleTemplate(iT, iT, p, diplo::KM.GetType(j->v2), 0);
				tripleTemplates.insert(tripleTempl);

				if (!(*i)->second.CheckIfEntityExists(tripleTempl, j->v2, 0))
					continue;
				multimap<TYPE_ID, size_t> templates;

				diplo::TM.GetTemplates(iT, p, -1, 0, &templates);
				for (multimap<TYPE_ID, size_t>::iterator g = templates.begin(); g != templates.end(); g++) {
					tripleTemplates.insert(g->second);
				}

				(*i)->second.GetEntity(&tripleTemplates, 0, 0, &property_v);
				x.v3 = (*i)->first;

				for (unordered_set<KEY_ID>::iterator j = property_v.begin(); j != property_v.end(); j++) {
					x.v4 = *j;
					ready.push_back(x);
				}
			}
		}
	}

	s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(val4 ));

}

void DBP_network::q02_m() {
	string q = "Dq02";
	KEY_ID v = diplo::KM.Get("<http://dbpedia.org/resource/Kevin_Bacon>");
	KEY_ID p = diplo::KM.Get("<http://dbpedia.org/property/starring>");

	size_t s = q.size() + sizeof(v) + sizeof(p);
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
	*((KEY_ID *) data_tmp) = p;
	data_tmp = data_tmp + sizeof(p);

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<val4> s1;
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

	vector<val4> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {

		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val4), i);

	}

	for (vector<val4>::iterator i = results.begin(); i != results.end(); i++) {
		diplo::KM.Get(i->v1);
		diplo::KM.Get(i->v2);
		diplo::KM.Get(i->v3);
		diplo::KM.Get(i->v4);
//		cout << diplo::KM.Get(i->v1) << "\t" << diplo::KM.Get(i->v2) << "\t" << diplo::KM.Get(i->v3) << "\t" << diplo::KM.Get(i->v4) << endl;
	}

	PRINT_NO_RESULTS

}

void DBP_network::q03_n(char* data) {
	/*
	 * PREFIX p: <http://dbpedia.org/property/>
	 SELECT ?artist ?artwork ?museum ?director
	 WHERE {
	 ?artwork p:artist ?artist .
	 ?artwork p:museum ?museum .
	 ?museum p:director ?director
	 }
	 *
	 */

//	KEY_ID artist = diplo::KM.Get("<http://dbpedia.org/property/artist>");
//	KEY_ID museum = diplo::KM.Get("<http://dbpedia.org/property/museum>");
//	KEY_ID director = diplo::KM.Get("<http://dbpedia.org/property/director>");
	KEY_ID artist = *((KEY_ID *) data);
	data = data + sizeof(artist);
	KEY_ID museum = *((KEY_ID *) data);
	data = data + sizeof(museum);
	KEY_ID director = *((KEY_ID *) data);
	data = data + sizeof(director);

	multimap<TYPE_ID, size_t> templatesartist;
	multimap<TYPE_ID, size_t> templatesmuseum;
	multimap<TYPE_ID, size_t> templatesdirector;

	diplo::TM.GetTemplates(-1, artist, -1, 0, &templatesartist);
	diplo::TM.GetTemplates(-1, museum, -1, 0, &templatesmuseum);
	diplo::TM.GetTemplates(-1, director, -1, 0, &templatesdirector);

	val4 x;
	vector<val4> s1;
	unordered_set<KEY_ID> allmuseums;
	vector<KEY_ID> vallmuseums;

	for (multimap<TYPE_ID, size_t>::iterator g = templatesmuseum.begin(); g != templatesmuseum.end(); g++) {
		unordered_set<KEY_ID>* el;
		el = diplo::TypesM.GetElementsOfType(g->first);

		unordered_set<size_t> tripleTemplatesartist;
		unordered_set<size_t> tripleTemplatessmuseum;
		tripleTemplatessmuseum.insert(g->second);

		pair<multimap<TYPE_ID, size_t>::iterator, multimap<TYPE_ID, size_t>::iterator> ttpair = templatesartist.equal_range(g->first);
		for (multimap<TYPE_ID, size_t>::iterator g2 = ttpair.first; g2 != ttpair.second; g2++) {
			tripleTemplatesartist.insert(g2->second);
		}

		for (unordered_set<KEY_ID>::iterator i = el->begin(); i != el->end(); i++) {
			unordered_set<KEY_ID> museums;
			unordered_set<KEY_ID> artists;
			Molecule *m = diplo::M.Get(*i);
			if (m) {
				m->GetEntity(&tripleTemplatessmuseum, 0, 0, &museums);
				m->GetEntity(&tripleTemplatesartist, 0, 0, &artists);
				for (unordered_set<KEY_ID>::iterator mu = museums.begin(); mu != museums.end(); mu++) {
					for (unordered_set<KEY_ID>::iterator ar = artists.begin(); ar != artists.end(); ar++) {
						allmuseums.insert(*mu);
						x.v1 = *ar;
						x.v2 = *i;
						x.v3 = *mu;
						s1.push_back(x);
					}
				}
			}
		}
	}

	copy(allmuseums.begin(), allmuseums.end(), inserter(vallmuseums, vallmuseums.begin()));
	size_t s = vallmuseums.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vallmuseums.data(), s * sizeof(KEY_ID));

	diplo::CLI.Receive((char*) &s, sizeof(s));
	vallmuseums.resize(s);
	diplo::CLI.Receive((char *) (vallmuseums.data()), s * sizeof(KEY_ID));
	copy(vallmuseums.begin(), vallmuseums.end(), inserter(allmuseums, allmuseums.begin()));

	vector<pair<KEY_ID, KEY_ID> > vmuseumdirector;
	unordered_multimap<KEY_ID, KEY_ID> museumdirector;

	for (unordered_set<KEY_ID>::iterator g = allmuseums.begin(); g != allmuseums.end(); g++) {
		Molecule *m = diplo::M.Get(*g);
		if (m) {

			unordered_set<size_t> tripleTemplatesdirector;
			pair<multimap<TYPE_ID, size_t>::iterator, multimap<TYPE_ID, size_t>::iterator> ttpair = templatesdirector.equal_range(diplo::KM.GetType(*g));
			for (multimap<TYPE_ID, size_t>::iterator g2 = ttpair.first; g2 != ttpair.second; g2++) {
				tripleTemplatesdirector.insert(g2->second);
			}
			unordered_set<KEY_ID> directors;
			m->GetEntity(&tripleTemplatesdirector, 0, 0, &directors);
			for (unordered_set<KEY_ID>::iterator dire = directors.begin(); dire != directors.end(); dire++) {
				vmuseumdirector.push_back(pair<KEY_ID, KEY_ID>(*g, *dire));
			}
		}
	}

	s = vmuseumdirector.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) vmuseumdirector.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

	diplo::CLI.Receive((char*) &s, sizeof(s));
	vmuseumdirector.resize(s);
	diplo::CLI.Receive((char *) (vmuseumdirector.data()), s * sizeof(pair<KEY_ID, KEY_ID> ));
	copy(vmuseumdirector.begin(), vmuseumdirector.end(), inserter(museumdirector, museumdirector.begin()));

	vector<val4> ready;
	for (vector<val4>::iterator g = s1.begin(); g != s1.end(); g++) {
		pair<unordered_map<KEY_ID, KEY_ID>::iterator, unordered_map<KEY_ID, KEY_ID>::iterator> i = museumdirector.equal_range(g->v3);

		for (unordered_map<KEY_ID, KEY_ID>::iterator dir = i.first; dir != i.second; dir++) {
			g->v4 = dir->second;
			ready.push_back(*g);
		}
	}

	s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(val4));

}

void DBP_network::q03_m() {
	string q = "Dq03";
	KEY_ID artist = diplo::KM.Get("<http://dbpedia.org/property/artist>");
	KEY_ID museum = diplo::KM.Get("<http://dbpedia.org/property/museum>");
	KEY_ID director = diplo::KM.Get("<http://dbpedia.org/property/director>");

	size_t s = q.size() + sizeof(artist) + sizeof(museum) + sizeof(director);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());

	data_tmp = data_tmp + q.size();

	*((KEY_ID *) data_tmp) = artist;
	data_tmp = data_tmp + sizeof(artist);
	*((KEY_ID *) data_tmp) = museum;
	data_tmp = data_tmp + sizeof(museum);
	*((KEY_ID *) data_tmp) = director;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<KEY_ID> vallmuseums;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = vallmuseums.size();
		vallmuseums.resize(n + s);
		diplo::SRV.Receive((char *) (vallmuseums.data() + n), s * sizeof(KEY_ID), i);
	}

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = vallmuseums.size();
		diplo::SRV.Send((char*) &s, sizeof(s), i);
		diplo::SRV.Send((char *) (vallmuseums.data()), s * sizeof(KEY_ID), i);
	}

	vector<pair<KEY_ID, KEY_ID> > vmuseumdirector;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = vmuseumdirector.size();
		vmuseumdirector.resize(n + s);
		diplo::SRV.Receive((char *) (vmuseumdirector.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);
	}

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = vmuseumdirector.size();
		diplo::SRV.Send((char*) &s, sizeof(s), i);
		diplo::SRV.Send((char *) (vmuseumdirector.data()), s * sizeof(pair<KEY_ID, KEY_ID> ), i);
	}

	vector<val4> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val4), i);

	}

	for (vector<val4>::iterator i = results.begin(); i != results.end(); i++) {
		diplo::KM.Get(i->v1);
		diplo::KM.Get(i->v2);
		diplo::KM.Get(i->v3);
		diplo::KM.Get(i->v4);
//		cout << diplo::KM.Get(i->v1) << "\t" << diplo::KM.Get(i->v2) << "\t" << diplo::KM.Get(i->v3) << "\t" << diplo::KM.Get(i->v4) << endl;
	}

	PRINT_NO_RESULTS
}

void DBP_network::q04_n(char* data) {
	/*
	 SELECT ?s ?homepage WHERE {
	 <http://dbpedia.org/resource/Berlin> geo:lat ?berlinLat .
	 <http://dbpedia.org/resource/Berlin> geo:long ?berlinLong .
	 ?s geo:lat ?lat .
	 ?s geo:long ?long .
	 ?s foaf:homepage ?homepage .
	 FILTER (
	 ?lat        <=      ?berlinLat + 0.03190235436 &&
	 ?long         >=      ?berlinLong - 0.08679199218 &&
	 ?lat        >=     ?berlinLat - 0.03190235436 &&
	 ?long         <=      ?berlinLong + 0.08679199218)
	 }
	 */
//	KEY_ID lat = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#lat>");
//	KEY_ID longg = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#long>");
//	KEY_ID homepage = diplo::KM.Get("<http://xmlns.com/foaf/0.1/homepage>");
//	KEY_ID berlin = diplo::KM.Get("<http://dbpedia.org/resource/Berlin>");
	KEY_ID lat = *((KEY_ID *) data);
	data = data + sizeof(lat);
	KEY_ID longg = *((KEY_ID *) data);
	data = data + sizeof(longg);
	KEY_ID homepage = *((KEY_ID *) data);
	data = data + sizeof(homepage);
	KEY_ID berlin = *((KEY_ID *) data);
	data = data + sizeof(berlin);

	multimap<TYPE_ID, size_t> templateslat;
	multimap<TYPE_ID, size_t> templateslongg;
	multimap<TYPE_ID, size_t> templateshomepage;

	diplo::TM.GetTemplates(-1, lat, -1, 0, &templateslat);
	diplo::TM.GetTemplates(-1, longg, -1, 0, &templateslongg);
	diplo::TM.GetTemplates(-1, homepage, -1, 0, &templateshomepage);

	Molecule *m = diplo::M.Get(berlin);

	pair<KEY_ID, KEY_ID> y;
	vector<pair<KEY_ID, KEY_ID>> yy;
	if (m != NULL) {
		for (multimap<TYPE_ID, size_t>::iterator g = templateslat.begin(); g != templateslat.end(); g++) {

			unordered_set<size_t> tripleTemplatesslat;
			tripleTemplatesslat.insert(g->second);

			unordered_set<size_t> tripleTemplateslongg;
			pair<multimap<TYPE_ID, size_t>::iterator, multimap<TYPE_ID, size_t>::iterator> ttpair = templateslongg.equal_range(g->first);
			for (multimap<TYPE_ID, size_t>::iterator g2 = ttpair.first; g2 != ttpair.second; g2++) {
				tripleTemplateslongg.insert(g2->second);
			}
			unordered_set<KEY_ID> late;
			unordered_set<KEY_ID> longge;
			m->GetEntity(&tripleTemplatesslat, 0, 0, &late);
			m->GetEntity(&tripleTemplateslongg, 0, 0, &longge);
			for (unordered_set<KEY_ID>::iterator mu = late.begin(); mu != late.end(); mu++) {
				for (unordered_set<KEY_ID>::iterator ar = longge.begin(); ar != longge.end(); ar++) {
					y.first = *mu; //lat
					y.second = *ar; //long
					yy.push_back(y);
				}
			}
		}
	}

	val4 x;
	vector<val4> s1;

	for (multimap<TYPE_ID, size_t>::iterator g = templateslat.begin(); g != templateslat.end(); g++) {
		unordered_set<KEY_ID>* el;
		el = diplo::TypesM.GetElementsOfType(g->first);

		unordered_set<size_t> tripleTemplatesslat;
		tripleTemplatesslat.insert(g->second);

		unordered_set<size_t> tripleTemplateslongg;
		pair<multimap<TYPE_ID, size_t>::iterator, multimap<TYPE_ID, size_t>::iterator> ttpair = templateslongg.equal_range(g->first);
		for (multimap<TYPE_ID, size_t>::iterator g2 = ttpair.first; g2 != ttpair.second; g2++) {
			tripleTemplateslongg.insert(g2->second);
		}

		unordered_set<size_t> tripleTemplateshomepage;
		ttpair = templateshomepage.equal_range(g->first);
		for (multimap<TYPE_ID, size_t>::iterator g2 = ttpair.first; g2 != ttpair.second; g2++) {
			tripleTemplateshomepage.insert(g2->second);
		}

		for (unordered_set<KEY_ID>::iterator i = el->begin(); i != el->end(); i++) {
			unordered_set<KEY_ID> late;
			unordered_set<KEY_ID> longge;
			unordered_set<KEY_ID> homapagee;
			Molecule *m = diplo::M.Get(*i);
			if (m) {
				m->GetEntity(&tripleTemplatesslat, 0, 0, &late);
				m->GetEntity(&tripleTemplateslongg, 0, 0, &longge);
				m->GetEntity(&tripleTemplateshomepage, 0, 0, &homapagee);
				for (unordered_set<KEY_ID>::iterator mu = late.begin(); mu != late.end(); mu++) {
					for (unordered_set<KEY_ID>::iterator ar = longge.begin(); ar != longge.end(); ar++) {
						for (unordered_set<KEY_ID>::iterator ab = homapagee.begin(); ab != homapagee.end(); ab++) {
							x.v1 = *i;
							x.v2 = *mu;
							x.v3 = *ar;
							x.v4 = *ab;
							s1.push_back(x);
						}
					}
				}
			}
		}
	}

	size_t s = yy.size(); //info about the city
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) yy.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

	s = s1.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) s1.data(), s * sizeof(val4));

}

void DBP_network::q04_m() {
	string q = "Dq04";
	KEY_ID lat = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#lat>");
	KEY_ID longg = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#long>");
	KEY_ID homepage = diplo::KM.Get("<http://xmlns.com/foaf/0.1/homepage>");
	KEY_ID berlin = diplo::KM.Get("<http://dbpedia.org/resource/Berlin>");

	size_t s = q.size() + sizeof(lat) + sizeof(longg) + sizeof(homepage) + sizeof(berlin);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());

	data_tmp = data_tmp + q.size();

	*((KEY_ID *) data_tmp) = lat;
	data_tmp = data_tmp + sizeof(lat);
	*((KEY_ID *) data_tmp) = longg;
	data_tmp = data_tmp + sizeof(longg);
	*((KEY_ID *) data_tmp) = homepage;
	data_tmp = data_tmp + sizeof(homepage);
	*((KEY_ID *) data_tmp) = berlin;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<val4> s1;
	vector<pair<KEY_ID, KEY_ID>> yy;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = yy.size();
		yy.resize(n + s);
		diplo::SRV.Receive((char *) (yy.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

		s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = s1.size();
		s1.resize(n + s);
		diplo::SRV.Receive((char *) (s1.data() + n), s * sizeof(val4), i);

	}

	//this must be done on Master
	string berlinlong = diplo::KM.Get(yy[0].second);
	string berlinlat = diplo::KM.Get(yy[0].first);

	long double berlinLong = getDuble(berlinlong);
	long double berlinLat = getDuble(berlinlat);
	long double llong, llat;

	string slat, slong;
	vector<pair<KEY_ID, KEY_ID> > results;
	for (vector<val4>::iterator g = s1.begin(); g != s1.end(); g++) {
		slat = diplo::KM.Get(g->v2);
		slong = diplo::KM.Get(g->v3);
		llat = getDuble(slat);
		llong = getDuble(slong);

		if (llat <= (berlinLat + 0.03190235436) and llong >= (berlinLong - 0.08679199218) and llat >= (berlinLat - 0.03190235436) and llong <= (berlinLong + 0.08679199218)) {
			results.push_back(pair<KEY_ID, KEY_ID>(g->v1, g->v4));
		}

	}

	for (vector<pair<KEY_ID, KEY_ID> >::iterator i = results.begin(); i != results.end(); i++) {
		diplo::KM.Get(i->first);
		diplo::KM.Get(i->second);
//		cout << diplo::KM.Get(i->first) << "\t" << diplo::KM.Get(i->second) << endl;
	}

	PRINT_NO_RESULTS

}

void DBP_network::q05_n(char* data) {
	/*
	 SELECT ?s ?a ?homepage WHERE {
	 <http://dbpedia.org/resource/New_York_City> geo:lat ?nyLat .
	 <http://dbpedia.org/resource/New_York_City> geo:long ?nyLong .
	 ?s geo:lat ?lat .
	 ?s geo:long ?long .
	 ?s p:architect ?a .
	 ?a foaf:homepage ?homepage .
	 FILTER (
	 ?lat        <=      ?nyLat + 0.3190235436 &&
	 ?long         >=      ?nyLong - 0.8679199218 &&
	 ?lat        >=     ?nyLat - 0.3190235436 &&
	 ?long         <=      ?nyLong + 0.8679199218)
	 }

	 */

//	KEY_ID lat = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#lat>");
//	KEY_ID longg = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#long>");
//	KEY_ID homepage = diplo::KM.Get("<http://xmlns.com/foaf/0.1/homepage>");
//	KEY_ID architect = diplo::KM.Get("<http://dbpedia.org/property/architect>");
//
//	KEY_ID berlin = diplo::KM.Get("<http://dbpedia.org/resource/New_York_City>");


	KEY_ID lat = *((KEY_ID *) data);
	data = data + sizeof(lat);
	KEY_ID longg = *((KEY_ID *) data);
	data = data + sizeof(longg);
	KEY_ID homepage = *((KEY_ID *) data);
	data = data + sizeof(homepage);
	KEY_ID architect = *((KEY_ID *) data);
	data = data + sizeof(architect);
	KEY_ID berlin = *((KEY_ID *) data);
	data = data + sizeof(berlin);

	multimap<TYPE_ID, size_t> templateslat;
	multimap<TYPE_ID, size_t> templateslongg;
	multimap<TYPE_ID, size_t> templateshomepage;
	multimap<TYPE_ID, size_t> templatesarchitect;

	diplo::TM.GetTemplates(-1, lat, -1, 0, &templateslat);
	diplo::TM.GetTemplates(-1, longg, -1, 0, &templateslongg);
	diplo::TM.GetTemplates(-1, homepage, -1, 0, &templateshomepage);
	diplo::TM.GetTemplates(-1, architect, -1, 0, &templatesarchitect);

	val4 x;
	vector<val4> s1;

	Molecule *m = diplo::M.Get(berlin);

	pair<KEY_ID, KEY_ID> y;
	vector<pair<KEY_ID, KEY_ID>> yy;
	if (m != NULL) {
		for (multimap<TYPE_ID, size_t>::iterator g = templateslat.begin(); g != templateslat.end(); g++) {

			unordered_set<size_t> tripleTemplatesslat;
			tripleTemplatesslat.insert(g->second);

			unordered_set<size_t> tripleTemplateslongg;
			pair<multimap<TYPE_ID, size_t>::iterator, multimap<TYPE_ID, size_t>::iterator> ttpair = templateslongg.equal_range(g->first);
			for (multimap<TYPE_ID, size_t>::iterator g2 = ttpair.first; g2 != ttpair.second; g2++) {
				tripleTemplateslongg.insert(g2->second);
			}
			unordered_set<KEY_ID> late;
			unordered_set<KEY_ID> longge;
			m->GetEntity(&tripleTemplatesslat, 0, 0, &late);
			m->GetEntity(&tripleTemplateslongg, 0, 0, &longge);
			for (unordered_set<KEY_ID>::iterator mu = late.begin(); mu != late.end(); mu++) {
				for (unordered_set<KEY_ID>::iterator ar = longge.begin(); ar != longge.end(); ar++) {
					y.first = *mu; //lat
					y.second = *ar; //long
					yy.push_back(y);
				}
			}
		}
	}

	for (multimap<TYPE_ID, size_t>::iterator g = templateslat.begin(); g != templateslat.end(); g++) {
		unordered_set<KEY_ID>* el;
		el = diplo::TypesM.GetElementsOfType(g->first);

		unordered_set<size_t> tripleTemplatesslat;
		tripleTemplatesslat.insert(g->second);

		unordered_set<size_t> tripleTemplateslongg;
		pair<multimap<TYPE_ID, size_t>::iterator, multimap<TYPE_ID, size_t>::iterator> ttpair = templateslongg.equal_range(g->first);
		for (multimap<TYPE_ID, size_t>::iterator g2 = ttpair.first; g2 != ttpair.second; g2++) {
			tripleTemplateslongg.insert(g2->second);
		}

		unordered_set<size_t> tripleTemplatesarchitect;
		ttpair = templatesarchitect.equal_range(g->first);
		for (multimap<TYPE_ID, size_t>::iterator g2 = ttpair.first; g2 != ttpair.second; g2++) {
			tripleTemplatesarchitect.insert(g2->second);
		}

		for (unordered_set<KEY_ID>::iterator i = el->begin(); i != el->end(); i++) {
			unordered_set<KEY_ID> late;
			unordered_set<KEY_ID> longge;
			unordered_set<KEY_ID> architect;
			Molecule *m = diplo::M.Get(*i);
			if (m) {
				m->GetEntity(&tripleTemplatesslat, 0, 0, &late);
				m->GetEntity(&tripleTemplateslongg, 0, 0, &longge);
				m->GetEntity(&tripleTemplatesarchitect, 0, 0, &architect);
				for (unordered_set<KEY_ID>::iterator mu = late.begin(); mu != late.end(); mu++) {
					for (unordered_set<KEY_ID>::iterator ar = longge.begin(); ar != longge.end(); ar++) {
						for (unordered_set<KEY_ID>::iterator ab = architect.begin(); ab != architect.end(); ab++) {
							x.v1 = *i;
							x.v2 = *mu;
							x.v3 = *ar;
							x.v4 = *ab;
							s1.push_back(x);
						}
					}
				}
			}
		}
	}

	size_t s = yy.size(); //info about the city
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) yy.data(), s * sizeof(pair<KEY_ID, KEY_ID> ));

	s = s1.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) s1.data(), s * sizeof(val4));


	//send artists to nodes and get their homepages
	vector<val4> s2;
	s = 0;
	diplo::CLI.Receive((char*) &s, sizeof(s));
	s2.resize(s);
	diplo::CLI.Receive((char *) (s2.data()), s * sizeof(val4));


	vector<val4> ready;
	for (vector<val4>::iterator i = s2.begin(); i != s2.end(); i++) {
		Molecule *m = diplo::M.Get(i->v2);
		if (m != NULL) {
			for (multimap<TYPE_ID, size_t>::iterator g = templateshomepage.begin(); g != templateshomepage.end(); g++) {
				unordered_set<size_t> tripleTemplateshomepage;
				tripleTemplateshomepage.insert(g->second);
				unordered_set<KEY_ID> homepage;
				m->GetEntity(&tripleTemplateshomepage, 0, 0, &homepage);
				for (unordered_set<KEY_ID>::iterator mu = homepage.begin(); mu != homepage.end(); mu++) {
					i->v3 = *mu; //lat
					ready.push_back(*i);
				}
			}
		}
	}


	s = ready.size();
	diplo::CLI.Send((char *) &s, sizeof(s));
	diplo::CLI.Send((char *) ready.data(), s * sizeof(val4));


}

void DBP_network::q05_m() {
	string q = "Dq05";
	KEY_ID lat = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#lat>");
	KEY_ID longg = diplo::KM.Get("<http://www.w3.org/2003/01/geo/wgs84_pos#long>");
	KEY_ID homepage = diplo::KM.Get("<http://xmlns.com/foaf/0.1/homepage>");
	KEY_ID architect = diplo::KM.Get("<http://dbpedia.org/property/architect>");
	KEY_ID berlin = diplo::KM.Get("<http://dbpedia.org/resource/New_York_City>");

	size_t s = q.size() + sizeof(lat) + sizeof(longg) + sizeof(homepage) + sizeof(architect) + sizeof(berlin);
	if (s > diplo::network_buf_size) {
		ERROR_OUT
		;
		exit(1);
	}

	char *data = (char *) malloc(s);
	char * data_tmp = data;
	strcpy(data_tmp, q.c_str());

	data_tmp = data_tmp + q.size();

	*((KEY_ID *) data_tmp) = lat;
	data_tmp = data_tmp + sizeof(lat);
	*((KEY_ID *) data_tmp) = longg;
	data_tmp = data_tmp + sizeof(longg);
	*((KEY_ID *) data_tmp) = homepage;
	data_tmp = data_tmp + sizeof(homepage);
	*((KEY_ID *) data_tmp) = architect;
	data_tmp = data_tmp + sizeof(architect);
	*((KEY_ID *) data_tmp) = berlin;

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		diplo::SRV.Send(data, diplo::network_buf_size, i);
	}

	vector<val4> s1;
	vector<pair<KEY_ID, KEY_ID>> yy;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = yy.size();
		yy.resize(n + s);
		diplo::SRV.Receive((char *) (yy.data() + n), s * sizeof(pair<KEY_ID, KEY_ID> ), i);

		s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		n = s1.size();
		s1.resize(n + s);
		diplo::SRV.Receive((char *) (s1.data() + n), s * sizeof(val4), i);

	}

	//this must be done on Master
	string berlinlong = diplo::KM.Get(yy[0].second);
	string berlinlat = diplo::KM.Get(yy[0].first);

	long double berlinLong = getDuble(berlinlong);
	long double berlinLat = getDuble(berlinlat);
	long double llong, llat;

	string slat, slong;
	vector<val4> s2;
	val4 x;
	for (vector<val4>::iterator g = s1.begin(); g != s1.end(); g++) {
		slat = diplo::KM.Get(g->v2);
		slong = diplo::KM.Get(g->v3);
		llat = getDuble(slat);
		llong = getDuble(slong);


		if (llat <= (berlinLat + 0.3190235436) and llong >= (berlinLong - 0.8679199218) and llat >= (berlinLat - 0.3190235436) and llong <= (berlinLong + 0.8679199218)) {
			x.v1 = g->v1;
			x.v2 = g->v4;
			x.v3 = g->v4; //should be homepage of artist
			s2.push_back(x);
		}

	}

	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {
		size_t s = s2.size();
		diplo::SRV.Send((char*) &s, sizeof(s), i);
		diplo::SRV.Send((char *) (s2.data()), s * sizeof(val4), i);
	}


	vector<val4> results;
	for (size_t i = 0; i < diplo::SRV.GetNbOfNodes(); i++) {

		size_t s = 0;
		diplo::SRV.Receive((char*) &s, sizeof(s), i);
		size_t n = results.size();
		results.resize(n + s);
		diplo::SRV.Receive((char *) (results.data() + n), s * sizeof(val4), i);

	}

	for (vector<val4>::iterator i = results.begin(); i != results.end(); i++) {
		diplo::KM.Get(i->v1);
		diplo::KM.Get(i->v2);
		diplo::KM.Get(i->v3);
//		cout << diplo::KM.Get(i->v1) << "\t" << diplo::KM.Get(i->v2) << "\t" << diplo::KM.Get(i->v3) << endl;
	}

	PRINT_NO_RESULTS

}

DBP_network::~DBP_network() {
	// TODO Auto-generated destructor stub
}

} /* namespace queries */
