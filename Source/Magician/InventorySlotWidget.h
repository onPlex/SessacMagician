// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class MAGICIAN_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 아이템 데이터를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	void SetItemData(const FItemData& ItemData);

protected:
	// 아이템 썸네일과 이름을 표시할 UI 컴포넌트
	UPROPERTY(meta = (BindWidget))
	UImage* ItemThumbnail;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StackCount;
	
};
