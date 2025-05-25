#include "Anim/StepAnimNotify.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

void UStepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner()))
	{
		return;
	}
		
	FHitResult HitResult;
	const FVector Start = MeshComp->GetComponentLocation();
	const FVector End = Start - FVector(0., 0., 150.);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(MeshComp->GetOwner());

	const bool bHit = UKismetSystemLibrary::LineTraceSingle(
		MeshComp, 
		Start, 
		End, 
		ETraceTypeQuery::TraceTypeQuery1, 
		false, 
		IgnoreActors, 
		EDrawDebugTrace::None, HitResult, true
	);

	if (bHit)
	{
		switch (HitResult.PhysMaterial->SurfaceType)
		{
		// Water
		case SurfaceType1:
			Play(MeshComp, FootStepInfo_Water);
			break;

		// Default
		default:
			Play(MeshComp, FootStepInfo_Default);
			break;
		}
	}
}

void UStepAnimNotify::Play(USkeletalMeshComponent* MeshComp, const FFootStepInfo& InFootStepInfo)
{
	if (!IsValid(MeshComp))
	{
		return;
	}

	// Play sound
	if (InFootStepInfo.Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(MeshComp, InFootStepInfo.Sound
			, MeshComp->GetComponentLocation(), InFootStepInfo.VolumeMultiplier);
	}

	// Spawn particle
	if (InFootStepInfo.Particle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp, InFootStepInfo.Particle
			, MeshComp->GetComponentLocation(), FRotator::ZeroRotator, InFootStepInfo.ParticleScale);
	}
}
