This is Jonathan Zajac's submission for Kent State's Operating Systems coding assignment 1. This is a possible solution to the producer-consumer problem.
It is ran by doing: 
$ gcc producer.c -pthread -lrt -o producer
$ gcc consumer.c -pthread -lrt -o consumer
$ ./producer & ./consumer &
