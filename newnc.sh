#!/bin/bash

{
    echo PASS lalala
    echo NICK ceci
    echo USER jfkls 0 \* :jfkdls
    echo JOIN \#la
        while true
        do
            echo  PRIVMSG \#la :jfkdlsjfkdlsjfkdlsjfkdlsfjldsjfkdls
        done
} | nc -C localhost 1234
