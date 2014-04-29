#include "Client.hpp"

#include <conio.h>
#include <string>

bool Connect();
void Chat();

void ReceiveMessages();
void SendMessage(char* cs_Message);

bool Start(int a_argc, char* a_argv[]) {

	printf("Starting AIEChat Client...\n");
	m_pInterface = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	m_pInterface->Startup(1, &sd, 1, 0);	
	return true;
}

bool Run() {
	
	//	Clear the Screen
	printf("--------------------------\n");
	printf("What would you like to do?\n");
	printf("[1] Connect to a Server\n");
	printf("[2] Chat\n");
	printf("[0] Exit\n");

	//	Get Input
	char input;
	std::cin >> input;
	std::cin.clear(); std::cin.sync();
	system("CLS");

	//	Determine Case
	switch(input) {
	case '1':	Connect(); return true;
	case '2':	Chat(); return true;
	case '0':	return false;
	default:	printf("Invalid Choice!\n\n"); return true;
	}
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

bool Connect() {

	bool retry;

	do {
		char serverAddress[32];
		//	Determine Server to Connect to
		printf("Enter Server IP Address: ");
		std::cin >> serverAddress;

		printf("Connecting to Server at %s\n", serverAddress);
		m_pInterface->Connect(serverAddress, SERVER_PORT, 0, 0);

		RakNet::Packet *pPacket = nullptr;
		double timer = 0;
		while(timer < 2.0) {
			timer += 0.00000005f;

			pPacket = m_pInterface->Receive();

			if (pPacket != nullptr) {
				if (pPacket->data[0] == ID_CONNECTION_REQUEST_ACCEPTED) {
					m_ServerAddress = pPacket->systemAddress;
					printf("Connected to Server!\n");
					return true;
				}
			}
		}

		//	Couldn't Connect
		printf("Could not connect to Server...\n");
		printf("Connection attempt timed out!\n");

		char input;
		do {
		
			printf("Would you like to try again?\n");
			printf("[1] Yes\n");
			printf("[2] No\n");

			//	Get Input
			std::cin >> input;
			system("CLS");

			switch(input) {
				case '1': retry = true; break;
				case '2': retry = false; break;
				default:	printf("Invalid Choice!\n\n"); break;
			}
		} while (input != '1' && input != '2');

	} while(retry);
	return false;
}

void Chat() {
	
	printf("Press Tab to Chat\n");
	printf("Press ESC to Return to the Menu\n");

	char input = 0;
	std::string buffer;
	do {
		if (kbhit()) {
			input = getch();
			if (input == 9) {
				printf("->");
				buffer.clear();				
				std::getline(std::cin, buffer);
				std::cin.clear(); std::cin.sync();
				SendMessage((char*)buffer.c_str());
			}
		}	

		//	Check for any messages from server
		ReceiveMessages();

	} while (input != 27);
}

void SendMessage(char* cs_Message) {

	if (stricmp(cs_Message, "") == 0)
		return;

	//	Output	
	RakNet::BitStream outputStream;
	outputStream.Write((unsigned char)HEADER_MESSAGE);
	outputStream.Write(cs_Message);
	m_pInterface->Send(&outputStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_ServerAddress, false);
}

void ReceiveMessages() {
	// Get Messages from Server
	RakNet::Packet *pPacket = nullptr;
	for (pPacket = m_pInterface->Receive(); pPacket != nullptr; m_pInterface->DeallocatePacket(pPacket), pPacket = m_pInterface->Receive()) {
		if (pPacket->data[0] == HEADER_MESSAGE_RESPONSE) {
			printf("Received message from other client!\n");
		}
	}
}