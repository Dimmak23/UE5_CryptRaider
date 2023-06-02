// Fill out your copyright notice in the Description page of Project Settings.

#include "MultipleMover.h"

UMultipleMover::UMultipleMover() : UMover()
{
	//
	States.resize(CellsQty);
}

void UMultipleMover::SetterWrapper(const bool& _State, const uint8& _CellNumber)
{
	if (_State)
	{
		States.at(_CellNumber) = true;
		if (std::all_of(States.begin(), States.end(), [](bool State) { return State; }))
		{
			this->SetMoveByOffset(true);
		}
	}
	else
	{
		States.at(_CellNumber) = false;
		if (std::all_of(States.begin(), States.end(), [](bool State) { return !State; }))
		{
			this->SetMoveByOffset(false);
		}
	}
}
