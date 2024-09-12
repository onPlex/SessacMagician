// Copyright Epic Games, Inc. All Rights Reserved.

#include "MagicianGameMode.h"
#include "MagicianCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMagicianGameMode::AMagicianGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
