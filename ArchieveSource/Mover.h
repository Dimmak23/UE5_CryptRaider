// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

// should be the last header always!
#include "Mover.generated.h"

/**
 *@class mover component
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	// encapsulate InitMove
	UFUNCTION(BlueprintCallable, Category = Door)
	void SetMoveByOffset(const bool& value);
	UFUNCTION(BlueprintCallable, Category = Door)
	bool GetMoveByOffset() const;

	// All triggers call this
	virtual void SetterWrapper(const bool& _State, const uint8& _CellNumber) {}

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
};
