#include "Table.h"
#include "View.h"
#include "User.h"
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
			View::getInstance().showMessage(name, "Error:同名数据库 ");
			return;
		}
		if (all_txtName.count(filename))
		{
			View::getInstance().showMessage(filename, "Error:该文件已打开 ");
			return;
		}
		auto tmp = new Table(filename, name, user);
		all_name[name] = tmp;
		all_txtName[filename] = tmp;
		user->Drop[name].p.insert(User::all_user["__ADMIN__"]);
		user->Delete[name].p.insert(User::all_user["__ADMIN__"]);
		user->Insert[name].p.insert(User::all_user["__ADMIN__"]);
		user->Select[name].p.insert(User::all_user["__ADMIN__"]);
		tmp->saveToFile();
		User::save();
	}
	else
	{
		View::getInstance().showMessage(filename, "Error:找不到文件 ");
		return;
	}
}

void Table::creatTable(const vector<string> &keys, const string &name, const string &filename, User *user)
{
	if (all_name.count(name))
	{
		View::getInstance().showMessage(name, "Error:同名数据库 ");
		return;
	}
	if (all_txtName.count(filename))
	{
		View::getInstance().showMessage(filename, "Error:该文件已打开 ");
		return;
	}
	auto tmp = new Table(keys, name, filename, user);
	all_name[name] = tmp;
	all_txtName[filename] = tmp;
	user->Drop[name].p.insert(User::all_user["__ADMIN__"]);
	user->Delete[name].p.insert(User::all_user["__ADMIN__"]);
	user->Insert[name].p.insert(User::all_user["__ADMIN__"]);
	user->Select[name].p.insert(User::all_user["__ADMIN__"]);
	tmp->saveToFile();
	User::save();
}

void Table::deleteTable(const string &name)
{
	if (!all_name.count(name))
	{
		View::getInstance().showMessage(name, "Error:该数据库不存在 ");
		return;
	}
	auto tmp = all_name[name];
	tmp->m_owner->Drop[name].p.clear();
	tmp->m_owner->Delete[name].p.clear();
	tmp->m_owner->Insert[name].p.clear();
	tmp->m_owner->Select[name].p.clear();

	tmp->m_owner->revokedel(name);
	tmp->m_owner->revokedrop(name);
	tmp->m_owner->revokeins(name);
	tmp->m_owner->revokesel(name);
	all_name.erase(tmp->m_name);
	all_name.erase(tmp->m_txtName);
	system((string("rm ") + tmp->m_txtName).c_str());
	User::save();
	ofstream os;
	os.open("__TableList__");
	for (auto t : all_name)
	{
		os << t.second->m_name << " " << t.second->m_txtName << " " << t.second->m_owner->m_name << endl;
	}
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
	for (int i = 0; i < m_info[0].size(); i++)
	{
		m_where[m_info[0][i]] = i;
	}
	//cout << m_info[0][0];
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
	for (int i = 0; i < m_info[0].size(); i++)
	{
		m_where[m_info[0][i]] = i;
	}
}

