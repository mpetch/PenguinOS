// https://academy.cba.mit.edu/classes/networking_communications/SD/FAT.pdf

#pragma once

#include <bitmask.h>
#include <cstring.h>
#include <ide.h>
#include <string.h>
#include <sys.h>

// Maximum number of characters in a file name
#define MAX_FILENAME 256
// Byte size of the MBR (Boot Sector)
#define MBR_BYTE_SIZE 446
// Byte size of a FAT entry (short or long)
#define FAT_ENTRY_SIZE 32
#define FAT_END_OF_CLUSTER 0x0FFFFFF8
#define FAT_ENTRIES_PER_SECTOR 16

namespace FAT32
{
	enum class FATType
	{
		ExFAT,
		FAT12,
		FAT16,
		FAT32
	};

	enum class FATEntryType
	{
		Free,
		Allocated,
		Reserved,
		Defective,
		EOF
	};

	enum class FATAttribute : uint8_t
	{
		Empty = 0,
		ReadOnly = (1 << 0),
		Hidden = (1 << 1),
		System = (1 << 2),
		VolumeID = (1 << 3),
		LongFileName = 0x0F,
		Directory = (1 << 4),
		Archive = (1 << 5),
		LastEntry = 0x41,
		Deleted = 0xE5,
	};
	DEFINE_BITMASK_OPERATORS(FATAttribute);

	struct FATShortEntry // 32 bytes
	{
		uint8_t		 name[8];
		uint8_t		 ext[3];
		FATAttribute attribute;
		uint8_t		 ntRes;
		uint8_t		 creationTimeSeconds;
		uint16_t	 creationTime;
		uint16_t	 creationDate;
		uint16_t	 lastAccDate;
		uint16_t	 clusterHigh;
		uint16_t	 writeTime;
		uint16_t	 writeDate;
		uint16_t	 clusterLow;
		uint32_t	 fileSize = 0;

		bool isValid() const
		{
			return name[0] != FATAttribute::Empty && name[0] != FATAttribute::Deleted;
		}
		// Returns the full 32-bit cluster number composed of the low 16 bits
		// of `clusterLow` and high 16 bits of `clusterHigh`.
		uint32_t cluster() const { return clusterLow | (clusterHigh << 16); }
	} __attribute__((packed));

	struct FATLongEntry // 32 bytes
	{
		uint8_t	 id;
		uint8_t	 data0[10];
		uint8_t	 attr; // Should always be 0x0F
		uint8_t	 longEntryType;
		uint8_t	 checksum;
		uint8_t	 data1[12];
		uint16_t always0;
		uint8_t	 data2[4];

		bool isLast() const { return (id & 0x40) == 0x40; }

	} __attribute__((packed));

	bool openFile(const String& filename, void* file);

	String toShortName(const String& longName);
	String sanitize(const String& longName);
	bool   isValidChar(char c);

	bool findEntry(uint32_t startCluster, const String& name, FATShortEntry* entry);
	bool isLongEntry(uint8_t* buffer);
	void parseLongEntry(FATLongEntry* entry, uint32_t count, char* filename);

	uint32_t getNextCluster(uint32_t cluster);
	uint32_t getClusterSector(uint32_t n);
	uint32_t getClusterCount();
	uint32_t getSize();

} // namespace FAT32