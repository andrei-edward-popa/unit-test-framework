#!/bin/bash

. common_script.sh

if [ "$SECONDS" = "" ] ; then
	export SECONDS=20
fi
if [ "$VPL_GRADEMIN" = "" ] ; then
	export VPL_GRADEMIN=0
	export VPL_GRADEMAX=10
fi

#exist run script?
if [ ! -s vpl_run.sh ] ; then
	echo "I'm sorry, but I haven't a default action to evaluate the type of submitted files"
else

	mv vpl_evaluate.cpp vpl_evaluate.cpp_
    mv functions.h functions.h_
    mv structures.h structures.h_
    mv tests.h tests.h_
    
    SOURCE_FILES=""

    header_files=$((`ls -1 *.h 2>/dev/null | wc -l` + `ls -1 *.hpp 2>/dev/null | wc -l`))
    if [[ $header_files != 0 ]]; then
        get_source_files h hpp
    fi

	for file in $SOURCE_FILES
	do
	    grep -v "#ifndef" $file > __temp__.txt && mv __temp__.txt $file
	    grep -v "#endif" $file > __temp__.txt && mv __temp__.txt $file
    	sed -i 's/class/struct/g' $file
    	sed -i 's/private\s*:/public:/g' $file
    	sed -i 's/protected\s*:/public:/g' $file
    	sed -i 's/operator\s*=\s*/operatorEqual/g' $file
	done
	
	get_source_files cpp
	
	for file in $SOURCE_FILES
	do
	    line=`zgrep -nE "^\s*int\s+main\s*()\s*" $file | cut -f1 -d:`
	    if [[ ! -z $line ]]; then
	        main_file="$file"
	        main_line="$line"
	        break
	    fi
	done
	
	tail -n +$main_line $main_file > student_after_main.txt
	count_lines=0
	count_brackets=0
	first_occ=0
	while IFS= read -r line
	do
		
		for (( i=0; i<${#line}; i++ )); do   

			if [ "${line:$i:1}" == "}" ]; then
				count_brackets=$(($count_brackets - 1))
			fi
			if [ "${line:$i:1}" == "{" ]; then
				count_brackets=$(($count_brackets + 1))
				first_occ=1
			fi
					
		done
		count_lines=$(($count_lines + 1))
		if [[ $first_occ == 1 && $count_brackets == 0 ]]; then
			break
		fi

	done < "student_after_main.txt"

	get_source_files cpp h hpp
	
	mv vpl_evaluate.cpp_ vpl_evaluate.cpp
    mv functions.h_ functions.h
    mv structures.h_ structures.h
    mv tests.h_ tests.h
    
    grep -e "^\s*CLASS" tests.h | cut -d'(' -f 2 | cut -d')' -f 1 > classes.txt
	
	let prev_line_main=$main_line-1
	head -n $prev_line_main $main_file > student_impl.txt
	all_lines=$(($count_lines + $main_line))
	if [[ `grep -c ^ $main_file` != $all_lines ]]; then
	    tail -n +$(($count_lines+1)) student_after_main.txt >> student_impl.txt
	fi
	sed -i -E 's/^\s{0,1}[A-Za-z_][A-Za-z0-9_\*\&]*\s+[\*\&A-Za-z_]*[A-Za-z0-9_]*\s*\((\s*[A-Za-z_][A-Za-z0-9_\*\&]*\s+[\*\&A-Za-z_]*[A-Za-z0-9_]*\s*,{0,1}\s*){1,}\s*\)\s*;\s*$//g' student_impl.txt
	sed -i 's/class/struct/g' student_impl.txt
	sed -i 's/private\s*:/public:/g' student_impl.txt
	sed -i 's/protected\s*:/public:/g' student_impl.txt
	sed -i 's/operator\s*=\s*/operatorEqual/g' student_impl.txt
	
	cp vpl_evaluate.cpp saved_vpl_evaluate.cpp
	cat student_impl.txt > vpl_evaluate.cpp
	sed -i '1s/^/struct __HACK__ {\n/' student_impl.txt
	echo "};" >> student_impl.txt
	sed -i -E 's/\s*using\s+namespace\s+[A-Za-z0-9:_]+\s*;\s*//g' student_impl.txt
	while IFS= read -r class
	do
		sed -i '2s/^/class '${class}';\n/' student_impl.txt
	done < "classes.txt"
	cat student_impl.txt >> vpl_evaluate.cpp
	cat saved_vpl_evaluate.cpp >> vpl_evaluate.cpp
	rm -f saved_vpl_evaluate.cpp
	rm -f student_impl.txt
	rm -f classes.txt
	rm -f student_after_main.txt

	#avoid conflict with C++ compilation
	./vpl_run.sh

	mv vpl_evaluate.cpp vpl_evaluate.cpp.save
	#Prepare run
	if [ -f vpl_execution ] ; then
		mv vpl_execution vpl_test
		if [ -f vpl_evaluate.cases ] ; then
			mv vpl_evaluate.cases evaluate.cases
		else
			echo "Error need file 'vpl_evaluate.cases' to make an evaluation"
			exit 1
		fi
		#Add constants to vpl_evaluate.cpp
		echo "const float VPL_GRADEMIN=$VPL_GRADEMIN;" >vpl_evaluate.cpp
		echo "const float VPL_GRADEMAX=$VPL_GRADEMAX;" >>vpl_evaluate.cpp
		let VPL_MAXTIME=VPL_MAXTIME-$SECONDS-1;
		echo "const int VPL_MAXTIME=$VPL_MAXTIME;" >>vpl_evaluate.cpp
		cat vpl_evaluate.cpp.save >> vpl_evaluate.cpp
		check_program g++
		SOURCE_FILES_WITHOUT_MAIN=${SOURCE_FILES//$main_file/}
		g++ -std=c++17 $SOURCE_FILES_WITHOUT_MAIN vpl_evaluate.cpp -g -lm -lutil -o .vpl_tester
		mv functions.h functions.h_
		mv tests.h tests.h_
		mv structures.h structures.h_
		mv vpl_evaluate.cpp vpl_evaluate.cpp_
		if [ ! -f .vpl_tester ] ; then
			echo "Error compiling evaluation program"
		else
			echo "#!/bin/bash" >> vpl_execution
			echo "./.vpl_tester" >> vpl_execution
		fi
	else
		echo "#!/bin/bash" >> vpl_execution
		echo "echo" >> vpl_execution
		echo "echo '<|--'" >> vpl_execution
		echo "echo '-$VPL_COMPILATIONFAILED'" >> vpl_execution
		if [ -f vpl_wexecution ] ; then
			echo "echo '======================'" >> vpl_execution
			echo "echo 'It seems you are trying to test a program with a graphic user interface'" >> vpl_execution
		fi
		echo "echo '--|>'" >> vpl_execution		
		echo "echo" >> vpl_execution		
		echo "echo 'Grade :=>>$VPL_GRADEMIN'" >> vpl_execution
	fi
    
    chmod +x vpl_execution
fi

