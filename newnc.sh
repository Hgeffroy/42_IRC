#!/bin/bash

{
	echo PASS popopo
	echo NICK bot
	echo USER bot
	echo JOIN \#lol
	echo PART \#lol
	echo QUIT
} | nc -C localhost 2000
