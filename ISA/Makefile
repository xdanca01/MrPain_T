

run:
	g++ -pthread -o dns ./main.cpp
	chmod +x dns

test: run
	@chmod +x TEST/test.sh
	@./dns -s 8.8.8.8 -p 4400 -f ./TEST/filter_test.txt 2>/dev/null &
	@TEST/test.sh

clean:
	rm dns
