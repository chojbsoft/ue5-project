// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomListWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API URoomListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void UpdateRoomButtons(const TArray<uint16>& RoomPorts);

protected:
    UPROPERTY(meta = (BindWidget))
    class UPanelWidget* RoomListPanel;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class URoomButton> RoomButtonClass;
};
