#!/bin/bash

PROJECT=$1
MODULE=$2

if [ -z "$PROJECT" ]; then >&2 echo "usage : $0 <project_name> <module_name>"; exit 1; fi
if [ -z "$MODULE" ]; then >&2 echo "usage : $0 <project_name> <module_name>"; exit 1; fi


pgproject=$(pgcc_resolve_project $PROJECT)

if [ -z "$pgproject" ]; then >&2 echo "No such project : $PROJECT"; exit 2; fi

inddir=`dirname $pgproject`/ind

if [ ! -d "$inddir" ]; then >&2 echo "Project $PROJECT must be indexed first (using pgcc_indexer)"; exit 3; fi;

ind=$inddir/$MODULE.ind

if [ ! -f "$ind" ]; then  >&2 echo "No such indexed module in project $PROJECT : $MODULE"; exit 4; fi;

cat $ind
