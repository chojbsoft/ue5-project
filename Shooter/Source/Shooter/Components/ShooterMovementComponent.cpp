#include "Components/ShooterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "AI/Navigation/NavigationDataInterface.h"

UShooterMovementComponent::UShooterMovementComponent()
{
	DefaultLandMovementMode = EMovementMode::MOVE_NavWalking;
}

bool UShooterMovementComponent::FindNavFloor(const FVector& TestLocation, FNavLocation& NavFloorLocation) const
{
	const INavigationDataInterface* NavData = GetNavData();
	if (NavData == nullptr || CharacterOwner == nullptr)
	{
		return false;
	}

	const FNavAgentProperties& AgentProps = CharacterOwner->GetNavAgentPropertiesRef();
	const float SearchRadius = AgentProps.AgentRadius * SearchRadiusFactor; /*AgentProps.AgentRadius * 2.0f;*/
	const float SearchHeight = AgentProps.AgentHeight * AgentProps.NavWalkingSearchHeightScale;

	return NavData->ProjectPoint(TestLocation, NavFloorLocation, FVector(SearchRadius, SearchRadius, SearchHeight));
}

//void UShooterMovementComponent::PhysNavWalking(float deltaTime, int32 Iterations)
//{
//	//SCOPE_CYCLE_COUNTER(STAT_CharPhysNavWalking);
//
//	if (deltaTime < MIN_TICK_TIME)
//	{
//		return;
//	}
//
//	if ((!CharacterOwner || !CharacterOwner->Controller) && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
//	{
//		Acceleration = FVector::ZeroVector;
//		Velocity = FVector::ZeroVector;
//		return;
//	}
//
//	RestorePreAdditiveRootMotionVelocity();
//
//	// Ensure velocity is horizontal.
//	MaintainHorizontalGroundVelocity();
//	//devCode(ensureMsgf(!Velocity.ContainsNaN(), TEXT("PhysNavWalking: Velocity contains NaN before CalcVelocity (%s)\n%s"), *GetPathNameSafe(this), *Velocity.ToString()));
//
//	//bound acceleration
//	Acceleration.Z = 0.f;
//	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
//	{
//		CalcVelocity(deltaTime, GroundFriction, false, GetMaxBrakingDeceleration());
//		//devCode(ensureMsgf(!Velocity.ContainsNaN(), TEXT("PhysNavWalking: Velocity contains NaN after CalcVelocity (%s)\n%s"), *GetPathNameSafe(this), *Velocity.ToString()));
//	}
//
//	ApplyRootMotionToVelocity(deltaTime);
//
//	if (IsFalling())
//	{
//		// Root motion could have put us into Falling
//		StartNewPhysics(deltaTime, Iterations);
//		return;
//	}
//
//	Iterations++;
//
//	FVector DesiredMove = Velocity;
//	DesiredMove.Z = 0.f;
//
//	const FVector OldLocation = GetActorFeetLocation();
//	const FVector DeltaMove = DesiredMove * deltaTime;
//	const bool bDeltaMoveNearlyZero = DeltaMove.IsNearlyZero();
//
//	FVector AdjustedDest = OldLocation + DeltaMove;
//	FNavLocation DestNavLocation;
//
//	bool bSameNavLocation = false;
//	if (CachedNavLocation.NodeRef != INVALID_NAVNODEREF)
//	{
//		if (bProjectNavMeshWalking)
//		{
//			const float DistSq2D = (OldLocation - CachedNavLocation.Location).SizeSquared2D();
//			const float DistZ = FMath::Abs(OldLocation.Z - CachedNavLocation.Location.Z);
//
//			const float TotalCapsuleHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;
//			const float ProjectionScale = (OldLocation.Z > CachedNavLocation.Location.Z) ? NavMeshProjectionHeightScaleUp : NavMeshProjectionHeightScaleDown;
//			const float DistZThr = TotalCapsuleHeight * FMath::Max(0.f, ProjectionScale);
//
//			bSameNavLocation = (DistSq2D <= UE_KINDA_SMALL_NUMBER) && (DistZ < DistZThr);
//		}
//		else
//		{
//			bSameNavLocation = CachedNavLocation.Location.Equals(OldLocation);
//		}
//
//		if (bDeltaMoveNearlyZero && bSameNavLocation)
//		{
//			if (const INavigationDataInterface* NavData = GetNavData())
//			{
//				if (!NavData->IsNodeRefValid(CachedNavLocation.NodeRef))
//				{
//					CachedNavLocation.NodeRef = INVALID_NAVNODEREF;
//					bSameNavLocation = false;
//				}
//			}
//		}
//	}
//
//	if (bDeltaMoveNearlyZero && bSameNavLocation)
//	{
//		DestNavLocation = CachedNavLocation;
//		UE_LOG(LogTemp, VeryVerbose, TEXT("%s using cached navmesh location! (bProjectNavMeshWalking = %d)"), *GetNameSafe(CharacterOwner), bProjectNavMeshWalking);
//	}
//	else
//	{
//		//SCOPE_CYCLE_COUNTER(STAT_CharNavProjectPoint);
//
//		// Start the trace from the Z location of the last valid trace.
//		// Otherwise if we are projecting our location to the underlying geometry and it's far above or below the navmesh,
//		// we'll follow that geometry's plane out of range of valid navigation.
//		if (bSameNavLocation && bProjectNavMeshWalking)
//		{
//			AdjustedDest.Z = CachedNavLocation.Location.Z;
//		}
//
//		// Find the point on the NavMesh
//		const bool bHasNavigationData = FindNavFloor(AdjustedDest, DestNavLocation);
//		if (!bHasNavigationData)
//		{
//			//SetMovementMode(MOVE_Walking);
//			return;
//		}
//
//		CachedNavLocation = DestNavLocation;
//	}
//
//	if (DestNavLocation.NodeRef != INVALID_NAVNODEREF)
//	{
//		FVector NewLocation(AdjustedDest.X, AdjustedDest.Y, DestNavLocation.Location.Z);
//		if (bProjectNavMeshWalking)
//		{
//			//SCOPE_CYCLE_COUNTER(STAT_CharNavProjectLocation);
//			const float TotalCapsuleHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;
//			const float UpOffset = TotalCapsuleHeight * FMath::Max(0.f, NavMeshProjectionHeightScaleUp);
//			const float DownOffset = TotalCapsuleHeight * FMath::Max(0.f, NavMeshProjectionHeightScaleDown);
//			NewLocation = ProjectLocationFromNavMesh(deltaTime, OldLocation, NewLocation, UpOffset, DownOffset);
//		}
//
//		FVector AdjustedDelta = NewLocation - OldLocation;
//
//		if (!AdjustedDelta.IsNearlyZero())
//		{
//			FHitResult HitResult;
//			SafeMoveUpdatedComponent(AdjustedDelta, UpdatedComponent->GetComponentQuat(), bSweepWhileNavWalking, HitResult);
//		}
//
//		// Update velocity to reflect actual move
//		if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasVelocity())
//		{
//			Velocity = (GetActorFeetLocation() - OldLocation) / deltaTime;
//			MaintainHorizontalGroundVelocity();
//		}
//
//		bJustTeleported = false;
//	}
//	else
//	{
//		StartFalling(Iterations, deltaTime, deltaTime, DeltaMove, OldLocation);
//	}
//}

