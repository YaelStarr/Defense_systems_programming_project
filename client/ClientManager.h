#pragma once
#include "transfer_for_client.h"
#include "Client.h"
#include "handlingRequest.h"
#include <cstring>
#include <string.h>
#include <exception>

constexpr unsigned int ID_LENGTH = 16;

class ClientManager
{
public:
	static void interaction_between_server_client(Client client, transfer_for_client transfer);
	static bool exists_meInfo();
};

