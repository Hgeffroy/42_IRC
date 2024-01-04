#!/bin/bash

{
	echo PASS popopo
	echo NICK thea
	echo USER thea
	echo JOIN \#lol
	echo INVITE arthur \#lol
} | nc -C localhost 2000
