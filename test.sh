#!/bin/bash

for f in `find . -name \*.rt -print`
do
	echo ./miniRT $f
	./miniRT -r 400 300 -m 3 -a 2 -f $f
	echo
done
