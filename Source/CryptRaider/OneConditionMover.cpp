//* Fill out your copyright notice in the Description page of Project Settings.

#include "OneConditionMover.h"

#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

//@ Sets default values for this component's properties
UOneConditionMover::UOneConditionMover()
{
	//? Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	//? features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

//@ Called when the game starts
void UOneConditionMover::BeginPlay()
{
	Super::BeginPlay();

	//? Setup on begin
	OriginalLocation = GetOwner()->GetActorLocation();
	Speed = MoveOffset.Length() / MoveTime;
	MoveAudioComponent = UGameplayStatics::CreateSound2D(this, MoveSound, 1.0f, 1.0f, 0.0f);
}

//@ Called every frame
void UOneConditionMover::TickComponent(float DeltaTime, ELevelTick TickType,
									   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//? Finally move
	if (MoveByOffset)	 //? Move to the target when allowed
	{
		TargetLocation = OriginalLocation + MoveOffset;
	}
	else	//? Comeback when Mover are dissallowed
	{
		TargetLocation = OriginalLocation;
	}

	//? Update location
	FVector _CurrentLocation = GetOwner()->GetActorLocation();
	FVector _NewLocation = FMath::VInterpConstantTo(_CurrentLocation, TargetLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(_NewLocation);
}

//@ We can control in the blueprints when to move the actor
void UOneConditionMover::SetMoveByOffset(const bool& __State)
{
	if (__State != GetMoveByOffset())
	{
		MoveByOffset = __State;

		//? If door is inverted we are going to stay - if we got object in trigger area
		//? or we going to move if we don't have object
		if (Inverted) MoveByOffset = !MoveByOffset;

		if (MoveSound)
		{
			//! No need in this, we re-assign 'MoveAudioComponent' later
			// if (MoveAudioComponent && MoveAudioComponent->IsPlaying()) MoveAudioComponent->Stop();

			MoveAudioComponent =
				UGameplayStatics::SpawnSoundAtLocation(this, MoveSound, GetOwner()->GetActorLocation());
		}
	}
}

//@ Read from the blueprints value
bool UOneConditionMover::GetMoveByOffset() const
{
	//? If door is inverted we are going to stay - if we got object in trigger area
	//? or we going to move if we don't have object
	return Inverted ? !MoveByOffset : MoveByOffset;
}

//@ Could be usefull for future polymorphism
void UOneConditionMover::SetterWrapper(const bool& __State, const uint8& __CellNumber)
{
	this->SetMoveByOffset(__State);
}
