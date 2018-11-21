#!/bin/bash
file=$1
tr ' ' '\n' < $file | sort | uniq -c | wc -l

