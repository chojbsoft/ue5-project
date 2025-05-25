#pragma once

#include "Connection.h"

struct URL
{
	string _ip;
	uint16 _port;
};

class NetworkManager
{
public:
	virtual bool Init(const URL& url, const uint8 backlogCnt);

protected:
	void StartAccept(shared_ptr<Connection> reuseConn = nullptr);

public:
	virtual void Tick(float deltaTime);

public:
	NetworkManager();
	virtual ~NetworkManager();

private:
	virtual void OnAccept(Connection* conn);
	virtual void OnClosed(Connection* conn);
	virtual void OnRecv(Connection* conn, PacketHeader* packetHeader);

protected:
	URL _url;

private:
	boost::asio::io_context _context;
	shared_ptr<tcp::acceptor> _acceptor;
	unordered_map<Connection*, shared_ptr<Connection>> _backlogConn;
	unordered_map<Connection*, shared_ptr<Connection>> _pendingConn;

private:
	uint32 _maxPollCntPerTick = 1000;

private:
	uint32 _dediServerPort = 7122;

};

