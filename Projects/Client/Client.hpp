#pragma once

#include <iostream>
#include <string>

#include <RakNet\RakPeerInterface.h>

#include <Defines.hpp>

RakNet::RakPeerInterface *pInterface;
RakNet::SystemAddress saServerAddress;

std::string sServerAddress;
std::string sServerPort;
std::string sUsername;
std::string sIPAddressLocal;
std::string sIPAddressOnline;

void Chat();

void MessageReceive(RakNet::Packet *pPacket);
void MessageSend(char* cs_Message);

bool ClientConnect();
void ClientReceive();