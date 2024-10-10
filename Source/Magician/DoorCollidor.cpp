// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorCollidor.h"
#include "NiagaraComponent.h"
#include "TPSPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoorCollidor::ADoorCollidor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    colliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(colliderComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	meshComp->SetupAttachment(colliderComp);

	niagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FxComp"));
	niagaraComp->SetupAttachment(colliderComp);	 
}

// Called when the game starts or when spawned
void ADoorCollidor::BeginPlay()
{
	Super::BeginPlay();

	colliderComp->OnComponentBeginOverlap.AddDynamic(this,&ADoorCollidor::ADoorCollidor::OverlapLevelLoadBeing);
}

void ADoorCollidor::OverlapLevelLoadBeing(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATPSPlayer* _MyCharacter = Cast<ATPSPlayer>(OtherActor);

	if(OtherActor && levelToLoad != "")
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"DoorCollidor::OverlapLevelLoadBeing");
		//UGameplayStatics::OpenLevel(this, levelToLoad);
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("OnLevelStreamComplete");
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = __LINE__;
		
		
		UGameplayStatics::LoadStreamLevel(this, levelToLoad, true, false,LatentInfo );
		//UGameplayStatics::UnloadStreamLevel()
	}
}

// Called every frame
void ADoorCollidor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorCollidor::OnLevelStreamComplete()
{
	//GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,"TestCallBack");
	UE_LOG(LogTemp,Display,TEXT("OnLevelStreamComplete"));
}

