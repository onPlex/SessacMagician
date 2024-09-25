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
    if(ItemDataTable)
    {
	    TArray<FName> RowNames = ItemDataTable->GetRowNames();
    	for(const FName& RowName : RowNames)
    	{
    		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(RowName,"");
    		if(ItemData)
    		{
    			FInventoryItem NewItem;
    			NewItem.ItemData = *ItemData;
    			NewItem.Quantity = 1;

    			Inventory.Add(ItemData->ItemId,NewItem);
    			//AddItemToInventory(ItemData->ItemId,1 ); 
    		}    		
    	}
    }
}

void UInventoryActorComponent::AddItemToInventory(int32 ItemID, int32 Quantity)
{
	//인벤토리가 가득 찼는지 확인 -> 넣을 자리 있는가 ?
    if(IsInventoryFull())
    {
    	UE_LOG(LogTemp, Display, TEXT("Inventory Is Full"));
	    return;
    }

	//인벤토리 넣을 자리가 있으면-

	//이미 인벤토리에 해당아이템이 있는지 확인 || ItemID.stackable 스택형 아이템인지 ?
	if(Inventory.Contains(ItemID))
	{
		FInventoryItem& ExistingItem = Inventory[ItemID];

		//스택형인지 ?
		if(ExistingItem.ItemData.bIsStackable)
		{
			ExistingItem.Quantity += Quantity;
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Item with ID %d is not stackable"), ItemID);
		}	
	}
	//기존에 인벤토리에 없던 아이템을 추가
	else
	{   
		FInventoryItem NewItem;
		NewItem.ItemData.ItemId = ItemID;
		NewItem.Quantity = Quantity;
		Inventory.Add(ItemID,NewItem);
	}
}

void UInventoryActorComponent::RemoveItemFromInventory(int32 ItemID, int32 Quantity)
{
	//삭제할 아이템이 인벤토리에 존재하는지 
	if(Inventory.Contains(ItemID))
	{
		FInventoryItem& ItemData = Inventory[ItemID];

		//아이템의 수량을 감소시키고 0 이하일 경우에 제거
		ItemData.Quantity -= Quantity;
		if(ItemData.Quantity <= 0)
		{
			Inventory.Remove(ItemID);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Item with ID %d Decreased"), ItemID);
		}
	}
	else //없는데, 삭제 = Error 
	{
		UE_LOG(LogTemp, Warning, TEXT("Item with ID %d not found in inventory"), ItemID);
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


