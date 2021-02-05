/*main.cpp*/

//
// Vincent Weaver
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "hash.h"
#include "hashmap.h"
#include "util.h"

using namespace std;


//
// string2int
// 
// Converts a string to an integer, unless string is empty, in
// which case 0 is returned.
// 
int string2int(string s)
{
	if (s == "")
		return 0;
	else
		return stoi(s);
}



// ------------------------------------------------------------------------------------------------------

//
// TripData:
// 
// trip id: string consisting of “Tr” following by 1 or more digits
// start time: date and time of trip start
// stop time: data and time of trip end
// bike id: string consisting of “B” followed by 1 or more digits
// duration: integer, in seconds
// from: integer, station where bike was checked out
// to: integer, station where bike was returned
// identifies: gender the rider identifies as (string, optional)
// birthyear: the rider’s year of birth (integer, optional)
// 
struct TripData
{
	string TripID;
	string StartTime;
	string EndTime;
	string BikeID;
	string Duration;
	string From;
	string To;
	string Identifies;
	string BirthYear;
	
	TripData()
	{
		TripID = "";
		StartTime = "";
		EndTime = "";
		BikeID = "";
		Duration = "";
		From = "";
		To = "";
		Identifies = "";
		BirthYear = "";
	}
};

struct BikeData
{
	string BikeID;
	int Duplicates;
	
	BikeData()
	{
		BikeID = "";
		Duplicates = 0;
	}
};

struct StationData
{
	string ID;
	string abbrev;
	string fullname;
	string lat;
	string lon;
	string capacity;
	string date;
	
	
	StationData()
	{
		ID = "";
		abbrev = "";
		fullname = "";
		lat = "";
		lon = "";
		capacity = "";
		date = "";

	}
};
// ---------------------------------------------------------------------------------------
// 

bool inputData(string filename, 
			   hashmap<string, BikeData>& bikesById, 
			   hashmap<string, TripData>& tripsById, 
			   hashmap<string, StationData>& stationsById,
			   hashmap<string, string>& stationsByAbbrev)
{
	
	string filenameTrips = ""; // "trips.csv";
	
	cout << "Enter trips file> ";
	cin >> filenameTrips; 
	


	//----------------------------------------------------
	
	ifstream  infile(filename);
	cout << endl;
	cout << "Reading " << filename << endl;
	
	if (!infile.good())
	{
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}
	
	// file is open, start processing:
	string line;
	
	getline(infile, line);  // input and discard first row --- header row
	
	int count = 0;
	
	while (getline(infile, line))
	{
		stringstream s(line);
		
		string ID, abbrev, fullname, lat, lon, capacity, date;
		
		//
		// format: date,xx xx xx xx xx,yy,z
		// 
		// NOTE: z may be missing (the multiplier)
		// 
		getline(s, ID, ',');  // first value => region
		getline(s, abbrev, ',');   // second value => country
		getline(s, fullname, ',');
		getline(s, lat, ',');
		getline(s, lon, ',');
		getline(s, capacity, ',');
		getline(s, date, ',');
		

		
		int indexID = string2int(ID);
		
		StationData sd;
		
		sd.ID = ID;
		sd.abbrev = abbrev;
		sd.fullname = fullname;
		sd.lat = lat;
		sd.lon = lon;
		sd.capacity = capacity;
		sd.date = date;
		
		stationsById.insert(ID, sd, Hash);
		
		stationsByAbbrev.insert(abbrev, sd.ID, HashStationAbbrev);


		count++;
	}
	infile.close();
	
	//---------------------------------------------------------------------------
	//
	//
	
	
	ifstream inf(filenameTrips);
	
	cout << "Reading " << filenameTrips << endl;
	cout << endl;
	if (!inf.good())
	{
		cout << "No data, file not found?" << endl;
		return false;
	}
	string line1;
	
	getline(inf, line1);  // input and discard first row --- header row
	
	int count1 = 0;
	int numBikes = 0;
	
	while (getline(inf, line1))
	{
		stringstream t(line1);
		
		string tripid, starttime, stoptime, bikeid, duration, from, to, identifies, birthyear;
		
		
		getline(t, tripid, ',');  // first value => region
		getline(t, starttime, ',');   // second value => country
		getline(t, stoptime, ',');
		getline(t, bikeid, ',');
		getline(t, duration, ',');
		getline(t, from, ',');
		getline(t, to, ',');
		getline(t, identifies, ',');
		getline(t, birthyear, ',');
		

		TripData td;
		
		td.TripID = tripid;
		td.StartTime = starttime;
		td.EndTime = stoptime;
		td.BikeID = bikeid;
		td.Duration = duration;
		td.From = from;
		td.To = to;
		td.Identifies = identifies;
		td.BirthYear = birthyear;
		
		
		tripsById.insert(tripid, td, HashTripID);
		
		
		
		BikeData bd;
		
		bool findBike = bikesById.search(bikeid, bd, HashBikeID);
		
		if (findBike){

			bd.Duplicates = bd.Duplicates + 1;
		}
		else {
			bd.BikeID = bikeid;
			bd.Duplicates = 1;
				
			numBikes++;
		}
		bikesById.insert(bikeid, bd, HashBikeID);
		

		count1++;
	}
	inf.close();	
	
	
	cout << "# of stations: " << count << endl;
	cout << "# of trips: " << count1 << endl;
	cout << "# of bikes: " << numBikes << endl;
	
	return true;  // we have data to be processed:
}

