// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "NPCShop.generated.h"

UCLASS()
class MAGICIAN_API ANPCShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 상점 아이템 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TMap<int32, FItemData> ItemsInStock;

	// 아이템 구매
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void BuyItem(class ATPSPlayer* Player, int32 ItemID);

	// 아이템 판매
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void SellItem(class ATPSPlayer* Player, int32 ItemID);
};
