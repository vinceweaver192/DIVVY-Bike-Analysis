/*hash.h*/

//
// Vincent Weaver
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 


#include <string>

using namespace std;

bool isNumeric(string s);
int Hash(string theDate, int N);
size_t HashStationAbbrev(string input, int N);
int HashTripID(string input, int N);
int HashBikeID(string ID, int N);
int string2int(string s);
