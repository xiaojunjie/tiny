#/bin/sh

curl -v -X POST  --header "Transfer-Encoding: chunked" -d @test/chunk.txt "http://localhost:8888/test/chunk"
