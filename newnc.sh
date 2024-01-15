#!/bin/bash

{
	echo PASS popopo \r\n
	sleep 1
	echo NICK henri \r\n
	sleep 1
	echo USER henri j j j \r\n
	sleep 1
	echo JOIN \#lol \r\n
  while true
    do echo PRIVMSG \#lol Yo \r\n
  done
} | nc -C localhost 2000
