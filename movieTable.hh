//
// Created by Daniel on 5/06/2019.
//

#ifndef _MOVIETABLE_HH_
#define _MOVIETABLE_HH_

#include <vector>
#include <iostream>

namespace table
{
//base data type to be stores/accessed
struct movie {
	char title[50];
	char director[50];
	unsigned short year;
	unsigned long oscar;
	char country[50];
};

class movieTable {
 private:
	std::vector<movie> table; //storage for movies

 public:
	//prototypes to implemented in source
	unsigned int rows() const;
	std::string display(unsigned int) const;
	bool add(movie &);

	//member function prototypes to load and save
	bool loadCSV(const char *);
};
}
#endif //_MOVIETABLE_HH_
