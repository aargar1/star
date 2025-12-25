#!/bin/bash

for file in *.c; do
    ./../star "$file" "$file.ll"
    lli "$file.ll"
    echo "$? $file"
done