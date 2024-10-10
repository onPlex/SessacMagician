// Fill out your copyright notice in the Description page of Project Settings.


#include "Clock.h"

#include "Components/DirectionalLightComponent.h"
#include "Engine/DirectionalLight.h"

// Sets default values
AClock::AClock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AClock::BeginPlay()
{
	Super::BeginPlay();
}

//30 FPS, 60 FPS -> 1초에 30 번을 렌더링했다, 1초에 60 번을 렌더링했다 , 90FPS
// pro 90hz -> 120hz
// 다른 누군가 렉이 튕김 , 롤
// MMORPG -> Tick 서버 -> 
// Called every frame
void AClock::Tick(float DeltaTime) // 한-프레임이 완료되기까지 걸리는 시간
{
	Super::Tick(DeltaTime);
	UpdateTime(DeltaTime);
	RotateDirectionalLightWithTime(Sun);
	UpdateSunColorByHourMinute(Sun);
	//GetTimeByTotalSec(TotalSeconds);
}

FString AClock::GetTimeByTotalSec(float totalSec)
{
	CurrentDay = (int)totalSec / ONE_DAY % 30;
	CurrentHour = (int)totalSec / ONE_HOUR % 24;
	CurrentMinute = (int)totalSec / ONE_MINUTE % 60;
	CurrentSecond = (int)totalSec % 60;

	FString Now;
	Now = "D:" + FString::SanitizeFloat(CurrentDay)
		+ "-H:" + FString::SanitizeFloat(CurrentHour)
		+ "-M:" + FString::SanitizeFloat(CurrentMinute)
		+ "-S:" + FString::SanitizeFloat(CurrentSecond);


	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, (TEXT("%s"), Now));
	return Now;
}

void AClock::UpdateTime(float DeltaSec)
{
	TotalSeconds += DeltaSec * TimeScale; //DeltaSec를 더하여서, 지속적으로 초당 시간이 증가 됨
	// TimeScale 을 활용해서 배속 구현 

	//확정적인 과학(자연) 값 
	CurrentDay = TotalSeconds / 86400; // -> 1일은 86400초
	int32 RemainingSecondsAfterDay = (int)TotalSeconds % 86400; //Day 을 제외한 나머지 초
	CurrentHour = RemainingSecondsAfterDay / 3600; // 60(1시간 -> 60분) * 60 (1분 -> 60초)
	int32 RemainingSecondsAfterHour = (int)RemainingSecondsAfterDay % 3600; //시간을 제외한 나머지 초
	CurrentMinute = RemainingSecondsAfterHour / 60; // 분으로 나눠서 몇 분인지 반환
	CurrentSecond = RemainingSecondsAfterHour % 60; // 분을 제외한 나머지 값이 -> 초
}

void AClock::RotateDirectionalLightWithTime(AActor* TargetActor)
{
	if (TargetActor)
	{
		ADirectionalLight* DirectionalLight = Cast<ADirectionalLight>(TargetActor);
		if (DirectionalLight)
		{
			//float _AnglePerHour = 360.0f / 24.0f; //24시간을 360도로 나눔 = 15
			//float _HourAngle = FMath::Fmod(CurrentHour * _AnglePerHour, 360.0f);
			//각도에 음수 개념이 있기에, 음수에 대한 나머지를 안전하게 처리하고자, FMath::Fmod 를 활용하여 나머지를 구함

			float _HourAngle = FMath::Fmod(CurrentHour * 15, 360.0f); //시간에따른 각도 계산
			float _MinuteAngle = (CurrentMinute / 60.0f) * 15; // 분에 따른 각도 계산
			float _SecondAngle = (CurrentSecond / 60.0f) * (15 / 60.0f); // 초에 따른 각도 계산
			float _CurrentAngle = _HourAngle + _MinuteAngle + _SecondAngle;

			FRotator _NewRotation = FRotator(_CurrentAngle, 0.0f, 0.0f);
			DirectionalLight->SetActorRotation(_NewRotation);
		}
	}
}

void AClock::UpdateSunColorByHourMinute(AActor* TargetActor)
{
	UDirectionalLightComponent* _DirectionalLightComp = Cast<ADirectionalLight>(TargetActor)->GetComponent();
	FLinearColor InterpolationColor;
	const float _LigthTime = CurrentHour * (CurrentMinute / 60);

	if (_LigthTime > 0 && _LigthTime <= 3)
	{
		InterpolationColor = FLinearColor::LerpUsingHSV(Color24, Color3, _LigthTime / 3.0f);
	}
	else if (_LigthTime > 3 && _LigthTime <= 6)
	{
		InterpolationColor = FLinearColor::LerpUsingHSV(Color24, Color3, (_LigthTime - 3) / 3.0f);
	}
	else if (_LigthTime > 6 && _LigthTime <= 9)
	{
		InterpolationColor = FLinearColor::LerpUsingHSV(Color24, Color3, (_LigthTime - 6) / 3.0f);
	}
	else if (_LigthTime > 9 && _LigthTime <= 12)
	{
		InterpolationColor = FLinearColor::LerpUsingHSV(Color24, Color3, (_LigthTime - 9) / 3.0f);
	}
	else if (_LigthTime > 12 && _LigthTime <= 15)
	{
		InterpolationColor = FLinearColor::LerpUsingHSV(Color24, Color3, (_LigthTime - 12) / 3.0f);
	}
	else if (_LigthTime > 15 && _LigthTime <= 18)
	{
		InterpolationColor = FLinearColor::LerpUsingHSV(Color24, Color3, (_LigthTime - 15) / 3.0f);
	}
	else if (_LigthTime > 18 && _LigthTime <= 21)
	{
		InterpolationColor = FLinearColor::LerpUsingHSV(Color24, Color3, (_LigthTime - 18) / 3.0f);
	}
	else if (_LigthTime > 21 && _LigthTime <= 24)
	{
		InterpolationColor = FLinearColor::LerpUsingHSV(Color24, Color3, (_LigthTime - 21) / 3.0f);
	}
	else
	{
		InterpolationColor = FLinearColor::White;
	}

	_DirectionalLightComp->SetLightColor(InterpolationColor);
}
