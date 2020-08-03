#pragma once

#include "PrimitiveTypes.h"


namespace Core {
	enum class ErrorCode : uint16
	{
		SUCCESS = 0,

		WSA_START_UP_FAIL,

		SOCKET_INIT_FAIL,
		SOCKET_INIT_REUSE_ADDR_FAIL,
		SOCKET_BIND_FAIL,
		SOCKET_LISTEN_FAIL,
		SOCKET_SET_OPTION_FAIL,
		SOCKET_SELECT_FAIL,
		SOCKET_SELECT_RESULT_ZERO,
		SOCKET_ACCEPT_CLIENT_FAIL,
		SOCKET_SET_FIONBIO_FAIL,
		SOCKET_RECEIVE_ZERO,
		SOCKET_RECEIVE_FAIL,
		SOCKET_RECEIVE_MAX_PACKET_SIZE,
		SOCKET_SEND_SIZE_ZERO,
		SOCKET_INDEX_POOL_IS_FULL,

		CLIENT_SESSION_NOT_CONNECTED,
		CLIENT_SESSION_SEND_BUFFER_IS_FULL,
	};
}
