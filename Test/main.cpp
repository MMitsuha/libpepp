#include "stdafx.h"

#include "libpepp.h"
#include "Details/ExportTable.h"
#include "Details/ImportTable.h"
#include "Details/BaseRelocTable.h"

int
main(
	void
)
{
	spdlog::set_level(spdlog::level::trace);

	std::string pathTarget = R"(C:\Windows\system32\kernel32.dll)";
	libpepp::Pe objTarget(pathTarget);

	auto exports = objTarget.enumDataDictionaries<libpepp::Details::ExportTable>();
	auto imports = objTarget.enumDataDictionaries<libpepp::Details::ImportTable>();
	auto baserelocs = objTarget.enumDataDictionaries<libpepp::Details::BaseRelocTable>();

	return 0;
}
