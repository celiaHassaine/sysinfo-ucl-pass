#!/bin/bash
make clean -s 2>1&>/dev/null
make clean_csv -s 2>1&>/dev/null
make test_and_set -s 2>1&>/dev/null
make test_test_and_set -s 2>1&>/dev/null

prog_names=("test_and_set" "test_and_test_and_set")

N=4*2

for ((i=0; i<3; i++))
do
	echo "coeur,sec_1,sec_2,sec_3,sec_4,sec_5,moyenne" >> mesures_${prog_names[$i]}.csv
    for ((t=1; t<$N+1; t++))
    do
        string_fin="$t"
        moyenne=0
        if ((${prog_names[$i]} == "test_and_set"))
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                temps=$(/usr/bin/time -f "%e" ./test_and_set $t 2>&1|tail -n 1)
                moyenne=$(echo "$moyenne+$temps" | bc -l)
                string_fin="$string_fin,$temps"
            done
        elif ((${prog_names[$i]} == "test_and_test_and_set"))
        then
            for ((repeat=0; repeat<5; repeat++))
            do
                temps=$(/usr/bin/time -f "%e" ./test_test_and_set $t 2>&1|tail -n 1)
                moyenne=$(echo "$moyenne+$temps" | bc -l)
                string_fin="$string_fin,$temps"
            done
        fi
        #getting the average time out of 5
        echo ${moyenne}

        if [ "${moyenne:0:1}" -eq "${moyenne:0:1}" ] 2>/dev/null;
        then
            moyenne=$(echo "$moyenne/5" | bc -l);
            moyenne=${moyenne:0:5}
        else
            moyenne=$(echo "$moyenne/5" | bc -l);
            moyenne=","${moyenne:1}
            moyenne="0$moyenne"
            moyenne=$(printf "%.3f" $moyenne)
            moyenne=${moyenne//[,]/.}
        fi
        

        string_fin="$string_fin,$moyenne"
        echo $string_fin >> mesures_${prog_names[$i]}.csv
    done

done

