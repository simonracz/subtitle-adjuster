//
//  Created by Simon Racz
//

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include "SubRip.h"
#include "Adjuster.h"

using namespace std;

ifstream processArgs(int argc, const char* argv[]);
bool getAnchorPointFromUser(int* index, int* time, const Subtitles::SubRip& sr);

//uncomment to test
//#define TESTSRT

#ifdef TESTSRT
bool testCIO();
bool testCPPIO();
bool test();
#endif

int main(int argc, const char * argv[])
{
#ifdef TESTSRT
	std::ios_base::sync_with_stdio(false);
	return !test();
#endif
	
	ifstream ifs = processArgs(argc,argv);

//	FILE* fin = fopen(argv[1], "r");
//	Subtitles::SubRip sr{fin};
	Subtitles::SubRip sr{ifs};
	
	if(!sr) {
		cout << "Couldn't read the subtitle file : " << argv[1] << '\n';
		return 1;
	}
	
	cout << sr.printNode(1);
	cout << sr.printNode(2);
	cout << sr.printNode(3);
	
	Subtitles::Adjuster adj{sr};
	
	int index, time;
	if(!getAnchorPointFromUser(&index, &time, sr)) { return 1; }
	adj.setAnchor(index, time);
	if(!getAnchorPointFromUser(&index, &time, sr)) { return 1; }
	adj.setAnchor(index, time);
	
	ofstream fout{"./modified.srt", ofstream::trunc | ofstream::out};
	adj.printToStream(fout);
	
	cout << "\nDone. New file \"modified.srt\" has been created.\n";
	
    return 0;
}

ifstream processArgs(int argc, const char* argv[])
{
	if (argc < 2) {
		cout << "Add the srt file name after the program name, e.g.\n"
		<< argv[0] << " ./PulpFiction.srt\n";
		exit(1);
	}
	
	ifstream ifs{argv[1]};
	if(ifs.fail()) {
		cout << "Can't open the subtitle file : " << argv[1] << endl;
		exit(1);
	}
	
	return ifs;
}

bool getTimeDataFromUser(const int index, const Subtitles::SubRip& sr, unsigned int*, unsigned int*, unsigned int*, unsigned int*);

bool getAnchorPointFromUser(int* index, int* time, const Subtitles::SubRip& sr)
{
	static int questionNum = 0;
	++questionNum;
	
	cout << "Please enter ANCHOR"<< questionNum << "'s index. (Indexing starts from 1. 0 means \"last index\".)\n";
	
	int in;
	if (!(cin >> in)) {
		//repeat
		cout << "Wrong input\nPlease enter ANCHOR"<< questionNum << ".'s index. (Indexing starts from 1. 0 means \"last index\".)\n";
		cin.clear();
		if (!(cin >> in)) { return false; }
	}
	
	if (in < 0) { return false; }
	if (in == 0) { in = static_cast<int>(sr.size()); }
	
	if (in > sr.size()) { cout << "Wrong index.\n"; return false; }

	unsigned int a, b, c, d;
	if (!getTimeDataFromUser(in, sr, &a, &b, &c, &d)) {
		//repeat
		cout << "Wrong time data. Please enter again.\n";
		cin.clear();
		if (!getTimeDataFromUser(in, sr, &a, &b, &c, &d)) { cout << "Wrong time data.\n"; return false; }
	}
	
	*index = in;
	*time = Subtitles::SubRip::aggregateTimes(a, b, c, d);
	return true;
}

bool getTimeDataFromUser(const int index, const Subtitles::SubRip& sr, unsigned int* a, unsigned int* b, unsigned int* c, unsigned int* d)
{
	cout << "Old time : \n";
	sr.printNodeToStream(index, cout);
	cout << "Enter the new time. (Only the start time, e.g. 00:00:12,345)\n";
	if (scanf("%u:%u:%u,%u", a, b, c, d) < 4)	{
		return false;
	}
	return true;
}

#ifdef TESTSRT
#include "TestStrings.h"

bool test()
{
	//timer
	clog << "cstdio test result : " << endl;
	bool s1 = testCIO();
	clog << s1 << endl;
	
	clog << "iostream test result : " << endl;
	bool s2 = testCPPIO();
	clog << s2 << endl;
	
	return s1 && s2;
}

bool testCIO()
{
	bool success = true;
	
	return success;
}

bool testCPPIO()
{
	bool success = true;

	return success;
}

#endif //TESTSRT
