#include "Anim/TankAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UTankAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Pawn = TryGetPawnOwner();
	if (!IsValid(Pawn))
	{
		return;
	}

	TurretRotation = Pawn->GetControlRotation();
}

void UTankAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(Pawn))
	{
		return;
	}

	FRotator DesiredRotation = Pawn->IsLocallyControlled()
		? Pawn->GetControlRotation()
		: Pawn->GetActorRotation();

	// Smoothly interpolate turret rotation
	TurretRotation = UKismetMathLibrary::RLerp(
		FRotator(0.0f, TurretRotation.Yaw, 0.0f),
		DesiredRotation,
		DeltaSeconds * TurretRotationSpeed,
		true
	);

	// Handle wheel and track movement
	const float PawnSpeed = Pawn->GetVelocity().Length();
	if (PawnSpeed > KINDA_SMALL_NUMBER)
	{
		const double DeltaAngle = PawnSpeed * DeltaSeconds;
		WheelRotation.Pitch = FRotator::ClampAxis(WheelRotation.Pitch - DeltaAngle);
		Track += DeltaAngle;

		// Set the MID parameter to the Track value
		UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(GetOwningComponent()->GetMaterial(1));
		if (MID)
		{
			MID->SetScalarParameterValue(TEXT("OffsetV"), Track);
		}
	}
}
