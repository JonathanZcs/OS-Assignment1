#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>


#include "shared.h"

void unlinkSHM(void) {
	shm_unlink("assign1");
}

int main() {
	atexit(unlinkSHM);
	
	int fd = shm_open("assign1", O_CREAT | O_RDWR, 0666);

	ftruncate(fd, 1024);

	struct buffer *mem = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	mem->table[0] = -1;
    mem->table[1] = -1;
	sem_init(&mem->mutex, 1, 1);
	srand(time(NULL));

	int i = 0;
	int attemps = 7;

	while(attemps > 0) {
		sem_wait(&mem->mutex);
		printf("Producing.\n");

		if(mem->table[i] == -1) {
			int temp = rand();
			mem->table[i] = temp;
			printf("Filling element %d with value %d \n", i, temp);
			i++;
		}
		else {
			printf("Element %d is filled, checking next critical section\n", i);
		}
		if(i > 1) {
			i = 0;
		}
		attemps--;
		sem_post(&mem->mutex);
		sleep(1);
	}
}