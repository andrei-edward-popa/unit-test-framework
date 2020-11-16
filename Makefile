.PHONY: run
run:
	cp utf/* .;                                         \
	./vpl_evaluate.sh

.PHONY: mrproper
mrproper:
	rm -f vpl_execution vpl_test 
	rm -f *.cases
	rm -f *.h_ *.cpp_ *.hpp_
	rm -f *.h *.cpp *.hpp
	rm -f *.cpp.save
	rm -f .vpl_tester
	rm -f *.sh
	rm -f tui/tests.h
	rm -f tcs/extract.conf
	rm -f tcs/tests.h
	rm -rf tcs/xml
	rm -f *.txt
	
.SILENT: test
test:
	printf "How do you want to create the tests? (automatic/manual): "
	read mode;                                          \
	if [ $$mode = "automatic" ]; then                   \
		cd tcs;                                         \
		if [ ! dpkg -s python3 >/dev/null 2>&1 ]; then  \
			sudo apt-get install python3;               \
		fi;                                             \
		if [ ! dpkg -s doxygen >/dev/null 2>&1 ]; then  \
			sudo apt-get install doxygen;               \
		fi;                                             \
		mv main.tcs main.cpp;                           \
		doxygen config.doxy;                            \
		python3 parser.py;                              \
		python3 test_creator.py;                        \
		mv main.cpp main.tcs;                           \
		pyclean .                                       \
		echo "Test file 'tcs/tests.h' was created!";    \
	elif [ $$mode = "manual" ]; then                    \
		cd tui;                                         \
		./start_tui.sh;                                 \
		echo "Test file 'tui/tests.h' was created!";    \
	else                                                \
		echo "Invalid command! Stop!";                  \
	fi

