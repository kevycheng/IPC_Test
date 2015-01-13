#pragma once

#include <assert.h>
#include <string>
#include <sstream>
#include <vector>
//#include <locale>
//#include <codecvt>
//#include "curl/curl.h"


using namespace std;


namespace StringConv
{
	inline std::wstring s2ws(const std::string& s)
	{
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//return converter.from_bytes(s.c_str());
		return std::wstring(s.begin(), s.end());
	}


#ifdef _UNICODE
	inline std::string ws2s(const std::wstring& s)
	{
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//return converter.to_bytes(s.c_str());
		return std::string(s.begin(), s.end());
	}
#else
	inline std::string ws2s(const std::string& s)
	{
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//return converter.to_bytes(s.c_str());
		//return std::string(s.begin(), s.end());
		return s;
	}
#endif

	inline std::string cs2s(CString s)
	{
		return std::string(CW2A(s.GetString()));
	}

	//inline std::string encode(std::string str)
	//{
	//	char *escaped = curl_easy_escape(NULL, str.c_str(), str.length());

	//	if(escaped == NULL)
	//		return ""; // failed

	//	std::string escapedStr(escaped);
	//	curl_free(escaped);
	//	return escapedStr;
	//}


	//inline std::string decode(std::string str)
	//{
	//	char *unescaped = curl_easy_unescape(NULL, str.c_str(), str.length(), NULL);

	//	if(unescaped == NULL)
	//		return ""; // failed

	//	std::string unescapedStr(unescaped);
	//	curl_free(unescaped);
	//	return unescapedStr;
	//}

	inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	inline std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	inline std::string string_To_UTF8(const std::string & str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

		wchar_t * pwBuf = new wchar_t[nwLen + 1];
		ZeroMemory(pwBuf, nwLen * 2 + 2);

		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

		char * pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);

		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string retStr(pBuf);

		delete []pwBuf;
		delete []pBuf;

		pwBuf = NULL;
		pBuf = NULL;

		return retStr;
	}

	inline std::string UTF8_To_string(const std::string & str)
	{
		int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

		wchar_t * pwBuf = new wchar_t[nwLen + 1];
		memset(pwBuf, 0, nwLen * 2 + 2);

		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

		int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

		char * pBuf = new char[nLen + 1];
		memset(pBuf, 0, nLen + 1);

		WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string retStr = pBuf;

		delete []pBuf;
		delete []pwBuf;

		pBuf = NULL;
		pwBuf = NULL;

		return retStr;
	}
}
