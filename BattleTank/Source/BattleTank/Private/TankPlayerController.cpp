// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "CollisionQueryParams.h"
#include "TankAimingComponent.h"

ATankPlayerController::ATankPlayerController()
	: APlayerController()
	, CrossHairPosition(0.5f, 0.33333f)
	, LineTraceRange(1000000.0f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; } // e.g. if not possessing
	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(AimingComponent != nullptr))
	{
		FVector HitLocation{ 0, 0, 0 };

		if (GetSightRayHitLocation(HitLocation))
		{
			AimingComponent->AimAt(HitLocation);
		}
	}

}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(AimingComponent != nullptr))
	{
		FoundAimingComponent(AimingComponent);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player controller can't find aiming component at begin play"));
	}
}

bool ATankPlayerController::GetLookDirection(const FVector2D& ScreenLocation, FVector& LookDirection) const
{
	FVector WorldLoctaion;
	// De-project the screen position of the crosshair to a world direction
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		WorldLoctaion,
		LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(
	const FVector& LookDirection, FHitResult& HitResult) const
{
	FTwoVectors TraceLine = GetReachLineEnds(LookDirection);
	return GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceLine.v1,
		TraceLine.v2,
		ECollisionChannel::ECC_Visibility
	);
}

FTwoVectors ATankPlayerController::GetReachLineEnds(const FVector& LookDirection) const
{
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector LineEnd = StartLocation + LookDirection * LineTraceRange;

	return FTwoVectors(StartLocation, LineEnd);
}


bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{

	// Find crosshair position in pixel coords.
	FIntPoint ViewportSize;
	GetViewportSize(ViewportSize.X, ViewportSize.Y);
	FVector2D ScreenLocation(
		ViewportSize.X * CrossHairPosition.X,
		ViewportSize.Y * CrossHairPosition.Y);
	
	FVector LookDirection;
	if(GetLookDirection(ScreenLocation, LookDirection))
	{
		FHitResult HitResult;
		// Line trace along that look direction, and see what we hit (up to max range)
		if (GetLookVectorHitLocation(LookDirection, HitResult))
		{
			HitLocation = HitResult.Location;
			return true;
		}
	}

	return false;
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AimTowardsCrosshair();
}
