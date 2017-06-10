#pragma once
#include "Neat\Types.h"

#include <initializer_list>
#include <random>

namespace Neat
{
	typedef uint8_t array2_t[2];
	typedef uint8_t array6_t[6];

	class Uuid
	{
	public:
		Uuid();
		Uuid(std::initializer_list<byte_t> list);

		Uuid(Uuid&& other);
		Uuid(const Uuid& other);

		Uuid& operator=(Uuid&& other);
		Uuid& operator=(const Uuid& other);

		static constexpr size_t SizeInBytes();
		static constexpr size_t LengthInHex();

		static constexpr uint16_t Version();
		static constexpr uint16_t Variant();

		bool operator<(const Uuid& other) const;
		bool operator<=(const Uuid& other) const;
		bool operator>(const Uuid& other) const;
		bool operator>=(const Uuid& other) const;
		bool operator==(const Uuid& other) const;
		bool operator!=(const Uuid& other) const;

		uint32_t& GetData1();
		uint16_t& GetData2();
		uint16_t& GetData3();
		array2_t& GetData4();
		array6_t& GetData5();

		const uint32_t& GetData1() const;
		const uint16_t& GetData2() const;
		const uint16_t& GetData3() const;
		const array2_t& GetData4() const;
		const array6_t& GetData5() const;

		uint16_t GetVersion() const;
		uint16_t GetVariant() const;

		void SetVersion(uint16_t version);
		void SetVariant(uint16_t variant);

	private:
		byte_t m_raw[16];

		friend class UuidTest;
		friend auto begin(Uuid& uuid);
		friend auto end(Uuid& uuid);
	};

	inline Uuid::Uuid()
	{
		memset(m_raw, 0, sizeof(m_raw));
	}

	inline Uuid::Uuid(Uuid&& other)
	{
		memcpy_s(m_raw, sizeof(m_raw), other.m_raw, sizeof(other.m_raw));
		memset(other.m_raw, 0, sizeof(other.m_raw));
	}

	inline Uuid::Uuid(const Uuid& other)
	{
		memcpy_s(m_raw, sizeof(m_raw), other.m_raw, sizeof(other.m_raw));
	}

	inline Uuid& Uuid::operator=(Uuid&& other)
	{
		memcpy_s(m_raw, sizeof(m_raw), other.m_raw, sizeof(other.m_raw));
		memset(other.m_raw, 0, sizeof(other.m_raw));
		return *this;
	}

	inline Uuid& Uuid::operator=(const Uuid& other)
	{
		memcpy_s(m_raw, sizeof(m_raw), other.m_raw, sizeof(other.m_raw));
		return *this;
	}

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

	inline bool Uuid::operator<(const Uuid& other) const
	{
		return memcmp(m_raw, other.m_raw, sizeof(m_raw)) < 0;
	}

	inline bool Uuid::operator<=(const Uuid& other) const
	{
		return memcmp(m_raw, other.m_raw, sizeof(m_raw)) <= 0;
	}

	inline bool Uuid::operator>(const Uuid& other) const
	{
		return !operator<=(other);
	}

	inline bool Uuid::operator>=(const Uuid& other) const
	{
		return !operator<(other);
	}

	inline bool Uuid::operator==(const Uuid& other) const
	{
		return 0 == memcmp(m_raw, other.m_raw, sizeof(m_raw));
	}

	inline bool Uuid::operator!=(const Uuid& other) const
	{
		return !operator==(other);
	}

	inline uint32_t& Uuid::GetData1()
	{
		return *reinterpret_cast<uint32_t*>(m_raw);
	}

	inline uint16_t& Uuid::GetData2()
	{
		auto offset = sizeof(uint32_t);
		return *reinterpret_cast<uint16_t*>(m_raw + offset);
	}

	inline uint16_t& Uuid::GetData3()
	{
		auto offset = sizeof(uint32_t) + sizeof(uint16_t);
		return *reinterpret_cast<uint16_t*>(m_raw + offset);
	}

	inline array2_t& Uuid::GetData4()
	{
		auto offset = sizeof(uint32_t) + sizeof(uint16_t) * 2;
		return *reinterpret_cast<array2_t*>(m_raw + offset);
	}

	inline array6_t& Uuid::GetData5()
	{
		auto offset = sizeof(uint32_t) + sizeof(uint16_t) * 3;
		return *reinterpret_cast<array6_t*>(m_raw + offset);
	}

	inline const uint32_t& Uuid::GetData1() const
	{
		return *reinterpret_cast<const uint32_t*>(m_raw);
	}

	inline const uint16_t& Uuid::GetData2() const
	{
		const auto offset = sizeof(uint32_t);
		return *reinterpret_cast<const uint16_t*>(m_raw + offset);
	}

	inline const uint16_t& Uuid::GetData3() const
	{
		const auto offset = sizeof(uint32_t) + sizeof(uint16_t);
		return *reinterpret_cast<const uint16_t*>(m_raw + offset);
	}

	inline const array2_t& Uuid::GetData4() const
	{
		const auto offset = sizeof(uint32_t) + sizeof(uint16_t) * 2;
		return *reinterpret_cast<const array2_t*>(m_raw + offset);
	}

	inline const array6_t& Uuid::GetData5() const
	{
		const auto offset = sizeof(uint32_t) + sizeof(uint16_t) * 3;
		return *reinterpret_cast<const array6_t*>(m_raw + offset);
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
		uint16_t variant = (data4[0] >> 6) & 0x3;
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
		uint16_t mask = variant << 6;
		data4[0] &= 0x3F;
		data4[0] |= mask;
	}

	inline auto begin(Uuid& uuid)
	{
		return reinterpret_cast<uint32_t*>(uuid.m_raw);
	}

	inline auto end(Uuid& uuid)
	{
		return reinterpret_cast<uint32_t*>(uuid.m_raw + sizeof(uuid.m_raw));
	}

	//
	// Generator
	//

	class UuidGenerator
	{
	public:
		UuidGenerator();

		Uuid Generate();

	private:
		std::random_device m_randDevice;
		std::default_random_engine m_randEngine;
		std::uniform_int_distribution<uint32_t> m_uint32;
	};
}

