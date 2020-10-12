

run:
	g++ -pthread -o dns ./main.cpp
	chmod +x dns

test: run
	chmod +x TEST/test.sh
	TEST/test.sh

clean:
	rm dns
