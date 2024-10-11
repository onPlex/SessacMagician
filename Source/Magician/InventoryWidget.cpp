// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryActorComponent.h"
#include "InventorySlotWidget.h"
#include "Components/GridPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Log, TEXT("InventoryWidget NativeConstruct"));


	CanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("CanvasPanel"));
	if(CanvasPanel)
	{
		WidgetTree->RootWidget = CanvasPanel;
	}

	GridPanel = WidgetTree->ConstructWidget<UGridPanel>(UGridPanel::StaticClass(),TEXT("GridPanel"));
	if(GridPanel && CanvasPanel)
	{
		CanvasPanel->AddChild(GridPanel);
	}
	
	//UpdateInventoryGrid(InventoryComponent);
}

void UInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UE_LOG(LogTemp, Log, TEXT("InventoryWidget NativeDestruct"));
}
void UInventoryWidget::ToggleInventory()
{
	//열려 있으면 ~ 
	if(bIsInventoryOpen)
	{
		RemoveFromParent();
	}
	else
	{
		AddToViewport();
	}

	//상태를 토글
	bIsInventoryOpen = !bIsInventoryOpen;

	//델리게이트 호출
	OnInventoryToggled.Broadcast(bIsInventoryOpen);
}

void UInventoryWidget::SetInventoryItems(UInventoryActorComponent* InventoryComponent)
{
	//외부 인벤토리 컴포넌트에서 아이템 데이터들이 넘어옴
	if(InventoryComponent)
	{
		UpdateInventoryGrid(InventoryComponent);
	}
}

void UInventoryWidget:: UpdateInventoryGrid(UInventoryActorComponent* InventoryComponent)
{
     if(!InventoryComponent|| !SlotWidgetClass || !GridPanel)
     {
	     return;
     }

	// 기존 그리드를 항목 제거
	GridPanel->ClearChildren();

	//그리드 행열 설정
	const int32 NumColumns = 5;
	int32 Row = 0;
	int32 Columns = 0;

	//인벤토리 맵을 순회하며 각 아이템을 그리드에 추가
	// Map - > Key & Value  -> Pair
	for(const TPair<FName,FInventoryItem>& InventoryItem : InventoryComponent->Inventory)
	{
		//슬롯 위젯 생성

		if(UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this,SlotWidgetClass))
		{
			// InventoryItem 구조체에서 FItemData + 수량을 가져와서 슬롯에 설정
			const FItemData& ItemData = InventoryItem.Value.ItemData;
			int32 Quantity = InventoryItem.Value.Quantity;

			// Slot Widget 에 아이템 데이터와 수량 설정
			SlotWidget->SetItemData(ItemData,Quantity);

			GridPanel->AddChildToGrid(SlotWidget,Row,Columns);

			//행열
			Columns++;
			if(Columns >= NumColumns)
			{
				Columns = 0;
				Row++;
			}
		}
	}	
}

