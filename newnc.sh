#!/bin/bash

{
	echo PASS popopo
	echo NICK h
	echo USER h
	echo JOIN \#lol
	echo JOIN \#kik
	echo MODERATE \#lol,\#kik
	echo QUIT
} | nc -C localhost 2000
