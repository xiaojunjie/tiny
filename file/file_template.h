#pragma once
#include <vector>
#include <fstream>
using namespace std;

namespace tiny{

class Template{
public:
    Template();
    Template(const string &);
    ~Template();
    string render();
    string render(vector<string>);
	static string Dir;
	static string TPL404;
    static int ReadFile(const string &,string &);
    static vector<string> ReadFileLines(const string &);
protected:
    string tpl;
};


}
