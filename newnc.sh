#!/bin/bash

{
	echo PASS popopo
	echo NICK h
	echo USER h
} | nc -C localhost 2000
