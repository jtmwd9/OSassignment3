//James MArkus
//OS assignment 2
//license/shared memory utility
#include "license.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <errno.h>
#include <string>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>

using namespace std;

struct mesg_buffer {
	long mesg_type;
	char mesg_text;
} messageRun;

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

int initMessageQueue () {

	key_t key;
	int msgid;
	key = ftok("JamesMessage", 69);

	msgid = msgget(key, 0666 | IPC_CREAT);
	messageRun.mesg_type = 1;
	messageRun.mesg_text = '1';
	msgsnd(msgid, (void*)&messageRun, sizeof(messageRun), 0);

	return msgid;
}

void destroyMessageQueue (int msgid) {
	msgctl(msgid, IPC_RMID, NULL);
}

void detachSharedMemory (License *l) {
	shmdt(l);
}

void destroySharedMemory (int shmid) {
	shmctl(shmid, IPC_RMID, NULL);
}

void spawn (int shmid) {
	struct configData con;
	ofstream myFile, copyFile;
	myFile.open("markusTesting.data");	//copy test data to a file i create
	int size = 0;
	string line;
	if (myFile.is_open()) {

		while (getline(cin, line)) {
			myFile << line << "\n";
			size++;
		}
		myFile.close();
		ifstream copyFile;
		copyFile.open("markusTesting.data");
		pid_t pidArray [size];
		string lineArray [3];
		int lineIndex = 0;
		int pidIndex = 0;
		string word = "";

		while (getline (copyFile, line)) {

			lineIndex = 0;
			lineArray[0] = "./";
			lineArray[1] = "";
			for (int x = 0; x < line.length(); x++) {

				if (line[x] == ' ') {
            				lineArray[lineIndex] += word;
					lineIndex++;
            				word = "";
        			} else {
            				word = word + line[x];
        			}
    			}
    			lineArray[lineIndex] = word;
			word = "";

			pidArray[pidIndex++] = fork();		//child

			if (pidArray[pidIndex -1] == -1) {
				perror("fork");
			} else if (pidArray[pidIndex - 1] > 0) {
				//parent
			} else {

				cout << lineArray[0] << " " << lineArray[1] << " " << lineArray[2] << endl;
				char* args[]= {const_cast<char*>(lineArray[0].c_str()), const_cast<char*>(lineArray[1].c_str()), const_cast<char*>(lineArray[2].c_str()), NULL};
				execvp(args[0],args);		//grandchild
				perror("execvp");
				exit(1);
			} 
		}
		pid_t timer_pid = fork();		//child used as timer
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
	copyFile.close();
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
	int msgid = initMessageQueue();
	spawn(shmid);

	detachSharedMemory(l);
	destroySharedMemory(shmid);
	destroyMessageQueue(msgid);	

	return 0;
}
