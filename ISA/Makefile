

run:
	g++ -pthread -o main ./main.cpp
	chmod +x main

test: run
	@chmod +x main
	@chmod +x TEST/test.sh
	@./main -s 8.8.8.8 -p 4400 -f ./TEST/filter_test.txt 2>/dev/null &
	@TEST/test.sh

clean:
	rm main
