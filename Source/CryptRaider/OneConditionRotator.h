//* Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//? Unreal Engine
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"

//? Should be the last header always!
#include "OneConditionRotator.generated.h"

/**
 *@class single one condition rotator
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UOneConditionRotator : public UActorComponent
{
	GENERATED_BODY()

public:
	//? Sets default values for this component's properties
	UOneConditionRotator();
	// virtual ~UOneConditionRotator() noexcept;

protected:
	//? Called when the game starts
	virtual void BeginPlay() override;

public:
	//? Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Rotator")
	bool Inverted{ false };

	UPROPERTY(EditAnywhere, Category = "Rotator")
	FRotator RotationAngle;

	UPROPERTY(EditAnywhere, Category = "Rotator")
	float RotationTime{ 4.0f };

	UPROPERTY(EditAnywhere, Category = "Rotator")
	USoundBase* RotateSound;

	UAudioComponent* RotateAudioComponent;

	UPROPERTY(EditAnyWhere)
	uint8 CellsQty{ 1 };

	bool RotateByAngle{ false };

	uint8 AquiredQty{};

	bool* TriggerStates;

	//? Utilities
	FRotator OriginalRotation;
	FRotator TargetRotation{};
	float Speed{};

public:
	//? Encapsulate RotateByAngle
	UFUNCTION(BlueprintCallable, Category = Door)
	void SetRotateByAngle(const bool& __State);
	UFUNCTION(BlueprintCallable, Category = Door)
	bool GetRotateByAngle() const;

	//? All triggers call this
	UFUNCTION(BlueprintCallable, Category = Door)
	void SetterWrapper(const bool& __State, const uint8& __CellNumber);

	//? Utility
	bool AllIsTriggered(/* const bool* __CheckThis */);

	void LogAllCells();
};
