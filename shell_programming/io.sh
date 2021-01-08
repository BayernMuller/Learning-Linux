#!/bin/sh

myvar="Hi Woo"
echo $myvar
echo "$myvar"
echo '$myvar'
echo \$myvar
echo input:
read myvar
echo '$myvar'=$myvar
exit 0
