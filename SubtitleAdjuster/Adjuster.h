//
//  Created by Simon Racz
//

#ifndef ADJUSTER_H
#define ADJUSTER_H

#include "SubRip.h"
#include <cstdio>
#include <string>
#include <map>
#include <set>

namespace Subtitles {
	
	class Adjuster {
	public:
		Adjuster(const SubRip&);
		
		void setAnchor(const int index, const int time); //currently supports 0, 1 or 2
		void clearAnchors();
		
		void lock(const int index);
		void clearLock(const int index);
		void clearLocks();

		void clear();
		
		std::string printNode(int index) const;
		bool printNodeToStream(const int index, std::ostream& os) const;
		
		void printToFILE(std::FILE* outFile) const;
		void printToStream(std::ostream& os) const;
		
		SubRip subRip() const; //returns the adjusted SubRip
		
		void commit(bool clearLocks = true); //applies all the changes, original SubRip is overriden
		
		//returns the original SubRip
		inline SubRip orig() const { return original; }
	private:
		SubRip apply() const;
		std::map<int, int> anchors;
		std::set<int> locks;
		SubRip original;
		double scale;
		int shift;
		bool adjusted;
	};
} //namespace

#endif //ADJUSTER_H
