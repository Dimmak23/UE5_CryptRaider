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

	// UE_LOG(LogTemp, Display,																			  //
	// 	   TEXT("MaxGrabDistance = %f, GrabRadius = %f, MaxHoldDistance = %f, MinHoldDistance = %f."),	  //
	// 	   MaxGrabDistance, GrabRadius, MaxHoldDistance, MinHoldDistance								  //
	// );
}

// Called every frame
void UGraber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (PhysicsHandle == nullptr) return;

	if (GrabingTarget != nullptr)
	{
		PhysicsHandle->SetTargetLocationAndRotation(	//
			this->GetClampedNewTargetLocation(),		// use clamp
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
	// result of the search for grabing
	FHitResult _HitResult;

	// can we grab (on such distance, with such collision sphere)?
	bool _HasHit{ this->CheckHitResult(_HitResult) };

	if (_HasHit && GrabingTarget == nullptr)	// if no target is grabbed already then - go!
	{
		UE_LOG(LogTemp, Display,								//
			   TEXT("Size of the _HitResult struct is: %d"),	//
			   sizeof(_HitResult)								//
		);

		// Prepare target
		GrabingTarget = _HitResult.GetComponent();
		// we can't use: PhysicsHandle->GetGrabbedComponent() - we didn't grabed anything yet
		GrabingTarget->WakeAllRigidBodies();	// we avoid here throwing target into the game textures
		// Actually grab something
		PhysicsHandle->GrabComponentAtLocationWithRotation(	   //
			_HitResult.GetComponent(),						   //
			NAME_None,										   //
			_HitResult.ImpactPoint,							   //
			GetComponentRotation()							   // use owner rotation
		);

		//!
		weGotSomething = true;
		//!

		// UE_LOG(LogTemp, Display,									   //
		// 	   TEXT("Current Time is: %f, grabber took target..."),	   //
		// 	   GetWorld()->TimeSeconds								   //
		// );
	}

	// Now you know that you miss the target
	// if (!_HasHit)
	// {
	// 	UE_LOG(													 //
	// 		LogTemp, Display,									 //
	// 		TEXT("Current Time is: %f, you miss the target"),	 //
	// 		GetWorld()->TimeSeconds								 //
	// 	);
	// }
}

void UGraber::ReleaseStuff()
{
	if (GrabingTarget != nullptr)
	{
		PhysicsHandle->ReleaseComponent();
		// UE_LOG(LogTemp, Display, TEXT("Grabber released object..."));
		GrabingTarget = nullptr;	// finally invalidating pointer and no longer accessing target

		//!
		weGotSomething = false;
		//!
	}
}

UPhysicsHandleComponent* UGraber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* _Result{ nullptr };
	_Result = this->GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (_Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Recompiled 12:47 PM, found a physics handler: %s..."), *(_Result->GetName()));
	}

	return _Result;
}

bool UGraber::CheckHitResult(FHitResult& __HitResult__) const
{
	// current position of the player
	FVector _StartPosition = GetComponentLocation();

	// destination vector from player
	FVector _DestPosition = _StartPosition + GetForwardVector() * MaxGrabDistance;

	// everything that collide this sphere we can grab
	FCollisionShape _HitSphere = FCollisionShape::MakeSphere(GrabRadius);

	// can we grab (on such distance, with such collision sphere)?
	return this->GetWorld()->SweepSingleByChannel(	  //
		__HitResult__,								  //
		_StartPosition,								  //
		_DestPosition,								  //
		FQuat::Identity,							  //
		ECC_GameTraceChannel2,						  //
		_HitSphere									  //
	);
}

FVector UGraber::GetClampedNewTargetLocation() const
{
	// some repetative utilities
	FVector _OwnerLocation{ GetComponentLocation() };
	FVector _ForwardVector{ GetForwardVector() };

	FVector _MaxTargetPush{ _OwnerLocation + _ForwardVector * MaxHoldDistance };
	FVector _MinTargetPush{ _OwnerLocation + _ForwardVector * MinHoldDistance };
	FVector _CurrentTargetLocation{ GrabingTarget->GetRelativeLocation() };

	// Let's try to push target far from us
	FVector _NewTargetLocation = _MaxTargetPush;

	float _DistanceToCurrent = FVector::Dist(_OwnerLocation, _CurrentTargetLocation);
	// float _DistanceToMax = FVector::Dist(_OwnerLocation, _MaxTargetPush);
	// float _DistanceToMin = FVector::Dist(_OwnerLocation, _MinTargetPush);

	bool _firstThird{ false };
	bool _secondThird{ false };
	bool _thirdThird{ false };

	// target object are too close to use let's push it a bit
	if (_DistanceToCurrent < FVector::Dist(_OwnerLocation, _MinTargetPush))
	{
		_firstThird = true;
		_NewTargetLocation = _MinTargetPush;	// here we return to the minimu possible distance
	}
	// targeted object are close to us, let's not throw too far, clamp new location instead
	else if (_DistanceToCurrent < FVector::Dist(_OwnerLocation, _MaxTargetPush))
	{
		// UE_LOG(LogTemp, Display,														   //
		// 	   TEXT("Current Time is: %f, targeted object to close to the grabber..."),	   //
		// 	   GetWorld()->TimeSeconds													   //
		// );
		_secondThird = true;

		// distance to the target are good, let's keep it
		_NewTargetLocation = _OwnerLocation + _ForwardVector * _DistanceToCurrent;
	}
	else
	{
		_thirdThird = true;
	}

	if (_firstThird)
	{
		UE_LOG(LogTemp, Display,					 //
			   TEXT("Target in the first third.")	 //
		);
	}
	else if (_secondThird)
	{
		UE_LOG(LogTemp, Display,					  //
			   TEXT("Target in the second third.")	  //
		);
	}
	else
	{
		UE_LOG(LogTemp, Display,					//
			   TEXT("Target in the last third.")	//
		);
	}
	UE_LOG(LogTemp, Display,					   //
		   TEXT("Distance to the target: %f."),	   //
		   _DistanceToCurrent					   //
	);

	return _NewTargetLocation;
}
