// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShooterGameInstance.generated.h"


#pragma pack(push, 1)
struct FPacketHeader
{
public:
	enum EPacketID : uint16
	{
		DediServerInfo = 1,
		End
	};

	EPacketID ID;
	uint16 PayLoad = 0;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FDediServerInfoPacket
{
    uint16 Port;
};
#pragma pack(pop)

UCLASS()
class SHOOTER_API UShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void OnStart() override;
	virtual void Shutdown() override;

protected:
	void ConnectToMatchmakingServer();
	void ReceiveLoop();
	void HandlePacket(uint8* Data, int32 DataSize);
	void AddRoomPortToUI(uint16 Port);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class URoomListWidget> RoomListWidgetClass;

	UPROPERTY()
	URoomListWidget* RoomListWidget;

protected:
    TSharedPtr<class FSocket> ClientSocket;
    std::atomic<bool> bRecvRunning = false;
	TArray<uint16> DediServerPorts;
};
