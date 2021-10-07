#ifndef LICENSE_H
#define LICENSE_H

#include <string>
#include <queue>

class License {
	private:
		int nLicense;
		int licenseID [20];
		bool choosing;
	public:
		void getLicense();
		void returnLicense();
		int returnLicenseCount();
		void initLicense(int);
		void addToLicenses(int);
		void removeLicenses(int);
		void logmsg(std::string);
};

#endif
