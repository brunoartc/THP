WORKING_DIR = $(pwd)
test:
	g++ tests.cpp -o tests; ./tests; rm -rf tests
build:
	g++ main.cpp -g -o THP.exe;


   