#!/bin/bash

{
	echo PASS popopo
	echo NICK thea
	echo USER thea
	echo JOIN \#lol
	echo JOIN \#pop
	echo JOIN \#kik
	echo PART \#lol,\#pop,\#kik voila
	echo QUIT
} | nc -C localhost 2000
