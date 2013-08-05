//
//  Created by Simon Racz
//

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
	
	void Adjuster::clear()
	{
		clearAnchors();
		clearLocks();
	}
	
	SubRip Adjuster::apply() const
	{
		SubRip temp{original};
		temp*=scale;
		temp+=shift;
		for (auto lock : locks) {
			temp[lock] = original[lock];
		}
		return move(temp);
	}
	
	string Adjuster::printNode(int index) const
	{
		if (!adjusted) { return original.printNode(index); }
		return apply().printNode(index);
	}
	
	bool Adjuster::printNodeToStream(const int index, std::ostream& os) const
	{
		if (!adjusted) { return original.printNodeToStream(index, os); }
		return apply().printNodeToStream(index, os);
	}
	
	void Adjuster::printToFILE(std::FILE* fout) const
	{
		if (!adjusted) {
			original.printToFILE(fout);
			return;
		}
		apply().printToFILE(fout);
		return;
	}
	
	void Adjuster::printToStream(std::ostream& os) const
	{
		if (!adjusted) {
			os << original;
			return;
		}
		os << apply();
		return;
	}
	
	void Adjuster::commit(bool clearLocks)
	{
		original = this->subRip();
		if (clearLocks) {
			this->clearLocks();
		}
	}
	
	void Adjuster::lock(const int index)
	{
		locks.insert(index);
	}
	
	void Adjuster::clearLock(const int index)
	{
		locks.erase(index);
	}
	void Adjuster::clearLocks()
	{
		locks.clear();
	}
	
	SubRip Adjuster::subRip() const
	{
		if (!adjusted) { return original; }
		return apply();
	}
	
}