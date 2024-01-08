#!/bin/bash

{
	echo PASS popopo
	echo NICK h
	echo USER h
	echo JOIN \#lol
	echo PRIVMSG \#lol coucou!
	echo PRIVMSG \#lol salaud!
	echo QUIT
} | nc -C localhost 2000
