#include "license.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string>

using namespace std;

int validateArguments (int count, int num) {
	if (count > 2) {
		//too many args
		return -1;
	}

	if (num < 1 || num > 20) {
		//warning use 20 as num
	}
	return num;
}

int initSharedMemory (License *l) {
	key_t key = ftok("/tmp", 'J');
	int shmid = shmget(key, sizeof(l), 0666|IPC_CREAT);
	if (shmid == -1) {
		perror("Shared memory");
		return -1;
	}
	l = (License*)shmat(shmid, NULL, 0);
	if (l == (void*) -1) {
		perror("Shared memory attach");
		return -1;
	}
	return shmid;
}

void detachSharedMemory (License *l) {
	shmdt(l);
}

void destroySharedMemory (int shmid) {
	shmctl(shmid, IPC_RMID, NULL);
}

int main (int argv, char *argc[]) {

	validateArguments (argv, atoi(argc[1]));
	License *l;
	int shmid = initSharedMemory (l);
	//do something
	detachSharedMemory(l);
	destroySharedMemory(shmid);	

	return 0;
}
