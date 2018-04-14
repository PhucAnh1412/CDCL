#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "BOW.h"
#include "TXL.h"

namespace fs = std::experimental::filesystem;
using namespace std;
class PhanLoai
{
public:
	string name;
	int doc_min;
	int doc_max;
private:
};

class DGLop
{
public:
	string name;
	
	int after_count;
	int before_count;

	int acc_count;

	double p;
	double r;
	double f;
private:
};

string tachtenclass(string s) // e.g: s = "test.txt"
{
	string str = "";
	int i = 0;
	while (s[i] != '.')
	{
		str = str + s[i];
		++i;
	}

	return str;
}
string tachtenfolder(string s) // e.g: s = "./class/data"
{
	string str = "\\";

	int i = s.length()  - 1;
	while (!(s[i- 1] == '.' && s[i] == '/'))
	{
		if (s[i] == '/') str = "\\" + str;
		else str = s[i] + str;
		--i;
	}
	return str;
}
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
void bow_tfidf(string output, string featurelist, string round, BOW &bag)
{
	ofstream ofile(output);
	ofstream ofeat(featurelist);
	
	int round_num = 3;
	ifstream iround(round);
	if (iround) iround >> round_num;
	iround.close();

	ofile << fixed;
	ofeat << fixed;

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
	int* top = bag.timKiem(s, mode, dtt_doc);
	for (int i = 0; i < k; ++i)
	{
		oresult << bag.docs[top[i]] << " " << setprecision(round_num) << dtt_doc[i] << endl;
	}

	isearch.close();
	oresult.close();
	delete[] top;
	delete[] dtt_doc;*/
}
void phanloai(string classfolder, string testinput, string result, string mode)
{
	// quet class data va gan label cho cac documents
	string folderName = tachtenfolder(classfolder);
	vector<PhanLoai> vClass;
	ofstream otempfile("data\\text.txt");
	int doc_count = 0;
	for (auto &p : fs::directory_iterator(classfolder))
	{
		PhanLoai temp; // 1 class co name va so thu tu cac doc
		temp.name = tachtenclass(p.path().filename().string());
		temp.doc_min = doc_count;

		string file = folderName + p.path().filename().string();
		ifstream ifile(file);
		if (ifile)
		{
			string line;
			while (getline(ifile, line))
			{
				otempfile << line;
				temp.doc_max = doc_count;
				otempfile << endl;
				++doc_count;
			}
		}
		ifile.close();
		vClass.push_back(temp);
	}

	otempfile.close();
	////////////////////////////////
	// tien hanh phan loai
	ifstream itest(testinput);
	ofstream ofile(result);
	// tinh tf_idf cua input
	BOW bag("data\\text.txt", "data\\stopwords.txt");
	bow_tfidf("data\\tf_idf.txt", "data\\featurelist.txt", "data\\roundnumber.txt", bag);
	
	vector<string> vTest;
	int k;
	// doc test input
	if (itest)
	{
		string line;
		while (getline(itest, line)) vTest.push_back(line);
	}
	itest.close();

	k = atoi(vTest.back().c_str());

	vTest.pop_back();

	// lay nhan da duoc danh dau tu bo test
	vector<string> vLabel_before;
	for (int i = 0; i < vTest.size(); ++i)
	{
		string temp = "";
		int j = vTest[i].length() - 1;
		while (vTest[i][j] != ' ')
		{
			temp = vTest[i][j] + temp;
			vTest[i].erase(vTest[i].length() - 1);
			--j;
		}
		vTest[i].erase(vTest[i].length() - 1);
		vLabel_before.push_back(temp);
	}

	vector<DGLop> vDG;
	for (int i = 0; i < vClass.size(); ++i)
	{
		DGLop temp;
		temp.name = vClass[i].name;
		temp.before_count = 0;
		for (int j = 0; j < vLabel_before.size(); ++j) // dem so van ban thuoc lop i truoc khi phan loai
		{
			if (temp.name.compare(vLabel_before[j]) == 0)
				++temp.before_count;
		}
		vDG.push_back(temp);
	}
	// tim kiem
	vector<string> vLabel_after;
	int* class_count = new int[vClass.size()];
	for (int i = 0; i < vTest.size(); ++i)
	{
		int* top = bag.timKiem(vTest[i], mode);

		int max_count = 0;
		int max_pos = 0;
		for (int m = 0; m < vClass.size(); ++m) class_count[m] = 0;

		for (int j = 0; j < k; ++j) // voi moi vb dj, kiem tra vb dj thuoc class nao
		{
			for (int m = 0; m < vClass.size(); ++m)
			{
				if (top[j] >= vClass[m].doc_min && top[j] <= vClass[m].doc_max)
				{
					++class_count[m];
					if (class_count[m] > max_count)
					{
						max_count = class_count[m];
						max_pos = m;
					}
					break;
				}
			}
		}

		vLabel_after.push_back(vClass[max_pos].name);
	}
	// ~Doan nay chua duoc tot lam~
	for (int i = 0; i < vDG.size(); ++i)
	{
		vDG[i].acc_count = 0;
		vDG[i].after_count = 0;
		for (int j = 0; j < vLabel_after.size(); ++j)
		{
			if (vLabel_after[j].compare(vDG[i].name))
			{
				++vDG[i].after_count;
				if (vLabel_after[j].compare(vLabel_before[j]) == 0)
					++vDG[i].acc_count;
			}
		}

		vDG[i].p = (double)vDG[i].acc_count / vDG[i].after_count;
		vDG[i].r = (double)vDG[i].acc_count / vDG[i].before_count;
		vDG[i].f = 2 * vDG[i].p*vDG[i].r / (vDG[i].p + vDG[i].r);
	}
	int C_count = vClass.size();
	double p_macro = 0, r_macro = 0, f_macro = 0, f_micro = 0;
	for (int i = 0; i < C_count; ++i)
	{
		p_macro += vDG[i].p;
		r_macro += vDG[i].r;
		f_micro += vDG[i].acc_count;
	}
	p_macro = p_macro / C_count;
	r_macro = r_macro / C_count;
	f_macro = 2 * p_macro*r_macro / (p_macro + r_macro);
	f_micro = f_micro / (double)vTest.size();

	for (int i = 0; i < vTest.size(); ++i)
		ofile << vTest[i] << " " << vLabel_after[i] << endl;
	for (int i = 0; i < vDG.size(); ++i)
	{
		ofile << "P" << " " << vDG[i].name << ": " << vDG[i].p * 100 << endl;
		ofile << "R" << " " << vDG[i].name << ": " << vDG[i].r * 100 << endl;
		ofile << "F" << " " << vDG[i].name << ": " << vDG[i].f * 100 << endl;
	}

	ofile << "F_macro: " << f_macro * 100 << endl;
	ofile << "F_micro: " << f_micro * 100;

	delete[] class_count;
	ofile.close();
}

int main()
{
	/*tienxuly("data\\text.txt", "data\\outtext.txt");
	bow_tfidf("data\\outtext.txt", "data\\tf_idf.txt", "data\\featlist.txt", "data\\roundnumber.txt", "data\\mode.txt");*/
	phanloai("./class", "test\\test.txt", "test\\result.txt", "test\\mode.txt");
	return 0;
}