// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MAGICIAN_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 상호작용 위젯을 표시하는 함수 (상호작용이 가능할 때 호출됨)
	virtual void DisplayInteractionWidget() = 0;	
	// 상호작용이 종료되었을 때 위젯을 숨기는 함수
	virtual void HideInteractionWidget() = 0;
	// 상호작용이 실행될 때 호출되는 함수
	virtual void Interact() = 0;
};
