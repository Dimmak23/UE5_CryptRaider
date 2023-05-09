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
}

// Called when the game starts
void UGraber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (MaxGrabDistance == 0.0f) MaxGrabDistance = 400.0f;
	if (GrabRadius == 0.0f) GrabRadius = 20.f;
	if (HoldDistance == 0.0f) HoldDistance = 200.0f;

	PhysicsHandle = GetPhysicsHandle();
}

// Called every frame
void UGraber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (PhysicsHandle == nullptr) return;

	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, ObjectRotation);
}

void UGraber::GrabStuff()
{
	if (PhysicsHandle == nullptr) return;

	StartPosition = GetComponentLocation();									// current position of the player
	DestPosition = StartPosition + GetForwardVector() * MaxGrabDistance;	// destiantion vector from player

	HitSphere = FCollisionShape::MakeSphere(GrabRadius);

	HasHit = this->GetWorld()->SweepSingleByChannel(	//
		HitResult,										//
		StartPosition,									//
		DestPosition,									//
		FQuat::Identity,								//
		ECC_GameTraceChannel2,							//
		HitSphere										//
	);
	// FQuat::Identity --> no rotation
	// ECollisionChannel TraceChannel --> {project_folder}/Config/DefaultEngine.ini --> Channel=ECC_GameTraceChannel2,
	// for: Name="GraberChannel"
	// +DefaultChannelResponses=(Channel=ECC_GameTraceChannel2, DefaultResponse=ECR_Ignore, bTraceType=True,
	// bStaticObject=False, Name="GraberChannel")

	if (HasHit)
	{
		ObjectRotation = HitResult.GetComponent()->GetComponentRotation();
		// Actually grab something
		PhysicsHandle->GrabComponentAtLocationWithRotation(	   //
			HitResult.GetComponent(),						   //
			NAME_None,										   //
			HitResult.ImpactPoint,							   //
			GetComponentRotation()							   //
		);
	}
	else
	{
		UE_LOG(											//
			LogTemp, Display,							//
			TEXT("Current Time is: %f, no hitting"),	//
			GetWorld()->TimeSeconds						// get time
		);
	}
}

void UGraber::ReleaseStuff() { UE_LOG(LogTemp, Display, TEXT("Grabber have released object...")); }

UPhysicsHandleComponent* UGraber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = this->GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Actually found a physics handler: %s..."), *(Result->GetName()));
	}
	Result->RegisterComponent();

	return Result;
}
