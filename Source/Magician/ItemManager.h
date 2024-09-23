// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.h"
#include "ItemManager.generated.h"

/**
 * 
 */
UCLASS()
class MAGICIAN_API UItemManager : public UObject
{
	GENERATED_BODY()

	/*
	//아이템 데이터를 저장하는 맵 (ItemId -> FItemData)
	TMap<int32, FItemData> ItemDataMap;

	//Data 테이블에서 아이템 데이터를 로드
	UFUNCTION(BlueprintCallable, Category = "Item")
	void LoadItemData(UDataTable* ItemDataTable);

	//ItemID로 아이템 데이터 검색
	UFUNCTION(BlueprintCallable, Category = "Item")
	TOptional<FItemData> GetItemDataByIDOptional(int32 ItemID);

	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemData& GetItemDataByID(int32 ItemID);
	*/
};
