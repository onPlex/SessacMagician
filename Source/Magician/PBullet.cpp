// Fill out your copyright notice in the Description page of Project Settings.


#include "PBullet.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APBullet::APBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollComp = CreateDefaultSubobject<USphereComponent>(TEXT("MainCollidor"));
	SphereCollComp->SetCollisionProfileName(TEXT("BlockAll"));
	SphereCollComp->SetSphereRadius(13);
	RootComponent = SphereCollComp;

	SMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SMeshComp->SetupAttachment(RootComponent);
	SMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//포물체 컴포넌트 생성
	PMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMovementComp"));

	//발사체 컴포넌트를 Update 시킬 대상(컴포넌트) 지정
	PMovementComponent->SetUpdatedComponent(SphereCollComp);

	//초기속도
	PMovementComponent->InitialSpeed = 3000.f;

	//최대속도
	PMovementComponent->MaxSpeed = 5000.f;

	//반동 , 튐 
	PMovementComponent->bShouldBounce = true;

	//반동 크기
	PMovementComponent->Bounciness = 0.3;

	//생명 길이 
	//InitialLifeSpan = 2.f;
}

// Called when the game starts or when spawned
void APBullet::BeginPlay()
{
	Super::BeginPlay();

	//Timer -> 시간 개념 -> 쓰레드를 독립적 할당 해줘
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer,
		FTimerDelegate::CreateLambda([this]()->void
			{Destroy();}),
			2.0f,
			false);	
}

// Called every frame
void APBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


