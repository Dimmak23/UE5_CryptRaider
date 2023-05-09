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
	void GrabStuff();

	UFUNCTION(BlueprintCallable)
	void ReleaseStuff();

private:
	// utilities
	FVector StartPosition;
	FVector DestPosition;

	// our virtual stick, that we use for grabing
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance{};

	UPROPERTY(EditAnywhere)
	float GrabRadius{};

	// but we hold object closer to us
	UPROPERTY(EditAnywhere)
	float HoldDistance{};

	FCollisionShape HitSphere;
	FHitResult HitResult;
	bool HasHit{ false };
	// physics
	UPhysicsHandleComponent* PhysicsHandle;

	// target
	FRotator ObjectRotation;

	UPhysicsHandleComponent* GetPhysicsHandle() const;
};
