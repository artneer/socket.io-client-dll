/**
*
*	Socket.IO Client Library Functions Definitions
*	Internal Function Definitions
*
*	service_manager.cpp
*
*	Created by haneul kim, Modified by kyungho kim
*
**/

#include "service_manager.h"
#include "json/json.h"

#define HIGHLIGHT(__O__) std::cout<<__O__<<std::endl
#define EM(__O__) std::cout<<__O__<<std::endl


namespace service
{
	CServiceManager* CServiceManager::s_ServiceManager = NULL;
	CInstanceLifeCycle<CServiceManager> s_ServiceManagerLifeCycle;

	std::mutex _lock;
	std::condition_variable_any _cond;

	bool connect_finish = false;
	bool add_finish = false;

	int current_clients = -1;

	class CServiceManager::connection_listener
	{
		sio::client &handler;

		public:

		connection_listener(sio::client& h):
			handler(h)
		{
		}

		void on_connected()
		{
			_lock.lock();
			_cond.notify_all();
			connect_finish = true;
			_lock.unlock();
		}
		void on_close(sio::client::close_reason const& reason)
		{
			std::cout<<"sio closed "<<std::endl;
			exit(0);
		}

		void on_fail()
		{
			std::cout<<"sio failed "<<std::endl;
			exit(0);
		}
	};

	CServiceManager* CServiceManager::GetInstance()
	{
		s_ServiceManagerLifeCycle.lock();
		if (!s_ServiceManager) {
			s_ServiceManager = new CServiceManager();
		}
		s_ServiceManagerLifeCycle.unlock();

		return s_ServiceManager;
	}

	void CServiceManager::Release()
	{
		s_ServiceManagerLifeCycle.lock();
		if (s_ServiceManager != NULL) {
			delete s_ServiceManager;
			s_ServiceManager = NULL;
		}
		s_ServiceManagerLifeCycle.unlock();
	}

	void CServiceManager::bind_event()
	{
		current_socket->on("client joined", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp)
					{
					_lock.lock();
					std::string client = data->get_map()["clientIp"]->get_string();
					current_clients = (int)data->get_map()["numClients"]->get_int();
					HIGHLIGHT(client<<" joined (current clients: "<<current_clients<<")");
					_lock.unlock();
					}));

		current_socket->on("client left", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp)
					{
					_lock.lock();
					std::string client = data->get_map()["clientIp"]->get_string();
					current_clients = (int)data->get_map()["numClients"]->get_int();
					HIGHLIGHT(client<<" left (current clients: "<<current_clients<<")");
					_lock.unlock();
					}));
	}

	CServiceManager::CServiceManager()
	{
		m_cl = new connection_listener(m_client);
	}

	CServiceManager::~CServiceManager()
	{
		m_client.sync_close();
		m_client.clear_con_listeners();
	}

	void CServiceManager::connect(std::string client_ip)
	{
		m_client.set_open_listener(std::bind(&connection_listener::on_connected, m_cl));

		m_client.set_open_listener(std::bind(&connection_listener::on_connected, m_cl));
		m_client.set_close_listener(std::bind(&connection_listener::on_close, m_cl, std::placeholders::_1));
		m_client.set_fail_listener(std::bind(&connection_listener::on_fail, m_cl));
		m_client.connect("http://localhost:8888");

		_lock.lock();
		if(!connect_finish)
		{
			_cond.wait(_lock);
		}
		_lock.unlock();

		current_socket = m_client.socket();

		current_socket->on("added", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp){
					_lock.lock();
					current_clients = (int)data->get_map()["numClients"]->get_int();
					HIGHLIGHT("Currently connected clients : "<<current_clients);
					add_finish = true;
					_cond.notify_all();
					_lock.unlock();
					current_socket->off("added");
					}));

		current_socket->on("already added", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp){
					_lock.lock();
					HIGHLIGHT("This client IP is already added");
					_cond.notify_all();
					_lock.unlock();
					current_socket->off("already added");
					}));

		current_socket->emit("add client", client_ip);

		_lock.lock();	
		if(current_clients < 0) {
			_cond.wait(_lock);
		}
		_lock.unlock();

		_lock.lock();	
		if(!add_finish) {
			_cond.wait(_lock);
		}
		_lock.unlock();

		bind_event();
	}

	bool CServiceManager::is_connected()
	{
		return connect_finish;
	}

	void CServiceManager::disconnect()
	{
		current_socket->emit("disconnect", nullptr);
	}
}
