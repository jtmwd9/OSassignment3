#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "license.h"

using namespace std;

struct mesg_buffer {
	long mesg_type;
	char mesg_text;
} message;

void License::getLicense() {		//gatekeeper for critical section
	key_t key;
	int msgid;
	key = ftok("JamesMessage", 69);

	msgid = msgget(key, 0666);
	
	msgrcv(msgid, &message, sizeof(message), 1, 0);
	cout << message.mesg_text << endl;
	while (message.mesg_text != '1') {	
		msgrcv(msgid, (void*)&message, sizeof(message), 1, 0);
	};
cout << "break" << endl;
	this->removeLicenses(1);
}

void License::returnLicense() {		//exiting critical section
	this->addToLicenses(1);

	key_t key;
	int msgid;
	key = ftok("JamesMessage", 69);

	msgid = msgget(key, 0666);
	message.mesg_text = '1';
	msgsnd(msgid, (void*)&message, sizeof(message), 0);
}

int License::returnLicenseCount() {
	return this->nLicense;
}

void License::initLicense(int num) {
	this->nLicense = num;
	this->licenseID[0] = -1;
}

void License::addToLicenses(int n) {
	this->nLicense += n;
}

void License::removeLicenses(int n) {
	this->nLicense -= n;
}

void License::logmsg(string msg) {
	ofstream myFile;
	myFile.open("logfile", fstream::app);
	myFile << msg << endl;
	myFile.close();
}
