#include "View.h"
#include "Table.h"
#include "StringOperator.h"
using Mysql::View;
using namespace My;
using namespace std;

int main()
{
	//View::getInstance().showMessage("name");
	//cout << CutBetween("5242(213,2141) 123", '(', ')');
	//View::getInstance().showTXT("View.h");
	//cout << 123;
	vector<string> xx;
	xx.push_back("课程名称");
	xx.push_back("课程编号");
	xx.push_back("任课教师");
	Table::creatTable(xx, "course", "course.txt", nullptr);
	Table::creatTable("student.txt", "student", nullptr);
	//Table::all_name["student"]->insertValue("(姓名, 专业)(张六, 计算机)");
	Table::all_name["student"]->select("SELECT DISTINCT 学号 FROM student ORDER BY 专业 DESC TO class.txt");

	//Table::creatTable("course.txt", "course", nullptr);
	//Table::deleteTable("course");
	cout << CutBetween("23e(afwe),(wer32r)fawe(afwji)", '(', ')')[2] << endl;
	return 0;
}