#ifndef REGEX_MATCH_HELPER_H
#define REGEX_MATCH_HELPER_H
#include <regex>
#include <string>


namespace regex_match_helper
{
	bool matchOfficeTempFile(const std::string& str);
	bool matcheEmalAddress(const std::string& str);

	bool match(const std::string& reg_str, const std::string& str);
}

#endif //REGEX_MATCH_HELPER_H
