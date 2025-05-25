#include "pch.h"
#include "Connection.h"
#include "Logger.h"

void Connection::Init(boost::asio::io_context& context
	, function<void(Connection*)> onAccept
	, function<void(Connection*)> onClosed
	, function<void(Connection*, PacketHeader*)> onRecv)
{
	SetConnState(EConnectionState::Invalid);

	_onAccept = onAccept;
	_onClosed = onClosed;
	_onRecv = onRecv;
	_socket = make_unique<Socket>(context);
}

Socket* Connection::GetSocket() const
{
	return _socket.get();
}

void Connection::Clean()
{
	if (_socket)
	{
		SetConnState(EConnectionState::Closed);

		_socket->close();
		_onClosed(this);
	}
}

void Connection::OnAccept()
{
	SetConnState(EConnectionState::Pending);

	_onAccept(this);
	ReadPacketHeader();
}

void Connection::ReadPacketHeader()
{
	// 우선 header만 읽기
	boost::asio::async_read(*_socket, boost::asio::buffer(&_recvHeaderBuff, sizeof(_recvHeaderBuff)),
		[this](boost::system::error_code ErrorCode, uint32 recvSize)
		{
			// 사이즈만큼 차야 호출됨 
			// recvSize는 결국 요청한 사이즈와 같음
			if (ErrorCode)
			{
				LOG(Error, "async_read error: {}", ErrorCode.message());
				Clean();
				return;
			}

			ReadPacketBody(&_recvHeaderBuff);
		}
	);
}

void Connection::ReadPacketBody(const PacketHeader* header)
{
	uint32 payload = header->GetPayload();
	PacketHeader::EPacketID id = header->GetId();

	// 버퍼 오버플로우 방지
	uint32 packetSize = sizeof(PacketHeader) + payload;
	const uint32 bufferSize = _bufferPool.get_requested_size();
	if (packetSize > bufferSize)
	{
		LOG(Error, "Read buffer overflow");
		Clean();
		return;
	}

	// 진짜 바디 읽기
	PacketHeader* allocHeader = static_cast<PacketHeader*>(_bufferPool.malloc());
	new(allocHeader)PacketHeader(id, payload);
	boost::asio::async_read(*_socket, boost::asio::buffer(allocHeader + 1, payload),
		[this, allocHeader](boost::system::error_code ErrorCode, uint32 recvSize)
		{
			if (ErrorCode)
			{
				LOG(Error, "async_read error: {}", ErrorCode.message());
				Clean();
				return;
			}

			_onRecv(this, allocHeader);
			_bufferPool.free(allocHeader);

			ReadPacketHeader();
		}
	);
}


void Connection::Send(const PacketHeader::EPacketID id, void* body, const uint32 payload)
{
	const int32 bufferSize = _bufferPool.get_requested_size();
	if (sizeof(PacketHeader) + payload > bufferSize)
	{
		LOG(Error, "Send buffer overflow");
		return;
	}

	PacketHeader* header = reinterpret_cast<PacketHeader*>(_bufferPool.malloc());
	header->SetId(id);
	header->SetPayload(payload);

	// body가 있다면 _bufferPool로 copy
	if (payload > 0)
	{
		memcpy(header + 1, body, payload);
		delete body;
	}
	const uint32 packetSize = sizeof(PacketHeader) + payload;

	// Send packet
	boost::asio::async_write(*_socket, boost::asio::buffer(header, sizeof(PacketHeader) + payload),
		[this, header](boost::system::error_code ErrorCode, uint32 length)
		{
			_bufferPool.free(header);
			if (ErrorCode)
			{
				LOG(Error, "async_write error: {}", ErrorCode.message());
				Clean();
				return;
			}
		}
	);

}

const EConnectionState Connection::GetConnState() const
{
	return _state;
}

void Connection::SetConnState(const EConnectionState state)
{
	_state = state;
}