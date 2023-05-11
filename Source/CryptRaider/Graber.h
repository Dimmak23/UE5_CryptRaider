// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// should be the last header always!
#include "Graber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UGraber : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGraber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TriggerGrab();

	UFUNCTION(BlueprintCallable)
	void GrabStuff();

	UFUNCTION(BlueprintCallable)
	void ReleaseStuff();

private:
	UPhysicsHandleComponent* GetPhysicsHandle() const;

	// utilities
	FVector StartPosition;	  // where the grabber now?
	FVector DestPosition;	  // how far the stick can get?

	// do we grab something already?
	bool weGotSomething{ false };

	// our virtual stick, that we use for grabing
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance{};
	// on the end ot the stick we have a sphere with such radius
	UPROPERTY(EditAnywhere)
	float GrabRadius{};
	// everything that collide this sphere we can grab
	FCollisionShape HitSphere;

	// but we hold object closer to us
	UPROPERTY(EditAnywhere)
	float HoldDistance{};

	// result of the searching for grabing
	FHitResult HitResult;
	// can we grab (on such distance, with such collision sphere)?
	bool HasHit{ false };

	// physics
	UPhysicsHandleComponent* PhysicsHandle;

	// target
	UPrimitiveComponent* GrabingTarget{ nullptr };
	// FVector CurrentTargetLocation;
	FRotator ObjectRotation;
};
