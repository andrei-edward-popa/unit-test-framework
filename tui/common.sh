#!/bin/bash

# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

function get_integer() {
	local -n NR_PARAM=$1
	TITLE=$2
	IBOX=$3
	MBOX=$4
	while true; do
		NR_PARAM=$(whiptail --inputbox "$IBOX" 8 65 --title "$TITLE" 3>&1 1>&2 2>&3)
		if ! [[ $NR_PARAM =~ ^[0-9]+$ ]]; then
			whiptail --title "$TITLE" --msgbox "$MBOX" 8 78
			continue
		fi
		break
	done
}

function get_types() {
	local -n LIST=$1
	local -n PARAMS=$2
	local -n NR_PARAMS=$3
	TITLE=$4
	IBOX=$5
	MBOX=$6
	PARAMS=""
	for ((i = 1 ; i <= $NR_PARAMS ; i++ )); do
		PARAMETER=$(whiptail --inputbox "$IBOX $i" 8 65 --title "$TITLE" 3>&1 1>&2 2>&3)
		if ! [[ $PARAMETER =~ ^[A-Za-z0-9\*\&\ ]+$ ]]; then
			whiptail --title "$TITLE" --msgbox "$MBOX" 8 78
			i=$(($i-1))
			continue
		fi
		if [ $i == $NR_PARAMS ]; then
			PARAMS=$PARAMS$PARAMETER
		else
			PARAMS=$PARAMS$PARAMETER","
		fi
		LIST+=("$PARAMETER")
	done
}

function split_string() {
	local -n STRING=$1
	local -n LIST=$2
	SEPARATOR=$3
	IFS=$SEPARATOR
	read -a LIST <<< "$STRING"
	unset IFS
}

function link_entity() {
	local -n LIST=$1
	local -n HASH_MAP=$2
	local -n ENTITY=$3
	for ((i = 0 ; i < ${#LIST[@]} ; i++ )); do
		LIST[$i]=$(echo ${LIST[$i]} | cut -d '"' -f 2)
		HASH_MAP[${LIST[$i]}]=${HASH_MAP[${LIST[$i]}]}" "$ENTITY
	done	
}

function create_checklist_args() {
	local -n ARGS=$1
	local -n LIST=$2
	TYPE=$3
	ARGS=""
	for ((i = 0 ; i < ${#LIST[@]} ; i++ )); do
		ARGS=$ARGS" "${LIST[$i]}" Select_"$TYPE"_"${LIST[$i]}" OFF "
	done
}

function get_message() {
	local -n MSG=$1
	TITLE=$2
	while true; do
		MSG=$(whiptail --inputbox "Enter a message if the test fails" 8 39 --title "$TITLE" 3>&1 1>&2 2>&3)
		if [ -z "$MSG" ]; then
			whiptail --title "$TITLE" --msgbox "No message provide! You need to provide a message." 8 78
			continue
		fi
		break
	done
}

function get_template_postfix() {
	local -n TEMPLATE=$1
	TITLE=$2
	TYPENAME=$3
	while true; do
		TEMPLATE=$(whiptail --inputbox "Enter an identifier for $TYPENAME. REMEMBER IT!" 8 65 --title "$TITLE" 3>&1 1>&2 2>&3)
		if ! [[ $TEMPLATE =~ ^[a-zA-Z0-9_]+$ ]]; then
			whiptail --title "$TITLE" --msgbox "Invalid identifier! Enter a valid identifier." 8 78
			continue
		fi
		break
	done
}

function get_radiolist_name() {
	local -n NAME=$1
	local -n NAMES=$2
	TITLE=$3
	RADIOLIST=$4
	MBOX=$5
	while true; do
		whiptail --title "$TITLE" --radiolist "$RADIOLIST" 20 78 13 $NAMES 3>&1 1>&2 2>out.txt
		NAME=$(cat out.txt && rm out.txt)
		if [[ -z $NAME ]]; then
			whiptail --title "$TITLE" --msgbox "$MBOX" 8 78
			continue
		fi
		break
	done
}

function get_entity() {
	local -n ENTITY=$1
	TITLE=$2
	IBOX=$3
	MBOX=$4
	TYPE=$5
	while true; do
		ENTITY=$(whiptail --inputbox "$IBOX" 8 39 --title "$TITLE" 3>&1 1>&2 2>&3)
		if ! [[ ( $TYPE != "operator" && $ENTITY =~ ^[a-zA-Z_][a-zA-Z0-9_]*$) || ($TYPE == "operator" && $ENTITY =~ ^operator[^A-Z0-9]*$ ) || ($TYPE == "value" && -z "$ENTITY" ) ]]; then
			whiptail --title "$TITLE" --msgbox "$MBOX" 8 78
			continue
		fi
		break
	done
}

