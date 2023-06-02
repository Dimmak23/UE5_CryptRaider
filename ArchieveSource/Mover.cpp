// Fill out your copyright notice in the Description page of Project Settings.

#include "Mover.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OriginalLocation = GetOwner()->GetActorLocation();
	// TargetLocation = OriginalLocation + MoveOffset;
	Speed = MoveOffset.Length() / MoveTime;
}

// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MoveByOffset)	 // Move to the target when allowed
	{
		TargetLocation = OriginalLocation + MoveOffset;
	}
	else	// Comeback when Mover are dissallowed
	{
		TargetLocation = OriginalLocation;
	}

	FVector _CurrentLocation = GetOwner()->GetActorLocation();
	// FRotator _CurrentAngles = GetOwner()->GetActorRotation();
	// FRotator TargetAngles = _CurrentAngles + FRotator(0, 0, 90);
	FVector _NewLocation = FMath::VInterpConstantTo(_CurrentLocation, TargetLocation, DeltaTime, Speed);
	// FRotator _NewAngle = FMath::RInterpConstantTo(_CurrentAngles, TargetAngles, DeltaTime, Speed);
	GetOwner()->SetActorLocation(_NewLocation);
	// GetOwner()->SetActorRotation(_NewAngle);
}

// We can control in the blueprints when to move the actor
void UMover::SetMoveByOffset(const bool& value) { MoveByOffset = value; }

// Read from the blueprints value
bool UMover::GetMoveByOffset() const { return MoveByOffset; }
