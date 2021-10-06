#include <string>
#include <iostream>
#include <fstream>
#include "license.h"

using namespace std;

int License::getLicense() {
	return 0;
}

int License::returnLicense() {
	return this->nLicense;
}

void License::initLicense(int num) {
	this->nLicense = num;
}

void License::addToLicenses(int n) {

}

void License::removeLicenses(int n) {

}

void License::logmsg(string msg) {
	ofstream myFile;
	myFile.open("logfile", fstream::app);
	myFile << msg << endl;
	myFile.close();
}
