// Fill out your copyright notice in the Description page of Project Settings.

#include "SynOpticonPlayerController.h"
//#include "Synopticon.h"

UWorld* ASynOpticonPlayerController::GetOwnerWorld() const
{
	if (Owner)
		return Owner->GetWorld();
	return nullptr;
}
