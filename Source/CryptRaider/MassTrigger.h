// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "OneConditionMover.h"
#include "OneConditionRotator.h"

// should be the last header always!
#include "MassTrigger.generated.h"

/**
 *@class mass trigger box area
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UMassTrigger : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMassTrigger();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	// OneConditionMover interface
	UFUNCTION(BlueprintCallable)
	void SetOCMover(UOneConditionMover* __ParsedOCMover);

	// OneConditionRotator interface
	UFUNCTION(BlueprintCallable)
	void SetOCRotator(UOneConditionRotator* __ParsedOCRotator);

	// Lighter interface
	UFUNCTION(BlueprintCallable)
	bool GetTriggingState() const;

private:
	// how we answer to the ActorAccepted() signal?
	void ConnectActorToComponent();
	// how we deal with different triggers?
	// void ProceedWrapper();
	//
	AActor* GetAcceptedActor();

private:
	//
	UPROPERTY(EditAnyWhere)
	float MinUnlockingMass;

	//
	UPROPERTY(EditAnyWhere)
	bool IsDisablePhysics{ true };

	//
	FName Forbids;

	//
	UPROPERTY(EditAnyWhere)
	uint8 CellNumber{};

	// if we would have any
	UOneConditionMover* OCMover{ nullptr };

	// if we would have any
	UOneConditionRotator* OCRotator{ nullptr };

	//
	bool TriggingState{ false };
};
