#!/bin/bash

# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

COMMAND="description"
rm -f tests.h && touch tests.h

rclasses=()
rattributes=()
rmethods=()
roperators=()
rfunctions=()

declare -A rattributes_class
declare -A rmethods_class
declare -A roperators_class
declare -A rfunctions_sigs
declare -A rattributes_sigs
declare -A rmethods_sigs
declare -A roperators_sigs

source ./menu_creator.sh
source ./add_class.sh
source ./add_attribute.sh
source ./add_method.sh
source ./add_operator.sh
source ./add_function.sh
source ./test_creator.sh

while true; do
	case $COMMAND in 
		"description") 
			whiptail --title "Unit Test Framework Creater" --msgbox "Create custom tests for VPL Moodle. Press OK to continue." 8 78
			add_class rclasses
			COMMAND="utf"
			;;
		"utf")
			menu_creator MENU	
			case $MENU in
				"Documentation")
					DOCS=$(cat ../DOC.txt)
					whiptail --title "Documentation" --scrolltext --msgbox "$DOCS" 50 200
					;;
				"Add Attribute")
					add_attribute rattributes rattributes_sigs rattributes_class rclasses
					;;
				"Add Method")
					add_method rmethods rmethods_sigs rmethods_class rclasses
					;;
				"Add Operator")
					add_operator roperators roperators_sigs roperators_class rclasses
					;;
				"Add Function")
					add_function rfunctions rfunctions_sigs
					;;
				"Create Test")			
					test_creator rattributes rattributes_sigs rattributes_class rmethods rmethods_sigs rmethods_class roperators roperators_sigs roperators_class rfunctions rfunctions_sigs rclasses
					;;
				*)
					rm create_test_tmp.txt 2> /dev/null
					break
					;;
			esac				
			;;
		*)
			break
			;;
	esac
done

