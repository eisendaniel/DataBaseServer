//
// Created by Daniel on 5/06/2019.
//

#ifndef _MOVIETABLE_HH_
#define _MOVIETABLE_HH_

#include <vector>
#include <iostream>

using namespace std;
namespace table
{
//base data type to be stores/accessed
struct movie {
	char title[50];
	char director[50];
	unsigned short year;
	unsigned long num;
	char country[50];
};

class movieTable {
 private:
	vector<movie> table; //storage for movies

 public:
	//prototypes to implemented in source
	unsigned int rows() const;
	string display(movie &) const;
	bool add(movie &);
	movie *get(unsigned int);

	//member function prototypes to load and save
	bool loadCSV(const char *);
};
}
#endif //_MOVIETABLE_HH_
