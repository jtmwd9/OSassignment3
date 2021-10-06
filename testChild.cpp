#include <sys/shm.h>
#include <sys/ipc.h>
#include <iostream>
#include <cstdlib>
#include <errno.h>
#include <cstdio>
#include "license.h"

using namespace std;

int main (int argv, char* argc[]) {
	cout << "Grandchild\n";
	License *l;
	key_t key = ftok("/tmp", 'J');
	int shmid = shmget(key, sizeof(l), 0666|IPC_CREAT);
	cout << shmid;
	l = (License*) shmat(shmid,NULL,0);
	if(l == (void*) -1) {
		perror("memory attach");
		return -1;
	}
	int num = l->returnLicense();
	cout << num << "num\n";
	shmdt(l);

	return 0;
}
