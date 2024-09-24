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

void UInventoryActorComponent::LoadItemFromDataTable(UDataTable* ItemDataTable)
{
	if (ItemDataTable)
	{
		TArray<FName> RowNames = ItemDataTable->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			FItemData* ItemData = ItemDataTable->FindRow<FItemData>(RowName, "");
			if (ItemData)
			{
				// ItemID를 키로 하여 인벤토리에 추가 (기본 수량은 1)
				FInventoryItem NewItem;
				NewItem.ItemData = *ItemData;
				NewItem.Quantity = 1; // 처음 로드 시 기본 수량을 1로 설정

				Inventory.Add(ItemData->ItemID, NewItem);
			}
		}
	}
}

void UInventoryActorComponent::AddItemToInventory(int32 ItemID, int32 Quantity)
{
	// 인벤토리가 가득 찼는지 확인
	if (IsInventoryFull())
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full!"));
		return;
	}

	// 이미 인벤토리에 해당 아이템이 있는지 확인
	if (Inventory.Contains(ItemID))
	{
		FInventoryItem& ExistingItem = Inventory[ItemID];

		// 아이템이 스택 가능할 경우 수량을 추가
		if (ExistingItem.ItemData.bIsStackable)
		{
			ExistingItem.Quantity += Quantity;
			UE_LOG(LogTemp, Log, TEXT("Increased quantity of item with ID %d to %d."), ItemID, ExistingItem.Quantity);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item with ID %d is not stackable!"), ItemID);
		}
	}
	else
	{
		// DataTable에서 해당 ID의 아이템을 로드하여 추가
		FInventoryItem NewItem;
		NewItem.ItemData.ItemID = ItemID; // 기본 데이터 설정
		NewItem.Quantity = Quantity;
		Inventory.Add(ItemID, NewItem);

		UE_LOG(LogTemp, Log, TEXT("Added new item with ID %d, Quantity: %d"), ItemID, Quantity);
	}
}

void UInventoryActorComponent::RemoveItemFromInventory(int32 ItemID, int32 Quantity)
{
	if (Inventory.Contains(ItemID))
	{
		FInventoryItem& ItemData = Inventory[ItemID];

		// 아이템의 수량을 감소시키고 0 이하일 경우 제거
		ItemData.Quantity -= Quantity;
		if (ItemData.Quantity <= 0)
		{
			Inventory.Remove(ItemID);
			UE_LOG(LogTemp, Log, TEXT("Removed item with ID %d from inventory."), ItemID);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Decreased quantity of item with ID %d to %d."), ItemID, ItemData.Quantity);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item with ID %d not found in inventory."), ItemID);
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
