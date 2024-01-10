#!/bin/bash

{
	echo PASS popopo
	sleep 1
	echo NICK h
	sleep 1
	echo USER h
	sleep 1
	echo JOIN \&lol
	echo JOIN \#lol
	echo QUIT
} | nc -C localhost 2000
