#pragma once

#include "../libpepp.h"

namespace libpepp {
	namespace Details {
		class Address
		{
		public:
			explicit
				Address() = delete;

			explicit
				Address(
					Pe& pe
				);

			size_t
				vaToRva(
					size_t va
				);

			size_t
				rvaToVa(
					size_t rva
				);

			size_t
				foToRva(
					size_t fo
				);

			size_t
				rvaToFo(
					size_t rva
				);

			size_t
				foToVa(
					size_t fo
				);

			size_t
				vaToFo(
					size_t va
				);

		private:
			Pe& m_pe;
		};
	}
}
