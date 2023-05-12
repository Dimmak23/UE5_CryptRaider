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

	if (GrabingTarget != nullptr)
	{
		PhysicsHandle->SetTargetLocationAndRotation(	//
			GetComponentLocation() + GetForwardVector() * HoldDistance,
			GetComponentRotation()	  // use owner rotation
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
	}
}

void UGraber::ReleaseStuff()
{
	if (GrabingTarget == nullptr) return;

	PhysicsHandle->ReleaseComponent();
	GrabingTarget = nullptr;	// finally invalidating pointer and no longer accessing target

	//!
	weGotSomething = false;
	//!
}

UPhysicsHandleComponent* UGraber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* _Result{ nullptr };
	_Result = this->GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (_Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
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
