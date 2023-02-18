#include "../include/FileHeader.h"

namespace libpepp {
	FileHeader::FileHeader(
		size_t offset
	)
	{
		spdlog::trace("File header constructed with offset: {}.", offset);
		open(offset);
	}

	void
		FileHeader::open(
			size_t offset
		)
	{
		spdlog::trace("Building file header with base object and offset: {}.", offset);
		open(m_buffer, offset);
	}

	void
		FileHeader::open(
			const Buffer& buffer,
			size_t offset
		)
	{
		if (buffer.empty()) {
			m_valid = false;
			return;
		}

		spdlog::trace("Building file header with given buffer and offset: {}.", offset);
		const auto pFileHeader = reinterpret_cast<IMAGE_FILE_HEADER const*>(buffer.data() + offset);

		// Copy to myself
		copyFrom(pFileHeader);
		m_valid = true;
	}

	void
		FileHeader::copyTo(
			size_t offset
		)
	{
		copyTo(m_buffer, offset);
	}

	void
		FileHeader::copyTo(
			Buffer& buffer,
			size_t offset
		)
	{
		spdlog::trace("Copied file header to buffer with allocate");
		auto pointer = reinterpret_cast<uint8_t*>(dynamic_cast<PIMAGE_FILE_HEADER>(this));
		buffer.resize(offset + sizeof(IMAGE_FILE_HEADER));
		std::copy(pointer, pointer + sizeof(IMAGE_FILE_HEADER), buffer.begin() + offset);
	}

	void
		FileHeader::copyToNoAlloc(
			size_t offset
		)
	{
		copyToNoAlloc(m_buffer, offset);
	}

	void
		FileHeader::copyToNoAlloc(
			Buffer& buffer,
			size_t offset
		)
	{
		spdlog::trace("Copied file header to buffer without allocate");
		auto pointer = reinterpret_cast<uint8_t*>(dynamic_cast<PIMAGE_FILE_HEADER>(this));
		std::copy(pointer, pointer + sizeof(IMAGE_FILE_HEADER), buffer.begin() + offset);
	}

	void
		FileHeader::copyFrom(
			IMAGE_FILE_HEADER const* pointer
		)
	{
		spdlog::trace("Copied to file header.");
		auto pFileHeader = dynamic_cast<PIMAGE_FILE_HEADER>(this);
		memcpy(pFileHeader, pointer, sizeof(IMAGE_FILE_HEADER));
	}
}
