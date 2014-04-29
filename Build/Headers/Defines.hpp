#pragma once

#include <RakNet\MessageIdentifiers.h>
#include <RakNet\BitStream.h>

enum MESSAGE_HEADER {
	HEADER_MESSAGE				= ID_USER_PACKET_ENUM+1,
	HEADER_MESSAGE_RESPONSE		= ID_USER_PACKET_ENUM+2,
};

#define SERVER_PORT 12001	// MAX 65000 MIN 1000