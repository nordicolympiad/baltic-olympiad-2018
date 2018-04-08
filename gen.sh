#!/bin/bash

set -e

if [ "$#" -ne 3 ]; then
		echo "Usage: ./gen CONTEST PROBLEM LANGCODE"
		exit
fi

cp templates/template-$1.tex compile.tex
sed -i "s#PROBLEM#$1/$2#" compile.tex
sed -i "s/LANG/$3/" compile.tex

pdflatex compile.tex
pdflatex compile.tex

mkdir -p compiled/$1/$2
mv compile.pdf compiled/$1/$2/$3.pdf

rm -f *.log
rm -f *.aux
rm -f *.toc
rm -f compile.tex
