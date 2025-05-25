#include "Actors/FollowPointsAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"

void AFollowPointsAIController::SetPointsAndMove(const TArray<FVector>& InPoints)
{
    if (InPoints.Num() == 0)
    {
        ensure(false);
        return;
    }

    Points = InPoints;

    Move();
}

void AFollowPointsAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);
    if (!Result.IsSuccess())
    {
        return;
    }

    // Move to the next point
    Move();
}

void AFollowPointsAIController::Move()
{
    if (CurrentPointIndex >= Points.Num())
    {
        return;
    }

    FVector& TargetLocation = Points[CurrentPointIndex++];
    MoveToLocation(TargetLocation);
}
