// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Mover.generated.h"

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
	void SetInitMove(const bool& value);
	UFUNCTION(BlueprintCallable, Category = Door)
	bool GetInitMove() const;

private:
	UPROPERTY(EditAnywhere)
	FVector MoveOffset;

	UPROPERTY(EditAnywhere)
	float MoveTime{ 4.0f };

	UPROPERTY(EditAnywhere)
	bool InitMove{ false };

	// utilities
	// UPROPERTY(EditAnywhere)
	FVector OriginalLocation;
	FVector TargetLocation{};
	float Speed{};
};
