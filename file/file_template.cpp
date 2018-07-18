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
    ReadFile(Dir+filename,tpl);
}
int Template::ReadFile(const string & filename, string & userbuf){
    ifstream file(filename, std::ifstream::binary | std::ifstream::in);
    if(!file)
        return -1;
    //string str((std::istreambuf_iterator<char>(file)),
    //             std::istreambuf_iterator<char>());
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    userbuf = buffer.str();
    return userbuf.length();
}
vector<string> Template::ReadFileLines(const string & filename){
	vector<string> elems;
	string content;
    int n = ReadFile(filename,content);
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
