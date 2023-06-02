// Fill out your copyright notice in the Description page of Project Settings.

#include "SingleTrigger.h"

void USingleTrigger::ProceedWrapper()
{
	// whom we capture?
	TArray<AActor*> _Actors;
	//
	GetOverlappingActors(_Actors);
	//
	AActor* _Actor = Trigger->ConditionalMove(_Actors);
	//
	ConnectActorToComponent(_Actor);
}