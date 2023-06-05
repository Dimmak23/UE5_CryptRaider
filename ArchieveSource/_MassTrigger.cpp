// Fill out your copyright notice in the Description page of Project Settings.

#include "MassTrigger.h"

// Sets default values for this component's properties
UMassTrigger::UMassTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Forbids = "Grabbed";
}

// Called when the game starts
void UMassTrigger::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UMassTrigger::TickComponent(					 //
	float DeltaTime,								 //
	ELevelTick TickType,							 //
	FActorComponentTickFunction* ThisTickFunction	 //
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Do we have any?
	this->ConnectActorToComponent();
}

// We set Mover in the blueprint
void UMassTrigger::SetOCMover(UOneConditionMover* __ParsedOCMover) { OCMover = __ParsedOCMover; }

// We set Rotator in the blueprint
void UMassTrigger::SetOCRotator(UOneConditionRotator* __ParsedOCRotator) { OCRotator = __ParsedOCRotator; }

void UMassTrigger::ConnectActorToComponent()
{
	AActor* _Actor = this->GetAcceptedActor();

	// We are going to attach AcceptedActor and set InitMove to true, so better have both pointers
	if (_Actor)
	{
		UPrimitiveComponent* _RootComponent = Cast<UPrimitiveComponent>(_Actor->GetRootComponent());
		if ((_RootComponent != nullptr) && IsDisablePhysics)
		{
			_RootComponent->SetSimulatePhysics(false);	  // we don't want actor falling down while moving
		}
		// use location, orientation, scale as in the world
		_Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

bool UMassTrigger::GetTriggingState() const { return TriggingState; }

AActor* UMassTrigger::GetAcceptedActor()
{
	// whom we capture?
	TArray<AActor*> _Actors;

	GetOverlappingActors(_Actors);

	for (AActor* _Actor : _Actors)
	{
		UPrimitiveComponent* _RootComponent = Cast<UPrimitiveComponent>(_Actor->GetRootComponent());
		float _ActorMass = _RootComponent->GetMass();

		// Actor should be cappable to unlock and not being grabbed at the moment
		if ((_ActorMass >= MinUnlockingMass) && !_Actor->ActorHasTag(Forbids))
		{
			TriggingState = true;
			if (OCMover) OCMover->SetterWrapper(TriggingState, CellNumber);		   // allow door to move
			if (OCRotator) OCRotator->SetterWrapper(TriggingState, CellNumber);	   // allow door to rotate
			return _Actor;
		}
	}
	// we got so far - disallow to door meet offset, return
	TriggingState = false;
	if (OCMover) OCMover->SetterWrapper(TriggingState, CellNumber);
	if (OCRotator) OCRotator->SetterWrapper(TriggingState, CellNumber);

	return nullptr;
}
