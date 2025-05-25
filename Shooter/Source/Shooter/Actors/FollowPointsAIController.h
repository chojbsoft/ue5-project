#pragma once
#include "Navigation/PathFollowingComponent.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "FollowPointsAIController.generated.h"

// AI Controller that moves along a series of given points.
UCLASS()
class SHOOTER_API AFollowPointsAIController : public AAIController
{
	GENERATED_BODY()

public:
	void SetPointsAndMove(const TArray<FVector>& InPoints);

protected:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	void Move();

// Follow data
protected:
	TArray<FVector> Points;

	int32 CurrentPointIndex = 0;
};
