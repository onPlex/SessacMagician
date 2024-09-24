// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANPC::ANPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	SetRootComponent(BoxComp);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FocusCamera"));
	CameraComp->SetupAttachment(RootComponent);
	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
	Skeletal->SetupAttachment(RootComponent);

	// UWidgetComponent 생성 및 초기화
	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(Skeletal);

	// 머리 위에 배치: 스켈레탈 메쉬의 특정 소켓 또는 본에 부착
	InteractionWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));  // 머리 위로 220 units 올림
	InteractionWidgetComponent->SetRelativeRotation(FRotator(90,0,0));// 정면으로 회전 
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::World);  // 월드에 고정된 형태로 표시 (또는 화면 Space)
	InteractionWidgetComponent->SetDrawSize(FVector2D(50.0f, 50.0f));  // 위젯 크기 설정

	// 위젯 클래스를 지정하고 위젯을 생성하여 컴포넌트에 적용
	if (InteractionWidgetClass)
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);		
	}
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	// 위젯을 비활성화 상태로 시작
	InteractionWidgetComponent->SetVisibility(false);
}


void ANPC::DisplayInteractionWidget()
{
	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(true);  // 위젯 표시
	}
}

void ANPC::HideInteractionWidget()
{
	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(false);  // 위젯 숨김
	}
}

void ANPC::Interact()
{
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this,0);

	OurPlayerController->SetViewTargetWithBlend(this,1.f);
	OurPlayerController->SetInputMode(FInputModeUIOnly());
	OurPlayerController->SetShowMouseCursor(true);

	HideInteractionWidget();

	if(ItemShopWidgetClass)
	{
		ItemShopWidget = CreateWidget<UUserWidget>(GetWorld(),ItemShopWidgetClass);
		
		// 블루프린트 변수 'OwnerNPC'에 접근
		FName PropertyName(TEXT("OwnerNPC"));
		FProperty* NPCProperty = ItemShopWidget->GetClass()->FindPropertyByName(PropertyName);
		if (NPCProperty)
		{
			// 변수가 객체인 경우, FObjectProperty로 캐스팅하여 설정
			FObjectProperty* ObjectProperty = CastField<FObjectProperty>(NPCProperty);
			if (ObjectProperty && ObjectProperty->PropertyClass == ANPC::StaticClass())
			{
				ObjectProperty->SetObjectPropertyValue_InContainer(ItemShopWidget, this);
			}
		}

		if(ItemShopWidget)ItemShopWidget->AddToViewport();		
	}
}
