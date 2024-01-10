#!/bin/bash

{
	echo PASS popopo
	echo NICK h
	echo USER h
	echo JOIN \#lol
	while true
		do echo PRIVMSG jean coucou
	done
} | nc -C localhost 2000
