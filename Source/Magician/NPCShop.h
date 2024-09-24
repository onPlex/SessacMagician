// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "NPCShop.generated.h"

/**
 * 
 */
UCLASS()
class MAGICIAN_API ANPCShop : public ANPC
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<FName> ItemOnSale;
};
