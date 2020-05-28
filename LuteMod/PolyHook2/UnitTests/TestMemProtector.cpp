#include "Catch.hpp"
#include "headers/MemProtector.hpp"

TEST_CASE("Test protflag translation", "[MemProtector],[Enums]") {
	SECTION("flags to native") {
		REQUIRE(PLH::TranslateProtection(PLH::ProtFlag::X) == PAGE_EXECUTE);
		REQUIRE(PLH::TranslateProtection(PLH::ProtFlag::R) == PAGE_READONLY);
		REQUIRE(PLH::TranslateProtection(PLH::ProtFlag::W) == PAGE_READWRITE);
		REQUIRE(PLH::TranslateProtection(PLH::ProtFlag::R | PLH::ProtFlag::W) == PAGE_READWRITE);
		REQUIRE(PLH::TranslateProtection(PLH::ProtFlag::X | PLH::ProtFlag::R) == PAGE_EXECUTE_READ);
		REQUIRE(PLH::TranslateProtection(PLH::ProtFlag::X | PLH::ProtFlag::W) == PAGE_EXECUTE_READWRITE);
		REQUIRE(PLH::TranslateProtection(PLH::ProtFlag::X | PLH::ProtFlag::W || PLH::ProtFlag::R) == PAGE_EXECUTE_READWRITE);
		REQUIRE(PLH::TranslateProtection(PLH::ProtFlag::NONE) == PAGE_NOACCESS);
	}

	SECTION("native to flags") {
		REQUIRE(PLH::TranslateProtection(PAGE_EXECUTE) == PLH::ProtFlag::X);
		REQUIRE(PLH::TranslateProtection(PAGE_READONLY) == PLH::ProtFlag::R);
		REQUIRE(PLH::TranslateProtection(PAGE_READWRITE) == (PLH::ProtFlag::W | PLH::ProtFlag::R));
		REQUIRE(PLH::TranslateProtection(PAGE_EXECUTE_READ) == (PLH::ProtFlag::X | PLH::ProtFlag::R));
		REQUIRE(PLH::TranslateProtection(PAGE_EXECUTE_READWRITE) == (PLH::ProtFlag::X | PLH::ProtFlag::W | PLH::ProtFlag::R));
		REQUIRE(PLH::TranslateProtection(PAGE_NOACCESS) == PLH::ProtFlag::NONE);
	}
}

TEST_CASE("Test setting page protections", "[MemProtector]") {
	char* page = (char*)VirtualAlloc(0, 4 * 1024, MEM_COMMIT, PAGE_NOACCESS);
	bool isGood = page != nullptr; // indirection because catch reads var, causing access violation
	REQUIRE(isGood);

	{
		PLH::MemoryProtector prot((uint64_t)page, 4 * 1024, PLH::ProtFlag::R);
		REQUIRE(prot.isGood());
		REQUIRE(prot.originalProt() == PLH::ProtFlag::NONE);

		PLH::MemoryProtector prot1((uint64_t)page, 4 * 1024, PLH::ProtFlag::W);
		REQUIRE(prot1.isGood());
		REQUIRE(prot1.originalProt() == PLH::ProtFlag::R);

		PLH::MemoryProtector prot2((uint64_t)page, 4 * 1024, PLH::ProtFlag::X);
		REQUIRE(prot2.isGood());
		REQUIRE((prot2.originalProt() & PLH::ProtFlag::W));
	}

	// protection should now be NOACCESS if destructors worked
	{
		PLH::MemoryProtector prot((uint64_t)page, 4 * 1024, PLH::ProtFlag::X | PLH::ProtFlag::R);
		REQUIRE(prot.isGood());
		REQUIRE(prot.originalProt() == PLH::ProtFlag::NONE);

		PLH::MemoryProtector prot1((uint64_t)page, 4 * 1024, PLH::ProtFlag::X | PLH::ProtFlag::W);
		REQUIRE(prot.isGood());
		REQUIRE((prot1.originalProt() == (PLH::ProtFlag::X | PLH::ProtFlag::R)));

		PLH::MemoryProtector prot2((uint64_t)page, 4 * 1024, PLH::ProtFlag::X | PLH::ProtFlag::R | PLH::ProtFlag::W);
		REQUIRE(prot.isGood());
		REQUIRE(prot2.originalProt() == (PLH::ProtFlag::X | PLH::ProtFlag::R | PLH::ProtFlag::W));
	}
	VirtualFree(page, 4 * 1024, MEM_RELEASE);
}
