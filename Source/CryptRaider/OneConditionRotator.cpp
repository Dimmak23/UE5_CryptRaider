//* Fill out your copyright notice in the Description page of Project Settings.

#include "OneConditionRotator.h"

#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

//@ Sets default values for this component's properties
UOneConditionRotator::UOneConditionRotator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

//@ Called when the game starts
void UOneConditionRotator::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// States.resize(CellsQty);
	TriggerStates = new bool[CellsQty];
	for (int _index{}; _index < CellsQty; _index++)
	{
		TriggerStates[_index] = false;
	}

	// ...
	OriginalRotation = GetOwner()->GetActorRotation();
	Speed = RotationAngle.Euler().Length() / RotationTime;
	RotateAudioComponent = UGameplayStatics::CreateSound2D(this, RotateSound, 1.0f, 1.0f, 0.0f);
}

//@ Called every frame
void UOneConditionRotator::TickComponent(float DeltaTime, ELevelTick TickType,
										 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//? Check do we need to rotate
	this->SetRotateByAngle(this->AllIsTriggered());

	if (RotateByAngle)	  //? Rotate to the target when allowed
	{
		TargetRotation = OriginalRotation + RotationAngle;
	}
	else	//? Comeback when Rotator are dissallowed
	{
		TargetRotation = OriginalRotation;
	}

	//? Update rotation
	FRotator _CurrentRotation = GetOwner()->GetActorRotation();
	FRotator _NewRotation = FMath::RInterpConstantTo(_CurrentRotation, TargetRotation, DeltaTime, Speed);
	GetOwner()->SetActorRotation(_NewRotation);
}

//@ We can control in the blueprints when to rotate the actor
void UOneConditionRotator::SetRotateByAngle(const bool& __State)
{
	if (__State != GetRotateByAngle())	  //? Do nothing if no new info given
	{
		UE_LOG(LogTemp, Warning, TEXT("New __State paresed = %d"), (int)__State);

		RotateByAngle = __State;
		UE_LOG(LogTemp, Warning, TEXT("RotateByAngle = %d"), (int)RotateByAngle);

		//? If door is inverted we are going to stay - if we got object in trigger area
		//? or we going to move if we don't have object
		if (Inverted) RotateByAngle = !RotateByAngle;

		if (RotateSound && RotateByAngle)
		{
			//! No need in this, we re-assign 'MoveAudioComponent' later
			// if (RotateAudioComponent && RotateAudioComponent->IsPlaying()) RotateAudioComponent->Stop();

			UE_LOG(LogTemp, Warning, TEXT("Playing open dungeon sound..."));

			RotateAudioComponent =
				UGameplayStatics::SpawnSoundAtLocation(this, RotateSound, GetOwner()->GetActorLocation());
		}
	}
}

//@ Read from the blueprints value
bool UOneConditionRotator::GetRotateByAngle() const { return RotateByAngle; }

//@ Return 'true' - all cells are in true state;
//@ Return 'false' - at least one cell in the false state;
bool UOneConditionRotator::AllIsTriggered(/* const bool* __CheckThis */)
{
	int _result{ 1 };
	for (int _index{}; _index < CellsQty; _index++)
	{
		_result *= (int)TriggerStates[_index];
	}
	return (bool)_result;
}

void UOneConditionRotator::LogAllCells()
{
	for (int _index{}; _index < CellsQty; _index++)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerStates[%d] = %d"), _index, (int)TriggerStates[_index]);
	}
}

//@ Triggers API
void UOneConditionRotator::SetterWrapper(const bool& __State, const uint8& __CellNumber)
{
	//? Do nothing if there is no new info
	if (__State == TriggerStates[__CellNumber]) return;

	if (__State)
	{
		TriggerStates[__CellNumber] = true;

		this->LogAllCells();
	}
	else
	{
		TriggerStates[__CellNumber] = false;

		this->LogAllCells();
	}
}
