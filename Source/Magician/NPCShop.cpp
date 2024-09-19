// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCShop.h"
#include "TPSPlayer.h"

// Sets default values
ANPCShop::ANPCShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANPCShop::BeginPlay()
{
	Super::BeginPlay();	
}

void ANPCShop::BuyItem(ATPSPlayer* Player, int32 ItemID)
{
	if (ItemsInStock.Contains(ItemID))
	{
		const FItemData& Item = ItemsInStock[ItemID];
		Player->AddItemToInventory(Item);
		UE_LOG(LogTemp, Warning, TEXT("Item bought: %s"), *Item.ItemName.ToString());
	}
}

void ANPCShop::SellItem(ATPSPlayer* Player, int32 ItemID)
{
	Player->RemoveItemFromInventory(ItemID);
	UE_LOG(LogTemp, Warning, TEXT("Item sold: %d"), ItemID);
}