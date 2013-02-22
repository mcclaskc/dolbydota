#ifndef _DADA_API_H_
#define _DADA_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
	#define DADA_EXPORT __declspec(dllexport)
#else
	#define DADA_EXPORT
#endif
	
// All char*'s are utf8 format

#include <cstddef>
	
//Constants
const size_t DADA_MAX_TITLE_BYTES = 512;
const size_t DADA_MAX_NAME_BYTES = 128;

//Chat ID. Globally unique.
typedef struct _DADA_CHATID
{
	unsigned int id_part[4];
} DADA_CHATID;

//Options when creating a chat
typedef enum _DADA_CHAT_OPTIONS
{
	DADA_CHAT_OPTION_NORMAL = 0,		//Normal voice chat
	DADA_CHAT_OPTION_DONT_HIDE = 1,		//Chat will be shown in UI. UI and API commands will compete against each other
	DADA_CHAT_OPTION_BEGIN_MUTED = 2,	//Starts with input and output of chat muted
	DADA_CHAT_OPTION_TEXT = 4,			//Create a text chat
} CHAT_OPTIONS;

//Room layout type. Same as in the Divide Map menu.
typedef enum _DADA_ROOMTYPE
{
	DADA_ROOMTYPE_NONE,		//Open map area
	DADA_ROOMTYPE_SPLIT,	//Map area split down the centre
	DADA_ROOMTYPE_CORNER,	//Upper right hand corner room
	DADA_ROOMTYPE_QUAD,		//Map area split into 4 corners
	DADA_ROOMTYPE_UNKNOWN
} DADA_ROOMTYPE;

//Status of the connection to Dolby Axon Desktop Client
typedef enum _DADA_STATUS
{
	DADA_STATUS_NORMAL,				//Connection is working
	DADA_STATUS_CONNECTION_DOWN,	//Unable to find the Axon Client
	DADA_STATUS_NOT_SIGNED_IN,		//Axon client is not signed in
	DADA_STATUS_UNKNOWN
} DADA_STATUS;

//Player position x and y are restricted to 0-100. rotation restricted to 0-359
// x = 0, y = 0 is bottom left hand corner of the map
//rotation = 0 is facing along the positive X axis, rotation = 90 is facing along the positive Y axis
typedef struct _DADA_POSITION
{
	unsigned int x;
	unsigned int y;
	unsigned int rotation;
} DADA_POSITION;

//User ID. Globally Unique.
typedef unsigned int DADA_USERID;

typedef struct _DADA_USERS
{
	const DADA_USERID* ids;
	size_t ids_len;
} DADA_USERS;

typedef struct _DADA_CHATS
{
	const DADA_CHATID* ids;
	size_t ids_len;
} DADA_CHATS;

//Your friendship relationship with another user.
typedef enum _DADA_FRIENDSTATUS
{
	DADA_FRIENDSTATUS_UNKNOWN = 0,		//Unknown or no friendship relationship
	DADA_FRIENDSTATUS_BLOCKED,			//You have marked this user as blocked
	DADA_FRIENDSTATUS_INTRODUCED,		//You have been introduced to this user
	DADA_FRIENDSTATUS_PENDING,			//You are waiting for them to accept the friendship
	DADA_FRIENDSTATUS_REQUIRES_AUTH,	//Waiting on you to accept the friendship
	DADA_FRIENDSTATUS_FRIEND,			//This user is your friend
	DADA_FRIENDSTATUS_SELF,				//This is you
	DADA_FRIENDSTATUS_INVALID
} DADA_FRIENDSTATUS;

//A users connection status - Set status in Axon Client
typedef enum _DADA_CONNECTIONSTATE
{
	DADA_CONNECTIONSTATE_OFFLINE = 0,	//User is offline
	DADA_CONNECTIONSTATE_ONLINE,		//User is online
	DADA_CONNECTIONSTATE_AWAY,			//User is marked as away
	DADA_CONNECTIONSTATE_BUSY,			//User is marked as busy
	DADA_CONNECTIONSTATE_UNKNOWN
} DADA_CONNECTIONSTATE;

//Detailed information about a user
typedef struct _DADA_USERINFO
{
	DADA_USERID id;
	char user_name[DADA_MAX_NAME_BYTES];
	char display_name[DADA_MAX_NAME_BYTES];
	DADA_FRIENDSTATUS friendship_status;
	DADA_CONNECTIONSTATE connection_state;
} DADA_USERINFO;

//Detailed information about a user in a chat.
typedef struct _DADA_CHATUSERINFO
{
	DADA_USERID id;
	char talking;	//This user is talking, 0-255 roughly indicates volume
	bool promoted;	//This user is promoted
	bool muted;		//you have muted this user
	bool joined;	//The user is active in the chat
} DADA_CHATUSERINFO;

typedef struct _DADA_CHATINFO
{
	DADA_CHATID id;
	DADA_USERID owner;		//Owner of a chat, only valid for permanent chats
	DADA_USERID inviter;	//User who invited me to the chat

	DADA_ROOMTYPE room;
	bool permanent;				//This is a permanent chat
	bool full;					//You cannot join the chat it is full
	bool promoted_invites_only;	//Only promoted users can invite other users

	char title[DADA_MAX_TITLE_BYTES];	//Chats title, also known as topic in Axon Client

	bool joined;		//You have joined this chat
	bool input_muted;	//You have muted your microphone for this chat
	bool output_muted;	//You have muted the output for this chat
} DADA_CHATINFO;

