#include "TXL.h"
#include <sstream>

TXL::TXL(string stopword)
{

	ifstream istop(stopword);
	if (istop)
	{
		string str;
		while (istop >> str)
		{
			str = TXL::toLower(str);
			if (!stopWordFound) stopWords.push_back(str);
		}
	}
	istop.close();
}

TXL::~TXL()
{}

string TXL::toLower(string s)
{
	string str = "";
	for (int i = 0; i < s.length(); ++i)
	{
		str = str + (char)tolower(s[i]);
	}

	return str;
}

bool TXL::stopWordFound(string s)
{
	for (int i = 0; i < stopWords.size(); ++i)
	{
		if (stopWords[i].find(s) != string::npos) return true;
	}
	return false;
}

string TXL::tienXuLy(string doc)
{
	stringstream ss;
	doc = TXL::toLower(doc);

	ss.str(doc); // Tao string stream tu 1 dong
	string tempWord, tempLine = "";
	while (ss >> tempWord) // Doc 1 tu cua dong
	{
		if (!stopWordFound(tempWord))
		{
			int i = 0;
			bool bo_tu = false;
			while (i < tempWord.length())
			{
				if (!('a' <= tempWord[i] && tempWord[i] <= 'z'))
				{
					tempWord.erase(tempWord.begin() + i);
					if (stopWordFound(tempWord)) {
						bo_tu = true;
						break;
					}
				}
				else ++i;
			}
			if (!bo_tu && tempWord.length())
			{
				Stem(tempWord);
				tempLine = tempLine + tempWord + " ";
			}
		}
	}

	while (tempLine.length() > 0 && tempLine[tempLine.length() - 1] == ' ')
	{
		tempLine.erase(tempLine.length() - 1);
	}

	return tempLine;
}