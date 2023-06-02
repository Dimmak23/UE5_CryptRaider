// Fill out your copyright notice in the Description page of Project Settings.

#include "MassTrigger.h"

// Called every frame
void UMassTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	this->ProceedWrapper();
}
