#include "../../include/Details/ImportTable.h"
#include "../../include/Details/Address.h"
#include <spdlog/spdlog.h>

namespace libpepp {
	namespace Details {
		ImportTable::ImportTable(
			Pe& pe
		)
		{
			// TODO: Support forwarder chain

			spdlog::trace("Import table constructed.");
			auto addrConv = pe.getDetailed<Address>();
			auto importDir = pe.getNtHeaders().getOptionalHeader().DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
			auto rva = importDir.VirtualAddress;
			auto size = importDir.Size;

			if (rva == 0) {
				return;
			}

			auto base = pe.getBuffer().data();
			auto pImport = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(base + addrConv.rvaToFo(rva));

			while (pImport->Name) {
				Entry entry{};
				std::string name = reinterpret_cast<char*>(base + addrConv.rvaToFo(pImport->Name));
				spdlog::debug("In {}:", name);
				entry.DllName = std::move(name);
				entry.TimeDateStamp = pImport->TimeDateStamp;
				entry.ForwarderChain = pImport->ForwarderChain;

				if (pImport->OriginalFirstThunk) {
					spdlog::debug("\tImport lookup table:");
					auto pIlt = reinterpret_cast<PIMAGE_THUNK_DATA>(base + addrConv.rvaToFo(pImport->OriginalFirstThunk));
					while (pIlt->u1.Function) {
						ImpFunc data{};
						if (FlagOn(pIlt->u1.Function, IMAGE_ORDINAL_FLAG)) {
							// Imported by ordinal
							spdlog::debug("\t\tImported by ordinal: 0x{:x}.", ClearFlag(pIlt->u1.Ordinal, IMAGE_ORDINAL_FLAG));
						}
						else {
							// Imported by name
							auto func = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(base + addrConv.rvaToFo(pIlt->u1.Function));
							spdlog::debug("\t\tImported by name: {} and hint: {}.", func->Name, func->Hint);
							data.ByName.Hint = func->Hint;
							data.ByName.Name = func->Name;
						}

						data.Ordinal = pIlt->u1.Function;
						entry.Ilt.emplace_back(std::move(data));
						pIlt++;
					}
				}
				else if (pImport->FirstThunk) {
					spdlog::debug("\tImport address table:");
					auto pIat = reinterpret_cast<PIMAGE_THUNK_DATA>(base + addrConv.rvaToFo(pImport->FirstThunk));
					while (pIat->u1.Function) {
						ImpFunc data{};
						if (pIat->u1.Function & IMAGE_ORDINAL_FLAG) {
							// Imported by ordinal
							spdlog::debug("\t\tImported by ordinal: 0x{:x}.", ClearFlag(pIat->u1.Ordinal, IMAGE_ORDINAL_FLAG));
						}
						else {
							// Imported by name
							auto func = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(base + addrConv.rvaToFo(pIat->u1.Function));
							spdlog::debug("\t\tImported by name: {} and hint: {}.", func->Name, func->Hint);
							data.ByName.Hint = func->Hint;
							data.ByName.Name = func->Name;
						}

						data.Ordinal = pIat->u1.Function;
						entry.Iat.emplace_back(std::move(data));
						pIat++;
					}
				}
				else spdlog::warn("No import was found in the file.");

				m_Table.emplace_back(std::move(entry));
				pImport++;
			}
		}

		void
			ImportTable::enumAll(
				std::function<bool(Entry&)> func
			)
		{
			for (auto& entry : m_Table) {
				func(entry);
			}
		}
	}
}
