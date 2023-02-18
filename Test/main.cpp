#include "stdafx.h"

#include "libpepp.h"
#include "Details/ExportTable.h"

int
main(
	void
)
{
	spdlog::set_level(spdlog::level::trace);

	std::string pathTarget = R"(C:\Windows\system32\kernel32.dll)";
	libpepp::Pe objTarget(pathTarget);

	auto exports = objTarget.enumDataDictionaries<libpepp::Details::ExportTable>();

	return 0;
}