void Table::saveToFile()
{
	ofstream os(m_txtName);
	for (auto t : m_info)
	{
		os << My::MergeVec(t);
	}
	os.close();
	os.open("__TableList__");
	for (auto t : all_name)
	{
		os << t.second->m_name << " " << t.second->m_txtName << " " << t.second->m_owner->m_name << endl;
	}
}
void Table::loadFromFile()
{
	ifstream is("__TableList__");
	string name, file, user;
	while (is >> name >> file >> user)
	{
		//cout << file << name << endl;
		creatTable(file, name, User::all_user[user]);
	}
	for (auto &user : User::all_user)
	{
		for (auto &table : all_name)
		{
			user.second->Drop[table.second->m_name] = TreeNode();
			user.second->Delete[table.second->m_name] = TreeNode();
			user.second->Insert[table.second->m_name] = TreeNode();
			user.second->Select[table.second->m_name] = TreeNode();
		}
	}
}
void Table::showTables(User *user)
{
	unordered_map<Table *, vector<string>> tables;
	for (auto t : all_name)
	{
		tables[t.second] = vector<string>();
	}
	//cout << user->Drop.size() << "drop size" << endl;
	for (auto treenode : user->Drop)
	{
		if (treenode.second.p.size())
		{
			tables[all_name[treenode.first]].push_back("DROP");
		}
	}
	for (auto treenode : user->Delete)
	{
		if (treenode.second.p.size())
		{
			tables[all_name[treenode.first]].push_back("DELETE");
		}
	}
	for (auto treenode : user->Insert)
	{
		if (treenode.second.p.size())
		{
			tables[all_name[treenode.first]].push_back("INSERT");
		}
	}
	for (auto treenode : user->Select)
	{
		if (treenode.second.p.size())
		{
			tables[all_name[treenode.first]].push_back("SELECT");
		}
	}
	int cnt = 0;
	for (auto t : tables)
	{
		if (t.second.size() != 0)
		{
			cnt++;
		}
	}

	cout << "\ttotal:" << cnt << endl;
	for (auto t : tables)
	{
		if (t.second.size() != 0)
		{
			cout << "\t\t";
			cout << t.first->m_name << " : (" << t.first->getLength() - 1 << "," << t.first->getSize() - 1 << ") [" << My::MergeVec(t.first->m_info[0], ", ", "] ") << My::MergeVec(t.second, ", ", "");
			//cout << t.first->m_owner->m_name << endl;
			if (t.first->m_owner == user)
				cout << "[OWNER]";
			cout << endl;
		}
	}
}
void Table::insertValue(const string &info)
{
	auto vec = My::CutBetween(info, '(', ')');
	if (vec.size() != 1 && vec.size() != 2)
	{
		View::getInstance().showMessage(info, "Error:非法输入 ");
		return;
	}
	if (vec.size() == 1)
	{
		vec[0] = My::Replace(vec[0], ' ', "");
		m_info.push_back(My::Split(vec[0], ',', My::UIntToStr(m_info.size())));
	}
	else
	{
		vec[0] = My::Replace(vec[0], ' ', "");
		vec[1] = My::Replace(vec[1], ' ', "");
		auto v1 = My::Split(vec[0], ',');
		auto v2 = My::Split(vec[1], ',');
		if (v1.size() != v2.size())
		{
			View::getInstance().showMessage(info, "Error:非法输入 ");
			return;
		}
		vector<string> res(getLength(), " ");
		res[0] = My::UIntToStr(m_info.size());
		for (int i = 0; i < v1.size(); i++)
		{
			if (!m_where.count(v1[i]))
			{
				View::getInstance().showMessage(v1[i], "Error:不存在关键字 ");
				return;
			}
			res[m_where[v1[i]]] = v2[i];
		}
		m_info.push_back(res);
	}
	saveToFile();
}
// string Table::insertValue(const string &info, const string &pos)
// {
// }
void Table::findAndDelete(const string &str)
{
	stringstream ss(str);
	string tmp, name, whc, whv;
	bool all = false;
	ss >> tmp;
	while (ss >> tmp)
	{
		if (tmp == "FROM")
		{
			ss >> name;
		}
		else if (tmp == "WHERE")
		{
			ss >> whc;
			ss >> tmp;
			ss >> whv;
			if (!m_where.count(whc))
			{
				View::getInstance().showMessage(whc, "Error:不存在关键字 ");
				return;
			}
			if (!ss)
			{
				View::getInstance().showMessage(str, "Error:非法输入");
				return;
			}
		}
		else if (tmp == "*")
		{
			all = true;
		}
	}
	if (name == "")
	{
		View::getInstance().showMessage("", "Error:请输入要查询的数据库 ");
		return;
	}
	if (all)
	{
		while (m_info.size() > 1)
		{
			m_info.erase(m_info.end());
		}
	}
	else
	{
		for (int i = 1; i < m_info.size(); i++)
		{
			if (m_info[i][m_where[whc]] == whv)
			{
				m_info.erase(m_info.begin() + i);
			}
			m_info[i][0] = My::UIntToStr(i);
		}
	}
	saveToFile();
}

