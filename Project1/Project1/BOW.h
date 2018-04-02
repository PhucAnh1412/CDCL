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
	BOW(string input_doc, string round, string stopword);
	~BOW();
	void tf_idf(string doc, double* v);
	void timKiem(string doc, int k);
private:
	TXL txl;
	double* idf_feat;
	double** tfidf_doc;
	int round_num;
	int n_doc;
	int n_feat;
	vector<string> docs;
	vector<string> features;
};

double dotuongtu_e(double* k, double* d, int n)
{
	double t = 0;
	for (int i = 0; i < n; ++i)
	{
		t = t + (k[i] - d[i])*(k[i] - d[i]);
	}

	return sqrt(t);
}
double dotuongtu_c(double* k, double* d, int n)
{
	double ddk = 0, ddd = 0, t = 0;
	for (int i = 0; i < n; ++i)
	{
		t = t + k[i] * d[i];
		ddk = ddk + k[i] * k[i];
		ddd = ddd + d[i] * d[i];
	}
	return (t / (sqrt(ddk)*sqrt(ddd)));
}
#endif
