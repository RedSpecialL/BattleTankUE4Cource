// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

#include "Projectile.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = StartingHealth;
}

float ATank::GetHealthPercent() const
{
	return static_cast<float>(CurrentHealth) / static_cast<float>(StartingHealth);
}

float ATank::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(Damage);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);
	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0) {
		OnDeath.Broadcast();
	}

	return static_cast<float>(DamageToApply);
}
