// Fill out your copyright notice in the Description page of Project Settings.

#include "Graber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGraber::UGraber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	MaxGrabDistance = 400.0f;
}

// Called when the game starts
void UGraber::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UGraber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	StartPosition = GetComponentLocation();
	DestPosition = StartPosition + GetForwardVector() * MaxGrabDistance;

	// UE_LOG(LogTemp, Display, TEXT("Grabber Rotation: %s"), *(GetComponentRotation().ToCompactString()));
	// UE_LOG(LogTemp, Display, TEXT("Current Time Is: %f"), GetWorld()->TimeSeconds);
	DrawDebugLine(this->GetWorld(), StartPosition, DestPosition, FColor::Red);
}
