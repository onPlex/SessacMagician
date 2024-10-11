// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIManager.generated.h"

class UInventoryWidget;

UCLASS()
class MAGICIAN_API AUIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIManager();

    // 인베토리 위젯을 생성하는 함수
	void CreateInventoryWidget();

	// 인벤토리 위젝의 Delegate 를 구독 함수
	void BindInventoryToggleDelegate();
	
	//제작된 위젯 (걔) 정확한 지칭 한 주소 
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
protected:
	virtual void BeginPlay() override;

	// static클래스 골라줘서 -> 해당 위젯형태로 제작
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
};
