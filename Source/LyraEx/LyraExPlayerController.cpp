// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraExPlayerController.h"

#include "LyraExPlayer.h"
#include "Abilities/TPAbilitySystemComponent/TPAbilitySystemComponent.h"


ALyraExPlayerController::ALyraExPlayerController()
{
}

void ALyraExPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);
	if (ALyraExPlayer* InPlayer=Cast<ALyraExPlayer>(GetPawn()))
	{
		if (UTPAbilitySystemComponent* InAbilitySystemComponent=InPlayer->GetAbilitySystemComponent())
		{
			InAbilitySystemComponent->ProcessAbilityInput(DeltaTime);
		}
	}
}
