//============================================================================
// Name        : SubtitleShifter.cpp
// Author      : Simon Rácz
// Version     :
// Copyright   : 
//============================================================================

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <vector>
#include <string>

using namespace std;


//1
//00:00:00,000 --> 00:00:01,800
//<i>A Charmed elızı Èvad·ban tˆrtÈnt:</i>
//
//hours:minutes:seconds,milliseconds

long timesToLong(int a, int b, int c, int d) {
	long lTime = d + 1000*c + 1000*60*b + 1000*60*60*a;
	return lTime;
}

void longToTimes(long lTime, int* a, int* b, int* c, int* d) {
	*d = lTime % 1000;
	lTime/=1000;
	*c = lTime % 60;
	lTime/=60;
	*b = lTime % 60;
	*a = lTime/60;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Add meg a file nevet a program utan!\n");
		printf("%s ./pelda.srt\n", argv[0]);
		return 1;
	}

	FILE* fin = fopen(argv[1], "r");
	if (fin == NULL) {
		printf("Rossz file nevet adtal meg!\n");
		printf("%s\n", argv[1]);
		return 1;
	}

	vector<pair<pair<long, long>, string> > nodes;

	bool go = true;
	int sa, sb, sc, sd;
	int ea, eb, ec, ed;
	int num;
	char str[200];
	do {
		fscanf(fin, "%d\r\n", &num);
		fscanf(fin, "%d:%d:%d,%d --> %d:%d:%d,%d\r\n", &sa, &sb, &sc, &sd, &ea, &eb, &ec, &ed);

		bool stop = false;

		string temp;
		bool hasFirstLine = false;
		while (!stop) {
			if (fgets(str, 200, fin) == NULL) {
				stop = true;
				go = false;
				break;
			}
			if (hasFirstLine) {
				if (strlen(str) == 2) {
					stop = true;
				}
			}
			hasFirstLine = true;
			temp.append(str);
		}

		long sTime = timesToLong(sa, sb, sc, sd);
		long eTime = timesToLong(ea, eb, ec, ed);

		nodes.push_back(make_pair(make_pair(sTime, eTime), temp));

	} while (go);

	//Elso beolvasasa
	printf("Hanyadik mondat legyen az elso tampont? (1-tol szamozva)\n");
	int tampont1;
	scanf("%d", &tampont1);

	longToTimes(nodes[tampont1-1].first.first, &sa, &sb, &sc, &sd);
	printf("%d. mondat\n",tampont1);
	printf("%.2d:%.2d:%.2d,%.3d\n", sa, sb, sc, sd);
	printf("%s", nodes[tampont1-1].second.c_str());
	scanf("%d:%d:%d,%d", &sa, &sb, &sc, &sd);

	long nsTime1 = timesToLong(sa, sb, sc, sd);

	//Masodik beolvasasa
	printf("Hanyadik mondat legyen a masodik tampont? (1-tol szamozva, ha 0-at irsz, az azt jelenti, hogy az utolso)\n");
	int tampont2;
	scanf("%d", &tampont2);

	if(tampont2 == 0) {
		tampont2 = nodes.size();
	}

	longToTimes(nodes[tampont2-1].first.first, &sa, &sb, &sc, &sd);
	printf("%d. mondat\n",tampont2);
	printf("%.2d:%.2d:%.2d,%.3d\n", sa, sb, sc, sd);
	printf("%s", nodes[tampont2-1].second.c_str());
	scanf("%d:%d:%d,%d", &sa, &sb, &sc, &sd);

	long nsTime2 = timesToLong(sa, sb, sc, sd);

	fclose(fin);

	//Processing
	// y1 = a * x1 + b
	// y2 = a * x2 + b
	// a = (y2 - y1) / (x2 - x1)
	// b = y1 - a * x1
	double a = (nsTime2 - nsTime1) / ((double) (nodes[tampont2-1].first.first - nodes[tampont1-1].first.first));
	long b = nsTime1 - (long) (a * nodes[tampont1-1].first.first);

	//Writing
	FILE* fout = fopen("./modified.srt", "w");
	for (int i=0; i<nodes.size(); ++i) {
		fprintf(fout, "%d\r\n", i+1);
		long ff = a * nodes[i].first.first + b;
		long fe = a * nodes[i].first.second + b;

		longToTimes(ff, &sa, &sb, &sc, &sd);
		longToTimes(fe, &ea, &eb, &ec, &ed);
		fprintf(fout, "%.2d:%.2d:%.2d,%.3d --> %.2d:%.2d:%.2d,%.3d\r\n", sa, sb, sc, sd, ea, eb, ec, ed);
		fprintf(fout, "%s", nodes[i].second.c_str());
	}

	fclose(fout);

	printf("Kesz. Az uj file ./modified.srt\n");



	return 0;
}
