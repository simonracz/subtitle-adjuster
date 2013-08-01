#include "SubRip.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

namespace Subtitles {

	using namespace std;
	
	SubRip::SubRip():valid{true}
	{
		//"int" is supposed to be at least 16 bits on all machines
		static_assert(numeric_limits<int>::max() > 362439999LL, "Too small int");
	}
	
	SubRip::SubRip(istream& is):valid{true}
	{
		//"int" is supposed to be at least 16 bits on all machines
		static_assert(numeric_limits<int>::max() > 362439999LL, "Too small int");

		valid = readFromStream(is);
	}
	
	SubRip::SubRip(std::FILE* inputFile):valid{true}
	{
		//"int" is supposed to be at least 16 bits on all machines
		static_assert(numeric_limits<int>::max() > 362439999LL, "Too small int");
		
		valid = readFromFILE(inputFile);
	}
	
	void SubRip::clear() noexcept
	{
		nodes.clear();
		valid = true;
	}
	
	int SubRip::aggregateTimes(const unsigned int a,
							   const unsigned int b,
							   const unsigned int c,
							   const unsigned int d)
	{
		return (d + 1000*c + 1000*60*b + 1000*60*60*a);
	}
	
	void SubRip::getTimesFromInt(int aggregate, unsigned int* a, unsigned int* b, unsigned int* c, unsigned int* d)
	{
		if (aggregate < 0) {
			*d = *c = *b = *a = 0;
			return;
		}
		*d = aggregate % 1000;
		aggregate/=1000;
		*c = aggregate % 60;
		aggregate/=60;
		*b = aggregate % 60;
		*a = aggregate/60;
	}

	
	bool SubRip::readFromStream(istream& is)
	{
		if ( !valid ) { clear(); }
		unsigned int sa, sb, sc, sd; //start times
		unsigned int ea, eb, ec, ed; //end times
		unsigned int num;
		bool go = true;
		auto flags = is.flags();
		char c, c1, c2;
		char cl[6];
		do {
			is >> skipws >> num >> sa;
			c = is.get(); if (c!=':') { valid = false; break; }
			is >> sb;
			c = is.get(); if (c!=':') { valid = false; break; }
			is >> sc;
			c = is.get(); if (c!=',') { valid = false; break; }
			is >> sd;
			is.get(cl, 6); if (strcmp(cl, " --> ")!=0) { valid = false; break; }
			is >> ea;
			c = is.get(); if (c!=':') { valid = false; break; }
			is >> eb;
			c = is.get(); if (c!=':') { valid = false; break; }
			is >> ec;
			c = is.get(); if (c!=',') { valid = false; break; }
			is >> ed;
			c1 = is.get(); c2 = is.get(); if ( c1!='\r' || c2!='\n') { valid = false; break; }
			is >> noskipws;
			string aggr;
			bool stop = false;
			while (!stop) {
				string temp;
				getline(is, temp, '\n'); temp.append("\n");
				if (temp.length() == 1) {
					go = false;
					break;
				}
				if (temp.length()==2) {
					stop = true;
				}
				aggr.append(temp);
			}

			int sTime = aggregateTimes(sa, sb, sc, sd);
			int eTime = aggregateTimes(ea, eb, ec, ed);
			
			nodes.push_back(make_pair(make_pair(sTime, eTime), aggr));
		} while (go);
		is.flags(flags);

		return valid;
	}
	
	bool SubRip::readFromFILE(std::FILE* fin)
	{
		if ( !valid ) { clear(); }
		unsigned int sa, sb, sc, sd; //start times
		unsigned int ea, eb, ec, ed; //end times
		bool go = true;
		unsigned int num;
		char str[200];
		do {
			if (fscanf(fin, "%u\r\n", &num) < 1) {
				valid = false;
				break;
			}
			if (fscanf(fin, "%u:%u:%u,%u --> %u:%u:%u,%u\r\n", &sa, &sb, &sc, &sd, &ea, &eb, &ec, &ed) < 8)	{
				valid = false;
				break;
			}
			
			bool stop = false;
			
			string temp;
			while (!stop) {
				if (fgets(str, 200, fin) == NULL) {
					go = false;
					break;
				}
				// "\r\n"
				if (strlen(str) == 2) {
					stop = true;
				}
				temp.append(str);
			}
			
			if (temp.length() <= 2) { valid = false; break; }
			
			int sTime = aggregateTimes(sa, sb, sc, sd);
			int eTime = aggregateTimes(ea, eb, ec, ed);
			
			nodes.push_back(make_pair(make_pair(sTime, eTime), temp));
			
		} while (go);
		
		return valid;
	}

	
	void SubRip::shift(int milliseconds)
	{
		for (auto& node : nodes) {
			node.first.first += milliseconds;
			node.first.second += milliseconds;
		}		
	}
	
	string SubRip::printNode(int index) const
	{
		if((index>0) && (index <= nodes.size())) {
			return printNode(nodes[index-1], index);
		}
		return "";
	}
	
	std::string SubRip::printNode(const Node& node, const int index)
	{
		ostringstream oss{};
		printNodeToStream(node, index, oss);
		return oss.str();
	}
	
	bool SubRip::printNodeToStream(int index, ostream& os) const
	{
		if((index>0) && (index <= nodes.size())) {
			printNodeToStream(nodes[index-1], index, os);
			return true;
		} else {
			return false;
		}
	}
	
	ostream& SubRip::printNodeToStream(const Node& node, const int index, std::ostream& os)
	{
		static unsigned int sa, sb, sc, sd; //start times
		static unsigned int ea, eb, ec, ed; //end times
		getTimesFromInt(node.first.first, &sa, &sb, &sc, &sd);
		getTimesFromInt(node.first.second, &ea, &eb, &ec, &ed);
		char prev = os.fill('0');
		os << index << "\r\n" << setw(2) << sa << ":" << setw(2) << sb << ":" << setw(2) << sc << "," << setw(3) << sd;
		os << " --> ";
		os << setw(2) << ea << ":" << setw(2) << eb << ":" << setw(2) << ec << "," << setw(3) << ed << "\r\n";
		os.fill(prev);
		return os << node.second;
	}
	
	void SubRip::printToFILE(std::FILE* fout) const
	{
		for (int i=0; i<nodes.size(); ++i) {
			fprintf(fout, "%d\r\n", i+1);
			static unsigned int sa, sb, sc, sd; //start times
			static unsigned int ea, eb, ec, ed; //end times
			getTimesFromInt(nodes[i].first.first, &sa, &sb, &sc, &sd);
			getTimesFromInt(nodes[i].first.second, &ea, &eb, &ec, &ed);
			
			fprintf(fout, "%.2u:%.2u:%.2u,%.3u --> %.2u:%.2u:%.2u,%.3u\r\n", sa, sb, sc, sd, ea, eb, ec, ed);
			fprintf(fout, "%s", nodes[i].second.c_str());
		}
	}

	ostream& operator<< (ostream& os, const SubRip& sr)
	{
		if (sr) {
			int index = 1;
			for (const auto& node : sr) {
				SubRip::printNodeToStream(node, index, os);
				++index;
			}
		}
		return os;
	}
	
	SubRip& SubRip::operator*=(double scale)
	{
		for (auto& node : nodes) {
			node.first.first*=scale;
			node.first.second*=scale;
		}
		return *this;
	}
	
} //namespace

