#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "shared.h"

sem_t mutexLock;

void unlinkSHM(void) {
	shm_unlink("assign1");
}

int main(int argc, char *argv[]) {
	atexit(unlinkSHM);
	int fd = shm_open("assign1", O_RDWR, 0666);

	while(fd < 0) {
		fd = shm_open("assign1", O_RDWR, 0);
	}
	ftruncate(fd, 1024);

	struct buffer *mem = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	int attemps = 7;
	int i = 1;

	while(attemps > 0) {
		sem_wait(&mem->mutex);
		printf("Consuming.\n");
		if(mem->table[i] >- 1) {
			printf("Element %d filled with value %d. Clearing.\n", i, mem->table[i]);
			mem->table[i] = -1;
			i++;
		}
		else {
			printf("Table element %d is already empty, attempting next critical section\n", i);
		}
		if(i > 1) {
			i = 0;
		}
		attemps--;
		sem_post(&mem->mutex);
		sleep(1);
	}
}