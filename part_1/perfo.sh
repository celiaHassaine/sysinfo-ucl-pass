#!/bin/bash
make clean -s 2>1&>/dev/null
make clean_csv -s 2>1&>/dev/null
make prod -s 2>1&>/dev/null
make reader -s 2>1&>/dev/null
make philo -s 2>1&>/dev/null

prog_names=("prod" "reader")

nbr_coeur=4
prod_writer=0
cons_reader=0

for ((i=0; i<2; i++))
do
	echo "thread,sec_1,sec_2,sec_3,sec_4,sec_5,moyenne" >> mesures_${prog_names[$i]}.csv
    for ((t=1; t<($nbr_coeur*2)+1; t++))
    do
        echo $t
        if (($t%2 == 0))
        then
            echo "FIRST"
            prod_writer=$(echo "$t/2" | bc)
            cons_reader=$(echo "$t/2" | bc)
        elif (($t==1))
        then
            echo "SECOND"
            prod_writer=$t
            cons_reader=$t
        else
            echo "THIRD"
            prod_writer=$(echo "($t-1)/2" | bc)
            cons_reader=$(echo "($t+1)/2" | bc)
        fi
        string_fin="$t"
        moyenne=0
        if [ "${prog_names[$i]}" = "prod" ];
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                temps=$(/usr/bin/time -f "%e" ./prod $prod_writer $cons_reader 2>&1|tail -n 1)
                moyenne=$(echo "$moyenne+$temps" | bc -l)
                string_fin="$string_fin,$temps"
            done
        elif [ "${prog_names[$i]}" = "reader" ];
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                temps=$(/usr/bin/time -f "%e" ./reader $prod_writer $cons_reader 2>&1|tail -n 1)
                moyenne=$(echo "$moyenne+$temps" | bc -l)
                string_fin="$string_fin,$temps"
            done
        elif [ "${prog_names[$i]}" = "philo" ];
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                echo $prod_writer
                temps=$(/usr/bin/time -f "%e" ./philo $t 2>&1|tail -n 1)
                echo $temps
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

