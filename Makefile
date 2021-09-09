.PHONY: run
run:
	cp utf/* .;      \
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
	rm -f gui/tests.h
	
.PHONY: test
test:
	cd gui;          \
	./createTests.sh
