#pragma once
#include "Neat\Types.h"
#include "Neat\Buffer.h"

namespace Neat
{
	typedef uint8_t array6_t[6];

	class Uuid : public Buffer
	{
		typedef Buffer Base;

	public:
		Uuid();
		Uuid(Buffer&& other);
		Uuid(const Buffer& other);

		Uuid(Uuid&& other) = default;
		Uuid(const Uuid& other) = default;

		Uuid& operator=(Uuid&& other) = default;
		Uuid& operator=(const Uuid& other) = default;

		static constexpr size_t SizeInBytes();
		static constexpr size_t LengthInHex();

		static constexpr uint16_t Version();
		static constexpr uint16_t Variant();

		static Uuid Generate();

		bool operator<(const Uuid& other);
		bool operator<=(const Uuid& other);
		bool operator>(const Uuid& other);
		bool operator>=(const Uuid& other);
		bool operator==(const Uuid& other);
		bool operator!=(const Uuid& other);

		uint32_t& GetData1();
		uint16_t& GetData2();
		uint16_t& GetData3();
		uint16_t& GetData4();
		array6_t& GetData5();

		const uint32_t& GetData1() const;
		const uint16_t& GetData2() const;
		const uint16_t& GetData3() const;
		const uint16_t& GetData4() const;
		const array6_t& GetData5() const;

		uint16_t GetVersion() const;
		uint16_t GetVariant() const;

		void SetVersion(uint16_t version);
		void SetVariant(uint16_t variant);
	};

	inline constexpr size_t Uuid::SizeInBytes()
	{
		return 16_sz;
	}

	inline constexpr size_t Uuid::LengthInHex()
	{
		return 36_sz;
	}

	inline constexpr uint16_t Uuid::Version()
	{
		return 4;
	}

	inline constexpr uint16_t Uuid::Variant()
	{
		return 0b10; // variant 1
	}

	inline bool Uuid::operator<(const Uuid& other)
	{
		return memcmp(GetBuffer(), other.GetBuffer(), GetSize()) < 0;
	}

	inline bool Uuid::operator<=(const Uuid& other)
	{
		return memcmp(GetBuffer(), other.GetBuffer(), GetSize()) <= 0;
	}

	inline bool Uuid::operator>(const Uuid& other)
	{
		return !operator<=(other);
	}

	inline bool Uuid::operator>=(const Uuid& other)
	{
		return !operator<(other);
	}

	inline bool Uuid::operator==(const Uuid& other)
	{
		return 0 == memcmp(GetBuffer(), other.GetBuffer(), GetSize());
	}

	inline bool Uuid::operator!=(const Uuid& other)
	{
		return !operator==(other);
	}

	inline uint32_t& Uuid::GetData1()
	{
		auto buffer = GetBuffer();
		return *reinterpret_cast<uint32_t*>(buffer);
	}

	inline uint16_t& Uuid::GetData2()
	{
		auto buffer = GetBuffer();
		auto offset = sizeof(uint32_t);
		return *reinterpret_cast<uint16_t*>(buffer + offset);
	}

	inline uint16_t& Uuid::GetData3()
	{
		auto buffer = GetBuffer();
		auto offset = sizeof(uint32_t) + sizeof(uint16_t);
		return *reinterpret_cast<uint16_t*>(buffer + offset);
	}

	inline uint16_t& Uuid::GetData4()
	{
		auto buffer = GetBuffer();
		auto offset = sizeof(uint32_t) + sizeof(uint16_t) * 2;
		return *reinterpret_cast<uint16_t*>(buffer + offset);
	}

	inline array6_t& Uuid::GetData5()
	{
		auto buffer = GetBuffer();
		auto offset = sizeof(uint32_t) + sizeof(uint16_t) * 3;
		return *reinterpret_cast<array6_t*>(buffer + offset);
	}

	inline const uint32_t& Uuid::GetData1() const
	{
		const auto buffer = GetBuffer();
		return *reinterpret_cast<const uint32_t*>(buffer);
	}

	inline const uint16_t& Uuid::GetData2() const
	{
		const auto buffer = GetBuffer();
		const auto offset = sizeof(uint32_t);
		return *reinterpret_cast<const uint16_t*>(buffer + offset);
	}

	inline const uint16_t& Uuid::GetData3() const
	{
		const auto buffer = GetBuffer();
		const auto offset = sizeof(uint32_t) + sizeof(uint16_t);
		return *reinterpret_cast<const uint16_t*>(buffer + offset);
	}

	inline const uint16_t& Uuid::GetData4() const
	{
		const auto buffer = GetBuffer();
		const auto offset = sizeof(uint32_t) + sizeof(uint16_t) * 2;
		return *reinterpret_cast<const uint16_t*>(buffer + offset);
	}

	inline const array6_t& Uuid::GetData5() const
	{
		const auto buffer = GetBuffer();
		const auto offset = sizeof(uint32_t) + sizeof(uint16_t) * 3;
		return *reinterpret_cast<const array6_t*>(buffer + offset);
	}

	inline uint16_t Uuid::GetVersion() const
	{
		const auto data3 = GetData3();
		uint16_t version = (data3 >> 12) & 0xF;
		return static_cast<uint16_t>(version);
	}

	inline uint16_t Uuid::GetVariant() const
	{
		const auto data4 = GetData4();
		uint16_t variant = (data4 >> 14) & 0x3;
		return static_cast<uint16_t>(variant);
	}

	inline void Uuid::SetVersion(uint16_t version)
	{
		auto& data3 = GetData3();
		uint16_t mask = version << 12;
		data3 &= 0x0FFF;
		data3 |= mask;
	}

	inline void Uuid::SetVariant(uint16_t variant)
	{
		auto& data4 = GetData4();
		uint16_t mask = variant << 14;
		data4 &= 0x3FFF;
		data4 |= mask;
	}

	inline auto begin(Uuid& uuid)
	{
		auto buffer = uuid.GetBuffer();
		return reinterpret_cast<uint16_t*>(buffer);
	}

	inline auto end(Uuid& uuid)
	{
		auto buffer = uuid.GetBuffer();
		auto size = uuid.GetSize();
		return reinterpret_cast<uint16_t*>(buffer + size);
	}
}

