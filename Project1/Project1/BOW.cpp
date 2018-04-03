#include "BOW.h"
#include <unordered_set>
#include <sstream>

BOW::BOW(string input_doc, string stopword) : txl(stopword)
{
	n_doc = 0;
	n_feat = 0;

	// doc cac dong van ban da duoc tien xu ly
	ifstream ifile(input_doc);
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
	ifile.open(input_doc);
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
	for (int i = 0; i < n_doc; ++i)
	{
		tfidf_doc[i] = new double[n_feat];
		tf_idf(docs[i], tfidf_doc[i]);
	}
}

BOW::~BOW()
{
	delete[] idf_feat;
	for (int i = 0; i < n_doc; ++i) delete[] tfidf_doc[i];
	delete[] tfidf_doc;
}

void BOW::tf_idf(string doc, double* v)
{
	stringstream ss;
	ss.str(doc);
	int* arr_count = new int[n_feat];

	for (int i = 0; i < n_feat; ++i) arr_count[i] = 0;
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
	
	delete[] arr_count;
}

void BOW::QuickSort_inc(double* Data, int l, int r, int* top)
{
	if (l <= r)
	{
		double key = Data[(l + r) / 2];

		int i = l;
		int j = r;

		while (i <= j)
		{
			while (Data[i] < key)
				i++;
			while (Data[j] > key)
				j--;

			if (i <= j)
			{
				swap(Data[i], Data[j]);
				swap(top[i], top[j]);
				i++;
				j--;
			}
		}

		if (l < j)
			QuickSort_inc(Data, l, j, top);
		if (r > i)
			QuickSort_inc(Data, i, r, top);
	}
}

void BOW::QuickSort_dec(double* Data, int l, int r, int* top)
{
	if (l <= r)
	{
		double key = Data[(l + r) / 2];

		int i = l;
		int j = r;

		while (i <= j)
		{
			while (Data[i] > key)
				i++;
			while (Data[j] < key)
				j--;

			if (i <= j)
			{
				swap(Data[i], Data[j]);
				swap(top[i], top[j]);
				i++;
				j--;
			}
		}

		if (l < j)
			QuickSort_dec(Data, l, j, top);
		if (r > i)
			QuickSort_dec(Data, i, r, top);
	}
}
int* BOW::timKiem(string doc, string mode)
{
	char m;
	ifstream imode(mode);
	if (imode) imode >> m;

	string s = txl.tienXuLy(doc); // txl van ban k
	double *vk = new double[n_feat];
	tf_idf(s, vk); // tinh tf_idf cho vb k

	// tim k ket qua tuong tu
	double* dtt_doc = new double[n_doc];
	int* top_docs = new int[n_doc]; // mang top de dung cho viec sap xep lay n vector tuong tu nhat voi vector k
	for (int i = 0; i < n_doc; ++i)
	{
		dtt_doc[i] = m == 'e'?dotuongtu_e(vk, this->tfidf_doc[i], n_feat): dotuongtu_c(vk, this->tfidf_doc[i], n_feat); // tinh do tuong tu cua vector k voi tung vector d[i]
		top_docs[i] = i; // khoi tao mang top
	}

	m == 'e' ? QuickSort_inc(dtt_doc, 0, n_doc - 1, top_docs) : QuickSort_dec(dtt_doc, 0, n_doc - 1, top_docs);

	delete[] vk;
	delete[] dtt_doc;

	return top_docs;
}

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
}