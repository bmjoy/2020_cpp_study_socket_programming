#pragma once

#include "PrimitiveTypes.h"


namespace NetworkLib
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
	struct PacketHeader
	{
		uint16 mPacketSize = 0;
		uint16 mPacketId = 0;
	};
#pragma pack(pop)

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

	//TODO ����� �Ʒ� ó�� ///////////////////////////////////// �̷� �ּ��� �ٴ� ������ Ư���� ������ ���ٸ� �����ϴ� ���� �����ϴ�
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const uint16 PACKET_ID_START = 10;
	const uint16 PACKET_ID_END = 50;

	//TODO �����. enum�� ��� ���� ���ڸ� �� �����ϼ���. ����ó�� �ϸ� ������ �þ�� �ѹ��� � ���� ��ȣ�� �����Ǿ����� �˼� �����ϴ�.
	enum class PacketId : uint16
	{
		PACKET_START = PACKET_ID_START,
		//////////////////////////////////////////////////////////////
		CONNECT,
		DISCONNECT,

		//////////////////////////////////////////////////////////////
		PACKET_END = PACKET_ID_END,
	};
}