#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include "SplineEnemySpawner.generated.h"

// Information for spawning an actor along a spline
USTRUCT()
struct FSplineSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float SpawnDelay = 1.f;

	UPROPERTY(EditAnywhere)
	int32 SpawnCount = 1;

	UPROPERTY(EditAnywhere)
	UClass* SpawnClass;
};

// Data Asset holding spawn information for spline spawner
UCLASS(BlueprintType)
class USplineSpawnDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FSplineSpawnInfo> SpawnInfoArray;
};

// Actor that spawns actors along a spline path based on a spawn data asset
UCLASS()
class SHOOTER_API ASplineSpawner : public AActor
{
	GENERATED_BODY()

public:	
	ASplineSpawner();

	UFUNCTION(BlueprintCallable)
	void StartWave(USplineSpawnDataAsset* InSplineSpawnDataAsset);

protected:
	void Spawn(FSplineSpawnInfo& InSplineSpawnInfo, const uint32 InRemainCount);

// Components
protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

// Spawn Data
protected:
	UPROPERTY(EditAnywhere, Category = "Spline Spawner")
	USplineSpawnDataAsset* SplineSpawnDataAsset;

	int32 CurrentSpawnIndex = 0;

	FTimerHandle TimerHandle;
};
