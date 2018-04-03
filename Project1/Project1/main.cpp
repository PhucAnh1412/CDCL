#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <string>
#include <iomanip>
#include "BOW.h"
#include "TXL.h"

using namespace std;

void tienxuly(string input, string output)
{
	TXL t("data\\stopwords.txt");
	ifstream ifile(input);
	ofstream ofile(output);

	if (ifile)
	{
		string line;
		while (getline(ifile, line))
		{
			string doc = t.tienXuLy(line);
			ofile << doc;
			if (doc != "" && !ifile.eof()) ofile << endl;
		}
	}

	ifile.close();
	ofile.close();
}

void bow_tfidf(string input, string output, string featurelist, string round)
{
	ofstream ofile(output);
	ofstream ofeat(featurelist);
	
	int round_num = 3;
	ifstream iround(round);
	if (iround) iround >> round_num;
	iround.close();

	ofile << fixed;
	ofeat << fixed;

	BOW bag(input, "data\\stopwords.txt");

	for (int i = 0; i < bag.n_feat; ++i)
	{
		ofeat << bag.features[i] << setprecision(round_num) << " " << bag.idf_feat[i];
		if (i < bag.n_feat - 1) ofeat << endl;
	}

	for (int i = 0; i < bag.n_doc; ++i)
	{
		for (int j = 0; j < bag.n_feat; ++j)
		{
			ofile << setprecision(round_num) << bag.tfidf_doc[i][j];
			if (j < bag.n_feat - 1) ofile << " ";
		}
		if (i < bag.n_doc - 1) ofile << endl;
	}
	
	ofile.close();
	ofeat.close();
	/*ifstream isearch("data\\search.txt");
	ofstream oresult("data\\searchresult.txt");
	oresult << fixed;
	string s;
	int k;
	if (isearch)
	{
		getline(isearch, s);
		isearch >> k;
	}
	double* dtt_doc = new double[bag.n_doc];
	int* top = bag.timKiem(s, "data\\mode.txt", dtt_doc);
	for (int i = 0; i < k; ++i)
	{
		oresult << bag.docs[top[i]] << " " << setprecision(round_num) << dtt_doc[i] << endl;
	}

	isearch.close();
	oresult.close();
	delete[] top;
	delete[] dtt_doc;*/
}


int main()
{
	tienxuly("data\\text.txt", "data\\outtext.txt");
	bow_tfidf("data\\outtext.txt", "data\\tf_idf.txt", "data\\featlist.txt", "data\\roundnumber.txt");
	return 0;
}