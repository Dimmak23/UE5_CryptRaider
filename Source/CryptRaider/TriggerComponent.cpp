// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UTriggerComponent::TickComponent(				 //
	float DeltaTime,								 //
	ELevelTick TickType,							 //
	FActorComponentTickFunction* ThisTickFunction	 //
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	this->ConnectActorToComponent();
}

void UTriggerComponent::SetMover(UMover* _ParsedMover) { Mover = _ParsedMover; }

AActor* UTriggerComponent::GetAcceptedActor() const
{
	// whom we capture?
	TArray<AActor*> _Actors;

	GetOverlappingActors(_Actors);

	for (AActor* _Actor : _Actors)
	{
		// Actor should be cappable to unlock and not being grabbed at the moment
		if (_Actor->ActorHasTag(UnlockingTag) && !_Actor->ActorHasTag("Grabbed"))
		{
			return _Actor;
		}
	}
	return nullptr;
}

void UTriggerComponent::ConnectActorToComponent()
{
	AActor* _Actor = this->GetAcceptedActor();

	if (Mover && _Actor)
	{
		UPrimitiveComponent* RootComponent = Cast<UPrimitiveComponent>(_Actor->GetRootComponent());
		if (RootComponent != nullptr)
		{
			RootComponent->SetSimulatePhysics(false);	 // we don't want actor falling down while moving
		}
		// use location, orientation, scale as in the world
		_Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		Mover->SetInitMove(true);	 // allow door to move
	}
}
