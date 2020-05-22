WORKING_DIR = $(pwd)
build-cpp:
	@echo '#!/bin/sh \npython3 Python/main.py \\$\1' > THP7.exe
	$(shell cp ./start.sh THP.exe; chmod +x THP.exe;)
	g++ Cpp/main.cpp -o THP1.exe;


   