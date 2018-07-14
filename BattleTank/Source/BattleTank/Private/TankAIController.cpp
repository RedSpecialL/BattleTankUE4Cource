// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ATank* ControlledTank = Cast<ATank>(GetPawn());
	if(PlayerTank != nullptr)
	{
		// Move towards the player.

		// Aim at the player.
		ControlledTank->AimAt(PlayerTank->GetActorLocation());
	
		// Fire if ready.
		ControlledTank->Fire(); // TODO: limit fire limit
	}
}
