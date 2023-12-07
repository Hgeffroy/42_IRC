#!/bin/bash

{
    echo "PASS a"
    echo "NICK user$1"
    echo "USER user$1 0 * :realname"
    echo "JOIN #test"
	while read line; do
        echo "$line"
    done
} | nc -C localhost 6667
