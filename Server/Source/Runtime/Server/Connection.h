#pragma once

enum class EConnectionState
{
	Invalid = 0,
	Closed = 1,
	Pending = 2,
	Open = 3
};

#pragma pack(push, 1)
struct PacketHeader
{
public:
	enum EPacketID : uint16
	{
		DediServerInfo = 1, // STC
		End
	};

public:
	PacketHeader() = default;
	PacketHeader(EPacketID id, uint32 payload)
		: _id(id)
		, _payload(payload)
	{
	};

public:
	uint32 GetPayload() const
	{
		return _payload;
	}
	EPacketID GetId() const
	{
		return _id;
	}
	void SetPayload(uint32 payload)
	{
		_payload = payload;
	}
	void SetId(EPacketID id)
	{
		_id = id;
	}

private:
	EPacketID _id;
	uint16 _payload = 0;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DediServerInfoPacket
{
	uint16 _port = 0;
};
#pragma pack(pop)

class Connection
{
public:
	void Init(boost::asio::io_context& context
		, function<void(Connection*)> onAccept
		, function<void(Connection*)> onClosed
		, function<void(Connection*, PacketHeader*)> onRecv);
	Socket* GetSocket() const;
	void Clean();
	void OnAccept();

public:
	void ReadPacketHeader();
	void ReadPacketBody(const PacketHeader* header);
	void Send(const PacketHeader::EPacketID id, void* body, const uint32 bodySize);

protected:
	const EConnectionState GetConnState() const;
	void SetConnState(const EConnectionState state);

private:
	unique_ptr<Socket> _socket;
	EConnectionState _state = EConnectionState::Invalid;

private:
	function<void(Connection*)> _onAccept;
	function<void(Connection*)> _onClosed;
	function<void(Connection*, PacketHeader*)> _onRecv;

private:
	PacketHeader _recvHeaderBuff;
	boost::pool<> _bufferPool{ 1460 * 2 };


};

