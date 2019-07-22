/**
*
*	Socket.IO Client Library Functions Definitions
*	Standard API Definitions
*
*	socket.io-client-dll.cpp
*
*	Created by kyungho kim
*
**/

#include "stdafx.h"

#include "socket.io-client.h"

#include "service_manager.h"

service::CServiceManager* service_instance()
{
	return service::CServiceManager::GetInstance();
}

bool is_connected()
{
	return service::CServiceManager::GetInstance()->is_connected();
}

void release_service()
{
	service::CServiceManager::Release();
}

SIOC_STATUS SIOC_API SIOC_Connect(char* client_ip)
{
	if (is_connected() == true) {
		return SIOC_ERR_ALREADY_CONNECTED;
	}

	if (client_ip == NULL) {
		return SIOC_ERR_INVALID_PARAMTERS;
	}

	service_instance()->connect(client_ip);

	return SIOC_SUCCESS;
}

SIOC_STATUS SIOC_API SIOC_Disconnect()
{
	if (service_instance() == NULL || is_connected() == false) {
		return SIOC_ERR_NOT_CONNECTED;
	}

	release_service();

	return SIOC_SUCCESS;
}