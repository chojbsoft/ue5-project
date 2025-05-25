#include "pch.h"
#include "NetworkManager.h"
#include "Logger.h"

#include <shellapi.h>

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::Init(const URL& url, const uint8 backlogCnt)
{
	this->_url = url;

	_acceptor = make_shared<boost::asio::ip::tcp::acceptor>(_context
		, tcp::endpoint(tcp::v4(), (boost::asio::ip::port_type)url._port), true);

	for (int i = 0; i < backlogCnt; i++)
	{
		StartAccept();
	}

	// DediServer 실행
	wstring cmd = L"-log -port=" + to_wstring(_dediServerPort);
	HINSTANCE Instance = ShellExecuteW(NULL, L"open", SERVER_DIR.c_str()
		, cmd.c_str(), NULL, SW_SHOWDEFAULT);
	if (Instance <= (HINSTANCE)32)
	{
		LOG(Error, "dediServer start error");
	}
	else
	{
		LOG(Info, "dediServer start");
	}

	return false;
}

void NetworkManager::StartAccept(shared_ptr<Connection> reuseConn)
{
	if (reuseConn)
	{
		reuseConn->Clean();
	}

	shared_ptr<Connection> conn = reuseConn
		? reuseConn
		: make_shared<Connection>();
	if (!reuseConn)
	{
		conn->Init(_context
			, bind(&NetworkManager::OnAccept, this, placeholders::_1)
			, bind(&NetworkManager::OnClosed, this, placeholders::_1)
			, bind(&NetworkManager::OnRecv, this, placeholders::_1, placeholders::_2));
		_backlogConn.insert(make_pair(conn.get(), conn));
	};

	_acceptor->async_accept(*conn->GetSocket()
		, [this, conn](const boost::system::error_code& error)
		{
			if (error)
			{
				LOG(Error, "accept error: " + error.message());

				// 실패시 기존 conn으로 accept
				StartAccept(conn);
				return;
			}

			_backlogConn.erase(conn.get());
			_pendingConn.insert(make_pair(conn.get(), conn));

			conn->OnAccept();

			// 성공시 새로운 conn으로 accept
			StartAccept();
		});
}

void NetworkManager::Tick(float deltaTime)
{
	boost::system::error_code errorCode;

	uint32 pollCnt = 0;
	while (uint32 n = _context.poll_one())
	{
		pollCnt += n;
		if (pollCnt > _maxPollCntPerTick)
		{
			break;
		}
	}
}

void NetworkManager::OnAccept(Connection* conn)
{
	LOG(Info, "accept");


	// Send dediServer ip and port to client
	DediServerInfoPacket* packet = new DediServerInfoPacket();
	packet->_port = _dediServerPort;
	conn->Send(PacketHeader::EPacketID::DediServerInfo, packet, sizeof(DediServerInfoPacket));
}

void NetworkManager::OnClosed(Connection* conn)
{

}

void NetworkManager::OnRecv(Connection* conn, PacketHeader* packetHeader)
{
}
