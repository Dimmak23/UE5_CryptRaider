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
	Forbids = "Grabbed";
	PermanentAttachment = "PermanentAttachment";
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

	if (PhysicsHandle && GrabingActor)
	{
		PhysicsHandle->SetTargetLocationAndRotation(					   //
			GetComponentLocation() + GetForwardVector() * HoldDistance,	   // use owner seeing direction
			GetComponentRotation()										   // use owner rotation
		);
		float DistanceToGrabingActor = FVector::Dist(GetComponentLocation(), GrabingActor->GetActorLocation());
		if (DistanceToGrabingActor > MaxGrabDistance) this->ReleaseStuff();
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

	// if some target is grabbed already or there is no hit, don't grab
	if ((GrabingActor != nullptr) || !_HasHit) return;

	// Disable grabbing if Actor should have permanent attachment
	GrabingActor = _HitResult.GetActor();
	if (GrabingActor->ActorHasTag(PermanentAttachment)) return;

	// Prepare target
	UPrimitiveComponent* _GrabingTarget = _HitResult.GetComponent();
	_GrabingTarget->WakeAllRigidBodies();		 // we avoid here throwing target into the game textures
	_GrabingTarget->SetSimulatePhysics(true);	 // revive physics, so you can grab

	// Actually grab something
	PhysicsHandle->GrabComponentAtLocationWithRotation(	   //
		_GrabingTarget,									   //
		NAME_None,										   //
		_HitResult.ImpactPoint,							   //
		GetComponentRotation()							   // use owner rotation
	);

	// Explicitly say that actor has been grabbed
	GrabingActor->Tags.Add("Grabbed");
	//!
	weGotSomething = true;
	//!
}

void UGraber::ReleaseStuff()
{
	if (GrabingActor == nullptr) return;

	// if (GrabingActor->ActorHasTag("Grabbed"))
	// {
	GrabingActor->Tags.Remove("Grabbed");
	// }
	// Here we release target
	PhysicsHandle->ReleaseComponent();

	// finally invalidating pointer and no longer accessing targeted actor
	GrabingActor = nullptr;

	//!
	weGotSomething = false;
	//!
}

bool UGraber::isGotSomething() const { return weGotSomething; }

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
