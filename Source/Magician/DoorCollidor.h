// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorCollidor.generated.h"

UCLASS()
class MAGICIAN_API ADoorCollidor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoorCollidor();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* colliderComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

    UPROPERTY(EditAnywhere)
	class UNiagaraComponent* niagaraComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OverlapLevelLoadBeing(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category= "Level")
	FName levelToLoad;

	UFUNCTION()
	void OnLevelStreamComplete();
};
