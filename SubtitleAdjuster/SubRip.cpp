#include "SubRip.h"
#include <iostream>
#include <limits>

namespace Subtitles {

	using namespace std;
	
	SubRip::SubRip(istream& istream):input{istream}{
		//"int" is supposed to be at least 16 bits on all machines
		static_assert(numeric_limits<int>::max() > 362439999LL, "Too small int");
	}
	
} //namespace

