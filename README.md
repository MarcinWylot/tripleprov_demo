# TripleProv 
is an in-memory RDF database capable to store, trace, and query provenance information in processing RDF queries.

[TripleProv: Tracking and Querying Provenance in Linked Data](https://exascale.info/linked/data/provenance/rdf/management/tripleprov/TripleProv-Tracking-and-Querying-Provenance-in-Linked-Data/)



--------------------
# build
```sh
cd release 
cmake ../
make 
```

# usage
```sh
./tripleprov ../query01 
./tripleprov ../query01 ../ProvQuery
./tripleprov ../query01 ../ProvQuery2
./tripleprov ../query02 
./tripleprov ../query02 ../ProvQuery
./tripleprov ../query02 ../ProvQuery2
./tripleprov ../query03
./tripleprov ../query03 ../ProvQuery
./tripleprov ../query03 ../ProvQuery2
./tripleprov ../query04 
./tripleprov ../query04 ../ProvQuery
./tripleprov ../query04 ../ProvQuery2
./tripleprov ../query05 
./tripleprov ../query05 ../ProvQuery
./tripleprov ../query05 ../ProvQuery2
./tripleprov ../query06
./tripleprov ../query06 ../ProvQuery
./tripleprov ../query06 ../ProvQuey2
```
