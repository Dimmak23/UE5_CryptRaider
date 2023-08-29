//* Designed and produced by Dmytro Kovryzhenko, all rights reserved

#pragma once

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Engine/World.h"

//! Should be the last header always!
#include "AudioTrigger.generated.h"

/**
 *@ class audio trigger component
 *@ trigger when overlap with something
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UAudioTrigger : public UBoxComponent
{
	GENERATED_BODY()

public:
	//? Sets default values for this component's properties
	UAudioTrigger();

protected:
	//? Called when the game starts
	virtual void BeginPlay() override;

public:
	//? Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

private:
	//^ Utilities
	void FetchOverlaps();

	UPROPERTY(EditAnywhere, Category = "Audio Trigger")
	USoundBase* ThrowSound;

	UPROPERTY(EditAnywhere, Category = "Audio Trigger")
	USoundBase* HitSound;

	UPROPERTY(EditAnyWhere, Category = "Audio Trigger")
	FName ThrowingTrigger{ FName(TEXT("GeneratesThrowSound")) };

	UPROPERTY(EditAnyWhere, Category = "Audio Trigger")
	FName HittingTrigger{ FName(TEXT("GeneratesHitSound")) };

	//
	FName Forbids{ FName(TEXT("Grabbed")) };

	UAudioComponent* AudioComponent;

	bool AlreadyPlaying{ true };

	// AActor* OverlappedActor{ nullptr };
};
