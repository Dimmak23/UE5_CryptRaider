// Fill out your copyright notice in the Description page of Project Settings.

#include "TagTrigger.h"

// Called every frame
void UTagTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	this->ProceedWrapper();
}
