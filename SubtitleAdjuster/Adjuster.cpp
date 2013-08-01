#include "Adjuster.h"
#include <iostream>

namespace Subtitles {
	
	using namespace std;

	Adjuster::Adjuster(const SubRip& sr):original{sr},scale{1.0}, shift{0},adjusted{false}
	{
	}
	
	void Adjuster::setAnchor(const int index, const int time)
	{
		//It won't be this complicated once it supports several anchors
		if (anchors.empty()) {
			anchors[index]=time;
			shift = time - original[index].first.first;
			adjusted = true;
			return;
		}
		if (anchors.size()==1) {
			// rare case
			if (anchors.begin()->first == index) {
				anchors[index] = time;
				shift = time - original[index].first.first;
				return;
			}
			anchors[index]=time;
			// y1 = a * x1 + b
			// y2 = a * x2 + b
			// a = (y2 - y1) / (x2 - x1)
			// b = y1 - a * x1
			int index1 = anchors.begin()->first;
			int index2 = (++anchors.begin())->first;
			scale = (anchors[index2] - anchors[index1]) / ((double) (original[index2].first.first - original[index1].first.first));
			shift = anchors[index1] - (int) ( scale * original[index1].first.first);
			adjusted = true;
			return;
		}		
		return;
	}
	
	void Adjuster::clearAnchors()
	{
		anchors.clear();
		scale = 1.0;
		shift = 0;
		adjusted = false;
	}
	
	string Adjuster::printNode(int index) const
	{
		if (!adjusted) { return original.printNode(index); }
		SubRip temp{original};
		temp*=scale;
		temp+=shift;
		return temp.printNode(index);
	}
	
	bool Adjuster::printNodeToStream(const int index, std::ostream& os) const
	{
		if (!adjusted) { return original.printNodeToStream(index, os); }
		SubRip temp{original};
		temp*=scale;
		temp+=shift;
		return temp.printNodeToStream(index, os);
	}
	
	void Adjuster::printToFILE(std::FILE* fout) const
	{
		if (!adjusted) {
			original.printToFILE(fout);
			return;
		}
		SubRip temp{original};
		temp*=scale;
		temp+=shift;
		temp.printToFILE(fout);
		return;
	}
	
	void Adjuster::printToStream(std::ostream& os) const
	{
		if (!adjusted) {
			os << original;
			return;
		}
		SubRip temp{original};
		temp*=scale;
		temp+=shift;
		os << temp;
		return;
	}
	
	SubRip Adjuster::subRip() const
	{
		if (!adjusted) { return original; }
		SubRip temp{original};
		temp*=scale;
		temp+=shift;
		return move(temp);
	}
	
}