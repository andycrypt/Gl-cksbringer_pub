#include <Windows.h>
#include "conv.h"

#include <boost/algorithm/string/replace.hpp>
#include <Aspose.Cells.h>


using namespace std;

conv::conv() { Aspose::Cells::Startup(); }

conv::~conv() { Aspose::Cells::Cleanup(); }

void conv::html2json(const std::filesystem::path& path) {
	Aspose::Cells::U16String tmp = path.generic_u16string().c_str();
	//Aspose::Cells::U16String tmp{ static_cast<Aspose::Cells::U16String>(path.c_str()) }; //--> idk, working but could result in problems ?!
	Aspose::Cells::HtmlLoadOptions loadOptions(Aspose::Cells::LoadFormat::Html);
	Aspose::Cells::JsonSaveOptions saveOptions;
	Aspose::Cells::Workbook wb(tmp, loadOptions);
	wb.Save(tmp + ".json", saveOptions);
}

string conv::replall(string inp,const string &from,const string &to) {
	boost::replace_all(inp, from, to);
	return inp;
}

char conv::intToAlphabet(int i) {
	assert(i >= 1 && i <= 26);
	return static_cast<char>('A' - 1 + i);
}

std::wstring conv::string_to_wide_string(const std::string& string)
{
	if (string.empty())
	{
		return L"";
	}

	const auto size_needed = MultiByteToWideChar(CP_UTF8, 0, string.data(), (int)string.size(), nullptr, 0);
	if (size_needed <= 0)
	{
		throw std::runtime_error("MultiByteToWideChar() failed: " + std::to_string(size_needed));
	}

	std::wstring result(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, string.data(), (int)string.size(), result.data(), size_needed);
	return result;
}

std::string conv::wide_string_to_string(const std::wstring& wide_string)
{
	if (wide_string.empty())
	{
		return "";
	}
	
	const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, wide_string.data(), (int)wide_string.size(), nullptr, 0, nullptr, nullptr);
	if (size_needed <= 0)
	{
		throw std::runtime_error("WideCharToMultiByte() failed: " + std::to_string(size_needed));
	}

	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wide_string.data(), (int)wide_string.size(), result.data(), size_needed, nullptr, nullptr);
	return result;
}
