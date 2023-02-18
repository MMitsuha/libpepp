#include "../../include/Details/ExportTable.h"
#include "../../include/Details/Address.h"
#include <spdlog/spdlog.h>

namespace libpepp {
	namespace Details {
		ExportTable::ExportTable(
			Pe& pe
		)
		{
			spdlog::trace("Export table constructed.");
			auto addrConv = pe.getDetailed<Address>();
			auto importDir = pe.getNtHeaders().getOptionalHeader().DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
			auto rva = importDir.VirtualAddress;
			auto size = importDir.Size;

			if (rva == 0) {
				return;
			}

			auto base = pe.getBuffer().data();
			auto pExport = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(base + addrConv.rvaToFo(rva));
			auto tableName = reinterpret_cast<uint32_t*>(base + addrConv.rvaToFo(pExport->AddressOfNames));
			auto tableAddr = reinterpret_cast<uint32_t*>(base + addrConv.rvaToFo(pExport->AddressOfFunctions));
			auto tableOrdName = reinterpret_cast<uint16_t*>(base + addrConv.rvaToFo(pExport->AddressOfNameOrdinals));
			auto baseOrdinal = pExport->Base;

			for (size_t i = 0; i < pExport->NumberOfFunctions; i++) {
				std::string funcName = reinterpret_cast<char*>(base + addrConv.rvaToFo(tableName[i]));
				auto funcOrd = tableOrdName[i] + baseOrdinal;
				auto funcRvaAddr = tableAddr[i];
				m_Table.emplace_back(funcOrd, funcName, funcRvaAddr);

				spdlog::debug("Ordinal: {}, name: {}, rva: 0x{:x}.", funcOrd, funcName, funcRvaAddr);
			}
		}

		void
			ExportTable::enumAll(
				std::function<bool(Entry&)> func
			)
		{
			for (auto& entry : m_Table) {
				func(entry);
			}
		}
	}
}
