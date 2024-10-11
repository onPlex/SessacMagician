// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"  // FItemData 구조체 포함
#include "InventoryWidget.generated.h"
// Delegate 선언: 인벤토리 열림/닫힘 상태를 다른 클래스에서 구독할 수 있도록 설정
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryToggled, bool, bIsOpen);

class UCanvasPanel;
class UGridPanel;
class UInventorySlotWidget;

UCLASS()
class MAGICIAN_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Delegate 인스턴스 생성 (인벤토리 열림/닫힘을 알림)
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryToggled OnInventoryToggled;

	// 인벤토리 열기/닫기 함수
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void ToggleInventory();
	
	// 인벤토리 아이템 리스트를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventoryItems(const TArray<FItemData>& Items);

protected:
	// 인벤토리 상태를 저장 (열림 또는 닫힘)
	bool bIsInventoryOpen = false;

	// 인벤토리 아이템 리스트
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<FItemData> InventoryItems;

	// 그리드 패널 (아이템 배치를 위한 GridPanel)
	UPROPERTY(meta = (BindWidget))
	UGridPanel* GridPanel;

	// 캔버스 패널 (UI 전체를 관리하는 패널)
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	// 그리드를 업데이트하는 함수
	void UpdateInventoryGrid();

	// 그리드에 아이템을 추가할 슬롯 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	// 위젯을 열고 닫는 함수
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
};
