// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANPC::ANPC()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("InteractionBox");
	SetRootComponent(BoxComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("FocusCamera");
	CameraComponent->SetupAttachment(RootComponent);
	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal");
	Skeletal->SetupAttachment(RootComponent);

	InterationWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InterationWidgetComponent->SetupAttachment(Skeletal);

	InterationWidgetComponent->SetRelativeLocation(FVector(0, 0, 220.f));
	InterationWidgetComponent->SetRelativeRotation(FRotator(90, 0, 90));
	InterationWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	InterationWidgetComponent->SetDrawSize(FVector2d(50, 50));

	//if(InteractionWidgetClass)
	//{
	//	InterationWidgetComponent->SetWidgetClass(InteractionWidgetClass);
	//}	
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	if (InteractionWidgetClass)
	{
		InterationWidgetComponent->SetWidgetClass(InteractionWidgetClass);
	}

	InterationWidgetComponent->SetVisibility(false);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANPC::DiplayInteractionWidget()
{
	if (InterationWidgetComponent)
	{
		InterationWidgetComponent->SetVisibility(true);
	}
}

void ANPC::HideInteractionWidget()
{
	if (InterationWidgetComponent)
	{
		InterationWidgetComponent->SetVisibility(false);
	}
}

void ANPC::Interact()
{
	//UE_LOG(LogTemp, Log, TEXT("Interact"));

	//TODO:상호작용 로직 추가

	APlayerController* myPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	myPlayerController->SetViewTargetWithBlend(this, 1.0f);
	myPlayerController->SetInputMode(FInputModeUIOnly());
	myPlayerController->SetShowMouseCursor(true);

	HideInteractionWidget();

	if (ItemShopWidgetClass)
	{
		ItemShopWidget = CreateWidget<UUserWidget>(GetWorld(), ItemShopWidgetClass);


		FName PropertyName(TEXT("OwnerNPC"));
		FProperty* NPCProperty = ItemShopWidget->GetClass()->FindPropertyByName(PropertyName);
		if (NPCProperty)
		{
			FObjectProperty* ObjectProperty = CastField<FObjectProperty>(NPCProperty);
			if (ObjectProperty && ObjectProperty->PropertyClass == ANPC::StaticClass())
			{
				ObjectProperty->SetObjectPropertyValue_InContainer(ItemShopWidget, this);
			}
		}

		if (ItemShopWidget)ItemShopWidget->AddToViewport();
	}
}
