#!/bin/bash

{
	echo PASS popopo
	echo NICK thea
	echo USER thea 0 \* :jfkdls
	echo JOIN \#lol
} | nc -C localhost 2000
