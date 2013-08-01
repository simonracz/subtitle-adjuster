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
#include <chrono>
#include <sstream>
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

	Subtitles::SubRip sr{ifs};
	
	if(!sr) {
		cout << "Couldn't read the subtitle file : " << argv[1] << '\n';
		return 1;
	}
	
	if (sr.size() >= 3) {
		cout << sr.printNode(1);
		cout << sr.printNode(2);
		cout << sr.printNode(3);
	}
	
	Subtitles::Adjuster adj{sr};
	
	int index, time;
	if(!getAnchorPointFromUser(&index, &time, sr)) { return 1; }
	adj.setAnchor(index, time);
	if(!getAnchorPointFromUser(&index, &time, sr)) { return 1; }
	adj.setAnchor(index, time);
	
	string outFile{argv[1]};
	auto pos = outFile.find(".srt");
	outFile.erase(pos, 4);
	outFile+="-mod.srt";
	ofstream fout{outFile, ofstream::trunc | ofstream::out};
	adj.printToStream(fout);
	
	cout << "\nDone. New file \"" << outFile << "\" has been created.\n";
	
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
	clog << "C cstdio test result : " << boolalpha << endl;
	auto time = chrono::high_resolution_clock::now();
	bool resC = testCIO();
	clog << resC << "\nTest took : ";
	auto duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - time).count();
	clog << duration << " nanoseconds\n" << endl;
	
	clog << "C++ iostream test result : " << endl;
	time = chrono::high_resolution_clock::now();
	bool resCPP = testCPPIO();
	clog << resCPP << "\nTest took : ";
	duration = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - time).count();
	clog << duration << " nanoseconds\n" << endl;
	
	return resC && resCPP;
}

bool testCIO()
{
	return true;
}

bool testCPPIO()
{
	istringstream iss{engIn};
	Subtitles::SubRip sr{iss};
	if (!sr) { return false; }
	Subtitles::Adjuster adj{sr};
	adj.setAnchor(1, Subtitles::SubRip::aggregateTimes(0, 0, 13, 440));
	adj.setAnchor(4, Subtitles::SubRip::aggregateTimes(0, 0, 23, 340));
	ostringstream oss{};
	adj.printToStream(oss);
	bool testRes1 = (oss.str().compare(engRes) == 0);
	
	iss.str(hunIn);
	sr.clear();
	sr.readFromStream(iss);
	if (!sr) { return false; }
	Subtitles::Adjuster adjHun{sr};
	adjHun.setAnchor(2, Subtitles::SubRip::aggregateTimes(0, 0, 21, 80));
	adjHun.setAnchor(static_cast<int>(sr.size()), Subtitles::SubRip::aggregateTimes(0, 0, 40, 920));
	oss.clear();
	adjHun.printToStream(oss);
	bool testRes2 = (oss.str().compare(hunRes) == 0);
	return (testRes1 && testRes2);
}

#endif //TESTSRT
