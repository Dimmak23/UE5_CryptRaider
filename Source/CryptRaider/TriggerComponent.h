// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Graber.h"
#include "Mover.h"

// always last
#include "TriggerComponent.generated.h"

/**
 *@class collision component
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

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* _ParsedMover);

private:
	// check: can we accepted this actor?
	bool ActorAccepted() const;

	// what we can accept?
	UPROPERTY(EditAnywhere)
	FName UnlockingTag;

	// what mover we will control?
	UMover* Mover{ nullptr };
};
