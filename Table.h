#pragma once
#include <bits/stdc++.h>
using namespace std;
class User;
class Table
{
  public:
	static unordered_map<string, Table *> all_name;
	static unordered_map<string, Table *> all_txtName;

  public:
	void saveToFile();
	static void loadFromFile();
	static void showTables(User *user);
	int getSize() { return m_info.size(); }
	int getLength() { return m_info[0].size(); }
	void insertValue(const string &info);
	//string insertValue(const string &info, const string &pos);
	void findAndDelete(const string &str);
	void select(const string &str);
	static void creatTable(const string &filename, const string &name, User *user);
	static void creatTable(const vector<string> &keys, const string &name, const string &filename, User *user);
	static void deleteTable(const string &name);

  private:
	Table(const string &filename, const string &name, User *user);
	Table(const vector<string> &keys, const string &name, const string &filename, User *user);
	Table(Table &&) = delete;
	Table(const Table &) = delete;
	Table &operator=(Table &&) = delete;
	Table &operator=(const Table &) = delete;
	~Table();
	string m_name;
	string m_txtName;
	vector<vector<string>> m_info;
	unordered_map<string, int> m_where;

	User *m_owner;
};
