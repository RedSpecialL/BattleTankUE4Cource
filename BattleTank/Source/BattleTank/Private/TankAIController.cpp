// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	ATank* Tank = GetControlledTank();
	if (Tank == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AI controller not possesing a tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI controller possesing %s"), *Tank->GetName())
	}

	ATank* PlayerTank = GetPlayerTank();
	if (PlayerTank == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AI controller cannot find player tank."))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI controller found player tank: %s"), *PlayerTank->GetName())
	}
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find player pawn"));
		return nullptr;
	}

	return Cast<ATank>(PlayerPawn);
}
