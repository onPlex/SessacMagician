// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryActorComponent.h"

// Sets default values for this component's properties
UInventoryActorComponent::UInventoryActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// DataTable에서 특정 RowName의 데이터를 찾아 반환하는 함수
FItemData UInventoryActorComponent::LoadItemFromDataTable(FName ItemRowName)
{
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid DataTable!"));
		return FItemData();  // 기본값 반환
	}

	// DataTable에서 RowName에 해당하는 데이터를 찾습니다.
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemRowName, TEXT("Item Lookup"));
	if (ItemData)
	{
		return *ItemData;  // 유효한 데이터를 반환
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item with RowName %s not found in DataTable."), *ItemRowName.ToString());
		return FItemData();  // 기본값 반환
	}
}

void UInventoryActorComponent::AddItemToInventory(FName ItemRowName, int32 Quantity)
{
	// 인벤토리가 가득 찼는지 확인
	if (IsInventoryFull())
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full!"));
		return;
	}

	// 이미 인벤토리에 해당 아이템이 있는지 확인
	if (Inventory.Contains(ItemRowName))
	{
		FInventoryItem& ExistingItem = Inventory[ItemRowName];

		// 아이템이 스택 가능할 경우 수량을 추가
		if (ExistingItem.ItemData.bIsStackable)
		{
			ExistingItem.Quantity += Quantity;
			UE_LOG(LogTemp, Log, TEXT("Increased quantity of item with RowName %s to %d."), *ItemRowName.ToString(), ExistingItem.Quantity);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item with RowName %s is not stackable!"), *ItemRowName.ToString());
		}
	}
	else
	{
		// DataTable에서 해당 RowName의 아이템을 로드하여 추가
		if (ItemDataTable)
		{
			FItemData ItemData = LoadItemFromDataTable(ItemRowName);  // 수정된 부분
			FInventoryItem NewItem;
			NewItem.ItemData = ItemData;  // DataTable에서 가져온 아이템 데이터
			NewItem.Quantity = Quantity;   // 입력받은 수량 설정
			Inventory.Add(ItemRowName, NewItem);

			UE_LOG(LogTemp, Log, TEXT("Added new item with RowName %s, Quantity: %d"), *ItemRowName.ToString(), Quantity);
		}
	}
	
}

void UInventoryActorComponent::RemoveItemFromInventory(FName ItemRowName, int32 Quantity)
{
	// 인벤토리에서 해당 아이템이 존재하는지 확인
	if (Inventory.Contains(ItemRowName))
	{
		FInventoryItem& ItemData = Inventory[ItemRowName];

		// 아이템의 수량을 감소시키고 0 이하일 경우 제거
		ItemData.Quantity -= Quantity;
		if (ItemData.Quantity <= 0)
		{
			Inventory.Remove(ItemRowName);
			UE_LOG(LogTemp, Log, TEXT("Removed item with RowName %s from inventory."), *ItemRowName.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Decreased quantity of item with RowName %s to %d."), *ItemRowName.ToString(), ItemData.Quantity);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item with RowName %s not found in inventory."), *ItemRowName.ToString());
	}
}

bool UInventoryActorComponent::IsInventoryFull() const
{
	return Inventory.Num() >= MaxInventorySize;
}


// Called when the game starts
void UInventoryActorComponent::BeginPlay()
{
	Super::BeginPlay();
}
