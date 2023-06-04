// Fill out your copyright notice in the Description page of Project Settings.

#include "OneConditionRotator.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UOneConditionRotator::UOneConditionRotator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOneConditionRotator::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// States.resize(CellsQty);
	TriggerStates = new bool[CellsQty];

	// ...
	OriginalRotation = GetOwner()->GetActorRotation();
	Speed = RotationAngle.Euler().Length() / RotationTime;
}

// Called every frame
void UOneConditionRotator::TickComponent(float DeltaTime, ELevelTick TickType,
										 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (RotateByAngle)	  // Rotate to the target when allowed
	{
		TargetRotation = OriginalRotation + RotationAngle;
	}
	else	// Comeback when Rotator are dissallowed
	{
		TargetRotation = OriginalRotation;
	}

	// Update rotation
	FRotator _CurrentRotation = GetOwner()->GetActorRotation();
	FRotator _NewRotation = FMath::RInterpConstantTo(_CurrentRotation, TargetRotation, DeltaTime, Speed);
	GetOwner()->SetActorRotation(_NewRotation);
}

// We can control in the blueprints when to rotate the actor
void UOneConditionRotator::SetRotateByAngle(const bool& __State) { RotateByAngle = __State; }

// Read from the blueprints value
bool UOneConditionRotator::GetRotateByAngle() const { return RotateByAngle; }

// Return 'true' - all cells are in true state;
// Return 'false' - at least one cell in the false state;
bool UOneConditionRotator::CheckAllCells(const bool* __CheckThis)
{
	int _result{};
	for (int _index{}; _index < CellsQty; _index++)
	{
		_result *= (int)__CheckThis[_index];
	}
	return (bool)_result;
}

// Triggers API
void UOneConditionRotator::SetterWrapper(const bool& __State, const uint8& __CellNumber)
{
	if (__State)
	{
		// States[__CellNumber] = true;
		TriggerStates[__CellNumber] = true;
		AquiredQty += 1;
		// if (std::all_of(States.begin(), States.end(), [](bool State) { return State; }))
		// if (this->CheckAllCells(TriggerStates))
		// {
		if (AquiredQty == CellsQty) this->SetRotateByAngle(true);
		// }
	}
	else
	{
		// States[__CellNumber] = false;
		TriggerStates[__CellNumber] = false;
		if (AquiredQty > 0) AquiredQty -= 1;
		// if (std::all_of(States.begin(), States.end(), [](bool State) { return !State; }))
		// if (!this->CheckAllCells(TriggerStates))
		// {
		if (AquiredQty != CellsQty) this->SetRotateByAngle(false);
		// }
	}
}
