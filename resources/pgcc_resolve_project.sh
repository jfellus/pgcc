#!/bin/bash

PROJECT=$1

if [ -z "$PROJECT" ]; then
	echo "Usage : pgcc_resolve_project <project_name>"
	exit 1
fi

mkdir -p $HOME/.pgcc

PROJECT_PATH=`grep "^$PROJECT "  $HOME/.pgcc/projects | cut -d" " -f2`


if [ -n "$PROJECT_PATH" ]; then echo $PROJECT_PATH; fi


