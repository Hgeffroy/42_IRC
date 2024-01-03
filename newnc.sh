#!/bin/bash

{
	echo PASS popopo
	echo NICK thea
	echo USER thea
	echo JOIN \#lol a
} | nc -C localhost 2000
