// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	APawn* ControlledTank = GetPawn();

	if (!ensure(PlayerTank && ControlledTank))
	{
		return;
	}

	// Move towards the player.
	MoveToActor(PlayerTank, AcceptanceRadius);
	UTankAimingComponent* AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	// Aim at the player.
	AimingComponent->AimAt(PlayerTank->GetActorLocation());
	
	// Fire if ready.
	if (AimingComponent->GetState() == EState::Ready)
	{
		AimingComponent->Fire(); // TODO: limit fire limit
	}
}
