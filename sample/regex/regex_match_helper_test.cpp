#include <iostream>
#include "regex_match_helper_test.h"
#include "regex_match_helper.h"

namespace regex_match_helper_test
{
	void output(bool re, const std::string& str)
	{
		std::string describe;
		describe = (re == true )?"匹配成功":"匹配失败";
		std::cout << str << "   " << describe<<std::endl;
	}

	void testOfficeTempFileMatch()
	{
		std::string name_1 = "12345.doc";
		std::string name_2 = "12345.docx";
		std::string pre_name_1 = "~$12345.doc";
		std::string pre_name_2 = "~$12345.docx";
		std::string pre_name_3 = "~$中国—你好.doc";
		std::string pre_name_4 = "~$$$11qqw你好_&()~.docx";
		bool re = regex_match_helper::matchOfficeTempFile(name_1);
		output(re, name_1);
		re = regex_match_helper::matchOfficeTempFile(name_2);
		output(re, name_2);
		re = regex_match_helper::matchOfficeTempFile(pre_name_1);
		output(re, pre_name_1);
		re = regex_match_helper::matchOfficeTempFile(pre_name_2);
		output(re, pre_name_2);
		re = regex_match_helper::matchOfficeTempFile(pre_name_3);
		output(re, pre_name_3);
		re = regex_match_helper::matchOfficeTempFile(pre_name_4);
		output(re, pre_name_4);
	}

	void testBoundary()
	{
		std::string reg_str = "\\bnev";

		std::string str_1 = "never";
		std::string str_2 = "never ";
		std::string str_3 = "never hello";
		std::string str_4 = "verb";
		bool re = regex_match_helper::match(reg_str, str_1);
		output(re, str_1);
		re = regex_match_helper::match(reg_str, str_2);
		output(re, str_2);
		re = regex_match_helper::match(reg_str, str_3);
		output(re, str_3);
		re = regex_match_helper::match(reg_str, str_4);
		output(re, str_4);
	}

}
