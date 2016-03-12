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

	void testEmailMatch()
	{
		std::string email_1 = "123456@qq.com";
		std::string email_2 = "qweqe_ww@qq.com";
		std::string email_3 = "ffff@qq.com.cn";
		std::string email_4 = "ffff.aa.bb@qq.com.cn";
		std::string email_5 = "www.baidu.com@168.com";
		std::string email_6 = "www_bbb_ccc_.a.b.c@168.net";
		std::string email_7 = "wwwwwwwww@gmail.com.cn";
		std::string email_8 = "uuuuuuu@gmail.japen";
		std::string email_9 = "yyyy@zzz";
		bool re = regex_match_helper::matcheEmalAddress(email_1);
		output(re, email_1);
		re = regex_match_helper::matcheEmalAddress(email_2);
		output(re, email_2);
		re = regex_match_helper::matcheEmalAddress(email_3);
		output(re, email_3);
		re = regex_match_helper::matcheEmalAddress(email_4);
		output(re, email_4);
		re = regex_match_helper::matcheEmalAddress(email_5);
		output(re, email_5);
		re = regex_match_helper::matcheEmalAddress(email_6);
		output(re, email_6);
		re = regex_match_helper::matcheEmalAddress(email_7);
		output(re, email_7);
		re = regex_match_helper::matcheEmalAddress(email_8);
		output(re, email_8);
		re = regex_match_helper::matcheEmalAddress(email_9);
		output(re, email_9);
	}

}
