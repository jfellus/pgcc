#!/bin/bash

OUTFILE=$1

rm -f $OUTFILE
cd `dirname $OUTFILE`
OUTFILE=`basename $OUTFILE`



function print_class {
	cur_class=`echo $1 | sed 's/[\t ]*class//g' | sed 's/{//g' | xargs`
	echo "class $cur_class" >> $OUTFILE
	echo -e "$2" >> $OUTFILE
}


g++ -MM ${*:2} | sed 's/.*://' | sed 's/.*\.cpp//' | sed 's/\\.*//' > .includes.txt
rm -f list2.txt
rm -f z.txt
while read file
do
	cat $file | grep -e "void[\t ]*process\|class" | grep -v -e "[^\t ]class" > .list.txt

	cat .list.txt >> z.txt

	processes=""
	cur_class=""	
	while read line
	do
		if [ -n "`echo $line | grep 'class'`" ]; then
			if [ -n "$cur_class" ] && [ -n "$processes" ]; then print_class "$cur_class" "$processes"; fi
			cur_class=$line
			processes=""
		else
			p=`echo $line | sed 's/.*process/process/' | sed 's/}.*//' |  sed 's/[{;].*//' | sed 's/[(,)]/\n/g' | sed 's/^ *//' | sed 's/ *$//'`
			processes="$processes$p\n"
		fi
	done < .list.txt 
	if  [ -n "$cur_class" ] && [ -n "$processes" ]; then print_class "$cur_class" "$processes"; fi
	
done < .includes.txt
rm -f .includes.txt .list.txt


