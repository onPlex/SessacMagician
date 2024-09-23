// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "TPSPlayer.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComp"));
	RootComponent = ItemMeshComp;
}

void AItem::OnPickUp(class ATPSPlayer* Player)
{	
	if (Player)
	{
		// 인벤토리에 아이템 추가 로직 (습득)
		Player->AddItemToInventory();
		//습득 후에 해당 (월드) 아이템은 삭제
		Destroy();
	}
}

void AItem::OnDrop(class ATPSPlayer* Player)
{
	if (Player)
	{
		FVector DropLocatoin =
			Player->GetActorLocation() + Player->GetActorForwardVector() * 100;

		//떨어진 아이템
		AItem* DroppedItem = GetWorld()->SpawnActor<AItem>(GetClass(), DropLocatoin, FRotator::ZeroRotator);
		DroppedItem->ItemData = ItemData;

		//플레이어 인벤토리에서 해당 아이템 삭제
		Player->RemoeItemToInventory();
	}
}
