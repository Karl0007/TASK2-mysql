
#include "View.h"

namespace Mysql
{
View *View::Instance = nullptr;
View::View()
{
}

View::~View()
{
}

View &View::getInstance()
{
	if (Instance == nullptr)
	{
		Instance = new View;
	}
	return *Instance;
}

void View::showMessage(const string &str, const string &bef)
{
	cout << bef;
	cout << str << std::endl;
}

string View::getString()
{
	string tmp;
	std::getline(std::cin, tmp);
	return tmp;
}

void View::showTXT(const string &str)
{
	system((string("cat ") + str).c_str());
}
} // namespace Mysql
