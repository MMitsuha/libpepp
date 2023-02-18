#include "../../include/Details/Address.h"

namespace libpepp {
	namespace Details {
		Address::Address(
			Pe& pe
		) :m_pe(pe)
		{
			spdlog::trace("Address constructed.");
		}

		size_t
			Address::vaToRva(
				size_t va
			)
		{
			auto vaEntry = m_pe.getNtHeaders().getOptionalHeader().AddressOfEntryPoint;

			if (va < vaEntry) {
				spdlog::error("Va: 0x{:x} is not in process's memory range.", va);
			}

			return va - vaEntry;
		}

		size_t
			Address::rvaToVa(
				size_t rva
			)
		{
			auto vaEntry = m_pe.getNtHeaders().getOptionalHeader().AddressOfEntryPoint;

			return rva + vaEntry;
		}

		size_t
			Address::foToRva(
				size_t fo
			)
		{
			auto& sec = m_pe.getSectionByFo(fo);

			return fo - sec.PointerToRawData + sec.VirtualAddress;
		}

		size_t
			Address::rvaToFo(
				size_t rva
			)
		{
			auto& sec = m_pe.getSectionByRva(rva);

			return rva - sec.VirtualAddress + sec.PointerToRawData;
		}

		size_t
			Address::foToVa(
				size_t fo
			)
		{
			auto& sec = m_pe.getSectionByFo(fo);

			return rvaToVa(fo - sec.PointerToRawData + sec.VirtualAddress);
		}

		size_t
			Address::vaToFo(
				size_t va
			)
		{
			auto& sec = m_pe.getSectionByVa(va);

			return vaToRva(va) - sec.VirtualAddress + sec.PointerToRawData;
		}
	}
}
