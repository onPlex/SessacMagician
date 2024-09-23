// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Item.generated.h"

UCLASS()
class MAGICIAN_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;

	//아이템 습득
	UFUNCTION(BlueprintCallable, Category = "Item")
	void OnPickUp(class ATPSPlayer* Player);

	//아이템 드롭
	UFUNCTION(BlueprintCallable, Category = "Item")
	void OnDrop(class ATPSPlayer* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMeshComponent* ItemMeshComp;
};
