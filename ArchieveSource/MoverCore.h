#pragma once

#include "CoreMinimal.h"

// should be the last header always!
#include "MoverCore.generated.h"

UCLASS(BlueprintType, ClassGroup = (Custom))
class MoverCore
{
	GENERATED_BODY()

public:
	// encapsulate InitMove
	UFUNCTION(BlueprintCallable, Category = Door)
	void SetMoveByOffset(const bool& _ParsedState) { MoveByOffset = _ParsedState; }
	UFUNCTION(BlueprintCallable, Category = Door)
	bool GetMoveByOffset() const { return MoveByOffset; }

	// All triggers call this
	UFUNCTION(BlueprintCallable, Category = Door)
	virtual void SetterWrapper(const bool& _State, const uint8& _CellNumber) = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector MoveOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MoveTime{ 4.0f };

	bool MoveByOffset{ false };

	// utilities
	FVector OriginalLocation;
	FVector TargetLocation{};
	float Speed{};
};

UCLASS(BlueprintType, ClassGroup = (Custom))
class SimpleMoverCore : public MoverCore
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Door)
	virtual void SetterWrapper(const bool& _State, const uint8& _CellNumber) override
	{
		//
		this->SetMoveByOffset(_State);
	}
};

UCLASS(BlueprintType, ClassGroup = (Custom))
class MultipleMoverCore : public MoverCore
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Door)
	virtual void SetterWrapper(const bool& _State, const uint8& _CellNumber) override
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint8 CellsQty{};

	std::vector<bool> States;
};
