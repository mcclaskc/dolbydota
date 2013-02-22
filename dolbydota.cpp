#include <stdlib.h>
#include <string>
#include <axonapi.h>
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
DADA_CHATID selectChat(string name);

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
	string name;
	cout << "Type the name of the chat: ";
	cin >> name;
	CID = selectChat(name);
	
}

int main()
{
	initialize();
	while(true)
	{
		move(CID);
		Sleep(500);
	}
	DADA_Destroy(D);
    return 0;
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

DADA_CHATID selectChat(string name)
{
	DADA_CHATS chats;
	DADA_GetChats(D, &chats);
	for (size_t i = 0; i < chats.ids_len; ++i)
	{
		DADA_CHATINFO chat_info;
		DADA_GetChatInfo(D, chats.ids[i], &chat_info);
		if (chat_info.title == name) 
		{
			cout << chat_info.title << endl;
			DADA_JoinChat(D, chats.ids[i]);
			return chat_info.id;
		}
	}
}