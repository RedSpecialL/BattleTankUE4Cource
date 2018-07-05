// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
#include "CollisionQueryParams.h"

ATankPlayerController::ATankPlayerController()
	: APlayerController()
	, CrossHairPosition(0.5f, 0.33333f)
	, ShotRange(1000000.0f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (GetControlledTank() == nullptr)
	{
		return;
	}

	FVector HitLocation{ 0, 0, 0 };

	if (GetSightRayHitLocation(HitLocation))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit location: %s"), *HitLocation.ToString());
		// TODO: Tell controlled tank to aim at this point
	}
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ATank* MyTank = GetControlledTank();
	if (MyTank == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller not possesing a tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player controller possesing %s"), *MyTank->GetName())
	}
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
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

bool ATankPlayerController::GetLookVectorHitLocation(const FVector& LookDirection, FHitResult& HitResult) const
{
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	
	FTwoVectors TraceLine = GetReachLineEnds(LookDirection);
	return GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceLine.v1,
		TraceLine.v2,
		ECollisionChannel::ECC_Visibility,
		TraceParameters
	);
}

FTwoVectors ATankPlayerController::GetReachLineEnds(const FVector& LookDirection) const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRatation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation, PlayerViewPointRatation);

	FVector LineEnd = PlayerViewPointLocation + LookDirection * ShotRange;
	return FTwoVectors(PlayerViewPointLocation, LineEnd);
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
		if (GetLookVectorHitLocation(LookDirection, HitResult))
		{
			FVector HitLocation = HitResult.GetActor()->GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("Screen to world direction: %s"), *HitLocation.ToString())
		}
	}

	// Line trace along that look direction, and see what we hit (up to max range)
	return true;
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AimTowardsCrosshair();
}
