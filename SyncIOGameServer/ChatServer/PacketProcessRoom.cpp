#include "../../NetworkLib/Network.h"
#include "../../NetworkLib/Logger.h"
#include "PacketHandler.h"
#include "RoomManager.h"
#include "Room.h"
#include "UserManager.h"
#include "User.h"


using namespace CS;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode PacketHandler::RoomEnter(const Packet& packet)
{
	RoomEnterResponse response;
	response.mErrorCode = ErrorCode::SUCCESS;
	response.mUserUniqueId = packet.mSessionUniqueId;

	User* user = mUserManager->FindUser(packet.mSessionUniqueId);
	if (nullptr == user)
	{
		response.mErrorCode = ErrorCode::USER_NOT_CONNECTED_STATE;
		mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::ROOM_ENTER_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));
		return response.mErrorCode;
	}

	RoomEnterRequest* request = reinterpret_cast<RoomEnterRequest*>(packet.mBodyData);

	Room* room = mRoomManager->FindRoom(request->mRoomIndex);
	if (nullptr == room)
	{
		response.mErrorCode = ErrorCode::ROOM_NOT_EXIST;
		mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::ROOM_ENTER_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));
		return response.mErrorCode;
	}

	ErrorCode errorCode = room->Enter(*user);
	if (ErrorCode::SUCCESS != errorCode)
	{
		response.mErrorCode = errorCode;
		mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::ROOM_ENTER_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));
		return response.mErrorCode;
	}
	mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::ROOM_ENTER_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));

	auto roomUserDeque = room->UserDeque();
	uint16 roomUserCount = static_cast<uint16>(roomUserDeque.size());
	RoomUserListNotify roomUserListNotify;
	roomUserListNotify.mUserCount = roomUserCount;

	// TODO ������: �ǵ�� ���� ������ �׽�Ʈ �ʿ�.
	roomUserListNotify.mUserUniqueIdList = new uint64[roomUserCount];
	roomUserListNotify.mUserIdList = new char*[roomUserCount];
	for (int i = 0; i < roomUserCount; ++i)
	{
		User& roomUser = roomUserDeque[i];
		roomUserListNotify.mUserUniqueIdList[i] = roomUser.mSessionUniqueId;

		roomUserListNotify.mUserIdList[i] = new char[MAX_USER_ID_SIZE];
		memcpy_s(roomUserListNotify.mUserIdList, MAX_USER_ID_SIZE, roomUserDeque[i].mUserId, MAX_USER_ID_SIZE);
	}
	mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::ROOM_USER_LIST_NOTIFY), reinterpret_cast<char*>(&roomUserListNotify), roomUserListNotify.Size());

	RoomNewUserBroadcast newUserBroadcast;
	newUserBroadcast.mUserUniueId = user->mSessionUniqueId;
	memcpy_s(newUserBroadcast.mUserId, MAX_USER_ID_SIZE, user->mUserId, MAX_USER_ID_SIZE);
	mNetwork->Broadcast(static_cast<uint16>(PacketId::ROOM_NEW_USER_BROADCAST), reinterpret_cast<char*>(&newUserBroadcast), sizeof(newUserBroadcast), 1, user->mSessionUniqueId);

	for (int i = 0; i < roomUserCount; ++i)
	{
		delete[] roomUserListNotify.mUserIdList[i];
	}
	delete[] roomUserListNotify.mUserIdList;

	return response.mErrorCode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode PacketHandler::RoomLeave(const Packet& packet)
{
	RoomLeaveResponse response;
	response.mErrorCode = ErrorCode::SUCCESS;

	User* user = mUserManager->FindUser(packet.mSessionUniqueId);
	if (nullptr == user)
	{
		response.mErrorCode = ErrorCode::USER_NOT_CONNECTED_STATE;
		mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::ROOM_LEAVE_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));
		return response.mErrorCode;
	}

	Room* room = mRoomManager->FindRoom(user->mRoomIndex);
	if (nullptr == room)
	{
		response.mErrorCode = ErrorCode::ROOM_NOT_EXIST;
		mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::ROOM_LEAVE_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));
		return response.mErrorCode;
	}

	room->Leave(*user);

	mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::ROOM_ENTER_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));

	RoomLeaveUserBroadcast leaveUserBroadcast;
	leaveUserBroadcast.mUserUniqueId = user->mSessionUniqueId;
	mNetwork->Broadcast(static_cast<uint16>(PacketId::ROOM_NEW_USER_BROADCAST), reinterpret_cast<char*>(&leaveUserBroadcast), sizeof(leaveUserBroadcast), 1, user->mSessionUniqueId);

	return response.mErrorCode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode PacketHandler::RoomChat(const Packet& packet)
{
	return ErrorCode::SUCCESS;
}