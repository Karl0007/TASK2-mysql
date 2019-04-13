#include "User.h"
#include "View.h"
#include "StringOperator.h"
using Mysql::View;

unordered_map<string, User *> User::all_user;

void User::init()
{
	ifstream is("__UserList__");
	if (is.is_open())
	{
		string n, p, str;
		getline(is, str);
		while (str != "__END__")
		{
			stringstream ss(str);
			ss >> n >> p;
			auto tmp = new User(n, p);
			all_user[n] = tmp;
			getline(is, str);
		}
		Table::loadFromFile();
		getline(is, str);
		while (is)
		{
			auto user = all_user[str];
			getline(is, str);
			while (str != "__END__")
			{
				stringstream ss(str);
				string name;
				ss >> name;
				string q, f;
				while (ss >> q >> f)
				{
					//cout << name << q << f << endl;
					if (q == "DROP")
					{
						user->Drop[name].p.insert(all_user[f]);
						all_user[f]->Drop[name].s.insert(user);
					}
					if (q == "INSERT")
					{
						user->Insert[name].p.insert(all_user[f]);
						all_user[f]->Insert[name].s.insert(user);
					}
					if (q == "DELETE")
					{
						user->Delete[name].p.insert(all_user[f]);
						all_user[f]->Delete[name].s.insert(user);
					}
					if (q == "SELECT")
					{
						user->Select[name].p.insert(all_user[f]);
						all_user[f]->Select[name].s.insert(user);
					}
				}
				getline(is, str);
			}
			getline(is, str);
		}
	}
	//cout << Table::all_name.size() << "rwfaw" << endl;
}

void User::revoke(const string &str)
{
	stringstream ss(str);
	string tmp, name, user;
	bool dro = false, del = false, sel = false, ins = false;
	while (ss >> tmp)
	{
		if (tmp == "REVOKE")
		{
		}
		else if (My::Replace(tmp, ',', "") == "SELECT")
		{
			sel = true;
		}
		else if (My::Replace(tmp, ',', "") == "DROP")
		{
			dro = true;
		}
		else if (My::Replace(tmp, ',', "") == "DELETE")
		{
			del = true;
		}
		else if (My::Replace(tmp, ',', "") == "INSERT")
		{
			ins = true;
		}
		else if (tmp == "on")
		{
			ss >> name;
		}
		else if (tmp == "from")
		{
			ss >> user;
		}
	}
	if (!Table::all_name.count(name))
	{
		View::getInstance().showMessage(name, "Error:不存在数据库 ");
		return;
	}
	if (!User::all_user.count(user))
	{
		View::getInstance().showMessage(user, "Error:不存在用户 ");
		return;
	}
	if (dro)
	{
		Drop[name].s.erase(User::all_user[user]);
		User::all_user[user]->Drop[name].p.erase(this);
		User::all_user[user]->revokedrop(name);
	}
	if (ins)
	{
		Insert[name].s.erase(User::all_user[user]);
		User::all_user[user]->Insert[name].p.erase(this);
		User::all_user[user]->revokeins(name);
	}
	if (del)
	{
		Delete[name].s.erase(User::all_user[user]);
		User::all_user[user]->Delete[name].p.erase(this);
		User::all_user[user]->revokedel(name);
	}
	if (sel)
	{
		Select[name].s.erase(User::all_user[user]);
		User::all_user[user]->Select[name].p.erase(this);
		User::all_user[user]->revokesel(name);
	}
}

void User::revokedrop(const string &name)
{
	for (auto i : Drop[name].s)
	{
		i->Drop[name].p.erase(this);
		i->revokedrop(name);
	}
	Drop[name].s.clear();
}

void User::revokedel(const string &name)
{
	for (auto i : Delete[name].s)
	{
		i->Delete[name].p.erase(this);
		i->revokedel(name);
	}
	Delete[name].s.clear();
}

