// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"
#include "InventoryWidget.h"
#include "TPSPlayer.h"

// Sets default values
AUIManager::AUIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AUIManager::BeginPlay()
{
	Super::BeginPlay();

	CreateInventoryWidget();
}

void AUIManager::CreateInventoryWidget()
{
	if(InventoryWidgetClass)
	{
		//인벤토리 위젯 생성
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);

		if(InventoryWidget)
		{
			//델리게이트 바인드 - 연결
			BindInventoryToggleDelegate();
		}
	}
}

void AUIManager::BindInventoryToggleDelegate()
{
	//델리게이트를 구독할 대상 -> 플레이어 -> 플레이어 찾아다가 등록(Add) 
	if(ATPSPlayer* Player = Cast<ATPSPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn()))
		//기존에 자주 쓰던 GetPlayerCharacter() ~ 
 	{
		//플레이어가 Delegate 구독
		InventoryWidget->OnInventoryToggled.AddDynamic(Player,&ATPSPlayer::HandleInventoryToggled);
	}
}


