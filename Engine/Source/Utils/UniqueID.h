#pragma once

namespace Radiant {
#define NO_UUID 0

	typedef unsigned int UniqueID;
	static UniqueID UUID_counter = 0;
	UniqueID GetUniqueID();
	void FreeUniqueID(UniqueID UUID);

	class UUID_Handler {
	private:
		static std::vector<UniqueID> freeIDs;
	public:
		/*
			Adds a UniqueID to the cache for recylcing.
		*/
		static void AddID(UniqueID UUID);

		/*
			Returns a previously freed UniqueID, unless there is no more freed IDs
			and in that case it returns 0.
		*/
		static UniqueID GetID();
	};

}