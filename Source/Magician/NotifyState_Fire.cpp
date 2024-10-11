// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_Fire.h"
#include "TPSPlayer.h"

void UNotifyState_Fire::NotifyBegin(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("NotifyBegin"));

	TpsPlayer = Cast<ATPSPlayer>(MeshComp->GetOwner());

	if (TpsPlayer)
	{
		TpsPlayer->SpawnBullet();
	}
	
}


void UNotifyState_Fire::NotifyTick(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("NotifyTick"));
}

void  UNotifyState_Fire::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NotifyEnd"));
}