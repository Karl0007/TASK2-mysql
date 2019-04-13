#include <bits/stdc++.h>
#include "View.h"
#include "Table.h"
#include "User.h"
#include "StringOperator.h"
using Mysql::View;
using namespace My;
using namespace std;

User *login()
{
	View::getInstance().showMessage("login:", "(mysql)==>");
	string user = View::getInstance().getString();
	View::getInstance().showMessage("password:", "(mysql)==>");
	string pass = View::getInstance().getString();
	if (User::all_user.count(user) && User::checklogin(user, pass))
		return User::all_user[user];
	View::getInstance().showMessage("登录失败");
	return nullptr;
}

int main()
{
	//cout << 123;
	User::init();
	string input;
	cout << "~$";
	input = View::getInstance().getString();
	while (input != "quit")
	{
		User *cur = nullptr;
		if (input == "mysql")
		{
			cur = login();
		}
		else
		{
			system(input.c_str());
		}
		while (cur)
		{
			cout << "(mysql)==>";
			input = View::getInstance().getString();
			if (input == "quit")
			{
				cur = nullptr;
			}
			else
			{
				auto tmp = My::Split(input, ' ');
				if (tmp.size() >= 5 && tmp[0] == "CREAT" && tmp[1] == "TABLE")
				{
					if (tmp[3] == "FROM")
					{
						Table::creatTable(tmp[4], tmp[2], cur);
					}
					else
					{
						Table::creatTable(My::Split(My::Replace(My::CutBetween(input, '(', ')')[0], ',', ""), ' '), tmp[2], tmp.back(), cur);
					}
				}
				else if (input == "TABLE LIST")
				{
					Table::showTables(cur);
				}
				else if (tmp.size() == 3 && tmp[0] == "DROP" && tmp[1] == "TABLE")
				{
					if (cur->Drop.count(tmp.back()) && cur->Drop[tmp.back()].p.size())
					{
						Table::deleteTable(tmp.back());
					}
					else
					{
						View::getInstance().showMessage("权限错误");
					}
				}
				else if (tmp.size() >= 3 && tmp[0] == "INSERT")
				{
					auto name = tmp[2];
					if (cur->Insert.count(name) && cur->Insert[name].p.size())
					{
						Table::all_name[name]->insertValue(input);
					}
					else
					{
						View::getInstance().showMessage("权限错误");
					}
				}
				else if (tmp.size() >= 3 && tmp[0] == "DELETE")
				{
					string name;
					for (auto i = 0; i < tmp.size(); i++)
					{
						if (tmp[i] == "FROM" && i + 1 != tmp.size())
						{
							name = tmp[i + 1];
							break;
						}
					}
					//cout << name;

					if (cur->Delete.count(name) && cur->Delete[name].p.size())
					{
						Table::all_name[name]->findAndDelete(input);
					}
					else
					{
						View::getInstance().showMessage("权限错误");
					}
				}
				else if (tmp.size() >= 3 && tmp[0] == "SELECT")
				{
					string name;
					for (auto i = 0; i < tmp.size(); i++)
					{
						if (tmp[i] == "FROM" && i + 1 != tmp.size())
						{
							name = tmp[i + 1];
							break;
						}
					}
					//cout << name;

					if (cur->Select.count(name) && cur->Select[name].p.size())
					{
						Table::all_name[name]->select(input);
					}
					else
					{
						View::getInstance().showMessage("权限错误");
					}
				}
				else if (tmp.size() && tmp[0] == "GRANT")
				{
					cur->grant(input);
				}
				else if (tmp.size() && tmp[0] == "REVOKE")
				{
					cur->revoke(input);
				}
			}
		}
		cout << "~$";
		input = View::getInstance().getString();
	}

	User::save();
	return 0;
}