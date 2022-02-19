.PHONY: run
run:
	cp utf/* .;                                     \
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
	rm -f tcs/extract.conf
	rm -f tcs/tests.h
	rm -f tcs/vpl_evaluate.cases
	rm -rf tcs/xml
	rm -f *.txt
	
.SILENT: test
test:
	cd tcs;                                         \
	rm -f tests.h;                                  \
	mv main.tcs main.cpp;                           \
	doxygen config.doxy;                            \
	python3 parser.py;                              \
	python3 test_creator.py;                        \
	mv main.cpp main.tcs;                           \
	pyclean .;                                      \
	echo "Test file 'tcs/tests.h' was created!"

.SILENT: requirements
requirements:
	OS=$(shell awk '/DISTRIB_ID=/' /etc/*-release | sed 's/DISTRIB_ID=//' | tr '[:upper:]' '[:lower:]');      \
	if [ $$OS = "ubuntu" ] || [ $$OS = "linuxmint" ] || [ $$OS = "debian" ]; then                             \
		sudo apt update;                                                                                      \
		sudo apt install -y python3;                                                                          \
		sudo apt install -y doxygen;                                                                          \
		sudo apt install -y python3-lxml;                                                                     \
	elif [ $$OS = "centos" ]; then                                                                            \
		sudo yum update -y;                                                                                   \
		sudo yum install -y python3;                                                                          \
		sudo yum install -y doxygen;                                                                          \
		sudo yum install -y python3-lxml;                                                                     \
	elif [ $$OS = "arch" ]; then                                                                              \
		sudo pacman -Syu;                                                                                     \
		yes | sudo pacman -S python3;                                                                         \
		yes | sudo pacman -S doxygen;                                                                         \
		yes | sudo pacman -S python-pip;                                                                      \
		pip install lxml;                                                                                     \
	else                                                                                                      \
		echo "Your distrubution is not added to the application. You are on your own.";                       \
	fi
	
