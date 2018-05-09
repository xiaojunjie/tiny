#include "tiny.h"
#include <vector>
#include <map>
#include <iostream>

using namespace tiny;
using namespace std;

RouterTable GetTest = {
	{"/index.html",[](vector<string> args){
		Template tpl("index.tpl");
		return new HttpResponse(tpl.render(),200);
	}}
};

RouterTable PostTest = {
	{"/list?page=&num=",[](vector<string> args){
		Template tpl("list.tpl");
		return new HttpResponse(tpl.render(),200);
	}}
};

RouterTable JsonTest = {
	{"/json",[](vector<string> args){
		std::cout << args.back() << std::endl;
		return new HttpResponse(args.back(),
				HttpMessage::TYPE_APPLICATION_JSON,
				200);
	}}
};