void command1(StationData sdID, hashmap<string, StationData>& stationsById, string command){
	bool found = stationsById.search(command, sdID, Hash);
	if (found) {
		double lat = stod(sdID.lat);
		double lon = stod(sdID.lon);
		cout << "Station:" << endl;
		cout << " ID: " << sdID.ID << endl;
		cout << " Abbrev: " << sdID.abbrev << endl;
		cout << " Fullname: " << sdID.fullname << endl;
		cout << " Location: (" << lat << ", " << lon << ")" << endl;
		cout << " Capacity: " << sdID.capacity << endl;
		cout << " Online date: " << sdID.date << endl;
	}
	else {
		cout << "station not found" << endl;
	}
}

void command2(StationData sdAB, 
			  hashmap<string, string>& stationsByAbbrev, 
			  hashmap<string, StationData>& stationsById, 
			  string command)
{
	bool findAbbrev = stationsByAbbrev.search(command, sdAB.ID, HashStationAbbrev);
	bool s = stationsById.search(sdAB.ID, sdAB, Hash);
	if (findAbbrev) {
		if (s) {
			double lat = stod(sdAB.lat);
			double lon = stod(sdAB.lon);
			cout << "Station:" << endl;
			cout << " ID: " << sdAB.ID << endl;
			cout << " Abbrev: " << sdAB.abbrev << endl;
			cout << " Fullname: " << sdAB.fullname << endl;
			cout << " Location: (" << lat << ", " << lon << ")" << endl;
			cout << " Capacity: " << sdAB.capacity << endl;
			cout << " Online date: " << sdAB.date << endl;					
		}
	}
	else {
		cout << "station not found" << endl;
	}
}

void command3(TripData td, 
			  StationData sdID,
			  hashmap<string, TripData>& tripsById, 
			  hashmap<string, StationData>& stationsById,
			  string command)
{
	bool findTrip = tripsById.search(command, td, HashTripID);
	if (command.length() > 3 && findTrip == true) {
		// time
		int time = stoi(td.Duration);
		int min = (time / 60);
		int sec = (time % 60);

		// station ID
		stationsById.search(td.From, sdID, Hash);
		string start = sdID.abbrev;

		stationsById.search(td.To, sdID, Hash);
		string finish = sdID.abbrev;

		cout << "Trip:" << endl;
		cout << " ID: " << td.TripID << endl;
		cout << " Starttime: " << td.StartTime << endl;
		cout << " Bikeid: " << td.BikeID << endl;
		cout << " Duration: " << min << " minutes, " << sec << " seconds" << endl;
		cout << " From station: " << start << " (" << td.From << ")" << endl;
		cout << " To station: " << finish << " (" << td.To << ")" << endl;
		cout << " Rider identifies as: " << td.Identifies << endl;
		cout << " Birthyear: " << td.BirthYear << endl;
	}
	else {
		cout << "Trip not found" << endl;
	}
}

