/**
*
*	Socket.IO Client Library Functions Declarations
*	Internal Function Declarations
*
*	service_manager.h
*
*	Created by haneul kim, Modified by kyungho kim
*
**/

#ifndef __SERVICE_MANAGER_H__
#define __SERVICE_MANAGER_H__

#include "sio_client.h"

#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

namespace service
{
	class CServiceManager : public std::mutex
	{
		public:
			static CServiceManager* GetInstance();
			static void Release();

			CServiceManager();
			~CServiceManager();

			void connect(std::string client_ip);
			void disconnect();
			bool is_connected();
			std::string add_user(std::string name, std::string pv_left, std::string pv_right);

		private:

			static CServiceManager *s_ServiceManager;

			sio::client m_client;
			sio::socket::ptr current_socket;

			class connection_listener;
			connection_listener *m_cl;

			void bind_event();
	};

	template<class L> class CInstanceLifeCycle : public std::mutex
	{
	public:
		CInstanceLifeCycle() {}
		~CInstanceLifeCycle() { L::Release(); }
	};
}


#endif //__SERVICE_MANAGER_H__