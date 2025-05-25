#include "SplineEnemySpawner.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "FollowPointsAIController.h"

ASplineSpawner::ASplineSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create Components
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Set up hierarchy
	SetRootComponent(SplineComponent);
	StaticMeshComponent->SetupAttachment(SplineComponent);
}

void ASplineSpawner::StartWave(USplineSpawnDataAsset* InSplineSpawnDataAsset)
{
	if (!HasAuthority() || !InSplineSpawnDataAsset)
	{
		return;
	}

	SplineSpawnDataAsset = InSplineSpawnDataAsset;

	if (CurrentSpawnIndex >= SplineSpawnDataAsset->SpawnInfoArray.Num())
	{
		return;
	}

	// Start the first spawn
	FSplineSpawnInfo& SplineSpawnInfo = SplineSpawnDataAsset->SpawnInfoArray[CurrentSpawnIndex++];
	Spawn(SplineSpawnInfo, SplineSpawnInfo.SpawnCount);
}

void ASplineSpawner::Spawn(FSplineSpawnInfo& InSplineSpawnInfo, const uint32 InRemainCount)
{
	// Set up a delayed spawn using timer
	GetWorldTimerManager().SetTimer(TimerHandle, [this, &InSplineSpawnInfo, InRemainCount]()
		{
			FTransform SpawnTransform = FTransform(GetActorRotation(), GetActorLocation());
			FActorSpawnParameters Param;
			Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APawn* Pawn = GetWorld()->SpawnActor<APawn>(InSplineSpawnInfo.SpawnClass, SpawnTransform, Param);

			// Generate points along spline
			TArray<FVector> Points;
			int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
			for (int32 i = 0; i < NumPoints; i++)
			{
				FVector PointLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);

				FNavLocation ProjectedPoint;
				UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

				if (NavSys && NavSys->ProjectPointToNavigation(PointLocation, ProjectedPoint))
				{
					Points.Add(ProjectedPoint.Location);
				}
				else
				{
					Points.Add(PointLocation);
				}

				Points.Add(PointLocation);
			}

			// Assign AI Controller to follow points
			AController* Controller = Pawn->GetController();
			if (Controller)
			{
				AFollowPointsAIController* FollowPointsAIController = Cast<AFollowPointsAIController>(Controller);
				if (FollowPointsAIController)
				{
					FollowPointsAIController->SetPointsAndMove(Points);
				}
			}

			// Continue spawning or move to next wave
			if (InRemainCount == 1)
			{
				StartWave(nullptr);
			}
			else
			{
				Spawn(InSplineSpawnInfo, InRemainCount - 1);
			}

		}, InSplineSpawnInfo.SpawnDelay, false);
}

