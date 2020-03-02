#!/bin/bash
echo "neco"
curl -i localhost:$1/resolve?name=147.229.14.131&type=PTR
echo "neco"
curl -i --data-binary @queries.txt -X POST http://localhost:$1/dns-quer
echo "neco"
curl  localhost:$1/resolve?name=www.fyt.vutbr.cz\&type=A
curl  localhost:$1/resolve?name=147.229.14.131\&type=PTR
curl  --data-binary @queries.txt -X POST http://localhost:$1/dns-query
