/*hash.cpp*/

//
// Vincent Weaver
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 


#include <iostream>
#include <string>
#include <cctype>  /*isdigit*/
#include <regex>   /*regular expressions*/

#include "hash.h"

using namespace std;

//
// isNumeric
//
// Returns true if the given string is numeric (all digits), false
// if not.  If the string is empty, false is returned since there 
// are no digits present.
//
bool isNumeric(string s)
{
	//
	// A string is numeric if it contains 1 or more digits, so let's
	// use a regular expression to check that.
	//
	// we are using POSIX basic notation for regular expressions, see
	//   https://en.cppreference.com/w/cpp/regex/ecmascript
	// where [:d:] means digit, [[:d:]] means match a single digit.
	// The + means match the preceding sub-expression 1 or more times.
	//
	regex pattern("[[:d:]]+", regex::ECMAScript);

	smatch matchResults;  // provides more details on the match

	if (regex_match(s, matchResults, pattern))
		return true;
	else
		return false;
}


//
// Hash
// 
// Hashes the given date "mm-dd-year" and returns an array index.
// If the date falls within the range "01/01/2000" .. "12/31/2099", 
// inclusive, a valid array index is returned in the range 0..N-1.
// If the date falls outside this range, or the date is not formatted
// mm/dd/yyyy, then -1 is returned.  If N >= 37200 then the function 
// is a perfect hash function; if N < 37200 then collisions will occur.
// 
// NOTE: the actual date is not validated, e.g. the function does not
// validate that 02/29/2020 is a leap year.  Validation is limited to 
// checking that the format is mm/dd/yyyy, where 1<=mm<=12, 1<=dd<=31,
// and 2000<=yy<=2099.
// 
// NOTE: the total # of combinations is 365.25 days per year * 100 total
// years.  That's 36,525 possible permutations.  We don't need to be that
// precise.  To simplify things, we'll assume 12 months per year, 31 days
// per month, across 100 years.  That's 12 * 31 * 100 = 37,200 possible
// permutations.  If N >= 37,200, the hash function is perfect and will
// return a value in the range -1 .. 37199, inclusive.
//
int Hash(string input, int N)
{
	int hashNum;

    if(!isNumeric(input) && input.length() > 3){
        return -1;
    }

    hashNum = string2int(input);

    int index = hashNum % N;

    return index;

}


size_t HashStationAbbrev(string input, int N){
    unsigned long long hash = 10000;

    for(char c: input){
        hash = ((hash << 5) + hash) + c;
    }

    return hash % N;
}

int HashTripID(string input, int N){
	// Tr 10 426 561
	// Tr 10 423 693
	// 

    string trLetters = input.substr(0,2);

    if(trLetters != "Tr"){
        return -1;
    }
    if(input.length() < 3){
        return -1;
    }

    string nums = input.substr(2,input.length()-1);

    int hashed = string2int(nums);
    int index = hashed % N;

    return index;
}


int HashBikeID(string ID, int N)
{


	// B5218
	
	string letter = ID.substr(0, 1);  // mm/...
	string numbers = ID.substr(1, ID.length()-1);  // mm/dd/...

	
	int num = stoi(numbers);

	return num % N;
}
