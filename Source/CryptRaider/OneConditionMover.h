// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

// should be the last header always!
#include "OneConditionMover.generated.h"

/**
 *@class single one condition mover
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UOneConditionMover : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOneConditionMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	FVector MoveOffset;

	UPROPERTY(EditAnywhere)
	float MoveTime{ 4.0f };

	bool MoveByOffset{ false };

	// utilities
	FVector OriginalLocation;
	FVector TargetLocation{};
	float Speed{};

public:
	// encapsulate InitMove
	UFUNCTION(BlueprintCallable, Category = Door)
	void SetMoveByOffset(const bool& __State);
	UFUNCTION(BlueprintCallable, Category = Door)
	bool GetMoveByOffset() const;

	// All triggers call this
	UFUNCTION(BlueprintCallable, Category = Door)
	void SetterWrapper(const bool& __State, const uint8& __CellNumber);
};