void Table::select(const string &str)
{
	stringstream ss(str);
	string tmp, name, to, whc, whv;
	bool distinct = false;
	vector<string> order, keys;
	vector<int> ordernum;
	int asc = true;
	keys.push_back("ID");
	ss >> tmp;
	while (ss >> tmp)
	{
		if (tmp == "DISTINCT")
		{
			distinct = true;
		}
		else if (tmp == "FROM")
		{
			ss >> name;
		}
		else if (tmp == "ORDER")
		{
			ss >> tmp;
			if (tmp != "BY")
			{
				View::getInstance().showMessage("", "Error:是否输入ORDER BY?");
				return;
			}
			while (ss >> tmp)
			{
				if (tmp == "ASC")
				{
					asc = true;
					break;
				}
				if (tmp == "DESC")
				{
					asc = false;
					break;
				}
				order.push_back(My::Replace(tmp, ',', ""));
				if (!m_where.count(order.back()))
				{
					View::getInstance().showMessage(order.back(), "Error:不存在关键字 ");
					return;
				}
				ordernum.push_back(m_where[order.back()]);
			}
		}
		else if (tmp == "TO")
		{
			ss >> to;
		}
		else if (tmp == "WHERE")
		{
			ss >> whc;
			ss >> tmp;
			ss >> whv;
			if (!m_where.count(whc))
			{
				View::getInstance().showMessage(whc, "Error:不存在关键字 ");
				return;
			}
			if (!ss)
			{
				View::getInstance().showMessage(str, "Error:非法输入");
				return;
			}
		}
		else
		{
			if (keys.back() == "*")
				continue;
			keys.push_back(My::Replace(tmp, ',', ""));
			if (!m_where.count(keys.back()) && keys.back() != "*")
			{
				View::getInstance().showMessage(keys.back(), "Error:不存在关键字 ");
				return;
			}
		}
	}
	if (name == "")
	{
		View::getInstance().showMessage("", "Error:请输入要查询的数据库 ");
		return;
	}
	//cout << My::MergeVec(order);
	//cout << My::MergeVec(keys);
	//cout << whc << "=" << whv << endl;
	//cout << name << endl;
	//cout << to << endl;

	vector<vector<string>> tempinfo = m_info;
	vector<vector<string>> res;
	if (order.size())
	{
		sort(tempinfo.begin() + 1, tempinfo.end(), [&](const vector<string> &l, const vector<string> &r) -> bool {
			for (auto i : ordernum)
			{
				if (l[i] == r[i])
				{
					continue;
				}
				if (l[i] < r[i])
				{
					return asc;
				}
				else
				{
					return !asc;
				}
			}
			return false;
		});
	}
	if (keys.back() == "*")
		keys = m_info[0];
	auto first = [&res](const vector<string> &x) -> bool {
		for (int i = 0; i < res.size(); i++)
		{
			bool flag = false;
			for (int j = 1; j < x.size(); j++)
			{
				if (res[i][j] != x[j])
					flag = true;
			}
			if (!flag)
				return false;
		}
		return true;
	};
	res.push_back(keys);
	for (int i = 1; i < tempinfo.size(); i++)
	{
		vector<string> tmp;
		for (int j = 0; j < keys.size(); j++)
		{
			//cout << keys[j] << tempinfo[i][m_where[keys[j]]] << endl;
			tmp.push_back(tempinfo[i][m_where[keys[j]]]);
		}
		tmp[0] = My::UIntToStr(res.size());
		if ((whc == "" || tempinfo[i][m_where[whc]] == whv) && (!distinct || first(tmp)))
		{
			res.push_back(tmp);
		}
	}
	if (to != "")
	{
		ifstream is(to);
		if (is.is_open())
		{
			View::getInstance().showMessage(to, "Error:文件已存在 ");
			return;
		}
		ofstream os(to);
		for (auto t : res)
		{
			os << My::MergeVec(t);
		}
	}
	else
	{
		for (auto t : res)
		{
			cout << My::MergeVec(t);
		}
	}
	saveToFile();
}