#pragma once

#include <vector>
#include <queue>

#include "../../NetworkLib/PrimitiveTypes.h"
#include "ErrorCode.h"


class User;


class UserManager
{
private:
	std::vector<User> mUserPool;
	std::queue<int32> mUserIndexPool;

	int mMaxUserNum = 0;


public:
	UserManager() = default;
	~UserManager() = default;


private:
	int32 AllocUserIndex();

	
public:
	CS::ErrorCode Init(const int maxUserNum);

	CS::ErrorCode Connect(User& user);
	CS::ErrorCode Disconnect(User& user);
};
