// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mover.h"

// should be the last header always!
#include "MultipleMover.generated.h"

/**
 *
 */
UCLASS()
class CRYPTRAIDER_API UMultipleMover : public UMover
{
	GENERATED_BODY()

public:
	UMultipleMover();

	UFUNCTION(BlueprintCallable, Category = Door)
	void SetterWrapper(const bool& _State, const uint8& _CellNumber) override;

private:
	UPROPERTY(EditAnyWhere)
	unsigned int CellsQty;

	std::vector<bool> States;
};
