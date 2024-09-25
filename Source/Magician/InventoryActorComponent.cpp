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
	
}

void UInventoryActorComponent::AddItemToInventory(int32 ItemID, int32 Quantity)
{
	
}

void UInventoryActorComponent::RemoveItemFromInventory(int32 ItemID, int32 Quantity)
{
	
}

bool UInventoryActorComponent::IsInventoryFill() const
{
	return Inventory.Num() >= MaxInventorySize;
}


// Called when the game starts
void UInventoryActorComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...	
}


