#!/bin/bash

make clean -s 2>1&>/dev/null
make clean_csv -s 2>1&>/dev/null
make prod

nbr_coeur=4

val= 1

for ((i=0; i<3; i++))
do
	echo "coeur, secondes écoulées 1, secondes écoulées 2, secondes écoulées 3, secondes écoulées 4, secondes écoulées 5" >> mesures_${programs[$i]}.csv
    for ((i=0; i<$nbr_coeur*2; i++))
    do
        #on incrémente val que si t est impair
            if (($t % 2 != 0 ))
            then
                val=$((val + 1 ))
            fi
            temps=$(/usr/bin/time -f "%e" ./prod $val $val 2>&1)
            echo  $t,$temps >> mesures_${programs[$i]}.csv
    fi

fi

