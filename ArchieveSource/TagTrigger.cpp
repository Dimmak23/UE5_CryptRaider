// Fill out your copyright notice in the Description page of Project Settings.

#include "TagTrigger.h"

// Sets default values for this component's properties
UTagTrigger::UTagTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Forbids = "Grabbed";
}
// Called when the game starts
void UTagTrigger::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UTagTrigger::TickComponent(					 //
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
void UTagTrigger::SetOCMover(UOneConditionMover* __ParsedOCMover) { OCMover = __ParsedOCMover; }

void UTagTrigger::ConnectActorToComponent()
{
	AActor* _Actor = this->GetAcceptedActor();

	// We are going to attach AcceptedActor and set InitMove to true, so better have both pointers
	if (_Actor)
	{
		UPrimitiveComponent* _RootComponent = Cast<UPrimitiveComponent>(_Actor->GetRootComponent());
		if (_RootComponent != nullptr)
		{
			_RootComponent->SetSimulatePhysics(false);	  // we don't want actor falling down while moving
		}
		// use location, orientation, scale as in the world
		_Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

AActor* UTagTrigger::GetAcceptedActor() const
{
	// whom we capture?
	TArray<AActor*> _Actors;

	GetOverlappingActors(_Actors);

	for (AActor* _Actor : _Actors)
	{
		// Actor should be cappable to unlock and not being grabbed at the moment
		if (_Actor->ActorHasTag(UnlockingTag) && !_Actor->ActorHasTag(Forbids))
		{
			if (OCMover) OCMover->SetterWrapper(true, CellNumber);	  // allow door to move
			return _Actor;
		}
	}

	// we got so far - disallow to door meet offset, return
	if (OCMover) OCMover->SetterWrapper(false, CellNumber);
	return nullptr;
}
