#include "../../include/Details/BaseRelocTable.h"
#include "../../include/Details/Address.h"
#include <spdlog/spdlog.h>

namespace libpepp {
	namespace Details {
		BaseRelocTable::BaseRelocTable(
			Pe& pe
		)
		{
			spdlog::trace("Base reloc table constructed.");
			auto addrConv = pe.getDetailed<Address>();
			auto relocsDir = pe.getNtHeaders().getOptionalHeader().DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
			auto rva = relocsDir.VirtualAddress;
			auto size = relocsDir.Size;

			if (rva == 0) {
				return;
			}

			auto base = pe.getBuffer().data();
			auto endOfRelocs = reinterpret_cast<uint8_t*>(base + addrConv.rvaToFo(rva) + size);
			auto pRelocsBlock = reinterpret_cast<PIMAGE_BASE_RELOCATION>(base + addrConv.rvaToFo(rva));
			auto i = 0;
			while (reinterpret_cast<uint8_t*>(pRelocsBlock) < endOfRelocs) {
				auto numRelocsInBlock = (pRelocsBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(uint16_t);
				auto reloc = reinterpret_cast<uint16_t*>(pRelocsBlock + 1);
				for (uint32_t i = 0; i < numRelocsInBlock; i++) {
					auto type = reloc[i] >> 12;
					auto offset = reloc[i] & 0x0FFF;
					auto rvaReloc = pRelocsBlock->VirtualAddress + offset;
					m_Table.emplace_back(type, rvaReloc);

					spdlog::debug("{}: Relocation type: {}, rva: 0x{:x}", i, type, rvaReloc);
				}

				pRelocsBlock = reinterpret_cast<PIMAGE_BASE_RELOCATION>(reinterpret_cast<uint8_t*>(pRelocsBlock) + pRelocsBlock->SizeOfBlock);
				i++;
			}
		}

		void
			BaseRelocTable::enumAll(
				std::function<bool(Entry&)> func
			)
		{
			for (auto& entry : m_Table) {
				func(entry);
			}
		}
	}
}
