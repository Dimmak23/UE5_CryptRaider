#pragma once

#include "Mover.h"

class TriggerCore
{
protected:
	// how we verify?
	virtual bool SpecificCondition(AActor* _Actor) = 0;

	FName Forbids{ "Grabbed" };

public:
	// how we move something?
	AActor* ConditionalMove(TArray<AActor*>& _ParsedArray)
	{
		for (auto Actor : _ParsedArray)
		{
			if (SpecificCondition(Actor))
			{
				this->Mover->SetterWrapper(true, CellNumber);
				return Actor;
			}
		}
		this->Mover->SetterWrapper(false, CellNumber);
		return nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = TriggerCore)
	void SetMover(UMover* _ParsedMover) { Mover = _ParsedMover; }

public:
	UPROPERTY(EditAnyWhere)
	uint8 CellNumber{};
	//
	UMover* Mover;
};

class TagTriggerCore : public TriggerCore
{
private:
	bool SpecificCondition(AActor* _Actor) override
	{
		return (_Actor->ActorHasTag(UnlockingTag) && !_Actor->ActorHasTag(Forbids));
	}

public:
	UPROPERTY(EditAnyWhere)
	FName UnlockingTag;
};

class MassTriggerCore : public TriggerCore
{
private:
	bool SpecificCondition(AActor* _Actor) override
	{
		UPrimitiveComponent* _RootComponent = Cast<UPrimitiveComponent>(_Actor->GetRootComponent());
		float _ActorMass = _RootComponent->GetMass();

		return ((_ActorMass >= MinUnlockingMass) && !_Actor->ActorHasTag(Forbids));
	}

public:
	UPROPERTY(EditAnyWhere)
	float MinUnlockingMass{};
};
