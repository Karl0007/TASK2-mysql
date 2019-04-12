#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
using std::cout;
using std::string;
using std::vector;
namespace Mysql
{
class View
{
private:
	static View *Instance;
	View();
	~View();
	View(View const &) = delete;
	View(View &&) = delete;
	View &operator=(View const &) = delete;
	View &operator=(View &&) = delete;

public:
	static View &getInstance();
	void clearScreen();
	void showMessage(const string &str, const string &bef = "(mysql)==>");
	string getString();
	void showTXT(const string &str);
};
} // namespace Mysql