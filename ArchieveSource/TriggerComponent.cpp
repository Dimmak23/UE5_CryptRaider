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

	// this->ConnectActorToComponent();
}

void UTriggerComponent::ConnectActorToComponent(AActor* _ParsedActor)
{
	//
	if (_ParsedActor)
	{
		UPrimitiveComponent* _RootComponent = Cast<UPrimitiveComponent>(_ParsedActor->GetRootComponent());
		if (_RootComponent != nullptr)
		{
			_RootComponent->SetSimulatePhysics(false);	  // we don't want actor falling down while moving
		}
		// use location, orientation, scale as in the world
		_ParsedActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}
