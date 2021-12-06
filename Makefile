all: perfo_normal
	./part_1/perfo.sh
	python3 graphs/graph.py

perfo_normal:
	./part_1/perfo.sh

#perfo_spinlock:
#	./bash_scripts/perfo_spinlock.sh

perfo_tas:
	./part_2/perfo_TAS.sh

philo: 
	gcc -pthread part_1/philosophes.c -lpthread -o philo

reader:
	gcc -pthread part_1/writers_readers.c -lpthread -o reader

prod:
	gcc -pthread part_1/producteurs_consommateurs.c -lpthread -o prod


philo_ttas:
	gcc -pthread part_2/philosophe_ttas.c part_2/semaphore.c -lpthread -lm -o philo_ttas

reader_ttas:
	gcc -pthread part_2/writers_readers_ttas.c part_2/semaphore.c -lm -lpthread -o reader_ttas

prod_ttas:
	gcc -pthread part_2/prod_cons_ttas.c part_2/semaphore.c -lm -lpthread -o prod_ttas


test_and_set:
	gcc -pthread part_2/test-and-set.c -lpthread -o test_and_set

test_test_and_set:
	gcc -pthread part_2/test-test-and-set.c -lpthread -o test_test_and_set

sema:
	gcc -pthread part_2/semaphore.c -lpthread -o sema

#backoff_test_test_and_set:
#	gcc -pthread spinlocks/backoff-test-test-and-set.c -lpthread -lm -o backoff_test_test_and_set


#graphs: graph_normal
#	python3 python_scripts/plots_spinlock.py

#graph_spinlock:
#	python3 python_scripts/plots_spinlock.py

#graph_both:
#	python3 python_scripts/plots_both.py

graph_normal: 
	python3 graphs/graph.py

graph_tas:
	python3 graphs/graph_test_and_set.py
  



clean_csv:
	-rm *.csv

clean:
	-@rm prod
	-@rm sema
	-@rm reader
	-@rm philo
	-@rm philo_spinlock
	-@rm reader_spinlock
	-@rm prod_spinlock
	-@rm test_and_set
	-@rm test_test_and_set
	-@rm semaphore
	-@rm backoff_test_test_and_set
	-rm graphs/*.png
