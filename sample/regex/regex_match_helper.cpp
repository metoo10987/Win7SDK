#include "regex_match_helper.h"

using namespace std;
namespace regex_match_helper
{
	const string regex_office_temp_file = "~\\$.*.(doc|docx|xlsx|xls|ppt|pptx)";
	const string regex_email = "(\\w+)+(\\.|\\w+)*@(\\w+)(\\.[A-Za-z0-9]+)+";


	// match interface define
	bool matchOfficeTempFile(const std::string& str)
	{
		std::regex pattern(regex_office_temp_file);
		return std::regex_match(str, pattern);
	}

	bool matcheEmalAddress(const std::string& str)
	{
		std::regex pattern(regex_email);
		return std::regex_match(str, pattern);
	}

	bool match( const std::string& reg_str, const std::string& str )
	{
		std::regex pattern(reg_str);
		return std::regex_match(str, pattern);
	}
	

	// search interface define
	size_t searchFirstEmailAddress(const std::string& src, std::string& re)
	{
		std::regex pattern(regex_email);
		std::smatch result;
		size_t pos = -1;
		if (std::regex_search(src, result, pattern) == true)
		{
			re = result.str();
			pos = result.position();
		}
		return pos;
	}

	void searchAllEmailAddress(const std::string& src, vector<std::string>& re)
	{
		std::regex pattern(regex_email);
		std::smatch result;
		const std::sregex_token_iterator end;
		for (std::sregex_token_iterator it(src.begin(),src.end(),pattern); it != end; ++it)
		{
			re.push_back(*it);
		}
	}

}