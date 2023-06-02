// Fill out your copyright notice in the Description page of Project Settings.

#include "DoubleTrigger.h"

void UDoubleTrigger::ProceedWrapper()
{
	// whom we capture?
	TArray<AActor*> _Actors;
	//
	GetOverlappingActors(_Actors);
	//
	AActor* _Actor = TagTrigger->ConditionalMove(_Actors);
	//
	ConnectActorToComponent(_Actor);
	//
	_Actor = nullptr;	 // clear it
	_Actor = MassTrigger->ConditionalMove(_Actors);
	//
	ConnectActorToComponent(_Actor);
}
