#!/bin/bash

# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

source ./common.sh

function add_function() {
	local -n functions=$1
	local -n functions_sigs=$2
	
	whiptail --title "Add Function" --menu "Choose an option" --cancel-button "Exit" 25 90 16 \
		"Register        " "Register function you want to test." \
		"Signature        " "Register a signature for function you want to check." \
		3>&1 1>&2 2>out.txt
		
	MENU=$(cat out.txt && rm out.txt)
	
	case $MENU in
		"Register        ")
			get_entity FUNCTION_NAME "Register function" "Enter the function name" "Invalid name! Enter a valid function name." "function"
			functions+=("$FUNCTION_NAME")
			echo -e 'CHECK_FUNCTION('$FUNCTION_NAME');\n' >> tests.h
			whiptail --title "Register function" --msgbox "Function "$FUNCTION_NAME" was added successfully!" 8 78
			;;
		"Signature        ")
			if [ "${#functions[@]}" -eq 0 ]; then
				whiptail --title "Register function signature" --msgbox "No function found! Return to main menu." 8 78
				return 0
			fi
			
			create_checklist_args whiplist_args functions "function"
			get_radiolist_name FUNCTION_NAME whiplist_args "Register function signature" "Select function" "No function selected! Please select a function."
			get_entity RETURN_TYPE "Register function signature" "Enter the return type of function $FUNCTION_NAME" "Invalid type! Enter a valid return type." "type"
			get_template_postfix TEMPLATE_POSTFIX "Register function signature" "function $FUNCTION_NAME"
			get_integer NR_INPUT_PARAMETERS "Register function signature" "How many parameters function $FUNCTION_NAME has?" "Invalid value! Enter a positive integer."
			get_types UNUSED ALL_PARAMETERS NR_INPUT_PARAMETERS "Register function signature" "Enter the type of parameter" "Invalid type! Enter a valid type."
			
			functions_sigs[$FUNCTION_NAME]=${functions_sigs[$FUNCTION_NAME]}" "$TEMPLATE_POSTFIX
			echo -e 'CHECK_FUNCTION_SIGNATURE('$FUNCTION_NAME', '$RETURN_TYPE'(T::*)('$ALL_PARAMETERS'), '$TEMPLATE_POSTFIX');\n' >> tests.h
			whiptail --title "Register Signature" --msgbox "Signature for function "$FUNCTION_NAME" was added successfully!" 8 78
			;;
		*)
			;;
	esac
}

