#include "Client.hpp"

#include <conio.h>


bool Start(int a_argc, char* a_argv[]) {

	printf("Starting AIEChat Client...\n");
	pInterface = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	pInterface->Startup(1, &sd, 1, 0);	

	//	Determine what is the local IP
	for (int i = 0; i < pInterface->GetNumberOfAddresses(); i++) {
		std::string ip = pInterface->GetLocalIP(i);
		if (pInterface->IsLocalIP(ip.c_str()))
			sIPAddressLocal = ip;
	}

	system("CLS");
	return true;
}

bool Run() {
	
	//	Clear the Screen
	printf("-----------------------\n");
	printf("[1] Connect to a Server\n");
	printf("[2] Enter Chat Room\n");
	printf("[X] Exit\n");
	printf("-----------------------\n");

	//	Get Input
	char input;
	std::cin >> input;
	std::cin.clear(); std::cin.sync();
	system("CLS");

	//	Determine Case
	switch(input) {
	case '1':	ClientConnect(); return true;
	case '2':	Chat(); return true;
	case 'X': case 'x':	return false;
	default:	printf("Invalid Choice!\n"); return true;
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

void Chat() {
	
	if (sServerAddress.size() == 0) {
		printf("Not Connected to a Server...\n");
		return;
	}

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
				MessageSend((char*)buffer.c_str());
			}
		}	

		//	Check for any packets from server
		ClientReceive();

	} while (input != 27);
}

void MessageReceive(RakNet::Packet *pPacket) {
	
	//	Input
	RakNet::BitStream inputStream(pPacket->data, pPacket->length, true);
	inputStream.IgnoreBytes(1);

	RakNet::RakString inputString;
	inputStream.Read(inputString);

	printf(inputString.C_String());
}
void MessageSend(char* cs_Message) {

	if (stricmp(cs_Message, "") == 0)
		return;

	//	Output	
	RakNet::BitStream outputStream;
	outputStream.Write((unsigned char)HEADER_MESSAGE);
	outputStream.Write(cs_Message);
	pInterface->Send(&outputStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, saServerAddress, false);
}

bool ClientConnect() {

	bool retry;

	do {
		//	Determine Server to Connect to
		printf("Enter Server IP Address: ");
		std::getline(std::cin, sServerAddress);
		std::cin.clear(); std::cin.sync();

		printf("Connecting to Server at %s\n", sServerAddress);
		pInterface->Connect(sServerAddress.c_str(), SERVER_PORT, 0, 0);

		RakNet::Packet *pPacket = nullptr;
		double timer = 0;
		while(timer < 2.0) {
			timer += 0.00000005f;

			pPacket = pInterface->Receive();

			if (pPacket != nullptr) {
				if (pPacket->data[0] == ID_CONNECTION_REQUEST_ACCEPTED) {
					saServerAddress = pPacket->systemAddress;
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
void ClientReceive() {
	// Get packets from Server
	RakNet::Packet *pPacket = nullptr;
	for (pPacket = pInterface->Receive(); pPacket != nullptr; pInterface->DeallocatePacket(pPacket), pPacket = pInterface->Receive()) {
		switch (pPacket->data[0]) {
			case HEADER_MESSAGE: MessageReceive(pPacket); break;
		}
	}
}
