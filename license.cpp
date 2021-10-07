#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <unistd.h>
#include "license.h"

using namespace std;

void License::getLicense() {		//gatekeeper for critical section
	this->choosing = true;

	int max, index, me;
	max = -1;
	index = getpid() % 10;
	if (this->licenseID[0] == -1) {
		licenseID[0] = NULL;
		me = 1;
		this->licenseID[index] = me;
	} else {
		for (int i = 0; i < (sizeof(this->licenseID) / sizeof(this->licenseID[0])); i++) {
			if (max > this->licenseID[i]) {
				max = this->licenseID[i];
			}
		}
		me = max + 1;
		this->licenseID[index] = me;
	}
	this->choosing = false;
	bool turn = false;
	while (turn == false || this->choosing == true || this->nLicense <= 0) {
		turn = true;		
		for (int i = 0; i < (sizeof(this->licenseID) / sizeof(this->licenseID[0])); i++) {
			if (me > this->licenseID[i]) {
				turn = false;
			}
		}
	};
	this->removeLicenses(1);
}

void License::returnLicense() {		//exiting critical section

	this->addToLicenses(1);
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
