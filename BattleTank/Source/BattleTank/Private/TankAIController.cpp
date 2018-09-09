// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

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

void ATankAIController::OnTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("GOT TANK DEATH EVENT"));
	
	if (!GetPawn())
	{
		return;
	}
	
	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn != nullptr)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) 
		{
			return;
		}

		// Subscribe to the tank's death event.
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
	}
}
