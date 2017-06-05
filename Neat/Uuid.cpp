#include "Neat\Uuid.h"

namespace Neat
{
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
