//
//  Created by Simon Racz
//

#ifndef ADJUSTER_H
#define ADJUSTER_H

#include "SubRip.h"
#include <cstdio>
#include <string>
#include <map>

namespace Subtitles {
	
	class Adjuster {
	public:
		Adjuster(const SubRip&);
		
		void setAnchor(const int index, const int time); //currently supports only 0, 1 or 2
		void clearAnchors();
		
		std::string printNode(int index) const;
		bool printNodeToStream(const int index, std::ostream& os) const;
		
		void printToFILE(std::FILE* outFile) const;
		void printToStream(std::ostream& os) const;
		
		SubRip subRip() const; //returns the adjusted SubRip
		
		//returns the original SubRip
		inline SubRip orig() const { return original; }
	private:
		std::map<int, int> anchors;
		SubRip original;
		double scale;
		int shift;
		bool adjusted;
	};
} //namespace

#endif //ADJUSTER_H