typedef enum _DADA_ERROR
{
	DADA_ERROR_NONE,				//No error
	DADA_ERROR_NULL,				//DADA pointer provided is NULL
	DADA_ERROR_NULL_PARAM,			//A prarmeter provided is NULL
	DADA_ERROR_CONNECTION,			//Conenction to Axon Client is down
	DADA_ERROR_NOT_LOGGED_IN,		//You are not logged into the Axon Client
	DADA_ERROR_CHAT_NOT_FOUND,		//Unable to find chat specified by DADA_CHATID
	DADA_ERROR_USER_NOT_FOUND,		//Unable to find user specified by DADA_USERID
	DADA_ERROR_NOT_PROMOTED,		//The action you tried requires you to be promoted
	DADA_ERROR_DISALLOWED,			//You just can't do that (Kicking owner, etc)
	DADA_ERROR_INTERNAL_FAILURE,	//Something very bad has happened (if you can reproduce this please let us know)
	DADA_ERROR_TIMEOUT,				//The the Axon Client took too long to respond
	DADA_ERROR_UNKNOWN
} DADA_ERROR;

//Context used when making API calls
typedef struct _DADA DADA;

//Create an DADA pointer to use for making API calls
DADA_EXPORT DADA* DADA_Create();

//Cleanup an DADA pointer
DADA_EXPORT void DADA_Destroy(DADA*);


//Get the connection status
DADA_EXPORT DADA_ERROR DADA_GetStatus(DADA*, DADA_STATUS*);

//Set you mod name. This appears in the Axon Client
DADA_EXPORT DADA_ERROR DADA_SetGameName(DADA*, const char* name);

//Open the default browser to the Dolby Axon home page
DADA_EXPORT void DADA_OpenWebsite();

//Create and join a new chat in the Axon Client. The CHATID returned is used to refer to the created chat.
//By default the chat is a voice chat that you automatically leave when the API is shutdown
DADA_EXPORT DADA_ERROR DADA_CreateChat(DADA*, DADA_CHATID*, unsigned int options);

//Get a list of all the Chat IDs in the Axon Client
DADA_EXPORT DADA_ERROR DADA_GetChats(DADA*, DADA_CHATS*);

//Get detailed information about a specific chat
DADA_EXPORT DADA_ERROR DADA_GetChatInfo(DADA*, const DADA_CHATID, DADA_CHATINFO*);


//Change the title of chat.
DADA_EXPORT DADA_ERROR DADA_SetChatTitle(DADA*, const DADA_CHATID, const char* title);

//Divide the map of the chat
DADA_EXPORT DADA_ERROR DADA_SetChatRoom(DADA*, const DADA_CHATID, const DADA_ROOMTYPE);

//Join a chat
DADA_EXPORT DADA_ERROR DADA_JoinChat(DADA*, const DADA_CHATID);

//Leave a chat you have joined. You will need to be reinvited unless this is a permanent chat
DADA_EXPORT DADA_ERROR DADA_LeaveChat(DADA*, const DADA_CHATID);


//Mute the output of a chat
DADA_EXPORT DADA_ERROR DADA_MuteOutput(DADA*, const DADA_CHATID);

//Unmute the output of a chat
DADA_EXPORT DADA_ERROR DADA_UnmuteOutput(DADA*, const DADA_CHATID);

//Mute the microphone input of a chat
DADA_EXPORT DADA_ERROR DADA_MuteMicrophone(DADA*, const DADA_CHATID);

//Unmute the microphone input of a chat
DADA_EXPORT DADA_ERROR DADA_UnmuteMicrophone(DADA*, const DADA_CHATID);


//Invite a user to a chat. The user must be your friend or be in another chat with you
DADA_EXPORT DADA_ERROR DADA_InviteUser(DADA*, const DADA_CHATID, const DADA_USERID);

//Kick a user from a chat, you must be promoted
DADA_EXPORT DADA_ERROR DADA_KickUser(DADA*, const DADA_CHATID, const DADA_USERID);

//Promote a user in a chat, you must be promoted
DADA_EXPORT DADA_ERROR DADA_PromoteUser(DADA*, const DADA_CHATID, const DADA_USERID);

//Demote a user in a chat, you must be promoted
DADA_EXPORT DADA_ERROR DADA_DemoteUser(DADA*, const DADA_CHATID, const DADA_USERID);

//Mute a user in a chat (for yourself only)
DADA_EXPORT DADA_ERROR DADA_MuteUser(DADA*, const DADA_CHATID, const DADA_USERID);

//Unmute a user in a chat (for yourself only)
DADA_EXPORT DADA_ERROR DADA_UnmuteUser(DADA*, const DADA_CHATID, const DADA_USERID);


//Move your avatar in the chat. x, y limited to 0-100 range
DADA_EXPORT DADA_ERROR DADA_Move(DADA*, const DADA_CHATID, const DADA_POSITION);


//Get the IDs of all the users that you know about
DADA_EXPORT DADA_ERROR DADA_GetUsers(DADA*, DADA_USERS*);

//Get the list of users in a chat
DADA_EXPORT DADA_ERROR DADA_GetChatUsers(DADA*, const DADA_CHATID, DADA_USERS*);

//Get detailed information about a user
DADA_EXPORT DADA_ERROR DADA_GetUserInfo(DADA*, const DADA_USERID, DADA_USERINFO*);

//Get detailed information about yourself
DADA_EXPORT DADA_ERROR DADA_GetSelfInfo(DADA*, DADA_USERINFO*);

//Get detailed information about a chat user
DADA_EXPORT DADA_ERROR DADA_GetChatUserInfo(DADA*, const DADA_CHATID, const DADA_USERID, DADA_CHATUSERINFO*);

#ifdef __cplusplus
};
#endif

#endif
