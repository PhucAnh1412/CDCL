#ifndef _TXL_H_
#define _TXL_H_
#include <vector>
#include <string>
#include "english_stem.h"
#include "stemming.h"
#include <fstream>

using namespace std;

class TXL
{
public:
	TXL(string stopword);
	~TXL();

	
	string tienXuLy(string doc);
private:
	vector<string> stopWords;
	stemming::english_stem<char, std::char_traits<char>> Stem;

	string toLower(string s);
	bool stopWordFound(string s);
};

#endif