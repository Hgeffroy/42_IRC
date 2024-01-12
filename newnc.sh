#!/bin/bash

{
	echo PASS popopo
	sleep 1
	echo NICK henri
	sleep 1
	echo USER henri
	sleep 1
	echo JOIN \#lol
	echo MODE \#lol +k po
	echo MODE \#lol +k password
	echo QUIT
} | nc -C localhost 2000
