#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include <functional>

#include "ErrorCode.h"


struct redisContext;


namespace NetworkLib
{
	struct CommandResult
	{
		const char* mResult = "";
		ErrorCode mErrorCode = ErrorCode::SUCCESS;
	};

	struct CommandRequest
	{
		const char* mCommand = nullptr; // TODO ������: �� �ʿ��� Ÿ���� ������? -> ����, command�� arguments�� �и���Ű�� �۾�
		std::function<void(const CommandResult&)> mCallBackFunc = nullptr;
	};


	class RedisManager
	{
	private:
		redisContext* mConnection = nullptr;

		std::unique_ptr<std::thread> mThread = nullptr;
		std::mutex mMutex;;

		std::queue<CommandRequest> mRequestQueue;


	public:
		RedisManager() = default;
		~RedisManager();


	private:
		void ExecuteCommandProcess();


	public:
		ErrorCode Connect(const char* ipAddress, const int portNum);
		void Disconnect();

		void ExecuteCommand(const CommandRequest& commandRequest);
		CommandResult ExecuteCommandSync(const CommandRequest& commandRequest);
	};

	//TODO: ����� �� Ŭ������ ����ϴ� ���� ���� �ʽ��ϴ�. std::function ���� ����ؼ� �۷ι� ������ �������� �ʵ��� �ϴ� ���� ��õ�մϴ�.
	// ���� �Ϸ�
}
