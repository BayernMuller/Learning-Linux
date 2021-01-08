#!/bin/sh
myfunc()
{
	str=""
	while [ "$1" != "" ]; do
		str="$str $1"
		shift
	done
	echo $str
}
myfunc AAA BBB CCC DDD EEE FFF  GGG HHH III JJJ KKK
exit 0
