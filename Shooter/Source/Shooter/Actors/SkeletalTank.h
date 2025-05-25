#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "AIController.h"
#include "SkeletalTank.generated.h"

// Skeletal Tank character class.
UCLASS()
class SHOOTER_API ASkeletalTank : public ACharacter
{
	GENERATED_BODY()

public:
	ASkeletalTank(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

// Components
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* MainCamera;

// AI
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AAIController> MyAIControllerClass;
};
