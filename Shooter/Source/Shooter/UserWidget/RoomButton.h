// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomButton.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class SHOOTER_API URoomButton : public UUserWidget
{
	GENERATED_BODY()

public:
    void InitRoomInfo(uint16 InPort);

protected:
    UFUNCTION()
    void OnClicked();

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* RoomNameText;

    UPROPERTY(meta = (BindWidget))
    UButton* JoinButton;

protected:
    uint16 RoomPort;
};
