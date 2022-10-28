#pragma once
#ifndef SHARED_H
#define SHARED_H
#include <semaphore.h>
#include <stdlib.h>
struct buffer {
	sem_t mutex;
	int table[2];
};

#endif