#!/bin/bash

rm resources.rqc &> /dev/null

echo "<!DOCTYPE RCC><RCC version=\"1.0\">" > resources.qrc
echo "<qresource>" >> resources.qrc

## pack icons
for i in $(find . -maxdepth 1 -type f -name \*.png); do
	echo "  <file>$i</file>" >> resources.qrc
done

## pack html stuff
for i in $(find html/ -type f ); do 
	echo "  <file>$i</file>" >> resources.qrc
done

echo "</qresource>" >> resources.qrc
echo "</RCC>" >> resources.qrc

rcc -binary resources.qrc -o resources.rcc
