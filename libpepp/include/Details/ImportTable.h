#pragma once
#include <vector>
#include <string>
#include <windows.h>

#include "../libpepp.h"

namespace libpepp {
	namespace Details {
		class ImportTable
		{
		public:
			using ImpFunc = struct _IMP_FUNC {
				size_t Ordinal = 0;
				struct
				{
					WORD Hint = 0;
					std::string Name;
				} ByName;
			};

			using Entry = struct _ENTRY {
				std::string DllName;
				DWORD TimeDateStamp = 0;
				DWORD ForwarderChain = 0;
				std::vector<ImpFunc> Iat;
				std::vector<ImpFunc> Ilt;
			};

			explicit
				ImportTable() = default;

			explicit
				ImportTable(
					Pe& pe
				);

			void
				enumAll(
					std::function<bool(Entry&)> func
				);

		private:
			std::vector<Entry> m_Table{};
		};
	}
}
