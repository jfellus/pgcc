#!/bin/bash

ME=`readlink -f $0`
DIR=`dirname $ME`/..
cd $DIR
./pgcc $*
