#include "logging.h"

FILE* fo;
FILE* fe;

void formatString(std::string *strtext) {

	time_t current_time = time(NULL);
	std::string timestring = ctime(&current_time);

	std::string insertstring(timestring.substr(0, timestring.length() - 1));

	strtext->insert(0, insertstring + ": ");

	if (strtext->length() > 0) {
		if (strtext->at((strtext->length() - 1)) != '\n') {
			strtext->resize(strtext->length() + 1, '\n');
		}
	}

}

void LOG_ERROR(std::string strtext) {

	LOGW(strtext.c_str());
	formatString(&strtext);

	fputs(strtext.c_str(), stderr);

}

void LOG_INFO(std::string strtext) {

	LOGI(strtext.c_str());
	formatString(&strtext);

	fputs(strtext.c_str(),stdout);

}

void START_LOGGING(const char* root) {

	std::string logfile = std::string(root) + "/cthlog.txt";
	std::string errlogfile = std::string(root) + "/ctherrlog.txt";
	// Redirect stdout and stderr
	fo = freopen(logfile.c_str(), "a", stdout);
	fe = freopen(errlogfile.c_str(), "a", stderr);

	// Disable buffering so messages are written instantly
	setvbuf(fo, 0, _IONBF, 0);
	setvbuf(fe, 0, _IONBF, 0);

	LOG_INFO(root);

}

