#!/bin/bash

COMMAND="description"
classes=()
attributes=()
methods=()
functions=()
declare -A attributes_class
declare -A methods_class
declare -A functions_sigs
declare -A attributes_sigs
declare -A methods_sigs

rm -f tests.h
touch tests.h

function menu_creater {
	whiptail --title "Framework Menu" --menu "Choose an option" --cancel-button "Exit" 25 90 16 \
				"Documentation" "Usage of assert functions." \
				"Add Attribute" "Define the name of the attribute you want to test." \
				"Add Method" "Define the name of the method or signatures you want to test." \
				"Add Function" "Define the name of the function you want to test." \
				"Create Test" "Create a unit test for classes/functions that you defined." \
				3>&1 1>&2 2>out.txt
	MENU=$(cat out.txt)	
	rm out.txt	
}

function add_classes {
	while true; do
		NR_CLASSES=$(whiptail --inputbox "How many classes do you want to add?" --cancel-button "" 8 65 --title "Add Class" 3>&1 1>&2 2>&3)
		if ! [[ $NR_CLASSES =~ ^[0-9]+$ ]]; then
			whiptail --title "Add Class" --msgbox "Invalid value! Enter a positive integer." 8 78
			COMMAND="utf"
			continue
		fi
		break
	done
	CLASSES=""
	for ((i = 1 ; i <= $NR_CLASSES ; i++ )); do
		CLASS_NAME=$(whiptail --inputbox "Enter the name of class "$i --cancel-button "" 8 65 --title "Add Class" 3>&1 1>&2 2>&3)
		if ! [[ $CLASS_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
			whiptail --title "Add Class" --msgbox "Invalid name! Enter a valid class name." 8 78
			i=$(($i-1))
			COMMAND="utf"
			continue
		fi
		if [ $i == $NR_CLASSES ]; then
			CLASSES=$CLASSES$CLASS_NAME
		else
			CLASSES=$CLASSES$CLASS_NAME', '
		fi
		classes+=("$CLASS_NAME")
		echo 'CLASS('$CLASS_NAME');' >> tests.h
		echo '' >> tests.h
	done
	if [ $NR_CLASSES -eq 0 ]; then
		whiptail --title "Add Class" --msgbox "No class added! You can assert only functions." 8 78
	elif [ $NR_CLASSES -eq 1 ]; then
		whiptail --title "Add Class" --msgbox "Class $CLASSES was added successfully!" 8 78
	else
		whiptail --title "Add Class" --msgbox "Classes $CLASSES were added successfully!" 8 78
	fi
	COMMAND="utf"
}

function add_attribute {
	whiptail --title "Add Attribute" --menu "Choose an option" --cancel-button "Exit" 25 90 16 \
		"Register            " "Register attribute you want to test." \
		"Signature        " "Register a type for attribute you want to check." \
		3>&1 1>&2 2>out.txt
	MENU=$(cat out.txt)
	rm out.txt
	case $MENU in
		"Register            ")
			if [ "${#classes[@]}" -eq 0 ]; then
				whiptail --title "Add attribute" --msgbox "No class found! Return to main menu." 8 78
				return 0
			fi
			while true; do
				ATTRIBUTE_NAME=$(whiptail --inputbox "Enter the attribute name" 8 39 --title "Add Attribute" 3>&1 1>&2 2>&3)
				if ! [[ $ATTRIBUTE_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Add Attribute" --msgbox "Invalid name! Enter a valid attribute name." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			attributes+=("$ATTRIBUTE_NAME")
			echo 'CHECK_CLASS_ATTRIBUTE('$ATTRIBUTE_NAME');' >> tests.h
			echo '' >> tests.h
			whiplist_args=''
			for ((i = 0 ; i < ${#classes[@]} ; i++ )); do
				whiplist_args=$whiplist_args' '${classes[$i]}' Select_Class_'${classes[$i]}' OFF '
			done
			whiptail --title "Add attribute" --checklist "Which classes have the attribute "$ATTRIBUTE_NAME"?" 20 78 4 $whiplist_args 3>&1 1>&2 2>out.txt
			CLASSES=$(cat out.txt)
			rm out.txt
			IFS=' '
			read -a classes_arr <<< "$CLASSES"
			unset IFS
			for ((i = 0 ; i < ${#classes_arr[@]} ; i++ )); do
				classes_arr[$i]=$(echo ${classes_arr[$i]} | cut -d '"' -f 2)
				attributes_class[${classes_arr[$i]}]=${attributes_class[${classes_arr[$i]}]}" "$ATTRIBUTE_NAME
			done	
			whiptail --title "Add Attribute" --msgbox "Attribute "$ATTRIBUTE_NAME" was added successfully!" 8 78
			COMMAND="utf"
			;;
		"Signature        ")
			if [ "${#attributes[@]}" -eq 0 ]; then
				whiptail --title "Attribute Signature" --msgbox "No attribute found! Return to main menu." 8 78
				return 0
			fi
			whiplist_args=''
			for ((i = 0 ; i < ${#attributes[@]} ; i++ )); do
				whiplist_args=$whiplist_args' '${attributes[$i]}' Select_Attribute_'${attributes[$i]}' OFF '
			done
			while true; do
				whiptail --title "Attribute Signature" --radiolist "Select an attribute" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
				ATTRIBUTE_NAME=$(cat out.txt)
				rm out.txt
				if ! [[ $ATTRIBUTE_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Attribute Signature" --msgbox "Invalid name! Enter a valid attribute name." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			while true; do
				ATTRIBUTE_TYPE=$(whiptail --inputbox "Enter the type of attribute "$ATTRIBUTE_NAME 8 65 --title "Attribute Signature" 3>&1 1>&2 2>&3)
				if ! [[ $ATTRIBUTE_TYPE =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Attribute Signature" --msgbox "Invalid type! Enter a valid attribute type." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			while true; do
				TEMPLATE_POSTFIX=$(whiptail --inputbox "Enter an identifier for attribute "$ATTRIBUTE_NAME". REMEMBER IT!" 8 65 --title "Attribute Signature" 3>&1 1>&2 2>&3)
				if ! [[ $TEMPLATE_POSTFIX =~ ^[a-zA-Z0-9_]+$ ]]; then
					whiptail --title "Attribute Signaturee" --msgbox "Invalid identified! Enter a valid identifier." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			attributes_sigs[$ATTRIBUTE_NAME]=${attributes_sigs[$ATTRIBUTE_NAME]}" "$TEMPLATE_POSTFIX
			echo 'CHECK_CLASS_ATTRIBUTE_SIGNATURE('$ATTRIBUTE_NAME', '$ATTRIBUTE_TYPE' T::*, '$TEMPLATE_POSTFIX');' >> tests.h	
			echo '' >> tests.h
			whiptail --title "Register Signature" --msgbox "Signature for attribute "$ATTRIBUTE_NAME" was added successfully!" 8 78
			COMMAND="utf"
			;;
		*)
			COMMAND="utf"
			;;
	esac
}

function add_method {
	whiptail --title "Add Method" --menu "Choose an option" --cancel-button "Exit" 25 90 16 \
		"Register        " "Register method you want to test." \
		"Signature        " "Register a signature for method you want to check." \
		3>&1 1>&2 2>out.txt
	MENU=$(cat out.txt)
	rm out.txt
	case $MENU in
		"Register        ")
			if [ "${#classes[@]}" -eq 0 ]; then
				whiptail --title "Add method" --msgbox "No class found! Return to main menu." 8 78
				return 0
			fi
			while true; do
				METHOD_NAME=$(whiptail --inputbox "Enter the method name" 8 39 --title "Register Method" 3>&1 1>&2 2>&3)
				if ! [[ $METHOD_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Add method" --msgbox "Invalid name! Enter a valid method name." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			methods+=("$METHOD_NAME")
			echo 'CHECK_CLASS_METHOD('$METHOD_NAME');' >> tests.h
			echo '' >> tests.h
			whiplist_args=''
			for ((i = 0 ; i < ${#classes[@]} ; i++ )); do
				whiplist_args=$whiplist_args' '${classes[$i]}' Select_Class_'${classes[$i]}' OFF '
			done
			whiptail --title "Add method" --checklist "Which classes have the method "$METHOD_NAME"?" 20 78 4 $whiplist_args 3>&1 1>&2 2>out.txt
			CLASSES=$(cat out.txt)
			rm out.txt
			IFS=' '
			read -a classes_arr <<< "$CLASSES"
			unset IFS
			for ((i = 0 ; i < ${#classes_arr[@]} ; i++ )); do
				classes_arr[$i]=$(echo ${classes_arr[$i]} | cut -d '"' -f 2)
				methods_class[${classes_arr[$i]}]=${methods_class[${classes_arr[$i]}]}" "$METHOD_NAME
			done
			whiptail --title "Register Method" --msgbox "Method "$METHOD_NAME" was added successfully!" 8 78
			;;
		"Signature        ")
			if [ "${#methods[@]}" -eq 0 ]; then
				whiptail --title "Method Signature" --msgbox "No method found! Return to main menu." 8 78
				return 0
			fi
			whiplist_args=''
			for ((i = 0 ; i < ${#methods[@]} ; i++ )); do
				whiplist_args=$whiplist_args' '${methods[$i]}' Select_Method_'${methods[$i]}' OFF '
			done
			while true; do
				whiptail --title "Method Signature" --radiolist "Select a method" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
				METHOD_NAME=$(cat out.txt)
				rm out.txt
				if ! [[ $METHOD_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Method Signature" --msgbox "Invalid name! Enter a valid method name." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			while true; do
				RETURN_VALUE=$(whiptail --inputbox "Enter the return type of method "$METHOD_NAME 8 65 --title "Method Signature" 3>&1 1>&2 2>&3)
				if ! [[ $RETURN_VALUE =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Method Signature" --msgbox "Invalid type! Enter a valid return type." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			while true; do
				TEMPLATE_POSTFIX=$(whiptail --inputbox "Enter an identifier for method "$METHOD_NAME". REMEMBER IT!" 8 65 --title "Method Signature" 3>&1 1>&2 2>&3)
				if ! [[ $TEMPLATE_POSTFIX =~ ^[a-zA-Z0-9_]+$ ]]; then
					whiptail --title "Method Signature" --msgbox "Invalid identifier! Enter a valid identifier." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			methods_sigs[$METHOD_NAME]=${methods_sigs[$METHOD_NAME]}" "$TEMPLATE_POSTFIX
			while true; do
				NR_INPUT_PARAMETERS=$(whiptail --inputbox "How many parameters method "$METHOD_NAME" has?" 8 65 --title "Method Signature" 3>&1 1>&2 2>&3)
				if ! [[ $NR_INPUT_PARAMETERS =~ ^[0-9]+$ ]]; then
					whiptail --title "Method Signature" --msgbox "Invalid value! Enter a positive integer." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			ALL_PARAMETERS=""
			for ((i = 1 ; i <= $NR_INPUT_PARAMETERS ; i++ )); do
				PARAMETER=$(whiptail --inputbox "Enter the type of parameter "$i 8 65 --title "Method Signature" 3>&1 1>&2 2>&3)
				if ! [[ $PARAMETER =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Method Signature" --msgbox "Invalid type! Enter a valid type." 8 78
					i=$(($i-1))
					COMMAND="utf"
					continue
				fi
				if [ $i == $NR_INPUT_PARAMETERS ]; then
					ALL_PARAMETERS=$ALL_PARAMETERS$PARAMETER
				else
					ALL_PARAMETERS=$ALL_PARAMETERS$PARAMETER','
				fi
			done
			echo 'CHECK_CLASS_METHOD_SIGNATURE('$METHOD_NAME', '$RETURN_VALUE'(T::*)('$ALL_PARAMETERS'), '$TEMPLATE_POSTFIX');' >> tests.h	
			echo '' >> tests.h
			whiptail --title "Register Signature" --msgbox "Signature for method "$METHOD_NAME" was added successfully!" 8 78
			COMMAND="utf"
			;;
		*)
			COMMAND="utf"
			;;
	esac
}

function add_function {
	whiptail --title "Add Function" --menu "Choose an option" --cancel-button "Exit" 25 90 16 \
		"Register        " "Register function you want to test." \
		"Signature        " "Register a signature for function you want to check." \
		3>&1 1>&2 2>out.txt
	MENU=$(cat out.txt)
	rm out.txt
	case $MENU in
		"Register        ")
			while true; do
				FUNCTION_NAME=$(whiptail --inputbox "Enter the function name" 8 39 --title "Register Function" 3>&1 1>&2 2>&3)
				if ! [[ $FUNCTION_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Add Function" --msgbox "Invalid name! Enter a valid function name." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			functions+=("$FUNCTION_NAME")
			echo 'CHECK_FUNCTION('$FUNCTION_NAME');' >> tests.h
			echo '' >> tests.h
			whiptail --title "Register FUNCTION" --msgbox "Function "$FUNCTION_NAME" was added successfully!" 8 78
			;;
		"Signature        ")
			if [ "${#functions[@]}" -eq 0 ]; then
				whiptail --title "Function Signature" --msgbox "No function found! Return to main menu." 8 78
				return 0
			fi
			whiplist_args=''
			for ((i = 0 ; i < ${#functions[@]} ; i++ )); do
				whiplist_args=$whiplist_args' '${functions[$i]}' Select_Function_'${functions[$i]}' OFF '
			done
			while true; do
				whiptail --title "Function Signature" --radiolist "Select a function" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
				FUNCTION_NAME=$(cat out.txt)
				rm out.txt
				if ! [[ $FUNCTION_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Function Signature" --msgbox "Invalid name! Enter a valid function name." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			while true; do
				RETURN_VALUE=$(whiptail --inputbox "Enter the return type of function "$FUNCTION_NAME 8 65 --title "Function Signature" 3>&1 1>&2 2>&3)
				if ! [[ $RETURN_VALUE =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Function Signature" --msgbox "Invalid type! Enter a valid type." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			while true; do
				TEMPLATE_POSTFIX=$(whiptail --inputbox "Enter an identifier for function "$FUNCTION_NAME". REMEMBER IT!" 8 65 --title "Function Signature" 3>&1 1>&2 2>&3)
				if ! [[ $TEMPLATE_POSTFIX =~ ^[a-zA-Z0-9_]+$ ]]; then
					whiptail --title "Function Signature" --msgbox "Invalid identifier! Enter a valid identifier." 8 78
					COMMAND="utf"
					continue
				fi
				break
			done
			functions_sigs[$FUNCTION_NAME]=${functions_sigs[$FUNCTION_NAME]}" "$TEMPLATE_POSTFIX
			while true; do
				NR_INPUT_PARAMETERS=$(whiptail --inputbox "How many parameters function "$FUNCTION_NAME" has?" 8 65 --title "Function Signature" 3>&1 1>&2 2>&3)
				if ! [[ $NR_INPUT_PARAMETERS =~ ^[0-9]+$ ]]; then
					whiptail --title "Function Signature" --msgbox "Invalid value! Enter a positive integer." 8 78
					COMMAND="utf"
					continue
				fi
				breal
			done
			ALL_PARAMETERS=""
			for ((i = 1 ; i <= $NR_INPUT_PARAMETERS ; i++ )); do
				PARAMETER=$(whiptail --inputbox "Enter the type of parameter "$i 8 65 --title "Function Signature" 3>&1 1>&2 2>&3)
				if ! [[ $PARAMETER =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
					whiptail --title "Function Signature" --msgbox "Invalid type! Enter a valid type." 8 78
					i=$(($i-1))
					COMMAND="utf"
					continue
				fi
				if [ $i == $NR_INPUT_PARAMETERS ]; then
					ALL_PARAMETERS=$ALL_PARAMETERS$PARAMETER
				else
					ALL_PARAMETERS=$ALL_PARAMETERS$PARAMETER','
				fi
			done
			echo 'CHECK_FUNCTION_SIGNATURE('$FUNCTION_NAME', '$RETURN_VALUE'(T::*)('$ALL_PARAMETERS'), '$TEMPLATE_POSTFIX');' >> tests.h
			echo '' >> tests.h	
			whiptail --title "Register Signature" --msgbox "Signature for function "$FUNCTION_NAME" was added successfully!" 8 78
			COMMAND="utf"
			;;
		*)
			COMMAND="utf"
			;;
	esac
}

function create_test {
	while true; do
		SUITE_NAME=$(whiptail --inputbox "Enter the suite name" 8 39 --title "Create Test" 3>&1 1>&2 2>&3)
		if ! [[ $SUITE_NAME =~ ^[a-zA-Z0-9_]+$ ]]; then
			whiptail --title "Create Test" --msgbox "Invalid name! Enter a valid suite name." 8 78
			COMMAND="utf"
			continue
		fi
		break
	done
	while true; do
		TEST_NAME=$(whiptail --inputbox "Enter the test name" 8 39 --title "Create Test" 3>&1 1>&2 2>&3)
		if ! [[ $TEST_NAME =~ ^[a-zA-Z0-9_]+$ ]]; then
			whiptail --title "Create Test" --msgbox "Invalid name! Enter a valid test name." 8 78
			COMMAND="utf"
			continue
		fi
		break
	done
	echo "TEST("$SUITE_NAME", "$TEST_NAME") {" > create_test_tmp.txt
	echo '' >> create_test_tmp.txt
	while true; do
		whiptail --title "Create Test" --menu "Choose an option" --cancel-button "Exit" 25 90 16 \
			"Assert Function" "Test if function exists." \
			"Assert Function Sig" "Test if function has a given signature." \
			"Assert Class" "Test if class exists." \
			"Assert Class Attribute" "Test if class attribute exists." \
			"Assert Class Attribute Sig" "Test if class attribute has a given type." \
			"Assert Class Method" "Test if class method exists." \
			"Assert Class Method Sig" "Test if class method has a given signature." \
			"Assert Class Constructor" "Test if class constructor exists and has a given signature." \
			"Save & Exit" "Save test and return to main menu" \
			3>&1 1>&2 2>out.txt
		MENU=$(cat out.txt)
		rm out.txt
		case $MENU in
			"Assert Function")
				if [ "${#functions[@]}" -eq 0 ]; then
					whiptail --title "Assert Function" --msgbox "No function found! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#functions[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${functions[$i]}' Select_Function_'${functions[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Function" --radiolist "Select a function" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					FUNCTION_NAME=$(cat out.txt)
					if ! [[ $FUNCTION_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Function" --msgbox "Invalid name! Enter a valid function name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				while true; do
					MESSAGE=$(whiptail --inputbox "Enter a message if the test fails" 8 39 --title "Assert Function" 3>&1 1>&2 2>&3)
					if [ -z "$MESSAGE" ]; then
						whiptail --title "Assert Function" --msgbox "No message provide! You need to provide a message." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				echo "    ASSERT_FUNCTION("$FUNCTION_NAME", "'"'$MESSAGE'"'");" >> create_test_tmp.txt
				echo '' >> create_test_tmp.txt
				whiptail --title "Create Test" --msgbox "Assert for function "$FUNCTION_NAME" was added successfully!" 8 78
				;;
			"Assert Function Sig")
				if [ "${#functions[@]}" -eq 0 ]; then
					whiptail --title "Assert Function Sig" --msgbox "No function found! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#functions[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${functions[$i]}' Select_Function_'${functions[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Function Sig" --radiolist "Select a function" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					FUNCTION_NAME=$(cat out.txt)
					if ! [[ $FUNCTION_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Function Sig" --msgbox "Invalid name! Enter a valid function name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				IFS=' '
				read -a current_function_sigs <<< "${functions_sigs[$FUNCTION_NAME]}"
				unset IFS
				if [ "${#current_function_sigs[@]}" -eq 0 ]; then
					whiptail --title "Assert Function Sig" --msgbox "No signatures found for function "$FUNCTION_NAME"! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#current_function_sigs[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${current_function_sigs[$i]}' - OFF '
				done
				while true; do
					whiptail --title "Assert Function Sig" --radiolist "Select a signature for a function. Do you remember them? :)" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					SIGNATURE_NAME=$(cat out.txt)
					if ! [[ $SIGNATURE_NAME =~ ^[a-zA-Z0-9_]+$ ]]; then
						whiptail --title "Assert Function Sig" --msgbox "Invalid signature! Enter a valid signature." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				while true; do
					MESSAGE=$(whiptail --inputbox "Enter a message if the test fails" 8 39 --title "Assert Function Sig" 3>&1 1>&2 2>&3)
					if [ -z "$MESSAGE" ]; then
						whiptail --title "Assert Function Sig" --msgbox "No message provide! You need to provide a message." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				echo "    ASSERT_FUNCTION_SIGNATURE("$FUNCTION_NAME", "$SIGNATURE_NAME", "'"'$MESSAGE'"'") BEGIN {" >> create_test_tmp.txt
				echo '' >> create_test_tmp.txt
				whiptail --title "Create Test" --msgbox "Assert signature for function "$FUNCTION_NAME" was added successfully!" 8 78			
				while true; do
					if (whiptail --title "Assert Function Sig" --yesno "Do you want to check return value of function "$FUNCTION_NAME" ?" 8 78); then
						while true; do
							NR_INPUT_PARAMETERS=$(whiptail --inputbox "How many parameters function "$FUNCTION_NAME" has?" 8 65 --title "Assert Function Sig" 3>&1 1>&2 2>&3)
							if ! [[ $NR_INPUT_PARAMETERS =~ ^[0-9]+$ ]]; then
								whiptail --title "Assert Function Sig" --msgbox "Invalid value! Enter a positive integer." 8 78
								COMMAND="utf"
								continue
							fi
							break
						done
						ALL_PARAMETERS=""
						for ((i = 1 ; i <= $NR_INPUT_PARAMETERS ; i++ )); do
							PARAMETER=$(whiptail --inputbox "Enter value of parameter "$i 8 65 --title "Assert Function Sig" 3>&1 1>&2 2>&3)
							if [ -z "$PARAMETER" ]; then
								whiptail --title "Assert Function Sig" --msgbox "No value provide! You need to provide a value." 8 78
								COMMAND="utf"
								i=$(($i-1))
								continue
							fi
							if [ $i == $NR_INPUT_PARAMETERS ]; then
								ALL_PARAMETERS=$ALL_PARAMETERS$PARAMETER
							else
								ALL_PARAMETERS=$ALL_PARAMETERS$PARAMETER','
							fi
						done
						while true; do
							RETURN_VALUE=$(whiptail --inputbox "Enter the return value of function "$FUNCTION_NAME 8 65 --title "Assert Function Sig" 3>&1 1>&2 2>&3)
							if [ -z "$RETURN_VALUE" ]; then
								whiptail --title "Assert Function Sig" --msgbox "No value provide! You need to provide a return value." 8 78
								COMMAND="utf"
								continue
							fi
							break
						done
						while true; do
							MESSAGE=$(whiptail --inputbox "Enter a message if the test fails" 8 39 --title "Assert Function Sig" 3>&1 1>&2 2>&3)
							if [ -z "$MESSAGE" ]; then
								whiptail --title "Assert Function Sig" --msgbox "No message provide! You need to provide a message." 8 78
								COMMAND="utf"
								continue
							fi
							break
						done
						echo "        ASSERT_EQUAL("$FUNCTION_NAME"("$ALL_PARAMETERS"), "$RETURN_VALUE', "'$MESSAGE'"'");" >> create_test_tmp.txt
						whiptail --title "Assert Function Sig" --msgbox "Assert return value for function "$FUNCTION_NAME" was added successfully!" 8 78
					else
						break
					fi
				done
				echo '' >> create_test_tmp.txt
				echo "    } END" >> create_test_tmp.txt
				echo '' >> create_test_tmp.txt
				;;
			"Assert Class")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert Class" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#classes[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${classes[$i]}' Select_Class_'${classes[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Class" --radiolist "Select a class" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					CLASS_NAME=$(cat out.txt)
					if ! [[ $CLASS_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class" --msgbox "Invalid name! Enter a valid class name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				while true; do
					MESSAGE=$(whiptail --inputbox "Enter a message if the test fails" 8 39 --title "Assert Class" 3>&1 1>&2 2>&3)
					if [ -z "$MESSAGE" ]; then
						whiptail --title "Assert Class" --msgbox "No message provide! You need to provide a message." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				echo "    ASSERT_CLASS("$CLASS_NAME", "'"'$MESSAGE'"'");" >> create_test_tmp.txt
				whiptail --title "Create Test" --msgbox "Assert for class "$CLASS_NAME" was added successfully!" 8 78
				;;
			"Assert Class Attribute")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Attribute" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#classes[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${classes[$i]}' Select_Class_'${classes[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Class Attribute" --radiolist "Select a class" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					CLASS_NAME=$(cat out.txt)
					if ! [[ $CLASS_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Attribute" --msgbox "Invalid name! Enter a valid class name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				IFS=' '
				read -a current_attributes <<< "${attributes_class[$CLASS_NAME]}"
				unset IFS
				if [ "${#current_attributes[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Attribute" --msgbox "No attribute found for class "$CLASS_NAME"! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#current_attributes[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${current_attributes[$i]}' Select_Attribute_'${current_attributes[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Class Attribute" --radiolist "Select an attribute" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					ATTRIBUTE_NAME=$(cat out.txt)
					if ! [[ $ATTRIBUTE_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Attribute" --msgbox "Invalid name! Enter a valid attribute name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				while true; do
					MESSAGE=$(whiptail --inputbox "Enter a message if the test fails" 8 39 --title "Assert Class Attribute" 3>&1 1>&2 2>&3)
					if [ -z "$MESSAGE" ]; then
						whiptail --title "Assert Class Attribute" --msgbox "No message provide! You need to provide a message." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				echo "    ASSERT_CLASS_ATTRIBUTE("$CLASS_NAME", "$ATTRIBUTE_NAME", "'"'$MESSAGE'"'");" >> create_test_tmp.txt
				whiptail --title "Create Test" --msgbox "Assert for class "$CLASS_NAME" for attribute "$ATTRIBUTE_NAME" was added successfully!" 8 78
				;;
			"Assert Class Attribute Sig")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Attribute Sig" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#classes[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${classes[$i]}' Select_Class_'${classes[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Class Attribute Sig" --radiolist "Select a class" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					CLASS_NAME=$(cat out.txt)
					if ! [[ $CLASS_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Attribute Sig" --msgbox "Invalid name! Enter a valid class name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				IFS=' '
				read -a current_attributes <<< "${attributes_class[$CLASS_NAME]}"
				unset IFS
				if [ "${#current_attributes[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Attribute Sig" --msgbox "No attribute found for class "$CLASS_NAME"! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#current_attributes[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${current_attributes[$i]}' Select_Attribute_'${current_attributes[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Class Attribute Sig" --radiolist "Select an attribute" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					ATTRIBUTE_NAME=$(cat out.txt)
					if ! [[ $ATTRIBUTE_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Attribute Sig" --msgbox "Invalid name! Enter a valid attribute name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				IFS=' '
				read -a current_attribute_sigs <<< "${attributes_sigs[$ATTRIBUTE_NAME]}"
				unset IFS
				if [ "${#current_attribute_sigs[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Attribute Sig" --msgbox "No signatures found for attribute "$ATTRIBUTE_NAME"! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#current_attribute_sigs[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${current_attribute_sigs[$i]}' - OFF '
				done
				while true; do
					whiptail --title "Assert Class Attributes Sig" --radiolist "Select a type for attribute. Do you remember them? :)" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					SIGNATURE_NAME=$(cat out.txt)
					if ! [[ $SIGNATURE_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Attribute Sig" --msgbox "Invalid signature! Enter a valid signature." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				while true; do
					MESSAGE=$(whiptail --inputbox "Enter a message if the test fails" 8 39 --title "Assert Class Attribute Sig" 3>&1 1>&2 2>&3)
					if [ -z "$MESSAGE" ]; then
						whiptail --title "Assert Class Attribute Sig" --msgbox "No message provide! You need to provide a message." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				echo "    ASSERT_CLASS_ATTRIBUTE_SIGNATURE("$CLASS_NAME", "$ATTRIBUTE_NAME", "$SIGNATURE_NAME", "'"'$MESSAGE'"'");" >> create_test_tmp.txt
				whiptail --title "Create Test" --msgbox "Assert signature for attribute "$ATTRIBUTE_NAME" in class "$CLASS_NAME" was added successfully!" 8 78
				;;
			"Assert Class Method")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Method" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#classes[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${classes[$i]}' Select_Class_'${classes[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Class Method" --radiolist "Select a class" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					CLASS_NAME=$(cat out.txt)
					if ! [[ $CLASS_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Method" --msgbox "Invalid name! Enter a valid class name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				IFS=' '
				read -a current_methods <<< "${methods_class[$CLASS_NAME]}"
				unset IFS
				if [ "${#current_methods[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Method" --msgbox "No method found for class "$CLASS_NAME"! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#current_methods[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${current_methods[$i]}' Select_Method_'${current_methods[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Class Method" --radiolist "Select a method" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					METHOD_NAME=$(cat out.txt)
					if ! [[ $METHOD_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Method" --msgbox "Invalid name! Enter a valid method name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				while true; do
					MESSAGE=$(whiptail --inputbox "Enter a message if the test fails" 8 39 --title "Assert Class Method" 3>&1 1>&2 2>&3)
					if [ -z "$MESSAGE" ]; then
						whiptail --title "Assert Class Method" --msgbox "No message provide! You need to provide a message." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				echo "    ASSERT_CLASS_METHOD("$CLASS_NAME", "$METHOD_NAME", "'"'$MESSAGE'"'");" >> create_test_tmp.txt
				whiptail --title "Create Test" --msgbox "Assert for class "$CLASS_NAME" for method "$METHOD_NAME" was added successfully!" 8 78
				;;
			"Assert Class Method Sig")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Method Sig" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#classes[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${classes[$i]}' Select_Class_'${classes[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Class Method Sig" --radiolist "Select a class" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					CLASS_NAME=$(cat out.txt)
					if ! [[ $CLASS_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Method Sig" --msgbox "Invalid name! Enter a valid class name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				IFS=' '
				read -a current_methods <<< "${methods_class[$CLASS_NAME]}"
				unset IFS
				if [ "${#current_methods[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Method Sig" --msgbox "No method found for class "$CLASS_NAME"! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#current_methods[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${current_methods[$i]}' Select_Method_'${current_methods[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Class Method Sig" --radiolist "Select a method" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					METHOD_NAME=$(cat out.txt)
					if ! [[ $METHOD_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Method Sig" --msgbox "Invalid name! Enter a valid method name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				IFS=' '
				read -a current_method_sigs <<< "${methods_sigs[$METHOD_NAME]}"
				unset IFS
				if [ "${#current_method_sigs[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Method Sig" --msgbox "No signatures found for method "$METHOD_NAME"! Return to main menu." 8 78
					continue
				fi
				whiplist_args=''
				for ((i = 0 ; i < ${#current_method_sigs[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${current_method_sigs[$i]}' - OFF '
				done
				while true; do
					whiptail --title "Assert Class Method Sig" --radiolist "Select a signature for method. Do you remember them? :)" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					SIGNATURE_NAME=$(cat out.txt)
					if ! [[ $SIGNATURE_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Method Sig" --msgbox "Invalid signature! Enter a valid signature." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				while true; do
					MESSAGE=$(whiptail --inputbox "Enter a message if the test fails" 8 39 --title "Assert Class Method Sig" 3>&1 1>&2 2>&3)
					if [ -z "$MESSAGE" ]; then
						whiptail --title "Assert Class Method Sig" --msgbox "No message provide! You need to provide a message." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				echo "    ASSERT_CLASS_METHOD_SIGNATURE("$CLASS_NAME", "$METHOD_NAME", "$SIGNATURE_NAME", "'"'$MESSAGE'"'");" >> create_test_tmp.txt
				whiptail --title "Create Test" --msgbox "Assert signature for method "$METHOD_NAME" in class "$CLASS_NAME" was added successfully!" 8 78
				;;
			"Assert Class Constructor")
		        if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert Class Constructor" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
		        whiplist_args=''
				for ((i = 0 ; i < ${#classes[@]} ; i++ )); do
					whiplist_args=$whiplist_args' '${classes[$i]}' Select_Class_'${classes[$i]}' OFF '
				done
				while true; do
					whiptail --title "Assert Class Constructor" --radiolist "Select a class" 20 78 13 $whiplist_args 3>&1 1>&2 2>out.txt
					CLASS_NAME=$(cat out.txt)
					if ! [[ $CLASS_NAME =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Constructor" --msgbox "Invalid name! Enter a valid class name." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				rm out.txt
				while true; do
				    NR_INPUT_PARAMETERS=$(whiptail --inputbox "How many parameters constructor of class "$CLASS_NAME" has?" 8 65 --title "Assert Class Constructor" 3>&1 1>&2 2>&3)
					if ! [[ $NR_INPUT_PARAMETERS =~ ^[0-9]+$ ]]; then
						whiptail --title "Assert Class Constructor" --msgbox "Invalid value! Enter a positive integer." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
				ALL_PARAMETERS=""
				for ((i = 1 ; i <= $NR_INPUT_PARAMETERS ; i++ )); do
					PARAMETER=$(whiptail --inputbox "Enter the type of parameter "$i 8 65 --title "Assert Class Constructor" 3>&1 1>&2 2>&3)
					if ! [[ $PARAMETER =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
						whiptail --title "Assert Class Constructor" --msgbox "Invalid type! Enter a valid type." 8 78
						COMMAND="utf"
						i=$(($i-1))
						return 0
					fi
					if [ $i == $NR_INPUT_PARAMETERS ]; then
						ALL_PARAMETERS=$ALL_PARAMETERS$PARAMETER
					else
						ALL_PARAMETERS=$ALL_PARAMETERS$PARAMETER','
					fi
				done
				while true; do
				    MESSAGE=$(whiptail --inputbox "Enter a message if the test fails" 8 39 --title "Assert Class Constructor" 3>&1 1>&2 2>&3)
					if [ -z "$MESSAGE" ]; then
						whiptail --title "Assert Class Constructor" --msgbox "No message provide! You need to provide a message." 8 78
						COMMAND="utf"
						continue
					fi
					break
				done
		        echo "    ASSERT_CLASS_CONSTRUCTOR("'"'$MESSAGE'"'", "$CLASS_NAME", "$ALL_PARAMETERS") BEGIN {" >> create_test_tmp.txt
		        
		        
		        
																	##########    TODO    ##########
				
				
				
				echo "    } END" >> create_test_tmp.txt
				whiptail --title "Create Test" --msgbox "Assert constructor for class "$CLASS_NAME" was added successfully!" 8 78	        	
				;;
			"Save & Exit")
				if (whiptail --title "Save & Exit" --yesno "Do you want to save changes?" 8 78); then
					whiptail --title "Save & Exit" --msgbox "Test "$SUITE_NAME"::"$TEST_NAME" was created!" 8 78
					echo '' >> create_test_tmp.txt
					echo "}" >> create_test_tmp.txt
					echo '' >> create_test_tmp.txt
					cat create_test_tmp.txt >> tests.h
					rm create_test_tmp.txt
					COMMAND="utf"
					break
				else
					continue
				fi
				;;
			*)
				if (whiptail --title "Close" --yesno "Your changes will be lost. Do you want to return to main menu?" 8 78); then
					rm create_test_tmp.txt
					break
				fi
				;;
		esac
	done
}

while true; do
	case $COMMAND in 
		"description") 
			whiptail --title "Unit Test Framework Creater" --msgbox "Create custom tests for VPL Moodle. Press OK to continue." 8 78
			add_classes
			COMMAND="utf"
			;;
		"utf")
			menu_creater		
			case $MENU in
				"Documentation")
					DOCS=$(cat ../DOC.txt)
					whiptail --title "Documentation" --scrolltext --msgbox "$DOCS" 50 200
					;;
				"Add Attribute")
					add_attribute
					;;
				"Add Method")
					add_method
					;;
				"Add Function")
					add_function
					;;
				"Create Test")			
					create_test
					;;
				*)
					rm create_test_tmp.txt
					break
					;;
			esac				
			;;
		*)
			break
			;;
	esac
done

