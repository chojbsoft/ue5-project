#include "pch.h"
#include "Engine.h"
#include "Logger.h"

Engine engine;

int32 Engine::Init()
{
	LOG(Info, "Engine Init");

	_url._ip = IP;
	_url._port = MAIN_SERVER_PORT;
	_networkManager = new NetworkManager();
	_networkManager->Init(_url, 5);


	/*_context = new boost::asio::io_context();
	_acceptor = new tcp::acceptor(*_context, tcp::endpoint(tcp::v4(), _port));

	_connectionThread = new thread([&]()
		{
			while (!_isExitRequested)
			{
				tcp::socket socket(*_context);
				_acceptor->accept(socket);
				char data[1024];
				size_t length = socket.read_some(boost::asio::buffer(data));

				auto now = chrono::system_clock::now();
				auto now_time = chrono::system_clock::to_time_t(now);

#pragma warning(disable: 4996)
				std::cout << "Received at " << std::put_time(std::localtime(&now_time), "%H:%M:%S");
#pragma warning(default: 4996)

				std::cout << " Message: ";
				std::cout.write(data, length);
				std::cout << "\n";

				std::string reply = "you want " + string(data);
				boost::asio::write(socket, boost::asio::buffer(reply));
			}
		});*/

	return 0;
}

void Engine::Tick(float deltaTime)
{
	//LOG(Info, "Engine Tick");
	_networkManager->Tick(deltaTime);

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		RequestExit("user exit");
	}
}

void Engine::Exit()
{
	LOG(Info, "Exit Engine");

	LOG(Info, "Delete NetDriver");
	delete _networkManager;

	logger.Exit();
}

void Engine::RequestExit(const string& reason)
{
	_isExitRequested = true;
	LOG(Info, "Exit Requested");
}
