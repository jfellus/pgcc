#!/bin/bash

PROJECT=$1

if [ -z "$PROJECT" ]; then
	echo "Usage : pgcc_add_project <file.pgproject>"
	exit 1
fi

PROJECT_NAME=`basename $PROJECT`
PROJECT_NAME="${PROJECT_NAME%.*}"
PROJECT_PATH=`readlink -f $PROJECT`


mkdir -p $HOME/.pgcc

touch $HOME/.pgcc/projects
grep -v -e "^$PROJECT_NAME " $HOME/.pgcc/projects > $HOME/.pgcc/projects.bak

cat $HOME/.pgcc/projects.bak > $HOME/.pgcc/projects
echo "$PROJECT_NAME $PROJECT_PATH" >> $HOME/.pgcc/projects

rm -f $HOME/.pgcc/projects.bak
