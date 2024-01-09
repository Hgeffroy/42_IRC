#!/bin/bash

{
	echo PASS popopo
	echo NICK h
	echo USER h
	echo JOIN \#lol
} | nc -C localhost 2000
