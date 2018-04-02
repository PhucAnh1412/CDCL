#include "BOW.h"
#include <unordered_set>
#include <sstream>

BOW::BOW(string input, string round, string stopword) : txl(stopword)
{
	n_doc = 0;
	n_feat = 0;

	// doc round number
	ifstream iround(round);
	if (iround) iround >> round_num;
	iround.close();
	
	// doc cac dong van ban da duoc tien xu ly
	ifstream ifile(input);
	if (ifile)
	{
		string line;
		while (getline(ifile, line))
		{
			docs.push_back(line);
			++n_doc;
		}
	}
	ifile.close();

	// doc cac features
	ifile.open(input);
	unordered_set<string> set_feat;
	if (ifile)
	{ 
		string str;
		while (ifile >> str)
		{
			if (set_feat.find(str) == set_feat.end())
				set_feat.insert(str);
		}
	}
	n_feat = set_feat.size();

	for (auto str : set_feat)
	{
		features.push_back(str);
	}
	ifile.close();

	// khoi tao mang idf_feat
	idf_feat = new double[n_feat];
	for (int i = 0; i < n_feat; ++i) idf_feat[i] = 0;

	for (int i = 0; i < n_feat; ++i)
	{
		for (int j = 0; j < n_doc; ++j)
		{
			if (docs[j].find(features[i]) != string::npos) // tim thay feature i trong doc j
			{
				++idf_feat[i];
			}
		}
		idf_feat[i] = log10((n_doc / idf_feat[i]));
	}
	
	//khoi tao mang chua tf_idf cho cac doc cua input
	tfidf_doc = new double*[n_doc];
	for (int i = 0; i < n_doc; ++i) tf_idf(docs[i], tfidf_doc[i]);
}

BOW::~BOW()
{
	delete[] idf_feat;
	for (int i = 0; i < n_doc; ++i) delete[] tfidf_doc[i];
	delete[] tfidf_doc;
}

void BOW::tf_idf(string doc, double* v)
{
	v = new double[n_feat];

	stringstream ss;
	ss.str(doc);
	int* arr_count = new int[n_feat];

	for (int i = 0; i < n_feat; ++i)
	{
		arr_count[i] = 0;
	}
	int max_count = 0;
	string w;
	while (ss >> w)
	{
		for (int i = 0; i < n_feat; ++i)
		{
			if (features[i].compare(w) == 0)
			{
				++arr_count[i];
				if (max_count < arr_count[i])
					max_count = arr_count[i];

				break;
			}
		}
	}

	for (int i = 0; i < n_feat; ++i)
		v[i] = (double)arr_count[i] / max_count * idf_feat[i];
}

void BOW::timKiem(string doc, int k)
{
	string s = txl.tienXuLy(doc); // txl van ban k
	double *vk;
	tf_idf(s, vk); // tinh tf_idf cho vb k
	// tim k ket qua tuong tu
	/*...*/
}