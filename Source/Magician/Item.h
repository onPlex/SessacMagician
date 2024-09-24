// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"  // FItemData 구조체 포함
#include "Item.generated.h"

UCLASS()
class MAGICIAN_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;  // 아이템의 데이터 구조체

	// 아이템을 습득
	UFUNCTION(BlueprintCallable, Category = "Item")
	void OnPickup(class ATPSPlayer* Player);
	//void OnPickup(class UInventoryActorComponent* InventoryComponent);
	// 아이템을 버림
	UFUNCTION(BlueprintCallable, Category = "Item")
	void OnDrop(class ATPSPlayer* Player);	
	//void OnDrop(class UInventoryActorComponent* InventoryComponent);

	// 아이템 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* ItemMeshComponent;
};
