#ifndef LICENSE_H
#define LICENSE_H

#include <string>
#include <vector>

class License {
	private:
		int nLicense;
		vector<int> licenseID;
	public:
		int getLicense();
		int returnLicense();
		void initLicense(int);
		void addToLicenses(int);
		void removeLicenses(int);
		void logmsg(std::string);
};

#endif
