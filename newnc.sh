#!/bin/bash

{
	echo PASS lalala
	echo NICK ceci
	echo USER jfkls 0 \* :jfkdls
} | nc -C localhost 1234
