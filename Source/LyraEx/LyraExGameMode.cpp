// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraExGameMode.h"
#include "LyraExCharacter.h"
#include "LyraExPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ALyraExGameMode::ALyraExGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass=ALyraExPlayerController::StaticClass();
}
