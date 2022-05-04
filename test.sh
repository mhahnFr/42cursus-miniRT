#!/bin/bash

for f in `find . -name \*.rt -print`
do
	echo ./miniRT $f
	./miniRT $f
	echo
done
