// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Clock.generated.h"

#define ONE_YEAR 31104000 // 360 일 기준 1년/ 초
#define ONE_MONTH 2592000 // 30일 기준 1달 /초
#define ONE_DAY 86400 // 1일 기준 24시간 / 초
#define ONE_HOUR 3600 // 1시간 24초
#define ONE_MINUTE 60 

UCLASS()
class MAGICIAN_API AClock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Time")
	float CurrentDay;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Time")
	float CurrentHour;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Time")
	float CurrentMinute;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Time")
	float CurrentSecond;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Time")
	float TotalSeconds;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Time")
	float TimeScale = 1.0f;

	UFUNCTION(BlueprintCallable)
	FString GetTimeByTotalSec(float totalSec);

	UPROPERTY(EditAnywhere,Category= "Sun")
	class AActor* Sun;

	UPROPERTY(EditAnywhere,Category= "Sun")
    FLinearColor Color3;
	UPROPERTY(EditAnywhere,Category= "Sun")
	FLinearColor Color6;
	UPROPERTY(EditAnywhere,Category= "Sun")
	FLinearColor Color9;
	UPROPERTY(EditAnywhere,Category= "Sun")
	FLinearColor Color12;
	UPROPERTY(EditAnywhere,Category= "Sun")
	FLinearColor Color15;
	UPROPERTY(EditAnywhere,Category= "Sun")
	FLinearColor Color18;
	UPROPERTY(EditAnywhere,Category= "Sun")
	FLinearColor Color21;
	UPROPERTY(EditAnywhere,Category= "Sun")
	FLinearColor Color24;
	  
protected:
	void UpdateTime(float DeltaSec);

	void RotateDirectionalLightWithTime(AActor* TargetActor);

	void UpdateSunColorByHourMinute(AActor* TargetActor);
};
