#ifndef _BOW_H_
#define _BOW_H_
#include <vector>
#include <string>
#include "english_stem.h"
#include "stemming.h"
#include <fstream>
#include "TXL.h"

using namespace std;

class BOW
{
public:
	BOW(string input_doc, string stopword);
	~BOW();
	void tf_idf(string doc, double* v);
	int* timKiem(string doc, string mode);

	int n_doc;
	int n_feat;
	vector<string> docs;
	vector<string> features;

	double* idf_feat;
	double** tfidf_doc;

private:
	TXL txl;
	int round_num;
	
	void QuickSort_inc(double* Data, int l, int r, int* top);
	void QuickSort_dec(double* Data, int l, int r, int* top);
};

double dotuongtu_e(double* k, double* d, int n);
double dotuongtu_c(double* k, double* d, int n);

#endif
