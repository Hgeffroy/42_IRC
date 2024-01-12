#!/bin/bash

{
	echo PASS popopo
	sleep 1
	echo NICK henri
	sleep 1
	echo USER henri
	sleep 1
	echo JOIN \#lol
} | nc -C localhost 2000
