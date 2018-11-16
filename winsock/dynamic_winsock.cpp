#include <windows.h>
/* Winsock */
typedef int (*WSASTARTUP)( _In_   WORD wVersionRequested, _Out_  LPWSADATA lpWSAData); //WSAStartup
//typedef unsigned long (*INET_ADDR)(_In_  const char *cp); // inet_addr        
typedef unsigned long (PASCAL FAR *INET_ADDR)(__in const char FAR * cp); //inet_addr
//typedef struct hostent* FAR (*GETHOSTBYNAME)(_In_  const char *name); //gethostbyname
typedef struct hostent FAR * (PASCAL FAR *GETHOSTBYNAME)(const char FAR * name);
typedef char* FAR (*INET_NTOA)( _In_  struct   in_addr in); //  inet_ntoa
typedef int (*WSACLEANUP)(void); //WSACleanup
typedef u_long (PASCAL FAR *NTOHL)(_In_  u_long netlong);
//typedef u_long (*NTOHL)(_In_  u_long netlong); //ntohl

typedef struct _DYNAMIC_WINSOCK {
	WSASTARTUP fpWSAStartup;
	WSACLEANUP fpWSACleanup;
	INET_ADDR fpinet_addr;
	GETHOSTBYNAME fpgethostbyname;
	INET_NTOA fpinet_ntoa;
	NTOHL fpntohl;
} DYNAMIC_WINSOCK, *PDYNAMIC_WINSOCK;
/* init dynamic winsock 
PDYNAMIC_WINSOCK dynamicWinsock = NULL;
if((dynamicWinsock = (PDYNAMIC_WINSOCK) malloc(sizeof(DYNAMIC_WINSOCK))) == NULL)
	return 0;
if(!API_LoadWinsock(&dynamicWinsock))
	return 0;
*/

BOOL API_LoadWinsock(PDYNAMIC_WINSOCK* pDynamicWinsock)
{
	PDYNAMIC_WINSOCK dynamicWinsock = *pDynamicWinsock;

	if  (dynamicWinsock)
	{
		WCHAR strLib[] = {'W',  'S',  '2',  '_',  '3',  '2',  0x0};
		HMODULE hMod = NULL;
		
		if((hMod = LoadLibrary(strLib)) == NULL)
			return FALSE;

		CHAR strWSAStartup[] = {'W',  'S',  'A',  'S',  't',  'a',  'r',  't',  'u',  'p',  0x0};
		dynamicWinsock->fpWSAStartup = (WSASTARTUP) GetProcAddress(hMod, strWSAStartup);

		CHAR strWSACleanup[] = {'W',  'S',  'A',  'C',  'l',  'e',  'a',  'n',  'u',  'p', 0x0};
		dynamicWinsock->fpWSACleanup = (WSACLEANUP) GetProcAddress(hMod, strWSACleanup);
		
		CHAR strinet_addr[] = {'i',  'n',  'e',  't',  '_',  'a',  'd',  'd',  'r', 0x0};
		dynamicWinsock->fpinet_addr = (INET_ADDR) GetProcAddress(hMod, strinet_addr);
		
		CHAR strgethostbyname[] = {'g',  'e',  't',  'h',  'o',  's',  't',  'b',  'y',  'n',  'a',  'm',  'e', 0x0};
		dynamicWinsock->fpgethostbyname = (GETHOSTBYNAME) GetProcAddress(hMod, strgethostbyname);
		
		CHAR strinet_ntoa[] = { 'i',  'n',  'e',  't',  '_',  'n',  't',  'o',  'a', 0x0};
		dynamicWinsock->fpinet_ntoa = (INET_NTOA) GetProcAddress(hMod, strinet_ntoa);

		CHAR strntohl[] = { 'n',  't',  'o',  'h',  'l', 0x0};
		dynamicWinsock->fpntohl = (NTOHL) GetProcAddress(hMod, strntohl);

		if (
			!dynamicWinsock->fpWSAStartup ||
			!dynamicWinsock->fpinet_addr ||
			!dynamicWinsock->fpgethostbyname ||
			!dynamicWinsock->fpinet_ntoa ||
			!dynamicWinsock->fpntohl 
			)
			return FALSE;
		else
			return TRUE; 
	}
	return FALSE;
}