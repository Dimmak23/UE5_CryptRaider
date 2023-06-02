// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Graber.h"
#include "TriggerCore.h"

// should be the last header always!
#include "TriggerComponent.generated.h"

/**
 *@class tag trigger collision component
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// how we deal with different triggers?
	virtual void ProceedWrapper() {}

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// how we answer to the ActorAccepted() signal?
	void ConnectActorToComponent(AActor* _ParsedActor);
};
