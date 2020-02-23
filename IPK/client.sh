#!/bin/bash
echo "bad GET"
curl  localhost:$1/resolv?name=147.229.14.131\&type=PTR
echo "bad POST"
curl  --data-binary @queries.txt -X POST http://localhost:$1/dns-quer
echo "good GET"
curl  localhost:$1/resolve?name=www.fit.vutbr.cz\&type=A
echo "good GET"
curl  localhost:$1/resolve?name=147.229.14.131\&type=PTR
echo "good POST"
curl  --data-binary @queries.txt -X POST http://localhost:$1/dns-query
