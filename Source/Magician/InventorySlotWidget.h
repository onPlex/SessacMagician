// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;

UCLASS()
class MAGICIAN_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//아이템 썸네일 이미지
	UPROPERTY(meta = (BindWidget))
	UImage* ItemThumbnail;
	//아이템 이름
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;
	// 아이템 수량
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StackCount;

public:
	UFUNCTION(BlueprintCallable, Category= "Inventory Slot")
	void SetItemData(const FItemData& ItemData, int32 Quantity);
};
