#include "include/tiny_core.h"

using namespace tiny;

int main(int argc, char const *argv[]) {
    Tiny tiny(argv[1]);
    tiny.get("/assets/", tiny_http_static_handler)
        .get("/hello", [](const tiny_http_request_t &request, tiny_http_response_t &response){
            response.body = "Hello Word!";
            response.status = TINY_HTTP_OK;
            return TINY_SUCCESS;
    });
    return tiny.run();
}
