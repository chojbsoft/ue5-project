#pragma once
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterMovementComponent.generated.h"

// Custom movement component for shooter characters
UCLASS()
class SHOOTER_API UShooterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UShooterMovementComponent();
	
protected:
	virtual bool FindNavFloor(const FVector& TestLocation, FNavLocation& NavFloorLocation) const override;

	//virtual void PhysNavWalking(float deltaTime, int32 Iterations) override;

public:
	UPROPERTY(EditAnywhere, Category = "Nav")
	float SearchRadiusFactor = 0.2f;
};
