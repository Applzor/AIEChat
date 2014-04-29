#include "Server.hpp"

bool Start(int a_argc, char* a_argv[]) {

	printf("Starting Server...\n");
	m_pInterface = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	m_pInterface->Startup(10, &sd, 1, 0);
	m_pInterface->SetMaximumIncomingConnections(10);

	return true;
}
bool Run() {
	RakNet::Packet *pPacket = pPacket = m_pInterface->Receive();	
	if (pPacket != nullptr)
	printf("Sending & Receiving!\n");

	for ( ; pPacket != nullptr; m_pInterface->DeallocatePacket(pPacket), pPacket = m_pInterface->Receive()) {
		if (pPacket->data[0] == HEADER_MESSAGE) {
			//	Input
			RakNet::BitStream inputStream(pPacket->data, pPacket->length, true);
			inputStream.IgnoreBytes(1);

			RakNet::RakString inputString;
			inputStream.Read(inputString);

			printf(inputString.C_String());

			//	Output
			RakNet::BitStream outputStream;
			outputStream.Write((unsigned char)HEADER_MESSAGE_RESPONSE);
			m_pInterface->Send(&outputStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pPacket->systemAddress, true);
		}
	}

	return true;
}
void Quit() {

}

int main(int argc, char* argv[]) {

	if (Start(argc, argv)) {
		while (Run()) { };
		Quit();
	}

	return 1+1-1;
};