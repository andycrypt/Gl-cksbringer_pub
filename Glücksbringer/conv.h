#pragma once

#include <string>
#include <filesystem>


class conv {
public:
	conv();
	~conv();
	//static void html2json(const Aspose::Cells::U16String& path);
	//static void html2json(const std::string& path);
	static void html2json(const std::filesystem::path& path);
	static std::string replall(std::string inp,const std::string &from,const std::string &to);
	static char intToAlphabet(int i);
	static std::wstring string_to_wide_string(const std::string& string);
	static std::string wide_string_to_string(const std::wstring& wide_string);
};


