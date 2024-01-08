#!/bin/bash

{
	echo PASS popopo
	echo NICK h
	echo USER h
	echo JOIN \#lol
	echo PRIVMSG bot voila voila
	echo QUIT
} | nc -C localhost 2000
