#!/bin/bash

# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

source ./common.sh

function add_operator {
	local -n operators=$1
	local -n operators_sigs=$2
	local -n operators_class=$3
	local -n classes=$4
	
	whiptail --title "Add Operator" --menu "Choose an option" --cancel-button "Exit" 25 90 16 \
		"Register        " "Register operator you want to test." \
		"Signature        " "Register a signature for operator you want to check." \
		3>&1 1>&2 2>out.txt
		
	MENU=$(cat out.txt && rm out.txt)

	case $MENU in
		"Register        ")
			if [ "${#classes[@]}" -eq 0 ]; then
				whiptail --title "Register operator" --msgbox "No class found! Return to main menu." 8 78
				return 0
			fi
			
			get_entity OPERATOR_NAME "Register operator" "Enter the operator name" "Invalid name! Enter a valid operator name." "operator"
			get_entity REAL_OPERATOR_NAME "Register operator" "Enter the real operator name" "Invalid name! Enter a valid real operator name." "real_operator"
			operators+=("$OPERATOR_NAME")
			echo -e 'CHECK_CLASS_OPERATOR('$OPERATOR_NAME', '$REAL_OPERATOR_NAME');\n' >> tests.h
			
			create_checklist_args whiplist_args classes "class"
			whiptail --title "Add operator" --checklist "Which classes have the operator "$OPERATOR_NAME"?" 20 78 4 $whiplist_args 3>&1 1>&2 2>out.txt
			
			CLASSES=$(cat out.txt && rm out.txt)
			split_string CLASSES classes_arr " "
			link_entity classes_arr operators_class OPERATOR_NAME
			whiptail --title "Register operator" --msgbox "Operator "$OPERATOR_NAME" was added successfully!" 8 78
			;;
		"Signature        ")
			if [ "${#operators[@]}" -eq 0 ]; then
				whiptail --title "Register operator signature" --msgbox "No operator found! Return to main menu." 8 78
				return 0
			fi
			
			create_checklist_args whiplist_args operators "operator"
			get_radiolist_name OPERATOR_NAME whiplist_args "Register operator signature" "Select operator" "No operator selected! Please select an operator."
			get_entity RETURN_TYPE "Register operator signature" "Enter the return type of operator $OPERATOR_NAME" "Invalid type! Enter a valid return type." "type"
			get_template_postfix TEMPLATE_POSTFIX "Register operator signature" "operator $OPERATOR_NAME"
			get_integer NR_INPUT_PARAMETERS "Register operator signature" "How many parameters operator $OPERATOR_NAME has?" "Invalid value! Enter a positive integer."
			get_types UNUSED ALL_PARAMETERS NR_INPUT_PARAMETERS "Register operator signature" "Enter the type of parameter" "Invalid type! Enter a valid type."
			
			operators_sigs[$OPERATOR_NAME]=${operators_sigs[$OPERATOR_NAME]}" "$TEMPLATE_POSTFIX
			echo -e 'CHECK_CLASS_OPERATOR_SIGNATURE('$OPERATOR_NAME', '$RETURN_TYPE'(T::*)('$ALL_PARAMETERS'), '$TEMPLATE_POSTFIX');\n' >> tests.h	
			whiptail --title "Register Signature" --msgbox "Signature for operator "$OPERATOR_NAME" was added successfully!" 8 78
			;;
		*)
			;;
	esac
}

