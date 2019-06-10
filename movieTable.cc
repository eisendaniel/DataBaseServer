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

string movieTable::display(unsigned int rowN) const
{
	if (rowN >= rows()) { // check to ensure passed index is with the used rows
		return "ERROR: invalid row index\n";
	}
	ostringstream out; //stream to construct movie to_string
	out << table.at(rowN).title << " | "
	    << table.at(rowN).director << " | "
	    << table.at(rowN).year << " | "
	    << table.at(rowN).oscar << " | "
	    << table.at(rowN).country << '\n';
	return out.str();
}
bool movieTable::add(movie &addition)
{
	unsigned int previous = rows(); //size before add attempt
	table.push_back(addition); //add movie to end of vector
	return previous < rows(); //return if the table grew or not
}

bool movieTable::loadCSV(const char *infn)
{
	ifstream input(infn); //open file to read
	string num, title, year, director, country;
	vector<movie> additions; //vector of movies to add

	if (!input) { //checks if file opened
		cerr << "ERROR: Failed to open " << infn << endl;
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
			a.oscar = stoul(num); //converts string -> ulong, assigned to field
			strcpy(a.country, country.c_str()); //copies country(converted) to field

			additions.push_back(a); //add to additions vector
		}
	}
	catch (...) { //if for what ever reason, parsing failed, say so and return
		cerr << "Error parsing file, check formatting\n";
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