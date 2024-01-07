#!/bin/bash

{
	echo PASS popopo
	echo NICK h
	echo USER h
	echo PRIVMSG bot yo comment ca va ?
	echo QUIT
} | nc -C localhost 2000
