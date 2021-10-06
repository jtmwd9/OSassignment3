#include "license.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
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

int initSharedMemory (License *&l) {
	key_t key = ftok("/tmp", 'J');
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
	struct configData con;
	ifstream myFile, copyFile;
	myFile.open("testing.data");
	int size = 0;
	string line;
	if (myFile.is_open()) {

		while (getline(myFile, line)) {
			size++;
		}
		pid_t pidArray [size];
		string lineArray [3];
		int lineIndex = 0;
		int pidIndex = 0;
		string word = "";


		myFile.clear();
		myFile.seekg(0);
		while (getline (myFile, line)) {

			lineIndex = 0;
			for (int x = 0; x < line.length(); x++) {

				if (line[x] == ' ') {
            				lineArray[lineIndex] = word;
					lineIndex++;
            				word = "";
        			} else {
            				word = word + line[x];
        			}
    			}
    			lineArray[lineIndex] = word;
			word = "";

			pidArray[pidIndex++] = fork();

			if (pidArray[pidIndex -1] == -1) {
				perror("fork");
			} else if (pidArray[pidIndex - 1] > 0) {
				//parent
			} else {
				cout << getpid() << "child\n";
				cout << lineArray[0] << " " << lineArray[1] << " " << lineArray[2] << endl;
				char* args[]= {const_cast<char*>(lineArray[0].c_str()), const_cast<char*>(lineArray[1].c_str()), const_cast<char*>(lineArray[2].c_str()), NULL};
				execvp(args[0],args);
				perror("execvp");
				exit(1);
			} 
		}
		pid_t timer_pid = fork();
		if (timer_pid == 0) {
			sleep(con.timeout);
			for (int i = 0; i < size; i++) {
				kill(pidArray[i], SIGKILL);
			}
			exit(0);
		}
		pid_t w;
		while (size > 0) {
			w = wait(NULL);
			--size;
		}
	}
	myFile.close();
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
	l->initLicense(num);
	spawn(shmid);

	detachSharedMemory(l);
	destroySharedMemory(shmid);	

	return 0;
}
