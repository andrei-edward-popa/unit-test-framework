#!/bin/bash

# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

source ./common.sh

function menu_creator {
	local -n MMENU=$1
	
	whiptail --title "Framework Menu" --menu "Choose an option" --cancel-button "Exit" 25 90 16 \
				"Documentation" "Usage of assert functions." \
				"Add Attribute" "Define the name of the attribute you want to test." \
				"Add Method" "Define the name of the method or signatures you want to test." \
				"Add Operator" "Define the name of the operator or signatures you want to test." \
				"Add Function" "Define the name of the function you want to test." \
				"Create Test" "Create a unit test for classes/functions that you defined." \
				3>&1 1>&2 2>out.txt
				
	MMENU=$(cat out.txt && rm out.txt)
}
