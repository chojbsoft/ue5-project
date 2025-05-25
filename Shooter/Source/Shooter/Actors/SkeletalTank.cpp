#include "Actors/SkeletalTank.h"
#include "Components/ShooterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ASkeletalTank::ASkeletalTank(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UShooterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	// Create and attach components
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	SpringArmComponent->SetupAttachment(RootComponent);
	MainCamera->SetupAttachment(SpringArmComponent);

	// Setup collision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Vehicle"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Setup spring arm
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritRoll = false;

	// AI control settings
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASkeletalTank::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MyAIControllerClass)
	{
		AIControllerClass = MyAIControllerClass;
	}
}
