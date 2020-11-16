#!/bin/bash

# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

source ./common.sh

function add_method {
	local -n methods=$1
	local -n methods_sigs=$2
	local -n methods_class=$3
	local -n classes=$4
	
	whiptail --title "Add Method" --menu "Choose an option" --cancel-button "Exit" 25 90 16 \
		"Register        " "Register method you want to test." \
		"Signature        " "Register a signature for method you want to check." \
		3>&1 1>&2 2>out.txt
		
	MENU=$(cat out.txt && rm out.txt)
	
	case $MENU in
		"Register        ")
			if [ "${#classes[@]}" -eq 0 ]; then
				whiptail --title "Register method" --msgbox "No class found! Return to main menu." 8 78
				return 0
			fi
			
			get_entity METHOD_NAME "Register method" "Enter the method name" "Invalid name! Enter a valid method name." "method"
			methods+=("$METHOD_NAME")
			echo -e 'CHECK_CLASS_METHOD('$METHOD_NAME');\n' >> tests.h
			
			create_checklist_args whiplist_args classes "class"
			whiptail --title "Register method" --checklist "Which classes have the method "$METHOD_NAME"?" 20 78 4 $whiplist_args 3>&1 1>&2 2>out.txt
			
			CLASSES=$(cat out.txt && rm out.txt)
			split_string CLASSES classes_arr " "
			link_entity classes_arr methods_class METHOD_NAME
			whiptail --title "Register method" --msgbox "Method "$METHOD_NAME" was added successfully!" 8 78
			;;
		"Signature        ")
			if [ "${#methods[@]}" -eq 0 ]; then
				whiptail --title "Register method signature" --msgbox "No method found! Return to main menu." 8 78
				return 0
			fi
			
			create_checklist_args whiplist_args methods "method"
			get_radiolist_name METHOD_NAME whiplist_args "Register method signature" "Select method" "No method selected! Please select a method."
			get_entity RETURN_TYPE "Register method signature" "Enter the return type of method $METHOD_NAME" "Invalid type! Enter a valid return type." "type"
			get_template_postfix TEMPLATE_POSTFIX "Register method signature" "method $METHOD_NAME"
			get_integer NR_INPUT_PARAMETERS "Register method signature" "How many parameters method $METHOD_NAME has?" "Invalid value! Enter a positive integer."
			get_types UNUSED ALL_PARAMETERS NR_INPUT_PARAMETERS "Register method signature" "Enter the type of parameter" "Invalid type! Enter a valid type."
			
			methods_sigs[$METHOD_NAME]=${methods_sigs[$METHOD_NAME]}" "$TEMPLATE_POSTFIX
			echo -e 'CHECK_CLASS_METHOD_SIGNATURE('$METHOD_NAME', '$RETURN_TYPE'(T::*)('$ALL_PARAMETERS'), '$TEMPLATE_POSTFIX');\n' >> tests.h	
			whiptail --title "Register method signature" --msgbox "Signature for method "$METHOD_NAME" was added successfully!" 8 78
			;;
		*)
			;;
	esac
}

