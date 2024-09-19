// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"
#include "Engine/DataTable.h"
#include "Misc/Optional.h"

UItemManager::UItemManager()
{
	// 생성자에서 특별한 초기화는 필요 없음
}

void UItemManager::LoadItemData(UDataTable* ItemDataTable)
{
	if (ItemDataTable)
	{
		FString ContextString;
		TArray<FItemData*> AllItems;
		ItemDataTable->GetAllRows(ContextString, AllItems);

		for (FItemData* Item : AllItems)
		{
			// ItemID를 키로 해서 아이템 데이터를 맵에 저장
			ItemDataMap.Add(Item->ItemID, *Item);
			UE_LOG(LogTemp, Warning, TEXT("Item loaded: %s"), *Item->ItemName.ToString());
		}
	}
}

// FItemData를 참조로 반환하기
FItemData& UItemManager::GetItemDataByID(int32 ItemID)
{
	// 맵에 아이템 ID가 있는지 확인
	if (ItemDataMap.Contains(ItemID))
	{
		// 해당하는 아이템 데이터를 참조로 반환
		return ItemDataMap[ItemID];
	}
	return ItemDataMap[0];  // 찾지 못했을 경우 ErrorItem 반환
}

// TOptional을 활용한 안전한 반환
TOptional<FItemData> UItemManager::GetItemDataByIDOptional(int32 ItemID)
{
	if (ItemDataMap.Contains(ItemID))
	{
		return ItemDataMap[ItemID];  // 값이 있으면 반환
	}
	return TOptional<FItemData>();  // 값이 없으면 빈 TOptional 반환
}

/*
*TOptional<FItemData> ItemData = ItemManager->GetItemDataByID(ItemID);

if (ItemData.IsSet())
{
	// 아이템 데이터가 존재하면 처리
	FItemData Data = ItemData.GetValue();
	UE_LOG(LogTemp, Warning, TEXT("Item found: %s"), *Data.ItemName.ToString());
}
else
{
	// 아이템 데이터가 없을 경우 처리
	UE_LOG(LogTemp, Warning, TEXT("Item not found"));
}
 */