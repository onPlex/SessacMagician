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

//DataTable에서 특정 RowName 으로 데이터를 찾아서 변환하는 함수
FItemData UInventoryActorComponent::LoadItemFromDataTable(FName ItemRowName)
{
	//데이터 테이블 Valid 
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataTable is NULL"));
		return FItemData();
	}

	//Data Table 에서 ItemRowName 해당하는 데이터를 찾기
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemRowName,TEXT("Item Lookup"));
	if (ItemData)
	{
		return *ItemData; // 데이터가 있으면 -> 해당 레퍼런스 데이터 반환
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataTable is NULL"));
		return FItemData();
	}
}

void UInventoryActorComponent::AddItemToInventory(FName ItemRowName, int32 Quantity)
{
	//인벤토리가 가득 찼는지 확인 ? bIsFull ?
	if (IsInventoryFull())
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataTable is Full"));
		return;
	}

	// 이미 인벤토리에 해당 아이템이 있는 경우
	if (Inventory.Contains(ItemRowName))
	{
		FInventoryItem& ExistingItem = Inventory[ItemRowName];

		//해당 아이템이 스택 가능할 경우
		if (ExistingItem.ItemData.bIsStackable)
		{
			//현재 존재하던 해당 아이템의 수량 += 새로들어오는 수량
			ExistingItem.Quantity += Quantity;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Item Is Not Stackable"));
		} //아닐 경우
	}
	else // 인벤토리에 중복될 아이템이 없는 경우
	{
		//DataTable 에서 해당 RowName 의 아이템을 로드하여 추가

		if (ItemDataTable)
		{
			FItemData ItemData = LoadItemFromDataTable(ItemRowName);
			FInventoryItem NewItem;
			NewItem.ItemData = ItemData;
			NewItem.Quantity = Quantity;
			Inventory.Add(ItemRowName, NewItem);
		}
	}
}

void UInventoryActorComponent::RemoveItemFromInventory(FName ItemRowName, int32 Quantity)
{
	//인벤토리에서 해당 아이템이 존재하는지 확인
	if (Inventory.Contains(ItemRowName))
	{
		FInventoryItem& ItemData = Inventory[ItemRowName];

		//아이템의 수량을 감소시키고 0이하면 제거

		ItemData.Quantity -= Quantity;
		if (ItemData.Quantity <= 0)
		{
			//해당 키(RowName)값의 아이템을 제거
			Inventory.Remove(ItemRowName);
		}
	}
	else
	{
		//인벤토리에서 존재하지 않는 아이템을 지우려는 오류
		UE_LOG(LogTemp, Error, TEXT("Try to Destroy, non existing Item "));
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
	// ...	
}
