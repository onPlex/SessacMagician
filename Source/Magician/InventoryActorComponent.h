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
	
	// 아이템의 RowName을 키로 하는 인벤토리 맵 (RowName -> FInventoryItem)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FName, FInventoryItem> Inventory;

	// DataTable을 블루프린트에서 할당할 수 있도록 UPROPERTY 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDataTable* ItemDataTable;
	
	// DataTable로부터 아이템을 로드 (포인터 대신 값 타입 반환)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FItemData LoadItemFromDataTable(FName ItemRowName);
	
	// 인벤토리에 아이템 추가
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(FName ItemRowName, int32 Quantity);

	// 인벤토리에서 아이템 제거
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromInventory(FName ItemRowName, int32 Quantity);

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
