// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/ShooterGameInstance.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "HAL/RunnableThread.h"
#include "HAL/PlatformProcess.h"
#include "Async/Async.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/RoomListWidget.h"


void UShooterGameInstance::OnStart()
{
    Super::OnStart();

    // 안정적인 시점에서 UI 생성
    UWorld* World = GetWorld();
    if (World && RoomListWidgetClass)
    {
        RoomListWidget = CreateWidget<URoomListWidget>(World, RoomListWidgetClass);
        if (RoomListWidget)
        {
            RoomListWidget->AddToViewport();
        }
    }

    ConnectToMatchmakingServer();
}

void UShooterGameInstance::ConnectToMatchmakingServer()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    TSharedRef<FInternetAddr> Addr = SocketSubsystem->CreateInternetAddr();

    bool bIsValid;
    Addr->SetIp(TEXT("127.0.0.1"), bIsValid);
    Addr->SetPort(6122);

    ClientSocket = TSharedPtr<FSocket>(
        SocketSubsystem->CreateSocket(NAME_Stream, TEXT("MM_ClientSocket"), false)
    );

    if (ClientSocket->Connect(*Addr))
    {
        UE_LOG(LogTemp, Log, TEXT("Connected to Matchmaking Server"));

        bRecvRunning = true;
        Async(EAsyncExecution::Thread, [this]()
            {
                ReceiveLoop();
            });
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect to matchmaking server"));
    }
}

void UShooterGameInstance::ReceiveLoop()
{
    constexpr int32 HeaderSize = sizeof(FPacketHeader);
    constexpr int32 MaxBufferSize = 1024;
    uint8 Buffer[MaxBufferSize];

    while (bRecvRunning && ClientSocket.IsValid()
        && ClientSocket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
    {
        int32 BytesPeeked = 0;
        if (ClientSocket->Recv(Buffer, HeaderSize, BytesPeeked, ESocketReceiveFlags::Peek))
        {
            if (BytesPeeked == HeaderSize)
            {
                const FPacketHeader* Header = reinterpret_cast<const FPacketHeader*>(Buffer);
                int32 TotalSize = HeaderSize + Header->PayLoad;

                int32 BytesReceived = 0;
                if (ClientSocket->Recv(Buffer, TotalSize, BytesReceived))
                {
                    if (BytesReceived == TotalSize)
                    {
                        HandlePacket(Buffer, TotalSize);
                    }
                }
            }
        }

        FPlatformProcess::Sleep(0.01f);
    }

    UE_LOG(LogTemp, Log, TEXT("Receive thread ended"));
}

void UShooterGameInstance::HandlePacket(uint8* Data, int32 DataSize)
{
    const FPacketHeader* Header = reinterpret_cast<const FPacketHeader*>(Data);

    switch (Header->ID)
    {
    case FPacketHeader::DediServerInfo:
    {
        if (Header->PayLoad != sizeof(FDediServerInfoPacket))
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid payload size for DediServerInfoPacket"));
            return;
        }

        const FDediServerInfoPacket* Packet = reinterpret_cast<const FDediServerInfoPacket*>(Data + sizeof(FPacketHeader));
        const uint16 Port = Packet->Port;

        AsyncTask(ENamedThreads::GameThread, [this, Port]()
            {
                AddRoomPortToUI(Port);
            });
        break;
    }

    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown packet ID: %d"), static_cast<uint16>(Header->ID));
        break;
    }
}

void UShooterGameInstance::AddRoomPortToUI(uint16 Port)
{
    DediServerPorts.Add(Port);

    if (RoomListWidget)
    {
        RoomListWidget->UpdateRoomButtons(DediServerPorts);
    }
}

void UShooterGameInstance::Shutdown()
{
    Super::Shutdown();

    // 수신 루프 중지
    bRecvRunning = false;

    // 소켓 종료
    if (ClientSocket.IsValid())
    {
        ClientSocket->Close();
        ClientSocket.Reset();  // 스마트 포인터 초기화
        UE_LOG(LogTemp, Log, TEXT("Matchmaking socket closed."));
    }

    UE_LOG(LogTemp, Log, TEXT("ShooterGameInstance shutdown complete."));
}