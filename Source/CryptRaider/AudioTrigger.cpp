//* Designed and produced by Dmytro Kovryzhenko, all rights reserved

#include "AudioTrigger.h"

#include "Kismet/GameplayStatics.h"

UAudioTrigger::UAudioTrigger()
{
	//? Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	//? features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

//? Called when the game starts
void UAudioTrigger::BeginPlay()
{
	Super::BeginPlay();

	//? Setup on begin
	// MoveAudioComponent = UGameplayStatics::CreateSound2D(this, MoveSound, 1.0f, 1.0f, 0.0f);
}

//? Called every frame
void UAudioTrigger::TickComponent(					 //
	float DeltaTime,								 //
	ELevelTick TickType,							 //
	FActorComponentTickFunction* ThisTickFunction	 //
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//? Do we have any?
	this->FetchOverlaps();
}

void UAudioTrigger::FetchOverlaps()
{
	//? We need at least one actor to make sound
	AActor* _OverlappedActor{ nullptr };

	//? Get all actors that we have overlapped
	TArray<AActor*> _Actors;
	GetOverlappingActors(_Actors);

	//? Generate sound on the first suitable actor
	for (AActor* _Actor : _Actors)
	{
		if (_Actor == GetOwner()) continue;
		// UE_LOG(LogTemp, Warning, TEXT("Got one hit actor: %s"), *(_Actor->GetName()));

		//? First check if hit something
		if (_Actor->ActorHasTag(HittingTrigger) && GetOwner()->ActorHasTag(Forbids))
		{
			//? Don't forget to make sure that you have sound base
			if (HitSound && !AlreadyPlaying)
			{
				//! No need in this, we re-assign 'AudioComponent' later
				// if (AudioComponent && AudioComponent->IsPlaying()) AudioComponent->Stop();

				AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, HitSound, GetOwner()->GetActorLocation());
				AlreadyPlaying = true;
			}

			return;
		}
		//? Then check maybe we can make throw sound
		else if (_Actor->ActorHasTag(ThrowingTrigger) && !GetOwner()->ActorHasTag(Forbids))
		{
			//? Don't forget to make sure that you have sound base
			if (ThrowSound && !AlreadyPlaying)
			{
				//! No need in this, we re-assign 'AudioComponent' later
				// if (AudioComponent && AudioComponent->IsPlaying()) AudioComponent->Stop();

				AudioComponent =
					UGameplayStatics::SpawnSoundAtLocation(this, ThrowSound, GetOwner()->GetActorLocation());
				AlreadyPlaying = true;
			}
		}

		//? We already overlapping with some valid (not with self) actor
		return;
	}

	//? Not a single overlapped actor found, we can allow to play music again
	AlreadyPlaying = false;
}
