#ifndef REGEX_MATCH_HELPER_H
#define REGEX_MATCH_HELPER_H
#include <regex>
#include <string>
#include <vector>

namespace regex_match_helper
{
	// match interface
	bool matchOfficeTempFile(const std::string& str);
	bool matcheEmalAddress(const std::string& str);
	bool match(const std::string& reg_str, const std::string& str);

	// search interface
	size_t searchFirstEmailAddress(const std::string& src,std::string& re);
	void searchAllEmailAddress(const std::string& src, std::vector<std::string>& re);
}

#endif //REGEX_MATCH_HELPER_H
