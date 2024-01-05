#!/bin/bash

{
	echo PASS popopo
	echo NICK thea
	echo USER thea
	echo JOIN \#lol
	echo PART \#lol
	echo QUIT
} | nc -C localhost 2000
