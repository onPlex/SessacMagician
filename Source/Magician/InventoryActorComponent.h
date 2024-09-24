// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "InventoryActorComponent.generated.h"


// 인벤토리에서 아이템 데이터와 그 수량을 관리하는 구조체
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	// 아이템의 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FItemData ItemData;

	// 아이템의 수량 (스택 가능한 경우 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	FInventoryItem()
		: ItemData(), Quantity(1) // 기본 수량을 1로 설정
	{}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable  )
class MAGICIAN_API UInventoryActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryActorComponent();
	
	// 아이템의 고유 ID를 키로 하는 인벤토리 맵 (ID -> FInventoryItem)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<int32, FInventoryItem> Inventory;

	// DataTable로부터 아이템을 로드
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void LoadItemFromDataTable(UDataTable* ItemDataTable);

	// 인벤토리에 아이템 추가
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(int32 ItemID, int32 Quantity);

	// 인벤토리에서 아이템 제거
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromInventory(int32 ItemID, int32 Quantity);

	// 인벤토리가 가득 찼는지 확인
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsInventoryFull() const;

	// 인벤토리 최대 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxInventorySize;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;		
};
