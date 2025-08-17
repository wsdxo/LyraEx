// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LyraExPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LYRAEX_API ALyraExPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALyraExPlayerController();

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
