#pragma once

#include "cocos2d.h"

class StringUtil {
public:
	StringUtil();
	~StringUtil();

	static std::string WStrToUTF8(const WCHAR* src);
	static std::string StrToUTF8(const char* src);

	static std::string GBKToUTF8(const char* gbk);
	static std::string UTF8ToGBK(const char* utf8);
	static std::string UnicodeToUTF8(const WCHAR* unicode);
};