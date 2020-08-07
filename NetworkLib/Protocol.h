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

	struct ReceivePacket
	{
		int32 mSessionIndex = -1;
		uint64 mSessionUniqueId = 0;
		uint16 mPacketId = 0;
		uint16 mBodyDataSize = 0;
		char* mBodyData = nullptr;
	};

	//TODO 최흥배 아래 처럼 ///////////////////////////////////// 이런 주석을 다는 이유가 특별한 이유가 없다면 삭제하는 것이 좋습니다
	// 적용 완료 (RAON 서버 코딩 스타일에서, cpp 파일의 함수 구현체부분은 주석으로 함수마다 구분을 해둬서 해당 부분은 남겨놓겠습니다)
	const uint16 PACKET_ID_START = 10;
	const uint16 PACKET_ID_END = 50;

	//TODO 최흥배. enum의 요소 마다 숫자를 다 지정하세요. 지금처럼 하면 개수가 늘어나면 한번에 어떤 숫자 번호가 지정되었는지 알수 없습니다.
	// 적용 완료
	enum class PacketId : uint16
	{
		PACKET_START = PACKET_ID_START,

		CONNECT = 11,
		DISCONNECT = 12,

		PACKET_END = PACKET_ID_END,
	};
}