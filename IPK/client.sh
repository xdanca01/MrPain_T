#!/bin/bash
curl  localhost:$1/resolv?name=147.229.14.131\&type=PTR
curl  --data-binary @queries.txt -X POST http://localhost:$1/dns-quer
curl  localhost:$1/resolve?name=www.fit.vutbr.cz\&type=A
curl  localhost:$1/resolve?name=147.229.14.131\&type=PTR
curl  --data-binary @queries.txt -X POST http://localhost:$1/dns-query
