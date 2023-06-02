// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerComponent.h"

// should be the last header always!
#include "SingleTrigger.generated.h"

/**
 *
 */
UCLASS()
class CRYPTRAIDER_API USingleTrigger : public UTriggerComponent
{
	GENERATED_BODY()

protected:
	// SingleTrigger implementation
	virtual void ProceedWrapper() override;

	//
	TriggerCore* Trigger{ nullptr };
};
