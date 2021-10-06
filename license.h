#ifndef LICENSE_H
#define LICENSE_H

#include <string>

class License {
	private:
		int nLicense;
	public:
		int getLicense();
		int returnLicense();
		void initLicense(int);
		void addToLicenses(int);
		void removeLicenses(int);
		void logmsg(std::string);
};

#endif
