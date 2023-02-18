#include <iostream>
#include <string>
#include <spdlog\spdlog.h>
#include "libpepp.h"
#include "Details/ExportTable.h"

int
main(
	void
)
{
	spdlog::set_level(spdlog::level::debug);

	std::string pathTarget = R"(C:\Windows\system32\kernel32.dll)";
	libpepp::Pe objTarget(pathTarget);

	auto exports = libpepp::Details::ExportTable(objTarget);

	return 0;
}
