// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerComponent.h"

// should be the last header always!
#include "DoubleTrigger.generated.h"

/**
 *
 */
UCLASS()
class CRYPTRAIDER_API UDoubleTrigger : public UTriggerComponent
{
	GENERATED_BODY()
public:
	UDoubleTrigger()
	{
		TagTrigger = new TagTriggerCore();
		MassTrigger = new MassTriggerCore();
	}
	~UDoubleTrigger()
	{
		delete TagTrigger;
		delete MassTrigger;
	}

protected:
	// DoubleTrigger implementation
	virtual void ProceedWrapper() override;

	//
	TriggerCore* TagTrigger{ nullptr };
	TriggerCore* MassTrigger{ nullptr };
};
