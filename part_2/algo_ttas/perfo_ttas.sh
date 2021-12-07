#!/bin/bash
make clean -s 2>1&>/dev/null
#make clean_csv -s 2>1&>/dev/null
make prod_ttas -s 2>1&>/dev/null
make reader_ttas -s 2>1&>/dev/null
make philo_ttas -s 2>1&>/dev/null

prog_names=("prod_ttas" "philo_ttas" "reader_ttas")

nbr_coeur=4
prod_writer=0
cons_reader=0

for ((i=0; i<3; i++))
do
	echo "thread,sec_1,sec_2,sec_3,sec_4,sec_5,moyenne" > mesures_${prog_names[$i]}.csv
    for ((t=1; t<($nbr_coeur*2)+1; t++))
    do
        if (($t%2 == 0))
        then
            prod_writer=$(echo "$t/2" | bc)
            cons_reader=$(echo "$t/2" | bc)
        elif (($t==1))
        then
            prod_writer=$t
            cons_reader=$t
        else
            prod_writer=$(echo "($t-1)/2" | bc)
            cons_reader=$(echo "($t+1)/2" | bc)
        fi
        string_fin="$t"
        moyenne=0
        if [ "${prog_names[$i]}" = "prod_ttas" ]
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                echo "beginning"
                #./prod_ttas $prod_writer $cons_reader
                temps=$(/usr/bin/time -f "%e" ./prod_ttas $prod_writer $cons_reader 2>&1|tail -n 1)
                moyenne=$(echo "$moyenne+$temps" | bc -l)
                echo $temps
                string_fin="$string_fin,$temps"
            done
        elif [ "${prog_names[$i]}" = "reader_ttas" ]
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                temps=$(/usr/bin/time -f "%e" ./reader_ttas $prod_writer $cons_reader 2>&1|tail -n 1)
                moyenne=$(echo "$moyenne+$temps" | bc -l)
                string_fin="$string_fin,$temps"
            done
        elif [ "${prog_names[$i]}" = "philo_ttas" ]
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                temps=$(/usr/bin/time -f "%e" ./philo_ttas $t 2>&1|tail -n 1)
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

