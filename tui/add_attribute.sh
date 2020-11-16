#!/bin/bash

# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

source ./common.sh

function add_attribute() {
	local -n attributes=$1
	local -n attributes_sigs=$2
	local -n attributes_class=$3
	local -n classes=$4
	
	whiptail --title "Add Attribute" --menu "Choose an option" --cancel-button "Exit" 25 90 16 \
		"Register            " "Register attribute you want to test." \
		"Signature        " "Register a type for attribute you want to check." \
		3>&1 1>&2 2>out.txt
	
	MENU=$(cat out.txt && rm out.txt)
	
	case $MENU in
		"Register            ")
			if [ "${#classes[@]}" -eq 0 ]; then
				whiptail --title "Register attribute" --msgbox "No class found! Return to main menu." 8 78
				return 0
			fi
			
			get_entity ATTRIBUTE_NAME "Register attribute" "Enter the attribute name" "Invalid name! Enter a valid attribute name." "attribute"
			attributes+=("$ATTRIBUTE_NAME")
			echo -e 'CHECK_CLASS_ATTRIBUTE('$ATTRIBUTE_NAME');\n' >> tests.h
			
			create_checklist_args whiplist_args classes "class"
			whiptail --title "Register attribute" --checklist "Which classes have the attribute "$ATTRIBUTE_NAME"?" 20 78 4 $whiplist_args 3>&1 1>&2 2>out.txt
			
			CLASSES=$(cat out.txt && rm out.txt)
			split_string CLASSES classes_arr " "
			link_entity classes_arr attributes_class ATTRIBUTE_NAME
			whiptail --title "Register Attribute" --msgbox "Attribute "$ATTRIBUTE_NAME" was added successfully!" 8 78
			;;
		"Signature        ")
			if [ "${#attributes[@]}" -eq 0 ]; then
				whiptail --title "Register attribute signature" --msgbox "No attribute found! Return to main menu." 8 78
				return 0
			fi
			
			create_checklist_args whiplist_args attributes "attribute"
			get_radiolist_name ATTRIBUTE_NAME whiplist_args "Register attribute signature" "Select attribute" "No attribute selected! Please select an attribute."
			get_entity ATTRIBUTE_TYPE "Register attribute signature" "Enter the type of attribute $ATTRIBUTE_NAME" "Invalid type! Enter a valid attribute type." "type"
			get_template_postfix TEMPLATE_POSTFIX "Register attribute signature" "attribute $ATTRIBUTE_NAME"

			attributes_sigs[$ATTRIBUTE_NAME]=${attributes_sigs[$ATTRIBUTE_NAME]}" "$TEMPLATE_POSTFIX
			echo -e 'CHECK_CLASS_ATTRIBUTE_SIGNATURE('$ATTRIBUTE_NAME', '$ATTRIBUTE_TYPE' T::*, '$TEMPLATE_POSTFIX');\n' >> tests.h	
			whiptail --title "Register Signature" --msgbox "Signature for attribute "$ATTRIBUTE_NAME" was added successfully!" 8 78
			;;
		*)
			;;
	esac
}

