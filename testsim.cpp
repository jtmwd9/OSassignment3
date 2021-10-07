#include <iostream>
#include <unistd.h>
#include <ctime>
#include <string>
#include <sstream>
#include <cstdlib>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdio.h>
#include "license.h"

using namespace std;

void getShm (License *&l) {
	key_t key = ftok("/tmp", 'J');
	int shmid = shmget(key, sizeof(l), 0666);
	l = (License*) shmat(shmid,0,0);
	if(l == (void*) -1) {
		perror("memory attach");
	}
}

int main (int argc, char *argv[]) {

	int repeatFactor = atoi(argv[2]);
	int sleepyTime = atoi(argv[1]);
	License *l;
	getShm(l);
	time_t now;
	struct tm *local;
	stringstream msg;

	l->getLicense();
	for (int i = 0; i < repeatFactor; i++) {
		sleep(sleepyTime);
		now = time(0);
		local = localtime(&now);
		msg << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec;
		msg << " " << getpid() << " Iteration #" << i + 1 << " of " << repeatFactor << "\n";
		l->logmsg(msg.str());
		msg.str(string());
	}
	l->returnLicense();
	
	shmdt(l);

	return 0;
}
