#!/bin/bash

{
	echo PASS popopo
	sleep 1
	echo NICK henri
	sleep 1
	echo USER henri
	sleep 1
	echo JOIN \&lol
	sleep 1
	echo MODERATE \&lol
	sleep 1
	echo QUIT
} | nc -C localhost 2000
