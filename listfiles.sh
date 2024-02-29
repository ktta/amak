#!/bin/sh

grep '#include' inc.c | sed -e 's/#include//g' -e 's/"//g' -e 's/ //g'
