﻿#include "../../NetworkLib/Network.h"
#include "../../NetworkLib/Logger.h"
#include "../../NetworkLib/RedisManager.h"
#include "PacketHandler.h"
#include "UserManager.h"
#include "User.h"


using namespace CS;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode PacketHandler::Connect(const Packet& packet)
{
	User newUser{ packet.mSessionIndex, packet.mSessionUniqueId };

	ErrorCode errorCode = mUserManager->Connect(newUser);
	if (ErrorCode::SUCCESS != errorCode)
	{
		return errorCode;
	}

	GLogger->PrintConsole(Color::LGREEN, L"<Connect> User: %lu\n", packet.mSessionUniqueId);

	return ErrorCode::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode PacketHandler::Disconnect(const Packet& packet)
{
	User* user = mUserManager->FindUser(packet.mSessionUniqueId);
	if (nullptr != user)
	{
		mUserManager->Disconnect(user->mIndex);
	}

	GLogger->PrintConsole(Color::LGREEN, L"<Disconnect> User: %lu\n", packet.mSessionUniqueId);

	return ErrorCode::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode PacketHandler::Login(const Packet& packet)
{
	User* user = mUserManager->FindUser(packet.mSessionUniqueId);
	if (nullptr == user)
	{
		LoginResponse response;
		response.mErrorCode = ErrorCode::USER_IS_INVALID;
		mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::LOGIN_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));
		return response.mErrorCode;
	}

	LoginRequest* request = reinterpret_cast<LoginRequest*>(packet.mBodyData);
	LoginResponse response;

	//TODO 최흥배: Redis로 요청하고 답변 받으면서 주고 받는 것을 패킷 데이터처럼 구성하시면 범용적으로 데이터를 주고 받을 수 있습니다. 지금처럼 std::function을 매번 대입하는 것은 성능적으로 나빠집니다.
	// 참고: https://docs.google.com/presentation/d/16DgIURxfR9jgHjLX7fCwruHT-vwm90BG1OkQVdE0j9A/edit?usp=sharing
	// 적용완료
	//TODO 최진우: 여러개의 스레드에서 GetCommandResult 호출 시 받아가는 결과 값이 꼬일 수 있다 -> 추후 개선
#ifndef _DEBUG
	NetworkLib::Redis::CommandRequest commandRequest;
	commandRequest.mCommandType = NetworkLib::Redis::CommandType::GET;
	NetworkLib::Redis::Get get;
	strcpy_s(get.mKey, CS::RedisLoginKey(request->mUserId).c_str());
	commandRequest.mCommandBody = reinterpret_cast<char*>(&get);
	commandRequest.mCommandBodySize = sizeof(get);
	mRedisManager->ExecuteCommandAsync(commandRequest);
	NetworkLib::Redis::CommandResponse commandResponse = mRedisManager->GetCommandResult();
	
	if (NetworkLib::ErrorCode::SUCCESS != commandResponse.mErrorCode)
	{
		response.mErrorCode = ErrorCode::USER_LOGIN_AUTH_FAIL;
		mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::LOGIN_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));
		return response.mErrorCode;
	}
	if (0 != std::strncmp(request->mAuthKey, commandResponse.mResult.c_str(), AUTH_KEY_SIZE))
	{
		response.mErrorCode = ErrorCode::USER_LOGIN_AUTH_FAIL;
		mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::LOGIN_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));
		return response.mErrorCode;
}
#endif

	response.mErrorCode = mUserManager->Login(packet.mSessionUniqueId, request->mUserId);
	mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::LOGIN_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));

	GLogger->PrintConsole(Color::LGREEN, L"<Login> User: %lu ErrorCode: %d\n", packet.mSessionUniqueId, static_cast<int>(response.mErrorCode));

	return ErrorCode::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode PacketHandler::Chat(const Packet& packet)
{
	ChatRequest* request = reinterpret_cast<ChatRequest*>(packet.mBodyData);

	ChatBroadcast broadcast;
	broadcast.mUid = packet.mSessionUniqueId;
	broadcast.mMessageLen = request->mMessageLen;
	wmemcpy_s(broadcast.mMessage, request->mMessageLen, request->mMessage, request->mMessageLen);
	mNetwork->Broadcast(static_cast<uint16>(PacketId::CHAT_BROADCAST), reinterpret_cast<char*>(&broadcast), sizeof(broadcast) - MAX_CHAT_SIZE - broadcast.mMessageLen);

	GLogger->PrintConsole(Color::LGREEN, L"<Chat> [%lu]: %ls\n", 5, broadcast.mMessage);

	ChatResponse response;
	response.mErrorCode = ErrorCode::SUCCESS;
	mNetwork->Send(packet.mSessionIndex, static_cast<uint16>(PacketId::CHAT_RESPONSE), reinterpret_cast<char*>(&response), sizeof(response));

	return ErrorCode::SUCCESS;
}