void command4(BikeData bd, hashmap<string, BikeData>& bikesById, string command){
	bool findBike = bikesById.search(command, bd, HashBikeID);
	if (command.length() >= 2 && findBike == true) {
		cout << "Bike:" << endl;
		cout << " ID: " << bd.BikeID << endl;
		cout << " Usage: " << bd.Duplicates << endl;
	}
	else {
		cout << "Bike not found" << endl;
	}
}

int main()
{
	cout << "** DIVVY analysis program **" << endl;
	cout << endl;


	hashmap<string, StationData> stationsById(10000); // 10K : station ID and 
	hashmap<string, string> stationsByAbbrev(10000); // 10K : abbreviation and 
	hashmap<string, TripData> tripsById(2500000); // 2.5M : trip ID and 
	hashmap<string, BikeData> bikesById(50000); // 50k : bike ID and num times used
	
	StationData sdID; // = new StationData[10000];
	StationData sdAB; //  = new StationData[10000];
	TripData td; //  = new TripData[2500000];
	BikeData bd; //  = new BikeData[50000];
	//
	// input the winning numbers:
	// 
	
	string filenameStations = ""; // = "stations.csv";
	cout << "Enter stations file> ";
	cin >> filenameStations;
	
	
	
	
	bool success = inputData(filenameStations, 
							   bikesById, 
							   tripsById, 
							   stationsById,
							   stationsByAbbrev);
	
	//
	// did we input anything?
	// 
	if (!success)
	{
		cout << "No data, file not found?" << endl;
		return 0;
	}
	
	//
	// allow the user to lookup winning numbers by date:
	// 
	string command;
	
	getline(cin, command);
	//
	// user testing:
	//
	while (command != "#")
	{
		cout << endl;
		cout << "Please enter a command, help, or #> ";
		getline(cin, command);
		
		
		if (command == "#"){
			break;
		}
		
		if (command == "help"){
			cout << "Available commands:" << endl;
			cout << " Enter a station id (e.g. 341)" << endl;
			cout << " Enter a station abbreviation (e.g. Adler)" << endl;
			cout << " Enter a trip id (e.g. Tr10426561)" << endl;
			cout << " Enter a bike id (e.g. B5218)" << endl;
			cout << " Nearby stations (e.g. nearby 41.86 -87.62 0.5)" << endl;
			cout << " Similar trips (e.g. similar Tr10424639 0.3)" << endl;
			cout << endl;
			
		}
		
		if (isNumeric(command)){ 
			command1(sdID, stationsById, command);
		}
		else if (command.substr(0, 2) == "Tr"){
			command3(td, sdID, tripsById, stationsById, command);
		}
		else if (command.substr(0, 1) == "B"){
			command4(bd, bikesById, command);
		}
// 		else if (command.substr(0, 6) == "nearby"){
// 			double lat;
// 			double lon; 
// 			double dist;
			
// 			getline(command, lat, ' ');
// 			getline(command, lon, ' ');
// 			getline(command, dist);

// 			int counter = 0;
// 			cout << "Stations within " << dist << " miles of (" << lat << ", " << lon << ")" << endl;
// 			for (int index = 0; index < 10000; index++){
// 				if (HT[index].ContainsData == true){  // array location now contains data)
// 					double lat2 = stod(sdID.lat);
// 					double lon2 = stod(sdID.lon);
// 					double range = distBetween2Points(lat, lon, lat2, lon2);
					
// 					if (range <= dist) {
// 						cout << "station " << sdID.ID << ": " << range << " miles" << endl;
// 						counter++;
// 					}
// 				}
// 			}
// 			if (counter == 0){
// 				cout << " none found" << endl;
// 			}
			
// 		}
		else{
			// stationsByAbbrev
			command2(sdAB, stationsByAbbrev, stationsById, command);
		}
		
	} // while loop end
	
	//
	// done!
	// 
}

