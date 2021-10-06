#include <iostream>
#include <unistd.h>
#include <ctime>
#include <string>
#include <sstream>
#include <cstdlib>
#include "license.h"

using namespace std;

int main (int argc, char *argv[]) {

	int repeatFactor = atoi(argv[2]);
	int sleepyTime = atoi(argv[1]);
	License l;
	time_t now;
	struct tm *local;
	stringstream msg;

	for (int i = 0; i < repeatFactor; i++) {
		sleep(sleepyTime);
		now = time(0);
		local = localtime(&now);
		msg << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec;
		msg << " " << getpid() << " Iteration #" << i + 1 << " of " << repeatFactor << "\n";
		l.logmsg(msg.str());
		msg.str(string());
	}

	return 0;
}
