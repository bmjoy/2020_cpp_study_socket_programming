#pragma once

#include "PrimitiveTypes.h"


namespace Core
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
	struct PacketHeader
	{
		uint16 mPacketSize = 0;
		uint16 mPacketId = 0;
	};
#pragma pack(pop)

	//TODO �����: �Ʒ��� ���� ��Ȳ�� ���� ���� �� �ִ� ���� ���� ������ Json���� �����, json�� �о ���� ����ϵ��� �մϴ�.	
	constexpr uint16 MAX_PACKET_BODY_SIZE = 1024;
	constexpr uint16 PACKET_HEADER_SIZE = sizeof(PacketHeader);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct ReceivePacket
	{
		int32 mSessionIndex = -1;
		uint64 mSessionUniqueId = 0;
		uint16 mPacketId = 0;
		uint16 mBodyDataSize = 0;
		char* mBodyData = nullptr;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TODO �����: �н��� ���ؼ� ����� ���̶�� �����մϴ�. �ٸ� �Ʒ��� ���� ����� Ư���� ������ ���ٸ� const�� ����մϴ�.
	constexpr static int PACKET_ID_START = 10;
	constexpr static int PACKET_ID_END = 50;

	enum class PacketId
	{
		PACKET_START = PACKET_ID_START,
		//////////////////////////////////////////////////////////////
		CONNECT,
		DISCONNECT,

		//////////////////////////////////////////////////////////////
		PACKET_END = PACKET_ID_END,
	};
}