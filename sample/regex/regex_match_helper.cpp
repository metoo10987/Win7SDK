#include "regex_match_helper.h"

namespace regex_match_helper
{
	bool matchOfficeTempFile(const std::string& str)
	{
		std::string reg_str = "~\\$.*.(doc|docx|xlsx|xls|ppt|pptx)";
		std::regex pattern(reg_str);
		return std::regex_match(str, pattern);
	}

	bool matcheEmalAddress(const std::string& str)
	{
		std::string reg_str = "(\\w+)(\\.|\\w+)*@(\\w+)(\\.[A-Za-z0-9]+)+";
		std::regex pattern(reg_str);
		return std::regex_match(str, pattern);
	}

	bool match( const std::string& reg_str, const std::string& str )
	{
		std::regex pattern(reg_str);
		return std::regex_match(str, pattern);
	}
}