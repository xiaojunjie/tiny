#include "file_template.h"
#include <iostream>
#include <sstream>
namespace tiny{

string Template::TPL404 = "404.tpl";
string Template::Dir;

Template::Template(){

}

std::string Template::render(){
    return tpl;
}

Template::Template(const string & filename){
    tpl = ReadFile(Dir+filename);
}
string Template::ReadFile(const string & filename){
    ifstream file(filename, std::ifstream::binary | std::ifstream::in);
    string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
    file.close();
    return str;
}
vector<string> Template::ReadFileLines(const string & filename){
	string content = ReadFile(filename);
	vector<string> elems;
	stringstream ss(content);
	string item;
	while (getline(ss, item)) {
		elems.push_back(item);
	}
	return elems;
}

Template::~Template(){

}

}
