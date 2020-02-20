#!/bin/sh

if [ $# -lt 1 ]; then
	echo "usage: ./memoryx.sh (0-100)"
	exit 1
fi

i=1

if [ $1 -eq 0 ]; then
	while [ $i -le 100 ]; do
		./memoryx.sh $i
		i=`echo $i | awk '{ printf "%03d\n", $1 + 1 }'`
	done

	exit 0
fi

if [ $1 -gt 0 -a $1 -le 100 ]; then
	enum=`echo $1 | awk '{ printf "%03d\n", $1 }'`
	valgrind --leak-check=full ./tarx < /home/jplank/cs360/labs/lab5/Gradescript-Examples/$enum.tarc > /dev/null

	chmod -R 0700 $enum
	rm -rf $enum
else
	echo "usage: ./memoryx.sh (1-100)"
fi
