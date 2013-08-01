#ifndef SUBRIP_H
#define SUBRIP_H

#include <iosfwd>
#include <vector>
#include <string>
#include <cstdio>

namespace Subtitles {
	
	//https://en.wikipedia.org/wiki/SubRip
	class SubRip {
	
	public:
		using Node = std::pair<std::pair<int, int>, std::string>;
		using iterator = std::vector<Node>::iterator;
		using const_iterator = std::vector<Node>::const_iterator;
		
		SubRip();
		explicit SubRip(std::istream&); //uses C++ streams io api internally
		explicit SubRip(std::FILE* inputFile); //uses C style cstdio api internally
				
		bool readFromStream(std::istream&); //uses C++ streams io api internally
		bool readFromFILE(std::FILE*); //uses C style cstdio api internally
		
		//the returned integer can be put inside a Node
		static int aggregateTimes(const unsigned int hours,
								  const unsigned int minutes,
								  const unsigned int seconds,
								  const unsigned int milliseconds);
		
		//get back time data from a Node integer
		//in case of negative aggregate parameter it returns 0,0,0,0
		static void getTimesFromInt(int aggregate,
									unsigned int* hours,
									unsigned int* minutes,
									unsigned int* seconds,
									unsigned int* milliseconds);
		

		std::string printNode(const int index) const; //indexing start from 1
		static std::string printNode(const Node& node, const int index);
		bool printNodeToStream(const int index, std::ostream& os) const; //indexing starts from 1!!
		static std::ostream& printNodeToStream(const Node& node, const int index, std::ostream& os);
		
		void printToFILE(std::FILE* outFile) const; //uses C style cstdio api internally
		
		void shift(const int milliseconds);

		void clear() noexcept;
		
		inline size_t size() const noexcept { return nodes.size(); }
		
		inline bool empty() const noexcept { return nodes.empty(); }
		
		inline const Node& operator[](const int index) const { return nodes[index-1]; }
		inline Node& operator[](const int index) {	return nodes[index-1]; }
		
		//For Range-based for loops
		inline iterator begin() noexcept { return nodes.begin(); }
		inline const_iterator begin() const noexcept { return nodes.begin(); }
		inline iterator end() noexcept { return nodes.end(); }
		inline const_iterator end() const noexcept { return nodes.end(); }
		
		inline void push_back(const Node& node) { nodes.push_back(node); }
		inline void push_back(Node&& node) { nodes.push_back(node); }
		
		explicit operator bool() const noexcept { return valid; }
		
		inline SubRip& operator+=(int milliseconds) { this->shift(milliseconds); return *this; }
		inline SubRip& operator-=(int milliseconds) { this->shift(-milliseconds); return *this; }
		SubRip& operator*=(double scale);
	private:
		std::vector<Node> nodes;
		bool valid;
	};
	
	std::ostream& operator<< (std::ostream& os, const SubRip& sr);
	
} //namespace

#endif //SUBRIP_H