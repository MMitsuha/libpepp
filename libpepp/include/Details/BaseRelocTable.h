#pragma once

#include "../libpepp.h"

namespace libpepp {
	namespace Details {
		class BaseRelocTable
		{
		public:
			using Entry = struct _ENTRY {
				uint8_t Type = 0;
				uint32_t Rva = 0;
			};

			explicit
				BaseRelocTable() = default;

			explicit
				BaseRelocTable(
					Pe& pe
				);

			void
				enumAll(
					std::function<bool(Entry&)> func
				);

		private:
			std::vector<Entry> m_Table;
		};
	}
}
