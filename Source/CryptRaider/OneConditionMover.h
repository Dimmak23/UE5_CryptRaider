//* Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"

//? Should be the last header always!
#include "OneConditionMover.generated.h"

class USoundBase;

/**
 *@class single one condition mover
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UOneConditionMover : public UActorComponent
{
	GENERATED_BODY()

public:
	//? Sets default values for this component's properties
	UOneConditionMover();

protected:
	//? Called when the game starts
	virtual void BeginPlay() override;

public:
	//? Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Mover")
	bool Inverted{ false };

	UPROPERTY(EditAnywhere, Category = "Mover")
	FVector MoveOffset;

	UPROPERTY(EditAnywhere, Category = "Mover")
	float MoveTime{ 4.0f };

	UPROPERTY(EditAnywhere, Category = "Mover")
	USoundBase* MoveSound;

	UAudioComponent* MoveAudioComponent;

	bool MoveByOffset{ false };

	//? Utilities
	FVector OriginalLocation;
	FVector TargetLocation{};
	float Speed{};

	bool AlreadyPlayingSound{ false };

public:
	//? Encapsulate InitMove
	UFUNCTION(BlueprintCallable, Category = Door)
	void SetMoveByOffset(const bool& __State);
	UFUNCTION(BlueprintCallable, Category = Door)
	bool GetMoveByOffset() const;

	//? All triggers call this
	UFUNCTION(BlueprintCallable, Category = Door)
	void SetterWrapper(const bool& __State, const uint8& __CellNumber);
};
