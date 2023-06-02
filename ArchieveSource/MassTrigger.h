// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SingleTrigger.h"

// should be the last header always!
#include "MassTrigger.generated.h"

/**
 *
 */
UCLASS()
class CRYPTRAIDER_API UMassTrigger : public USingleTrigger
{
	GENERATED_BODY()

public:
	UMassTrigger() { Trigger = new MassTriggerCore(); }
	~UMassTrigger() { delete Trigger; }

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
};
