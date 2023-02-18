#include "../include/BaseObject.h"
#include <fstream>

namespace libpepp {
	BaseObject::BaseObject(
		const std::filesystem::path& path
	)
	{
		spdlog::trace("Base object constructed.");
		open(path);
	}

	BaseObject::BaseObject(
		const Buffer& buffer
	)
	{
		spdlog::trace("Base object constructed.");
		open(buffer);
	}

	bool
		BaseObject::open(
			const std::filesystem::path& path
		)
	{
		spdlog::trace("Building base object with given path.");
		m_buffer.clear();
		std::ifstream file(path, std::ios_base::binary);

		if (!file.is_open()) {
			m_valid = false;
			return false;
		}

		std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(m_buffer));
		m_valid = true;
		return true;
	}

	void
		BaseObject::open(
			const Buffer& buffer
		)
	{
		spdlog::trace("Building base object with given buffer.");
		m_buffer.clear();
		std::copy(buffer.cbegin(), buffer.cend(), std::back_inserter(m_buffer));
		m_valid = true;
	}

	void
		BaseObject::copyFrom(
			void const* pointer,
			size_t size
		)
	{
		spdlog::trace("Copied to base object with size: {}.", size);
		m_buffer.clear();
		auto buffer = reinterpret_cast<const uint8_t*>(pointer);
		std::copy(buffer, buffer + size, std::back_inserter(m_buffer));
	}
}
