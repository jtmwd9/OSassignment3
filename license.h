#ifndef LICENSE_H
#define LICENSE_H

#include <string>

class License {
	public:
		int nLicenses;
		int getLicense();
		int returnLicense();
		int initLicense();
		void addToLicenses(int);
		void removeLicenses(int);
		void logmsg(std::string);
};

#endif
