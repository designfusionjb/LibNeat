#include "Neat\Uuid.h"

#include <random>

namespace Neat
{
	Uuid::Uuid()
	{
		Allocate(SizeInBytes());
	}

	Uuid::Uuid(Buffer&& other) :
		Base(std::move(other))
	{
	}

	Uuid::Uuid(const Buffer& other) :
		Base(other)
	{
	}

	Uuid Uuid::Generate()
	{
		Uuid uuid;

		{
			std::random_device randDevice;
			std::default_random_engine randEngine(randDevice());
			std::uniform_int_distribution<uint16_t> uint16;

			for (auto& word : uuid)
				word = uint16(randEngine);
		}
		
		uuid.SetVersion(Uuid::Version());
		uuid.SetVariant(Uuid::Variant());
		return uuid;
	}
}
