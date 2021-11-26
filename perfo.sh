#!/bin/bash

make clean -s 2>1&>/dev/null
make clean_csv -s 2>1&>/dev/null
make prod -s 2>1&>/dev/null

prog_names=("prod" "philo" "reader")

nbr_coeur=4

moyenne_philo=0
moyenne_reader=0

val=1

#for ((i=0; i<3; i++))
#do
	echo "coeur, secondes écoulées 1, secondes écoulées 2, secondes écoulées 3, secondes écoulées 4, secondes écoulées 5, moyenne" >> mesures_${prog_names[$i]}.csv
    for ((t=0; t<$nbr_coeur*2; t++))
    do
        #if (${prog_names[$i]} == "prod") 
        #then
            string_fin="$t"
            moyenne_prod=0
            for ((repeat=0; repeat<5; repeat++))
            do
                temps=$(/usr/bin/time -f "%e" ./prod $val $val 2>&1|tail -n 1)
                moyenne_prod=$(echo "$moyenne_prod+$temps" | bc -l)
                string_fin="$string_fin,$temps"
            done

            #getting the average time out of 5
            moyenne_prod=$(echo "$moyenne_prod/5" | bc -l);
            moyenne_prod=","${moyenne_prod:1}
            moyenne_prod="0$moyenne_prod"
            moyenne_prod=$(printf "%.3f" $moyenne_prod)


            string_fin="$string_fin,$moyenne_prod"
            echo $string_fin >> mesures_${prog_names[$i]}.csv
        #fi
        #if (${prog_names[$i]} == "prod") 
        #then
        #    temps=$(/usr/bin/time -f "%e" ./prod $val $val 2>&1)
        #fi
        #if (${prog_names[$i]} == "prod") 
        #then
        #    temps=$(/usr/bin/time -f "%e" ./prod $val $val 2>&1)
        #fi
    done

#done

