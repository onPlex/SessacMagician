// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "NPC.generated.h"

UCLASS()
class MAGICIAN_API ANPC : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
   class UBoxComponent* BoxComponent;
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
   class UCameraComponent* CameraComponent; // 상호작용하면 Focus, 
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
   class USkeletalMeshComponent* Skeletal;

	UPROPERTY(EditAnywhere, Category = "Interaction")
   class UWidgetComponent* InterationWidgetComponent;

// 	
protected:
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<UUserWidget> InteractionWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<UUserWidget> ItemShopWidgetClass;
	UPROPERTY()
	class UUserWidget* ItemShopWidget;
	
public:
	//상호작용 위젯을 표시하는 함수 (상호작용이 가능할때, )
	virtual void DiplayInteractionWidget() override;
	//상호작용이 종료되었을때, 위젯 숨기는 함수
	virtual void HideInteractionWidget() override;
	//상호작용이 실행될 때, 호출 되는 함수
	virtual void Interact()override;
	

	
};
