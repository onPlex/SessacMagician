// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "ItemData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::SetItemData(const FItemData& ItemData, int32 Quantity)
{
	//아이템 썸네일 설정
	if (ItemThumbnail && ItemData.ItemThumnail)
	{
		ItemThumbnail->SetBrushFromTexture(ItemData.ItemThumnail);
	}

	// 아이템이름 설정
	if (ItemName)
	{
		ItemName->SetText(ItemData.ItemName);
	}

	//아이템 수량 설정
	if (StackCount)
	{
		StackCount->SetText(FText::AsNumber(Quantity));
	}
}
