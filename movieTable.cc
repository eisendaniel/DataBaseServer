//
// Created by Daniel on 5/06/2019.
//

#include "movieTable.hh"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;
using namespace table;

unsigned int movieTable::rows() const
{ return table.size(); }

string movieTable::display(movie &dis) const
{
	ostringstream out;
	out << dis.title << " | "
	    << dis.director << " | "
	    << dis.year << " | "
	    << dis.num << " | "
	    << dis.country << '\n';
	return out.str();
}
bool movieTable::add(movie &addition)
{

	unsigned int previous = rows(); //size before add attempt
	table.push_back(addition); //add movie to end of vector
	return previous < rows(); //return if the table grew or not
}

movie *movieTable::get(unsigned int rowN)
{
	if (rowN >= rows()) { // check to ensure passed index is with the used rows
		//cout << "Error: Row " << rowN << " does not exist." << endl;
		return nullptr;
	}
	return &table.at(rowN);
}

bool movieTable::loadCSV(const char *infn)
{
	ifstream input(infn); //open file to read
	string num, title, year, director, country;
	vector<movie> additions; //vector of movies to add

	if (!input) { //checks if file opened
		cout << "ERROR: Failed to open " << infn << endl;
		return false;
	}
	try { //attempts parsing
		while (!input.eof()) {
			//stores input from stream into a specified string until the specified delimiter
			//breaks if gets empty line, breaks
			if (!getline(input, title, ',')) { break; }
			if (!getline(input, director, ',')) { break; }
			if (!getline(input, year, ',')) { break; }
			if (!getline(input, num, ',')) { break; }
			if (!getline(input, country, '\n')) { break; }

			struct movie a = {}; //empty movie

			strcpy(a.title, title.c_str()); //copies title(converted) to field
			strcpy(a.director, director.c_str()); //copies director(converted) to field
			a.year = (unsigned short)stoul(year); //converts string -> ulong cast and assigned to field
			a.num = stoul(num); //converts string -> ulong, assigned to field
			strcpy(a.country, country.c_str()); //copies country(converted) to field

			additions.push_back(a); //add to additions vector
		}
	}
	catch (...) { //if for what ever reason, parsing failed, say so and return
		cout << "Error parsing file, check formatting\n";
		return false;
	}
	//if parsing was successful add them to the table
	for (auto i : additions) {
		add(i);
	}
	//close file and clear vector
	input.close();
	additions.clear();
	return true;
}