#include "Neat\Uuid.h"

namespace Neat
{
	Uuid::Uuid(std::initializer_list<byte_t> list) :
		Uuid()
	{
		size_t i = 0;
		for (auto byte : list)
		{
			m_raw[i++] = byte;
			if (i >= Uuid::SizeInBytes())
				break;
		}
	}

	UuidGenerator::UuidGenerator() :
		m_randEngine(m_randDevice())
	{
	}

	Uuid UuidGenerator::Generate()
	{
		Uuid uuid;

		for (auto& dword : uuid)
			dword = m_uint32(m_randEngine);

		uuid.SetVersion(Uuid::Version());
		uuid.SetVariant(Uuid::Variant());
		return uuid;
	}
}
