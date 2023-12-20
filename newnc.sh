#!/bin/bash

{
	echo PASS popopo
	echo NICK henri
	echo USER henri 0 \* :jfkdls
	echo JOIN \#lol
} | nc -C localhost 2000
