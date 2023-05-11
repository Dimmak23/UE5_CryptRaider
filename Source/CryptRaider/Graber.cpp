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
		FVector _OwnerLocation = GetComponentLocation();
		FVector _CurrentTargetLocation = GrabingTarget->GetRelativeLocation();
		FVector _NewTargetLocation = _OwnerLocation + GetForwardVector() * HoldDistance;

		float _DistanceToTarget = FVector::Dist(_OwnerLocation, _CurrentTargetLocation);

		if (_DistanceToTarget < FVector::Dist(_OwnerLocation, _NewTargetLocation))
		{
			UE_LOG(LogTemp, Display,												  //
				   TEXT("Current Time is: %f, object to close to the grabber..."),	  //
				   GetWorld()->TimeSeconds											  //
			);
			_NewTargetLocation = _OwnerLocation + GetForwardVector() * _DistanceToTarget;
		}

		PhysicsHandle->SetTargetLocationAndRotation(	//
			_NewTargetLocation,							//
			GetComponentRotation()						// use owner rotation
		);
	}
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
	// current position of the player
	FVector _StartPosition = GetComponentLocation();

	// destination vector from player
	FVector _DestPosition = _StartPosition + GetForwardVector() * MaxGrabDistance;

	// result of the searching for grabing
	FHitResult _HitResult;

	// everything that collide this sphere we can grab
	FCollisionShape _HitSphere = FCollisionShape::MakeSphere(GrabRadius);

	// can we grab (on such distance, with such collision sphere)?
	bool _HasHit = this->GetWorld()->SweepSingleByChannel(	  //
		_HitResult,											  //
		_StartPosition,										  //
		_DestPosition,										  //
		FQuat::Identity,									  //
		ECC_GameTraceChannel2,								  //
		_HitSphere											  //
	);

	// FQuat::Identity --> no rotation
	// ECollisionChannel TraceChannel --> {project_folder}/Config/DefaultEngine.ini -->
	// Channel = ECC_GameTraceChannel2,
	// for: Name="GraberChannel"
	// +DefaultChannelResponses=(Channel=ECC_GameTraceChannel2, DefaultResponse=ECR_Ignore, bTraceType=True,
	// bStaticObject=False, Name="GraberChannel")

	if (_HasHit && GrabingTarget == nullptr)	// if no target is grabbed already then - go!
	{
		// Prepare target
		GrabingTarget = _HitResult.GetComponent();	  // it's better to receive grabbed object here

		// Actually grab something
		PhysicsHandle->GrabComponentAtLocationWithRotation(	   //
															   //! CAN'T USE PhysicsHandle->GetGrabbedComponent()
															   //! HERE, WE DIDN'T GRABBED ANYTHING YET
			_HitResult.GetComponent(),						   //
			NAME_None,										   //
			_HitResult.ImpactPoint,							   //
			GetComponentRotation()							   // use owner rotation
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
	if (!_HasHit)
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
	UPhysicsHandleComponent* _Result = this->GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (_Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Recompiled 11:09 PM, found a physics handler: %s..."), *(_Result->GetName()));
	}

	return _Result;
}
