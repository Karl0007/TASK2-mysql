#include "StringOperator.h"

namespace My
{
extern vector<string> Split(const string &_str, const char &_ch, const string &bef)
{
	vector<string> res;
	if (bef != "")
		res.push_back(bef);
	string tmp;
	for (int i = 0; i < _str.size(); i++)
	{
		if (_str[i] == _ch)
		{
			if (tmp.size() != 0)
			{
				res.push_back(tmp);
				tmp.clear();
			}
		}
		else
		{
			tmp += _str[i];
		}
	}
	if (tmp.size())
		res.push_back(tmp);
	return res;
}

vector<string> CutBetween(const string &str, const char left, const char right)
{
	vector<string> ans;
	string tmp = "";
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == left)
		{
			i++;
			while (str[i] != right)
			{
				tmp += str[i];
				if (++i >= str.size())
					return ans;
			}
			ans.push_back(tmp);
			tmp = "";
		}
	}
	return ans;
}

string Replace(const string &str, const char a, const string &b)
{
	string ans = "";
	for (auto c : str)
	{
		if (c == a)
		{
			ans += b;
		}
		else
		{
			ans += c;
		}
	}
	return ans;
}

string MergeVec(const vector<string> &strs, const string &split, const string &end)
{
	string ans;
	for (int i = 0; i < strs.size(); i++)
	{
		ans += strs[i];
		if (i != strs.size() - 1)
			ans += split;
	}
	ans += end;
	return ans;
}

extern int StrToUInt(string const &_str)
{
	int res = 0;
	for (auto ch : _str)
	{
		if (ch >= '0' && ch <= '9')
			res = res * 10 + ch - '0';
	}
	return res;
}

extern string UIntToStr(int _x)
{
	string res;
	while (_x)
	{
		res += _x % 10 + '0';
		_x /= 10;
	}
	std::reverse(res.begin(), res.end());
	if (res.empty())
		res = "0";
	return res;
}
} // namespace My