void User::revokesel(const string &name)
{
	for (auto i : Select[name].s)
	{
		i->Select[name].p.erase(this);
		i->revokesel(name);
	}
	Select[name].s.clear();
}

void User::revokeins(const string &name)
{
	for (auto i : Insert[name].s)
	{
		i->Insert[name].p.erase(this);
		i->revokeins(name);
	}
	Insert[name].s.clear();
}

void User::grant(const string &str)
{
	stringstream ss(str);
	string tmp, name, user;
	bool dro = false, del = false, sel = false, ins = false;
	while (ss >> tmp)
	{
		if (tmp == "GRANT")
		{
		}
		else if (My::Replace(tmp, ',', "") == "SELECT")
		{
			sel = true;
		}
		else if (My::Replace(tmp, ',', "") == "DROP")
		{
			dro = true;
		}
		else if (My::Replace(tmp, ',', "") == "DELETE")
		{
			del = true;
		}
		else if (My::Replace(tmp, ',', "") == "INSERT")
		{
			ins = true;
		}
		else if (tmp == "on")
		{
			ss >> name;
		}
		else if (tmp == "to")
		{
			ss >> user;
		}
	}
	if (!Table::all_name.count(name))
	{
		View::getInstance().showMessage(name, "Error:不存在数据库 ");
		return;
	}
	if (!User::all_user.count(user))
	{
		View::getInstance().showMessage(user, "Error:不存在用户 ");
		return;
	}
	if (dro)
	{
		if (Drop.count(name) && Drop[name].p.size() && this != all_user[user])
		{
			Drop[name].s.insert(User::all_user[user]);
			User::all_user[user]->Drop[name].p.insert(this);
		}
		else
		{
			View::getInstance().showMessage("DROP", "Error:权限错误 ");
		}
	}

	if (sel)
	{
		if (Select.count(name) && Select[name].p.size() && this != all_user[user])
		{
			Select[name].s.insert(User::all_user[user]);
			User::all_user[user]->Select[name].p.insert(this);
		}
		else
		{
			View::getInstance().showMessage("SELECT", "Error:权限错误 ");
		}
	}

	if (del)
	{
		if (Delete.count(name) && Delete[name].p.size() && this != all_user[user])
		{
			Delete[name].s.insert(User::all_user[user]);
			User::all_user[user]->Delete[name].p.insert(this);
		}
		else
		{
			View::getInstance().showMessage("DELETE", "Error:权限错误 ");
		}
	}
	if (ins)
	{
		if (Insert.count(name) && Insert[name].p.size() && this != all_user[user])
		{
			Insert[name].s.insert(User::all_user[user]);
			User::all_user[user]->Insert[name].p.insert(this);
		}
		else
		{
			View::getInstance().showMessage("INSERT", "Error:权限错误 ");
		}
	}
}

bool User::notpar(TreeNode const &t, User *u)
{
}

void User::save()
{
	ofstream os("userlist");
	for (auto &u : all_user)
	{
		os << u.second->m_name << " " << u.second->m_password << endl;
	}
	os << "__END__" << endl;
	for (auto &u : all_user)
	{
		if (u.first == "__ADMIN__")
			continue;
		os << u.first << endl;
		for (auto &t : Table::all_name)
		{
			if (u.second->Drop[t.first].p.size() || u.second->Delete[t.first].p.size() || u.second->Insert[t.first].p.size() || u.second->Select[t.first].p.size())
			{
				os << t.first << " ";
				for (auto x : u.second->Drop[t.first].p)
					os << "DROP " << x->m_name << " ";
				for (auto x : u.second->Delete[t.first].p)
					os << "DELETE " << x->m_name << " ";
				for (auto x : u.second->Insert[t.first].p)
					os << "INSERT " << x->m_name << " ";
				for (auto x : u.second->Select[t.first].p)
					os << "SELECT " << x->m_name << " ";
				os << endl;
			}
		}
		os << "__END__" << endl;
	}
}
User::User(string const &n, string const &p) : m_name(n), m_password(p) {}
User::~User() {}
