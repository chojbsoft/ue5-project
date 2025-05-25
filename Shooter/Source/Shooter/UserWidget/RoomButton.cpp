// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/RoomButton.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void URoomButton::InitRoomInfo(uint16 InPort)
{
    RoomPort = InPort;

    if (RoomNameText)
    {
        RoomNameText->SetText(FText::FromString(FString::Printf(TEXT("Room - Port %d"), RoomPort)));
    }

    if (JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &URoomButton::OnClicked);
    }
}

void URoomButton::OnClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Joining room on port: %d"), RoomPort);

    FString Address = FString::Printf(TEXT("127.0.0.1:%d"), RoomPort);

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
    }
}