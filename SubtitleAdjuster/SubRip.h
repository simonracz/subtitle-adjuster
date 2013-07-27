#ifndef SUBRIP_H
#define SUBRIP_H

#include <iosfwd>
#include <vector>
#include <string>

namespace Subtitles {

	class SubRip {
	public:
		SubRip(std::istream&);
		
		SubRip(SubRip&) = delete;
		SubRip& operator=(const SubRip&) = delete;
		
		SubRip(SubRip&&) = default;
		SubRip& operator=(SubRip&&) = default;
		
		~SubRip();
	private:
		std::vector<std::pair<std::pair<int, int>, std::string>> nodes;
		std::istream& input;
	};
	
} //namespace

#endif //SUBRIP_H