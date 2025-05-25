#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NiagaraSystem.h"
#include "StepAnimNotify.generated.h"

// Data structure to hold footstep effect information
USTRUCT()
struct FFootStepInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Footstep")
	UNiagaraSystem* Particle = nullptr;

	UPROPERTY(EditAnywhere, Category = "Footstep")
	USoundBase* Sound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Footstep")
	float VolumeMultiplier = 1.f;

	UPROPERTY(EditAnywhere, Category = "Footstep")
	FVector ParticleScale = FVector::OneVector;
};

// Animation notify to play footstep effectss
UCLASS()
class SHOOTER_API UStepAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	void Play(USkeletalMeshComponent* MeshComp, const FFootStepInfo& InFootStepInfo);

// Footstep Settings
protected:
	UPROPERTY(EditAnywhere)
	FFootStepInfo FootStepInfo_Default;

	UPROPERTY(EditAnywhere)
	FFootStepInfo FootStepInfo_Water;
};
