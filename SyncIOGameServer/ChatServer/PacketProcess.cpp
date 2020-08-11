#include "../../NetworkLib/Network.h"
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

	//TODO �����: Redis�� ��û�ϰ� �亯 �����鼭 �ְ� �޴� ���� ��Ŷ ������ó�� �����Ͻø� ���������� �����͸� �ְ� ���� �� �ֽ��ϴ�. ����ó�� std::function�� �Ź� �����ϴ� ���� ���������� �������ϴ�.
	// ����: https://docs.google.com/presentation/d/16DgIURxfR9jgHjLX7fCwruHT-vwm90BG1OkQVdE0j9A/edit?usp=sharing
	// ����Ϸ�
#ifndef _DEBUG
	NetworkLib::Redis::CommandRequest commandRequest;
	commandRequest.mCommandType = NetworkLib::Redis::CommandType::GET;
	NetworkLib::Redis::Get get;
	strcpy_s(get.mKey, CS::RedisLoginKey(request->mUserId));
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