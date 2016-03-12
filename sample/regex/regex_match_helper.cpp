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
		std::string reg_str = "\\w+@[a-zA-Z0-9]+.(com|cn|net)";
		std::regex pattern(reg_str);
		return std::regex_match(str, pattern);
	}

	bool match( const std::string& reg_str, const std::string& str )
	{
		std::regex pattern(reg_str);
		return std::regex_match(str, pattern);
	}

}