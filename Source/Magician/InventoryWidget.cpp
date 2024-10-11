// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/GridPanel.h"
#include "InventorySlotWidget.h"
#include "Blueprint/WidgetTree.h"

/*
위젯이 생성될 때 필요한 초기화 작업을 설정할 때 사용합니다.
예를 들어, 위젯의 상태를 초기화하거나, 위젯 내부의 다른 컴포넌트를 초기화하는 작업 등을 수행합니다.
위젯의 AddToViewport()와 같은 작업이 수행될 때 호출됩니다 *
 */
void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
    
	// UCanvasPanel 생성 및 위젯 트리에 추가
	CanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("CanvasPanel"));
	if (CanvasPanel)
	{
		// 위젯 트리의 루트로 설정
		WidgetTree->RootWidget = CanvasPanel;
	}

	// UGridPanel 생성 및 CanvasPanel의 자식으로 추가
	GridPanel = WidgetTree->ConstructWidget<UGridPanel>(UGridPanel::StaticClass(), TEXT("GridPanel"));
	if (GridPanel && CanvasPanel)
	{
		// GridPanel을 CanvasPanel에 자식으로 추가
		CanvasPanel->AddChild(GridPanel);
	}	
	// 인벤토리 그리드를 초기화 및 업데이트
	UpdateInventoryGrid();
}

/*
 * NativeDestruct()는 위젯이 제거될 때 호출됩니다. 이 함수는 위젯이 메모리에서 해제되기 직전에 호출
 */
void UInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// 위젯이 제거될 때 필요한 정리 작업
	UE_LOG(LogTemp, Log, TEXT("Inventory Widget Destructed"));
}

void UInventoryWidget::ToggleInventory()
{
	// 현재 상태에 따라 위젯을 화면에 추가하거나 제거
	if (bIsInventoryOpen)
	{
		RemoveFromParent(); // 위젯을 닫음
	}
	else
	{
		AddToViewport(); // 위젯을 엶
	}

	// 상태를 토글
	bIsInventoryOpen = !bIsInventoryOpen;

	// Delegate 호출: 현재 인벤토리가 열려 있는지 여부를 전달
	OnInventoryToggled.Broadcast(bIsInventoryOpen);
}

void UInventoryWidget::SetInventoryItems(const TArray<FItemData>& Items)
{
	InventoryItems = Items;

	// 인벤토리 그리드 업데이트
	UpdateInventoryGrid();
}

void UInventoryWidget::UpdateInventoryGrid()
{
	if (!GridPanel) return;

	// 기존 그리드의 모든 항목을 지움
	GridPanel->ClearChildren();

	const int32 NumColumns = 5;  // 그리드 열 수 설정
	int32 Row = 0;
	int32 Column = 0;

	// 인벤토리 아이템 리스트를 순회하여 그리드에 배치
	for (const FItemData& Item : InventoryItems)
	{
		if (UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass))
		{
			// 슬롯에 아이템 데이터를 설정
			SlotWidget->SetItemData(Item);

			// 그리드에 추가
			GridPanel->AddChildToGrid(SlotWidget, Row, Column);

			// 열 증가, 필요시 행 증가
			Column++;
			if (Column >= NumColumns)
			{
				Column = 0;
				Row++;
			}
		}
	}
}