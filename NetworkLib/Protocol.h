#pragma once

#include "PrimitiveTypes.h"

#pragma pack(push, 1)
struct PacketHeader
{
	unsigned short mPacketSize = 0;
	unsigned short mPacketId = 0;
};
#pragma pack(pop)

constexpr uint16 MAX_PACKET_BODY_SIZE = 1024;
constexpr uint16 PACKET_HEADER_SIZE = sizeof(PacketHeader);

struct ReceivePacket
{
	int32 mSessionIndex = -1;
	uint64 mSessionUniqueId = 0;
	uint16 mPacketId = 0;
	uint16 mBodyDataSize = 0;
	char* mBodyData = nullptr;
};