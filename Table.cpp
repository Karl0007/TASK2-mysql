#include "Table.h"
#include "View.h"
#include "StringOperator.h"
using Mysql::View;
unordered_map<string, Table *> Table::all_name;
unordered_map<string, Table *> Table::all_txtName;
Table::~Table()
{
}

void Table::creatTable(const string &filename, const string &name, User *user)
{
	ifstream is;
	is.open(filename);
	if (is.is_open())
	{
		if (all_name.count(name))
		{
			View::getInstance().showMessage("同名数据库", "Error:");
			return;
		}
		if (all_txtName.count(filename))
		{
			View::getInstance().showMessage("该文件已打开", "Error:");
			return;
		}
		auto tmp = new Table(filename, name, user);
		all_name[name] = tmp;
		all_txtName[filename] = tmp;
		tmp->saveToFile();
	}
	else
	{
		View::getInstance().showMessage("找不到文件", "Error:");
		return;
	}
}

void Table::creatTable(const vector<string> &keys, const string &name, const string &filename, User *user)
{
	if (all_name.count(name))
	{
		View::getInstance().showMessage("存在同名数据库", "Error:");
		return;
	}
	if (all_txtName.count(filename))
	{
		View::getInstance().showMessage("存在同名文件", "Error:");
		return;
	}
	auto tmp = new Table(keys, name, filename, user);
	all_name[name] = tmp;
	all_txtName[filename] = tmp;
	tmp->saveToFile();
}

void Table::deleteTable(const string &name)
{
	if (!all_name.count(name))
	{
		View::getInstance().showMessage("该数据库不存在", "Error:");
		return;
	}
	auto tmp = all_name[name];
	all_name.erase(tmp->m_name);
	all_name.erase(tmp->m_txtName);
	system((string("rm ") + tmp->m_txtName).c_str());
	delete tmp;
}

Table::Table(const string &filename, const string &name, User *user) : m_name(name), m_owner(user), m_txtName(filename)
{
	ifstream is(filename);
	char str[200];
	while (is.getline(str, 200))
	{
		m_info.push_back(My::Split(str, '\t'));
	}
}

Table::Table(const vector<string> &keys, const string &name, const string &filename, User *user) : m_name(name), m_owner(user), m_txtName(filename)
{
	//ofstream os(filename);
	vector<string> tmp;
	tmp.push_back("ID");
	for (auto s : keys)
		tmp.push_back(s);
	m_info.push_back(tmp);
	//os << My::MergeVec(tmp);
}

void Table::saveToFile()
{
	ofstream os(m_txtName);
	for (auto t : m_info)
	{
		os << My::MergeVec(t);
	}
}
void Table::loadFromFile()
{
}
void Table::showTables(User *user)
{
}
string Table::insertValue(const string &info)
{
	auto vec = My::CutBetween(info, '(', ')');
	if (vec.size() != 1 && vec.size() != 2)
	{
		View::getInstance().showMessage("非法输入", "Error:");
		return;
	}
	if (vec.size() == 1)
	{
		vec[0] = My::Replace(vec[0], ' ', "");
		m_info.push_back(My::Split(vec[0], ",", My::UIntToStr(m_info.size())));
	}
	else
	{
	}
	saveToFile();
}
// string Table::insertValue(const string &info, const string &pos)
// {
// }
string Table::findAndDelete(const string &key, const string &tofind)
{
}
string Table::deleteLine(const string &key)
{
}
string Table::findByKey(const string &key, bool distinct)
{
}