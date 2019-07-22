/**
*
*	Socket.IO Client Library Functions Declarations
*	Standard API Declarations
*
*	socket.io-client-dll.cpp
*
*	Created by kyungho kim
*
**/

#ifndef __SOCKET_IO_CLIENT__
#define __SOCKET_IO_CLIENT__

#ifdef WIN32
#ifdef SOCKET_IO_EXPORTS
#define SIOC_API __declspec(dllexport) __stdcall
#else //SOCKET_IO_EXPORTS
#define SIOC_API __stdcall
#endif //SOCKET_IO_EXPORTS
#else //WIN32
#define SIOC_API __stdcall
#endif //WIN32

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define SIOC_STATUS	int

#define SIOC_SUCCESS				0
#define SIOC_ERR_UNKNOWN			-1
#define SIOC_ERR_NOT_SUPPROTED		-101
#define	SIOC_ERR_INVALID_PARAMTERS	-102
#define SIOC_ERR_ALREADY_CONNECTED	-103
#define SIOC_ERR_NOT_CONNECTED		-104


/**
* @brief		Connect to server from client based on Socket.IO
* @details		
*
* @param[in]	client_ip	: to add client IP
*
* @return		Refer to error code
*
*/
SIOC_STATUS SIOC_API SIOC_Connect(char* client_ip);


/**
* @brief		Disconnect from server
* @details		
*
* @return		Refer to error code
*
*/
SIOC_STATUS SIOC_API SIOC_Disconnect();


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__SOCKET_IO_CLIENT__