// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBullet.generated.h"

UCLASS()
class MAGICIAN_API APBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* PMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = Collidor)
	class USphereComponent* SphereCollComp;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* SMeshComp;
};
