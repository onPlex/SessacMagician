// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractableInterface.h"
#include "NPC.generated.h"

UCLASS()
class MAGICIAN_API ANPC : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<class UUserWidget> InteractionWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<class UUserWidget> ItemShopWidgetClass;
	UPROPERTY()
	class UUserWidget* ItemShopWidget;
	
protected:
	// 위젯 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Interaction")
	class UWidgetComponent* InteractionWidgetComponent;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Interaction")
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Interaction")
	class UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Mesh")
	class USkeletalMeshComponent* Skeletal;

public:
	virtual void DisplayInteractionWidget() override;
	virtual void HideInteractionWidget() override;
	virtual void Interact() override;
};
