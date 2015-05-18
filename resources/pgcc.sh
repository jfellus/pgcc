#!/bin/bash

ME=`readlink -f $0`
DIR=`dirname $ME`/..

INPUT=`readlink -f $1`
OUTPUT=`readlink -f $2`

cd $DIR
./pgcc "$INPUT" "$OUTPUT"
