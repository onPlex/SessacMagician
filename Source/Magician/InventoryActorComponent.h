// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Components/ActorComponent.h"
#include "InventoryActorComponent.generated.h"


USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 Quantity;

	FInventoryItem() //기본 생성 수량 1개
		: ItemData(), Quantity(1)
	{
	}
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class MAGICIAN_API UInventoryActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryActorComponent();

	//아이템의 고유 ID를 키로 사용하는 인벤토리 맵
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TMap<FName, FInventoryItem> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	UDataTable* ItemDataTable;

	//DataTable로부터 아이템로드
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FItemData LoadItemFromDataTable(FName ItemRowName);

	//인벤토리에 아이템 추가
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void AddItemToInventory(FName ItemRowName, int32 Quantity);

	//인벤토리에 아이템 제거
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void RemoveItemFromInventory(FName ItemRowName, int32 Quantity);
	
	//인벤토리에 가득 찼는지 확인
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool IsInventoryFull() const;
	//인벤토리 최대 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 MaxInventorySize;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
