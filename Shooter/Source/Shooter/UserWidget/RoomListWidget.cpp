// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/RoomListWidget.h"
#include "RoomButton.h"
#include "Components/PanelWidget.h"
#include "Blueprint/WidgetTree.h"

void URoomListWidget::UpdateRoomButtons(const TArray<uint16>& RoomPorts)
{
    if (!RoomListPanel || !RoomButtonClass) return;

    RoomListPanel->ClearChildren();

    for (uint16 Port : RoomPorts)
    {
        URoomButton* Button = CreateWidget<URoomButton>(GetWorld(), RoomButtonClass);
        if (Button)
        {
            Button->InitRoomInfo(Port);
            RoomListPanel->AddChild(Button);
        }
    }
}