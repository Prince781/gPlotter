#!/bin/sh

astyle --style=linux -s4 --indent-cases --convert-tabs -xC80 --pad-first-paren-out *.[ch] core/*.[ch]
