#!/bin/bash

# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

source ./common.sh

function test_creator {
	local -n attributes=$1
	local -n attributes_sigs=$2
	local -n attributes_class=$3
	local -n methods=$4
	local -n methods_sigs=$5
	local -n methods_class=$6
	local -n operators=$7
	local -n operators_sigs=$8
	local -n operators_class=$9
	local -n functions=${10}
	local -n functions_sigs=${11}
	local -n classes=${12}
	
	get_entity SUITE_NAME "Register suite" "Enter the suite name" "Invalid name! Enter a valid suite name." "suite"
	get_entity TEST_NAME "Register test" "Enter the test name" "Invalid name! Enter a valid test name." "test"
	echo -e "TEST("$SUITE_NAME", "$TEST_NAME") {\n" > current_test.txt
	
	while true; do
		whiptail --title "Create Test" --menu "Choose an option" --cancel-button "Exit" 25 97 16 \
			"Assert function" "Test if function exists." \
			"Assert function signature" "Test if function has a given signature." \
			"Assert class" "Test if class exists." \
			"Assert class attribute" "Test if class attribute exists." \
			"Assert class attribute signature" "Test if class attribute has a given type." \
			"Assert class method" "Test if class method exists." \
			"Assert class method signature" "Test if class method has a given signature." \
			"Assert class operator" "Test if class operator exists." \
			"Assert class operator signature" "Test if class operator has a given signature." \
			"Assert class constructor" "Test if class constructor exists and has a given signature." \
			"Save & Exit" "Save test and return to main menu" \
			3>&1 1>&2 2>out.txt
			
		MENU=$(cat out.txt && rm out.txt)
		
		case $MENU in
			"Assert function")
				if [ "${#functions[@]}" -eq 0 ]; then
					whiptail --title "Assert function" --msgbox "No function found! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args functions "function"
				get_radiolist_name FUNCTION_NAME whiplist_args "Assert function" "Select function" "No function selected! Please select a function."
				get_message MESSAGE "Assert function"
				
				echo -e "    ASSERT_FUNCTION("$FUNCTION_NAME", "'"'$MESSAGE'"'");\n" >> current_test.txt
				whiptail --title "Create Test" --msgbox "Assert for function "$FUNCTION_NAME" was added successfully!" 8 78
				;;
			"Assert function signature")
				if [ "${#functions[@]}" -eq 0 ]; then
					whiptail --title "Assert function" --msgbox "No function found! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args functions "function"
				get_radiolist_name FUNCTION_NAME whiplist_args "Assert function signature" "Select function" "No function selected! Please select a function."
				split_string functions_sigs[$FUNCTION_NAME] current_function_sigs " "

				if [ "${#current_function_sigs[@]}" -eq 0 ]; then
					whiptail --title "Assert function" --msgbox "No signatures found for function "$FUNCTION_NAME"! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args current_function_sigs "function"
				get_radiolist_signature SIGNATURE_NAME whiplist_args "Assert function signature" "Select function signature. Do you remember them? :)"
				get_message MESSAGE "Assert function"

				echo "    ASSERT_FUNCTION_SIGNATURE("$FUNCTION_NAME", "$SIGNATURE_NAME", "'"'$MESSAGE'"'") BEGIN {\n" >> current_test.txt
				whiptail --title "Create Test" --msgbox "Assert signature for function "$FUNCTION_NAME" was added successfully!" 8 78			
				while true; do
					if (whiptail --title "Assert function value" --yesno "Do you want to check return value of function "$FUNCTION_NAME" ?" 8 78); then
					
						get_integer NR_INPUT_PARAMETERS "Assert function value" "How many parameters function $FUNCTION_NAME has?" "Invalid value! Enter a positive integer."
						get_types UNUSED ALL_PARAMETERS NR_INPUT_PARAMETERS "Assert function value" "Enter the type of parameter" "Invalid type! Enter a valid type."
						get_entity RETURN_VALUE "Assert function value" "Enter the return value of function $FUNCTION_NAME" "No value provide! You need to provide a return value." "value"
						get_message MESSAGE "Assert function value"
						
						echo "        ASSERT_EQUAL("$FUNCTION_NAME"("$ALL_PARAMETERS"), "$RETURN_VALUE', "'$MESSAGE'"'");" >> current_test.txt
						whiptail --title "Assert function value" --msgbox "Assert return value for function "$FUNCTION_NAME" was added successfully!" 8 78
					else
						break
					fi
				done
				echo "" >> current_test.txt
				echo "    } END" >> current_test.txt
				echo "" >> current_test.txt
				;;
			"Assert class")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert class" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args classes "class"
				get_radiolist_name CLASS_NAME whiplist_args "Assert class" "Select class" "No class selected! Please select a class."
				get_message MESSAGE "Assert class"
				
				echo "    ASSERT_CLASS("$CLASS_NAME", "'"'$MESSAGE'"'");" >> current_test.txt
				whiptail --title "Create Test" --msgbox "Assert for class "$CLASS_NAME" was added successfully!" 8 78
				;;
			"Assert class attribute")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert class attribute" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args classes "class"
				get_radiolist_name CLASS_NAME whiplist_args "Assert class attribute" "Select class" "No class selected! Please select a class."
				split_string attributes_class[$CLASS_NAME] current_attributes " "
				
				if [ "${#current_attributes[@]}" -eq 0 ]; then
					whiptail --title "Assert class attribute" --msgbox "No attribute found for class "$CLASS_NAME"! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args current_attributes "attribute"
				get_radiolist_name ATTRIBUTE_NAME whiplist_args "Assert class attribute" "Select attribute" "No attribute selected! Please select an attribute."
				get_message MESSAGE "Assert class attribute"

				echo "    ASSERT_CLASS_ATTRIBUTE("$CLASS_NAME", "$ATTRIBUTE_NAME", "'"'$MESSAGE'"'");" >> current_test.txt
				whiptail --title "Create Test" --msgbox "Assert for class "$CLASS_NAME" for attribute "$ATTRIBUTE_NAME" was added successfully!" 8 78
				;;
			"Assert class attribute signature")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert class attribute signature" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args classes "class"
				get_radiolist_name CLASS_NAME whiplist_args "Assert class attribute signature" "Select class" "No class selected! Please select a class."
				split_string attributes_class[$CLASS_NAME] current_attributes " "
				
				if [ "${#current_attributes[@]}" -eq 0 ]; then
					whiptail --title "Assert class attribute signature" --msgbox "No attribute found for class "$CLASS_NAME"! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args current_attributes "attribute"
				get_radiolist_name ATTRIBUTE_NAME whiplist_args "Assert class attribute signature" "Select attribute" "No attribute selected! Please select an attribute."
				split_string attributes_sigs[$ATTRIBUTE_NAME] current_attribute_sigs " "
				
				if [ "${#current_attribute_sigs[@]}" -eq 0 ]; then
					whiptail --title "Assert class attribute signature" --msgbox "No signatures found for attribute "$ATTRIBUTE_NAME"! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args current_attribute_sigs "attribute"
				get_radiolist_signature SIGNATURE_NAME whiplist_args "Assert class attribute signature" "Select attribute signature. Do you remember them? :)"
				get_message MESSAGE "Assert class attribute signature"
				
				echo "    ASSERT_CLASS_ATTRIBUTE_SIGNATURE("$CLASS_NAME", "$ATTRIBUTE_NAME", "$SIGNATURE_NAME", "'"'$MESSAGE'"'");" >> current_test.txt
				whiptail --title "Create Test" --msgbox "Assert signature for attribute "$ATTRIBUTE_NAME" in class "$CLASS_NAME" was added successfully!" 8 78
				;;
			"Assert class method")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert class method" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args classes "class"
				get_radiolist_name CLASS_NAME whiplist_args "Assert class method" "Select class" "No class selected! Please select a class."
				split_string methods_class[$CLASS_NAME] current_methods " "
				
				if [ "${#current_methods[@]}" -eq 0 ]; then
					whiptail --title "Assert class method" --msgbox "No method found for class "$CLASS_NAME"! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args current_methods "method"
				get_radiolist_name METHOD_NAME whiplist_args "Assert class method" "Select method" "No method selected! Please select a method."
				get_message MESSAGE "Assert class method"
				
				echo "    ASSERT_CLASS_METHOD("$CLASS_NAME", "$METHOD_NAME", "'"'$MESSAGE'"'");" >> current_test.txt
				whiptail --title "Create Test" --msgbox "Assert for class "$CLASS_NAME" for method "$METHOD_NAME" was added successfully!" 8 78
				;;
			"Assert class method signature")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert class method signature" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args classes "class"
				get_radiolist_name CLASS_NAME whiplist_args "Assert class method signature" "Select class" "No class selected! Please select a class."
				split_string methods_class[$CLASS_NAME] current_methods " "
				
				if [ "${#current_methods[@]}" -eq 0 ]; then
					whiptail --title "Assert class method signature" --msgbox "No method found for class "$CLASS_NAME"! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args current_methods "method"
				get_radiolist_name METHOD_NAME whiplist_args "Assert class method signature" "Select method" "No method selected! Please select a method."
				split_string methods_sigs[$METHOD_NAME] current_method_sigs " "
				
				if [ "${#current_method_sigs[@]}" -eq 0 ]; then
					whiptail --title "Assert class method signature" --msgbox "No signatures found for method "$METHOD_NAME"! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args current_method_sigs "method"
				get_radiolist_signature SIGNATURE_NAME whiplist_args "Assert class method signature" "Select method signature. Do you remember them? :)"
				get_message MESSAGE "Assert class method signature"
				
				echo "    ASSERT_CLASS_METHOD_SIGNATURE("$CLASS_NAME", "$METHOD_NAME", "$SIGNATURE_NAME", "'"'$MESSAGE'"'");" >> current_test.txt
				whiptail --title "Create Test" --msgbox "Assert signature for method "$METHOD_NAME" in class "$CLASS_NAME" was added successfully!" 8 78
				;;
			"Assert class operator")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert class operator" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args classes "class"
				get_radiolist_name CLASS_NAME whiplist_args "Assert class operator" "Select class" "No class selected! Please select a class."
				split_string operators_class[$CLASS_NAME] current_operators " "
				
				if [ "${#current_operators[@]}" -eq 0 ]; then
					whiptail --title "Assert class operator" --msgbox "No operator found for class "$CLASS_NAME"! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args current_operators "operator"
				get_radiolist_name OPERATOR_NAME whiplist_args "Assert class operator" "Select operator" "No operator selected! Please select an operator."
				get_message MESSAGE "Assert class operator"
				
				echo "    ASSERT_CLASS_OPERATOR("$CLASS_NAME", "$OPERATOR_NAME", "'"'$MESSAGE'"'");" >> current_test.txt
				whiptail --title "Create Test" --msgbox "Assert for class "$CLASS_NAME" for operator "$OPERATOR_NAME" was added successfully!" 8 78
				;;
			"Assert class operator signature")
				if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert class operator signature" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args classes "class"
				get_radiolist_name CLASS_NAME whiplist_args "Assert class operator signature" "Select class" "No class selected! Please select a class."
				split_string operators_class[$CLASS_NAME] current_operators " "
				
				if [ "${#current_operators[@]}" -eq 0 ]; then
					whiptail --title "Assert class operator signature" --msgbox "No operator found for class "$CLASS_NAME"! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args current_operators "operator"
				get_radiolist_name OPERATOR_NAME whiplist_args "Assert class operator signature" "Select operator" "No operator selected! Please select an operator."
				split_string operators_sigs[$OPERATOR_NAME] current_operator_sigs " "
				
				if [ "${#current_operator_sigs[@]}" -eq 0 ]; then
					whiptail --title "Assert class operator signature" --msgbox "No signatures found for operator "$OPERATOR_NAME"! Return to main menu." 8 78
					continue
				fi
				
				create_checklist_args whiplist_args current_operator_sigs "operator"
				get_radiolist_signature SIGNATURE_NAME whiplist_args "Assert class operator signature" "Select operator signature. Do you remember them? :)"
				get_message MESSAGE "Assert class operator signature"
				
				echo "    ASSERT_CLASS_OPERATOR_SIGNATURE("$CLASS_NAME", "$OPERATOR_NAME", "$SIGNATURE_NAME", "'"'$MESSAGE'"'");" >> current_test.txt
				whiptail --title "Create Test" --msgbox "Assert signature for operator "$OPERATOR_NAME" in class "$CLASS_NAME" was added successfully!" 8 78
				;;
			"Assert class constructor")
		        if [ "${#classes[@]}" -eq 0 ]; then
					whiptail --title "Assert class constructor" --msgbox "No class found! Return to main menu." 8 78
					continue
				fi
				
		        create_checklist_args whiplist_args classes "constructor"
				get_radiolist_name CLASS_NAME whiplist_args "Assert class constructor" "Select class" "No class selected! Please select a class."
				get_integer NR_INPUT_PARAMETERS "Assert class constructor" "How many parameters constructor of class $CLASS_NAME has?" "Invalid value! Enter a positive integer."
				get_types UNUSED ALL_PARAMETERS NR_INPUT_PARAMETERS "Assert class constructor" "Enter the type of parameter" "Invalid type! Enter a valid type."
				get_message MESSAGE "Assert class constructor"
				
		        echo "    ASSERT_CLASS_CONSTRUCTOR("'"'$MESSAGE'"'", "$CLASS_NAME", "$ALL_PARAMETERS") BEGIN {" >> current_test.txt      
																	##########    TODO    ##########	
				echo "" >> current_test.txt
				echo "    } END" >> current_test.txt
				whiptail --title "Create Test" --msgbox "Assert constructor for class "$CLASS_NAME" was added successfully!" 8 78	        	
				;;
			"Save & Exit")
				if (whiptail --title "Save & Exit" --yesno "Do you want to save changes?" 8 78); then
					whiptail --title "Save & Exit" --msgbox "Test "$SUITE_NAME"::"$TEST_NAME" was created!" 8 78
					echo -e "\n}\n" >> current_test.txt
					cat current_test.txt >> tests.h
					rm current_test.txt 2> /dev/null
					break
				else
					continue
				fi
				;;
			*)
				if (whiptail --title "Close" --yesno "Your changes will be lost. Do you want to return to main menu?" 8 78); then
					rm current_test.txt 2> /dev/null
					break
				fi
				;;
		esac
	done
}

