#!/bin/bash
for day in practice day1 day2
do
    for problem in $(ls $day)
    do
        for lang in de da et fi is lt lv no pl sv en ru
        do
            if [[ -f $day/$problem/problem_statement/problem.$lang.tex ]]
            then
                ./gen.sh $day $problem $lang
            fi
        done
    done
done
