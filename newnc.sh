#!/bin/bash

{
	echo PASS popopo
	echo NICK thea
	echo USER thea
	echo JOIN \#bot
} | nc -C localhost 2000
