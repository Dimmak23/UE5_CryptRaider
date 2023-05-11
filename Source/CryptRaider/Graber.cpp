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
	if (HoldDistance == 0.0f) HoldDistance = 250.0f;

	PhysicsHandle = GetPhysicsHandle();
}

// Called every frame
void UGraber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (PhysicsHandle == nullptr) return;

	//! NOTE: actual grabbed component: PhysicsHandle->GetGrabbedComponent()
	//! and what is on the target: GrabingTarget = HitResult.GetComponent()
	//! are not the same
	if (GrabingTarget != nullptr)
	{
		// GrabingTarget->WakeAllRigidBodies(); // strange behavior
		FVector OwnerLocation = GetComponentLocation();
		FVector CurrentTargetLocation = GrabingTarget->GetRelativeLocation();
		FVector NewTargetLocation = OwnerLocation + GetForwardVector() * HoldDistance;

		if (FVector::Dist(OwnerLocation, CurrentTargetLocation) < FVector::Dist(OwnerLocation, NewTargetLocation))
			NewTargetLocation =
				OwnerLocation + GetForwardVector() * FVector::Dist(OwnerLocation, CurrentTargetLocation);

		PhysicsHandle->SetTargetLocationAndRotation(	//
			NewTargetLocation,							//
			GetComponentRotation()						// use owner rotation
		);
	}
	// UE_LOG(LogTemp, Display,															   //
	// 	   TEXT("weGotSomething: %s."), (weGotSomething ? TEXT("true") : TEXT("false"))	   //
	// );
}

void UGraber::TriggerGrab()
{
	if (PhysicsHandle == nullptr) return;

	if (weGotSomething)
	{
		this->ReleaseStuff();
	}
	else
	{
		this->GrabStuff();
	}
}

void UGraber::GrabStuff()
{
	StartPosition = GetComponentLocation();									// current position of the player
	DestPosition = StartPosition + GetForwardVector() * MaxGrabDistance;	// destination vector from player

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
	// ECollisionChannel TraceChannel --> {project_folder}/Config/DefaultEngine.ini -->
	// Channel = ECC_GameTraceChannel2,
	// for: Name="GraberChannel"
	// +DefaultChannelResponses=(Channel=ECC_GameTraceChannel2, DefaultResponse=ECR_Ignore, bTraceType=True,
	// bStaticObject=False, Name="GraberChannel")

	if (HasHit && GrabingTarget == nullptr)	   // if no target is grabbed already then - go!
	{
		// Prepare target
		GrabingTarget = HitResult.GetComponent();	 // it's better to receive grabbed object here
		// CurrentTargetLocation = HitResult.Component.Get()->GetRelativeLocation();	 // save object location
		// GrabingTarget->WakeAllRigidBodies();		 // strange behavior
		// ObjectRotation = GrabingTarget->GetComponentRotation();	   // don't use

		// Actually grab something
		PhysicsHandle->GrabComponentAtLocationWithRotation(	   //
															   //! CAN'T USE PhysicsHandle->GetGrabbedComponent()
															   //! HERE, WE DIDN'T GRABBED ANYTHING YET
			HitResult.GetComponent(),
			NAME_None,				  //
			HitResult.ImpactPoint,	  //
			GetComponentRotation()	  // use owner rotation
		);

		//!
		weGotSomething = true;
		//!

		UE_LOG(LogTemp, Display,									   //
			   TEXT("Current Time is: %f, grabber took object..."),	   //
			   GetWorld()->TimeSeconds								   //
		);
	}

	// Now you know that you miss the target
	if (!HasHit)
	{
		UE_LOG(												 //
			LogTemp, Display,								 //
			TEXT("Current Time is: %f, you miss object"),	 //
			GetWorld()->TimeSeconds							 //
		);
	}
}

void UGraber::ReleaseStuff()
{
	//! NOTE: actual grabbed component: PhysicsHandle->GetGrabbedComponent()
	//! and what is on the target: GrabingTarget = HitResult.GetComponent()
	//! are not the same
	if (GrabingTarget != nullptr)
	{
		// PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		// GrabingTarget->WakeAllRigidBodies();	// strange behavior
		PhysicsHandle->ReleaseComponent();
		UE_LOG(LogTemp, Display, TEXT("Grabber released object..."));
		GrabingTarget = nullptr;	// finally invalidating pointer and no longer accessing target

		//!
		weGotSomething = false;
		//!
	}
}

UPhysicsHandleComponent* UGraber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = this->GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Recompiled 10:26 PM, found a physics handler: %s..."), *(Result->GetName()));
	}
	// Result->RegisterComponent(); // no need in this

	return Result;
}
