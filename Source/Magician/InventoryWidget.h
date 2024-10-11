// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryActorComponent;
class UInventorySlotWidget;
class UGridPanel;
class UCanvasPanel;
// 델리게이트(대리자) -> 인벤토리 On / Off , 의존성 줄일려고 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryToggled, bool, bIsOpen);
/**
 * 
 */
UCLASS()
class MAGICIAN_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category= "Inventory")
	FOnInventoryToggled OnInventoryToggled;

	//인벤토리 여/닫 함수 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventoryItems(UInventoryActorComponent* InventoryComponent);
	
protected:

	// 현재 인벤토리 열림상태
	bool bIsInventoryOpen = false;

	//TODO:해당 아이템 Array 데이터들이 --> Inventory Actor Component 에서 가져오기 
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	//TArray<FItemData> InventoryItems;

	//그리드 패널
	UPROPERTY(meta = (BindWidget))
	UGridPanel* GridPanel;

	//그리드 패널
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	//그리드를 업데이트는 함수
	void UpdateInventoryGrid(UInventoryActorComponent* InventoryComponent);

	//그리드에 아이템을 추가할 슬롯 위젯 클래스

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
