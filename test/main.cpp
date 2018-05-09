#include "tiny.h"
#include "router.h"

using namespace tiny;

int main(int argc, char const *argv[]) {
	Tiny tiny(argv[1]);
	tiny.route(GetTest)
		.route(PostTest)
		.route(JsonTest);
	return tiny.run();
}
