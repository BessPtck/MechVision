#include "ParseTxt.h"

ParseTxt::ParseTxt() { ; }
ParseTxt::~ParseTxt() { ; }

unsigned char ParseTxt::init(const string& inFile, const string& outFile) {
	m_inFile = inFile;
	m_outFile = outFile;
	return ECODE_OK;
}

unsigned char ParseTxt::readCSV(s_datLine dat[], int maxSize) {
	ifstream ffile(m_inFile);
	string line;
	int lcnt = 0;
	int arlen = 0;
	while (getline(ffile,line) && lcnt<maxSize && arlen>0) {
		arlen = readFloatLine(line, dat[lcnt].v);
		dat[lcnt].n = arlen;
	};
	ffile.close();
	return ECODE_OK;
}
unsigned char ParseTxt::writeCSV(const s_datLine dat[], int dat_size) {
	ofstream ffile;
	ffile.open(m_outFile);
	for (int i = 0; i < dat_size; i++) {
		string line = dumpFloatLine(dat[i].v, dat[i].n);
		ffile << line << '\n';
	}
	ffile.close();
	return ECODE_OK;
}

string ParseTxt::dumpFloatLine(const float* ar, int len) {
	string outStr = "";
	for (int i = 0; i < len; i++) {
		string el(to_string(ar[i]));
		outStr += el;
		outStr += ",";
	}
	return outStr;
}

int ParseTxt::readFloatLine(const string& str, float* ar) {
	string commaStr = ",";
	size_t foundLoc = str.find(commaStr);
	size_t startLoc = 0;
	int arLen = 0;
	while (foundLoc != string::npos) {
		size_t len = foundLoc - startLoc;
		if (len <= 0)
			break;
		startLoc = foundLoc + 1;
		string numStr = str.substr(startLoc, len);
		ar[arLen] = stof(numStr);
		arLen++;
		if (arLen >= PARSETXT_MAXAR)
			return -1;
		foundLoc = str.find(commaStr, startLoc);
	};
	return arLen;
}