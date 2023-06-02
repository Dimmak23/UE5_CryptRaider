// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "OneConditionMover.h"

// should be the last header always!
#include "TagTrigger.generated.h"

/**
 *@class tag trigger collision component
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTagTrigger : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTagTrigger();

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

private:
	// how we answer to the ActorAccepted() signal?
	void ConnectActorToComponent();
	// how we deal with different triggers?
	// void ProceedWrapper();
	//
	AActor* GetAcceptedActor() const;

private:
	UPROPERTY(EditAnyWhere)
	FName UnlockingTag;

	FName Forbids;

	UPROPERTY(EditAnyWhere)
	uint8 CellNumber{};

	// if we would have any
	UOneConditionMover* OCMover{ nullptr };
};
