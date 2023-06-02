// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleMover.h"

// Sets default values for this component's properties
USimpleMover::USimpleMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void USimpleMover::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OriginalLocation = GetOwner()->GetActorLocation();
	// TargetLocation = OriginalLocation + MoveOffset;
	Speed = MoveOffset.Length() / MoveTime;
}

// Called every frame
void USimpleMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (MoveByOffset)	 // Move to the target when allowed
	{
		TargetLocation = OriginalLocation + MoveOffset;
	}
	else	// Comeback when Mover are dissallowed
	{
		TargetLocation = OriginalLocation;
	}
	FVector _CurrentLocation = GetOwner()->GetActorLocation();
	FVector _NewLocation = FMath::VInterpConstantTo(_CurrentLocation, TargetLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(_NewLocation);
}
