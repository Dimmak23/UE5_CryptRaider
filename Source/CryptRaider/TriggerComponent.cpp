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

	if (Mover == nullptr) return;
	if (ActorAccepted())
	{
		Mover->SetInitMove(true);
	}
}

void UTriggerComponent::SetMover(UMover* _ParsedMover) { Mover = _ParsedMover; }

bool UTriggerComponent::ActorAccepted() const
{
	// whom we capture?
	TArray<AActor*> _Actors;

	GetOverlappingActors(_Actors);

	for (AActor* _Actor : _Actors)
	{
		if (_Actor->ActorHasTag(UnlockingTag))
		{
			return true;
		}
	}
	return false;
}
