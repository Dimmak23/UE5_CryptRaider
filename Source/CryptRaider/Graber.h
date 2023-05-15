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

	bool isGotSomething() const;

private:
	// physics
	UPhysicsHandleComponent* GetPhysicsHandle() const;

	// hit utility
	bool CheckHitResult(FHitResult& __HitResult__) const;

	// our virtual stick, that we use for grabing
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance{};
	// on the end ot the stick we have a sphere with such radius
	UPROPERTY(EditAnywhere)
	float GrabRadius{};
	// where we hold the object?
	UPROPERTY(EditAnywhere)
	float HoldDistance{};

	// physics
	UPhysicsHandleComponent* PhysicsHandle{ nullptr };

	// target
	UPrimitiveComponent* GrabingTarget{ nullptr };

	// do we grab something already?
	bool weGotSomething{ false };
};
