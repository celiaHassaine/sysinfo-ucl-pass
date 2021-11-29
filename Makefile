all: benchmarks_normal 
	./bash_scripts/benchmarks_spinlock.sh

benchmarks_normal:
	./bash_scripts/benchmarks.sh 

benchmarks_spinlock:
	./bash_scripts/benchmarks_spinlock.sh

benchmarks_tas:
	./bash_scripts/benchmarks_tas.sh

philo: 
	gcc -pthread part_1/philosophes.c -lpthread -o philo

reader:
	gcc -pthread part_1/writers_readers.c -lpthread -o reader

prod:
	gcc -pthread part_1/producteurs_consommateurs.c -lpthread -o prod


philo_spinlock:
	gcc -pthread philos/philo_with_spinlocks.c spinlocks.c -lpthread -lm -o philo_spinlock

reader_spinlock:
	gcc -pthread read-writers/readers-writers_with_spinlock.c spinlocks.c -lm -lpthread -o reader_spinlock

prod_spinlock:
	gcc -pthread prod-consom/prod-consom_with_spinlocks.c spinlocks.c -lm -lpthread -o prod_spinlock 


test_and_set:
	gcc -pthread spinlocks/test-and-set.c -lpthread -o test_and_set

test_test_and_set:
	gcc -pthread spinlocks/test-test-and-set.c -lpthread -o test_test_and_set

backoff_test_test_and_set:
	gcc -pthread spinlocks/backoff-test-test-and-set.c -lpthread -lm -o backoff_test_test_and_set


graphs: graph_normal
	python3 python_scripts/plots_spinlock.py

graph_spinlock:
	python3 python_scripts/plots_spinlock.py

graph_both:
	python3 python_scripts/plots_both.py

graph_normal: 
	python3 python_scripts/plots.py

graph_tas:
	python3 python_scripts/plots_tas.py
  



clean_csv:
	-rm *.csv

clean:
	-@rm prod
	-@rm reader
	-@rm philo
	-@rm philo_spinlock
	-@rm reader_spinlock
	-@rm prod_spinlock
	-@rm test_and_set
	-@rm test_test_and_set
	-@rm semaphore
	-@rm backoff_test_test_and_set