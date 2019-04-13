#pragma once
#include <bits/stdc++.h>
#include "Table.h"
using namespace std;
class User;

struct TreeNode
{
	unordered_set<User *> p;
	unordered_set<User *> s;
	TreeNode()
	{
		p = unordered_set<User *>();
		s = unordered_set<User *>();
	}
};

class User
{
	friend class Table;

private:
	User(const string &, const string &);
	User(User &&) = delete;
	User(const User &) = delete;
	User &operator=(User &&) = delete;
	User &operator=(const User &) = delete;
	~User();

public:
	static unordered_map<string, User *> all_user;
	static void init();
	static void save();
	static bool notpar(TreeNode const &t, User *);
	void revokedrop(const string &);
	void revokedel(const string &);
	void revokeins(const string &);
	void revokesel(const string &);
	void revoke(const string &);
	void grant(const string &);

private:
	string m_name;
	string m_password;
	unordered_map<string, TreeNode> Drop;
	unordered_map<string, TreeNode> Insert;
	unordered_map<string, TreeNode> Delete;
	unordered_map<string, TreeNode> Select;
};
