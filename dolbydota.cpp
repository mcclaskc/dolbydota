#include <axonapi.h>
#include <pcap.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <cstdlib>


using namespace std;

// Global variables

//Dolby API Client
DADA* D = DADA_Create();
DADA_STATUS DS;
DADA_CHATID CID;

void printStatus(DADA_STATUS status);
void move(DADA_CHATID cid);
DADA_CHATID selectChat();
void initialize();

int main()
{
	initialize();
	CID = selectChat();
	while(true)
	{
		//logic to check if in a chat
		move(CID);
		Sleep(500);
	}
	DADA_Destroy(D);
    return 0;
}

void initialize()
{
	DADA_GetStatus(D, &DS);
	DADA_STATUS PS = DS;
	do
	{
		DADA_GetStatus(D, &DS);
		if (DS != PS) 
		{
			printStatus(DS);
			PS = DS;
		}
	} while (DS != DADA_STATUS_NORMAL);
}


void printStatus(DADA_STATUS status)
{
	switch(status)
	{
	case DADA_STATUS_NORMAL:				//Connection is working
		cout << "Connected to Dolby Axon!" << endl;
		break;
	case DADA_STATUS_NOT_SIGNED_IN:		//Axon client is not signed in
		cout << "Dolby Axon Found - Please Sign in." << endl;
		break;
	default:
		cout << "Searching for Dolby Axon..." << endl;
		break;
	}
}

void move(DADA_CHATID cid)
{
	DADA_POSITION p;
	p.x = rand() % 100;
	p.y = rand() % 100;
	p.rotation = rand() % 180;
	cout << "moved to: (" << p.x << "," << p.y << ") and facing: " << p.rotation << " deg." << endl;
	DADA_Move(D, cid, p);
}

DADA_CHATID selectChat()
{
	DADA_CHATS chats;
	DADA_GetChats(D, &chats);
	DADA_CHATINFO chat_info;
	for (size_t i = 0; i < chats.ids_len; ++i)
	{
		DADA_GetChatInfo(D, chats.ids[i], &chat_info);
		cout << i << ":  " << chat_info.title << endl;
	}
	int id;
	cout << "type the id number of the chat you wish to join: ";
	cin >> id;
	DADA_JoinChat(D, chats.ids[id]);
	DADA_GetChatInfo(D, chats.ids[id], &chat_info);
	return chat_info.id;
}