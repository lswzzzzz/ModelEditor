#include "StringUtil.h"

StringUtil::StringUtil() {
}

StringUtil::~StringUtil() {
}

std::string StringUtil::GBKToUTF8(const char* gbk) {
	return StrToUTF8(gbk);
}

std::string StringUtil::UTF8ToGBK(const char* utf8) {
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

std::string StringUtil::UnicodeToUTF8(const WCHAR* unicode) {
	return WStrToUTF8(unicode);
}

std::string StringUtil::WStrToUTF8(const WCHAR* src) {
	int len = WideCharToMultiByte(CP_UTF8, 0, src, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, src, -1, str, len, NULL, NULL);
	std::string strTemp(str);
	if (str) delete[] str;
	return strTemp;
}

std::string StringUtil::StrToUTF8(const char* src) {
	int len = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src, -1, wstr, len);

	std::string utf = WStrToUTF8(wstr);
	if (wstr) delete[] wstr;
	return utf;
}