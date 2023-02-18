#pragma once
#include <vector>
#include <spdlog/spdlog.h>
#include <filesystem>

namespace libpepp {
	using Buffer = std::vector<uint8_t>;

	class BaseObject
	{
	public:
		explicit
			BaseObject() = default;

		explicit
			BaseObject(
				const std::filesystem::path& path
			);

		explicit
			BaseObject(
				const Buffer& buffer
			);

		bool
			open(
				const std::filesystem::path& path
			);

		void
			open(
				const Buffer& buffer
			);

		virtual
			~BaseObject() = default;

	protected:
		Buffer m_buffer;
		bool m_valid = false;

	private:
		void
			copyFrom(
				void const* pointer,
				size_t size
			);
	};
}
