#pragma once

#include <iostream>

#include <RakNet\RakPeerInterface.h>

#include <Defines.hpp>

RakNet::RakPeerInterface *m_pInterface;

bool Start(int a_argc, char* a_argv[]);
bool Run();
void Quit();