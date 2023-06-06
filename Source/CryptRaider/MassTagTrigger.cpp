// Fill out your copyright notice in the Description page of Project Settings.

#include "MassTagTrigger.h"

// Sets default values for this component's properties
UMassTagTrigger::UMassTagTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Forbids = "Grabbed";
}
// Called when the game starts
void UMassTagTrigger::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UMassTagTrigger::TickComponent(				 //
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
void UMassTagTrigger::SetOCMover(UOneConditionMover* __ParsedOCMover) { OCMover = __ParsedOCMover; }

// We set Rotator in the blueprint
void UMassTagTrigger::SetOCRotator(UOneConditionRotator* __ParsedOCRotator) { OCRotator = __ParsedOCRotator; }

void UMassTagTrigger::ConnectActorToComponent()
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

bool UMassTagTrigger::GetMassTriggingState() const { return MassTriggingState; }

bool UMassTagTrigger::GetTagTriggingState() const { return TagTriggingState; }

AActor* UMassTagTrigger::GetAcceptedActor()
{
	// whom we capture?
	TArray<AActor*> _Actors;

	GetOverlappingActors(_Actors);

	TagTriggingState = false;
	for (AActor* _Actor : _Actors)
	{
		// Actor should be cappable to unlock and not being grabbed at the moment
		if (_Actor->ActorHasTag(UnlockingTag) && !_Actor->ActorHasTag(Forbids))
		{
			TagTriggingState = true;
		}
	}

	for (AActor* _Actor : _Actors)
	{
		UPrimitiveComponent* _RootComponent = Cast<UPrimitiveComponent>(_Actor->GetRootComponent());
		float _ActorMass = _RootComponent->GetMass();

		// Actor should be cappable to unlock and not being grabbed at the moment
		if ((_ActorMass >= MinUnlockingMass) && !_Actor->ActorHasTag(Forbids))
		{
			MassTriggingState = true;
			if (OCMover) OCMover->SetterWrapper(MassTriggingState, CellNumber);		   // allow door to move
			if (OCRotator) OCRotator->SetterWrapper(MassTriggingState, CellNumber);	   // allow door to rotate
			return _Actor;
		}
	}
	// we got so far - disallow to door meet offset, return
	MassTriggingState = false;
	if (OCMover) OCMover->SetterWrapper(MassTriggingState, CellNumber);
	if (OCRotator) OCRotator->SetterWrapper(MassTriggingState, CellNumber);

	return nullptr;
}
