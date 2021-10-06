#include "license.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int validateArguments (int num) {
	if (num == -69) {
		//no arg
		return 10;
	}	
	if (num < 1 || num > 20) {
		//warning use 20 as num
		return 10;
	}
	return num;
}

int initSharedMemory (License *l) {
	key_t key = ftok("/tmp", 'J');
		cout << "key: " << key << endl;
	int shmid = shmget(key, sizeof(l), 0666|IPC_CREAT);
	if (shmid == -1) {
		perror("Shared memory");
		return -1;
	}
	l = (License*)shmat(shmid, (void*)0, 0);
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

void spawn (int shmid) {
	pid_t c_pid = fork();
	if (c_pid == -1) {
		perror("fork");
	} else if (c_pid > 0) {
		cout << "parent" << shmid << endl;
		c_pid = wait(NULL);
	} else {
		//stringstream tmp;
		//tmp << shmid;
		//char shmidNum[6];
		//for (int i = 0; i < tmp.str().length(); i++) {
		//	shmidNum[i] = tmp.str()[i];
		//}
		cout << "child" << endl;
		License *l;
		key_t key = ftok("/tmp", 'J');
		cout << "key: " << key << endl;
		int shmid = shmget(key, sizeof(l), 0666);
		cout << shmid;
		l = (License*) shmat(shmid,0,0);
		if(l == (void*) -1) {
			perror("memory attach");
			exit(0);
		}
		int num = l->returnLicense();
		cout << num << "num\n";
		shmdt(l);
		char* args[] = {"./testChild", NULL};
		execvp(args[0],args);
		exit(0);
	}
}

int main (int argv, char *argc[]) {
	int num;
	if (argv == 2) {
		num = atoi(argc[1]);
	} else {
		num = -69;
	}
	num = validateArguments (num);
	License *l;
	int shmid = initSharedMemory (l);
	License *tmp = l;
	tmp->initLicense(num);
	spawn(shmid);
	cout << l->returnLicense() << endl;
	detachSharedMemory(l);
	destroySharedMemory(shmid);	

	return 0;
}
