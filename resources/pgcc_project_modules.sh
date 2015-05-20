#!/bin/bash

PROJECT=$1

if [ -z "$PROJECT" ]; then >&2 echo "usage : $0 <project_name>"; exit 1; fi

pgproject=$(pgcc_resolve_project $PROJECT)

if [ -z "$pgproject" ]; then >&2 echo "No such project : $PROJECT"; exit 2; fi

inddir=`dirname $pgproject`/ind

if [ ! -d "$inddir" ]; then >&2 echo "Project $PROJECT must be indexed first (using pgcc_indexer)"; exit 3; fi;


ls -1 $inddir | cut -d"." -f1
