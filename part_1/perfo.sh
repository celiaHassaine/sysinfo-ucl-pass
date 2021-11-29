#!/bin/bash
make clean -s 2>1&>/dev/null
make clean_csv -s 2>1&>/dev/null
make prod -s 2>1&>/dev/null
make reader -s 2>1&>/dev/null
make philo -s 2>1&>/dev/null

prog_names=("prod" "philo" "reader")

nbr_coeur=4

val=1

for ((i=0; i<3; i++))
do
	echo "coeur,sec_1,sec_2,sec_3,sec_4,sec_5,moyenne" >> mesures_${prog_names[$i]}.csv
    for ((t=0; t<$nbr_coeur*2; t++))
    do
        string_fin="$t"
        moyenne=0
        if ((${prog_names[$i]} == "prod"))
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                temps=$(/usr/bin/time -f "%e" ./prod $val $val 2>&1|tail -n 1)
                moyenne=$(echo "$moyenne+$temps" | bc -l)
                string_fin="$string_fin,$temps"
            done
        elif ((${prog_names[$i]} == "reader"))
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                temps=$(/usr/bin/time -f "%e" ./reader $val $val 2>&1|tail -n 1)
                moyenne=$(echo "$moyenne+$temps" | bc -l)
                string_fin="$string_fin,$temps"
            done
        elif ((${prog_names[$i]} == "philo"))
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                temps=$(/usr/bin/time -f "%e" ./philo $val $val 2>&1|tail -n 1)
                moyenne=$(echo "$moyenne+$temps" | bc -l)
                string_fin="$string_fin,$temps"
            done
        fi
        #getting the average time out of 5
        moyenne=$(echo "$moyenne/5" | bc -l);
        moyenne=","${moyenne:1}
        moyenne="0$moyenne"
        moyenne=$(printf "%.3f" $moyenne)
        moyenne=${moyenne//[,]/.}

        string_fin="$string_fin,$moyenne"
        echo $string_fin >> mesures_${prog_names[$i]}.csv
    done

done

