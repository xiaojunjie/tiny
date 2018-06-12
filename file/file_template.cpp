#include "file_template.h"
#include <iostream>
#include <sstream>
#include <time.h>
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
    //string str((std::istreambuf_iterator<char>(file)),
    //             std::istreambuf_iterator<char>());
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
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
