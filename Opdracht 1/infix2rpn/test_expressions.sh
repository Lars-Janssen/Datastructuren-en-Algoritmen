#! /usr/bin/env bash


## The tests suggested in the assignment text are reproduced below

# Conversion output
./infix2rpn "~(3+2-32/349*20)^2904"
echo ''

# Exit status in case of success
./infix2rpn "(3+2)/3"; echo $?
echo ''

# Output goes to stdout
./infix2rpn "(3+2)/3" 2> /dev/null
echo ''

# Stats go to stderr
./infix2rpn "(3+2)/3" > /dev/null
echo ''

# Exit status in case of error
./infix2rpn "blablabla" > /dev/null 2>&1; echo $?
echo ''


## Add your own tests here

# TODO
