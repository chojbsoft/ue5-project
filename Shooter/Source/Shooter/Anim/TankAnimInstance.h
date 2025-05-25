#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TankAnimInstance.generated.h"

// Animation instance class for Tank animations
UCLASS()
class SHOOTER_API UTankAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

// Animation Properties
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Tank|Animation")
	FRotator TurretRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "Tank|Animation")
	float TurretRotationSpeed = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Tank|Animation")
	FRotator WheelRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "Tank|Animation")
	double Track = 0.0;

// Pawn
protected:
	UPROPERTY()
	APawn* Pawn = nullptr;
};
