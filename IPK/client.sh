#!/bin/bash
curl -i localhost:$1/resolve?name=147.229.14.131\&type=PTR
#curl --data-binary @queries.txt -X POST http://localhost:$1/dns-query
