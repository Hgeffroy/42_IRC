#!/bin/bash

{
	echo PASS popopo
	echo NICK bot
	echo USER bot
	echo JOIN \#lol
	echo INVITE arthur \#lol
} | nc -C localhost 2000
