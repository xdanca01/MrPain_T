#!/bin/bash
echo "bad GET"
curl -i localhost:$1/resolv?name=147.229.14.131\&type=PTR
echo "bad POST"
curl -i --data-binary @queries.txt -X POST http://localhost:$1/dns-quer
echo "good GET"
curl -i localhost:$1/resolve?name=147.229.14.131\&type=PTR
echo "good POST"
curl -i --data-binary @queries.txt -X POST http://localhost:$1/dns-query
