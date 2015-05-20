#!/bin/bash

if [ "$1" == "-l" ]; then 
	cat $HOME/.pgcc/projects | cut -f1 -d" "
else
	cat $HOME/.pgcc/projects
fi
