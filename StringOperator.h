#pragma once
#include <bits/stdc++.h>
using std::string;
using std::vector;

namespace My
{
extern vector<string> Split(const string &, const char &, const string &bef = "");

extern int StrToUInt(string const &);

extern string UIntToStr(int);

extern vector<string> CutBetween(const string &str, const char left, const char right);

extern string Replace(const string &str, const char a, const string &b);

extern string MergeVec(const vector<string> &strs, const string &split = "\t", const string &end = "\n");

template <class T>
extern int posInVec(const vector<T> &vec, T x)
{
	int pos = 0;
	for (auto i : vec)
	{
		if (i == x)
		{
			return pos;
		}
		pos++;
	}
}
} // namespace My