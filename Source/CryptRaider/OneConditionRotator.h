// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// STL
#include <vector>

// Unreal Engine
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

// should be the last header always!
#include "OneConditionRotator.generated.h"

/**
 *@class single one condition rotator
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UOneConditionRotator : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOneConditionRotator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	FRotator RotationAngle;

	UPROPERTY(EditAnywhere)
	float RotationTime{ 4.0f };

	bool RotateByAngle{ false };

	UPROPERTY(EditAnyWhere)
	uint8 CellsQty{ 1 };

	uint8 AquiredQty{};

	// std::vector<bool> States;
	bool* TriggerStates;

	// utilities
	FRotator OriginalRotation;
	FRotator TargetRotation{};
	float Speed{};

public:
	// encapsulate RotateByAngle
	UFUNCTION(BlueprintCallable, Category = Door)
	void SetRotateByAngle(const bool& __State);
	UFUNCTION(BlueprintCallable, Category = Door)
	bool GetRotateByAngle() const;

	// All triggers call this
	UFUNCTION(BlueprintCallable, Category = Door)
	void SetterWrapper(const bool& __State, const uint8& __CellNumber);

	// Utility
	bool CheckAllCells(const bool* __CheckThis);
};
