// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::AimAt(const FVector& AimLocation)
{
	if (ensure(Barrel != nullptr))
	{
		FVector LaunchVelocity;
		FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

		if (UGameplayStatics::SuggestProjectileVelocity(
			this,
			LaunchVelocity,
			StartLocation,
			AimLocation,
			LaunchSpeed,
			false,
			0.0f,
			0.0f,
			ESuggestProjVelocityTraceOption::DoNotTrace
		))
		{
			AimDirection = LaunchVelocity.GetSafeNormal();
			MoveBarrel(AimDirection);
		}
	}
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// So the first fire after initial reload.
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel != nullptr))
	{
		return;
	}

	if (State != EState::Reloading)
	{
		FVector SpawnLocation = Barrel->GetSocketLocation(FName("Projectile"));
		FRotator SpawnRotation = Barrel->GetSocketRotation(FName("Projectile"));
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBP, SpawnLocation, SpawnRotation);

		if (ensure(Projectile != nullptr))
		{
			Projectile->Launch(LaunchSpeed);
			LastFireTime = FPlatformTime::Seconds();
		}
	}
}

EState UTankAimingComponent::GetState() const
{
	return State;
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	if (ensure(BarrelToSet != nullptr) || ensure(TurretToSet != nullptr))
	{
		Barrel = BarrelToSet;
		Turret = TurretToSet;
	}
}

bool UTankAimingComponent::IsBarrelMoving() const
{
	if (!ensure(Barrel))
	{
		return false;
	}

	return !AimDirection.Equals(Barrel->GetForwardVector().GetSafeNormal(), 0.05f);
}

void UTankAimingComponent::MoveBarrel(const FVector& AimDirection) const
{
	if (!ensure(Barrel) || !ensure(Turret))
	{
		return;
	}

	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator Diff = AimAsRotator - BarrelRotator;

	Barrel->Elevate(Diff.Pitch);
	Turret->Rotate(Diff.GetNormalized().Yaw);
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTime)
	{
		State = EState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		State = EState::Aiming;
	}
	else
	{
		State = EState::Ready;
	}
}
