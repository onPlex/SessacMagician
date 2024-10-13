// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InteractableInterface.h"
#include "ItemData.h"
#include "PBullet.h"
#include "InventoryActorComponent.h"
#include "InventoryWidget.h"
#include "UIManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Skeletal Mesh Resource import
	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/MyResource/UnityChan/unitychan.unitychan'"));


	if (InitMesh.Succeeded()) //리소스가 제대로 확보되었으면 ( 접근 + 가져오기)
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88)
		                                          , FRotator(0, -90, 0));
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0, 0, 50),
	                                              FRotator(-20, 0, 0));
	SpringArmComp->TargetArmLength = 530;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	//FName -> 문자열 -> 검색 기능에 최적화
	//데이터 적으로 문자열을 취급 -> (F)String
	WeaponMeshComp->SetupAttachment(GetMesh(), FName(TEXT("Character1_RightHandSocket")));


	FireCoolTime = 1.85f;
	FireTimerTime = 0;
	FireReady = true;
	
		
	DefaultInventory = CreateDefaultSubobject<UInventoryActorComponent>(TEXT("DefaultInventory"));
	DefaultInventory->MaxInventorySize = 30;
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	//일정 주기마다 Trace 로 실행하는 타이머 설정 (0.2f)
	FTimerHandle TraceTimerHandle;
	GetWorldTimerManager().SetTimer
	(TraceTimerHandle,
		this,
		&ATPSPlayer::PerformInteractionTrace,
		0.2f, true);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsytem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PlayerController->GetLocalPlayer()))
		{
			Subsytem->AddMappingContext(PlayerMappingContext, 0);
		}
	}

	UIManagerRef= Cast<AUIManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AUIManager::StaticClass()));	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FireReady)
	{
		FireCoolTimer(FireCoolTime, DeltaTime);
	}
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveIA, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
		EnhancedInputComponent->BindAction(LookUpIA, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);
		EnhancedInputComponent->BindAction(TurnIA, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
		EnhancedInputComponent->BindAction(JumpIA, ETriggerEvent::Started, this, &ATPSPlayer::InputJump);
		EnhancedInputComponent->BindAction(FireIA, ETriggerEvent::Started, this, &ATPSPlayer::InputFire);
		EnhancedInputComponent->BindAction(InteractionIA, ETriggerEvent::Started, this,
		                                   &ATPSPlayer::InteractionPositive);
		EnhancedInputComponent->BindAction(InventoryIA, ETriggerEvent::Started, this,
										   &ATPSPlayer::ToggleInventory);
	}
}

void ATPSPlayer::Move(const FInputActionValue& Value)
{
	//입력 값을 FVector 형태로 가져옴
	const FVector _CurrentValue = Value.Get<FVector>();

	//컨트롤러 Valid Check
	if (Controller)
	{
		// MoveDircetion 벡터에 입력된 , X, Y 값을 설정
		MoveDircetion.X = _CurrentValue.Y; //좌우 방향 값 
		MoveDircetion.Y = _CurrentValue.X; // 전후 방향 값
	}

	// 카메라의 현재 회전에 맞춰서, MoveDircetion을 변환
	MoveDircetion = FTransform(GetControlRotation()).TransformVector(MoveDircetion);
	// [위치 - 스케일 - 회전]
	// 변환된 MoveDircetion 방향으로 캐릭터를 이동
	AddMovementInput(MoveDircetion);
	MoveDircetion = FVector::ZeroVector;
}

void ATPSPlayer::LookUp(const FInputActionValue& Value)
{
	//입력 값을 FVector 형태로 가져옴
	float _CurrentValue;

	if (isInvertLookUp)
	{
		_CurrentValue = Value.Get<float>() * -1;
	}
	else
	{
		_CurrentValue = Value.Get<float>();
	}
	AddControllerPitchInput(_CurrentValue);
}

void ATPSPlayer::Turn(const FInputActionValue& Value)
{
	//입력 값을 FVector 형태로 가져옴
	const float _CurrentValue = Value.Get<float>();

	AddControllerYawInput(_CurrentValue);
}

void ATPSPlayer::InputJump(const FInputActionValue& Value)
{
	Jump();
}

void ATPSPlayer::InputFire(const FInputActionValue& Value)
{
	if (FireReady)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AttackAnimMontage);
			//UE_LOG(LogTemp, Warning, TEXT("Mong"));
		}

		FireReady = false;
	}
}

void ATPSPlayer::InteractionPositive(const FInputActionValue& Value)
{
	//상호작용 수행 임시
	if (CachedInteractableActor)
	{
		IInteractableInterface* InteratableActor
			= Cast<IInteractableInterface>(CachedInteractableActor);

		InteratableActor->Interact();
	}

	/*
    // LineTrace 
	if(GetWorld()->LineTraceSingleByChannel(_HitOut,_StartPoint,_EndPoint,ECC_GameTraceChannel2,_TraceParams))
	{
		//LineTrace 성공시, 디버그 메세지 출력
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Actor: %s"),
			*_HitOut.GetActor()->GetName()));
		//LineTrace 성공시, 디버그 메세지 출력
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Location: %s"),
			*_HitOut.ImpactPoint.ToString()));

       // 히트된 위치까지의 디버그 라인 그리기		
		DrawDebugLine(GetWorld(), _StartPoint,_HitOut.ImpactPoint,FColor::Red,false,5.f,0,2.f);

		//히트된 위치에 디버그 스피어 그리기 
		DrawDebugSphere(GetWorld(), _HitOut.ImpactPoint,10.f,12,FColor::Blue,false,5.f);
	}
	else
	{
		//LineTrace 실패 시, 디버그 메세지 출력
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("No HIT"));

		// 히트된 위치까지의 디버그 라인 그리기		
		DrawDebugLine(GetWorld(), _StartPoint,_EndPoint,FColor::Blue,false,5.f,0,2.f);
	}

	//DrawDebugLine(GetWorld(),_StartPoint,_EndPoint, FColor::Green,true,10.f);

	
	//_HitResults 반복문을 통해서 확인
	//결과값을 Array 복수형태로 
	//TArray<FHitResult> _HitResults;
	//GetWorld()->LineTraceMultiByChannel(_HitResults,_StartPoint,_EndPoint,ECC_GameTraceChannel1,_TraceParams);
	//ECC_GameTraceChannel1 -> Collision Profile 셋팅에 직접 만든 첫번째 채널
	for(FHitResult& Hit: _HitResults)
	{
		FVector HitLocation = Hit.ImpactPoint;
		//FVector Debug 

		//트레이스 쏜, 시작 지점에서 , 충돌한 지점까지 그린 debug 선 
		DrawDebugLine(GetWorld(), _StartPoint,HitLocation,FColor::Red,true,10.f,0,5.f);

		DrawDebugSphere(GetWorld(), HitLocation,10.f,12,FColor::Blue,true,5.f);

		// 
		_StartPoint = HitLocation;
	}*/

}


void ATPSPlayer::PerformInteractionTrace()
{
	//시작점 
	FVector _StartPoint = GetActorLocation();
	//끝점
	FVector _EndPoint = _StartPoint + GetActorForwardVector() * 2000.f;
	//Trace 결과 값 Struct
	FHitResult _HitOut;
	FCollisionQueryParams _TraceParams;
	_TraceParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(_HitOut, _StartPoint, _EndPoint, ECC_GameTraceChannel2,
	                                                 _TraceParams);

	//bHit -> Trace (구조체) 결과 값 있으면
	if (bHit)
	{
		AActor* HitActor = _HitOut.GetActor();

		if (HitActor)
		{
			//해당 액터에, 특정 인터페이스를 소유하고 있는지 여부를 알고 싶으면, Casting을 해보면됨
			// Cast 성공하면 , 해당 인터페이스 소유   <-> 실패하면 , 없음
			IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(HitActor);
			if (InteractableActor) // 해당액터가 인터렉션 가능 
			{
				//새로운 상호작용 대상이 이전 상호작용 대상과 다르면 
				if (CachedInteractableActor != HitActor)
				{
					//이전 상호작용 대상이, 위젯을 Hide
					if (CachedInteractableActor)
					{
						IInteractableInterface* CachedInteractable
							= Cast<IInteractableInterface>(CachedInteractableActor);
						if (CachedInteractable)
						{
							CachedInteractable->HideInteractionWidget();
						}
					}
				}

				//새로운 Hit액터를 캐시하고, 위젯 표시
				CachedInteractableActor = HitActor;
				InteractableActor->DiplayInteractionWidget();
			}
		}
	}
	else //No Hit
	{
		//과게어 상호작용한 대상이 있으면 
		if (CachedInteractableActor)
		{
			IInteractableInterface* CachedInteractable = Cast<IInteractableInterface>(CachedInteractableActor);
			if (CachedInteractable)
			{
				//위젯 끄기
				CachedInteractable->HideInteractionWidget();
			}
			//상호작용하는 대상이 없으니 Null로 할당
			CachedInteractableActor = nullptr;
		}
	}

	//Debug 그리기
	//if (bHit)
	//{
	//	// 히트된 위치까지의 디버그 라인 그리기		
	//	DrawDebugLine(GetWorld(), _StartPoint, _HitOut.ImpactPoint, FColor::Red, false, 5.f, 0, 2.f);
	//	//히트된 위치에 디버그 스피어 그리기 
	//	DrawDebugSphere(GetWorld(), _HitOut.ImpactPoint, 10.f, 12, FColor::Blue, false, 5.f);
	//	//LineTrace 성공시, 디버그 메세지 출력
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Actor: %s"),
	//		                                 *_HitOut.GetActor()->GetName()));
	//}
	//else
	//{
	//	//LineTrace 실패 시, 디버그 메세지 출력
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("No HIT"));
	//	// 히트된 위치까지의 디버그 라인 그리기		
	//	DrawDebugLine(GetWorld(), _StartPoint, _EndPoint, FColor::Blue, false, 5.f, 0, 2.f);
	//}

}

void ATPSPlayer::SpawnBullet()
{
	FTransform _firePosition = WeaponMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<APBullet>(magazine, _firePosition);
}
void ATPSPlayer::FireCoolTimer(float Duration, float deltatTime)
{
	// 장전중
	if (FireTimerTime < Duration)
	{
		FireTimerTime += deltatTime;
	}
	else //재장전 
	{
		FireTimerTime = 0;
		FireReady = true;
	}
}

void ATPSPlayer::UpdateMoney(int64 inputVal)
{
	int64 _result;
	_result = CurrentMoney + inputVal;

	if(_result < 0 )
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"Your money is less than zero");
	}
	else
	{
		CurrentMoney = _result;
	}
}


void ATPSPlayer::ToggleInventory(const FInputActionValue& Value)
{
	if (UIManagerRef && UIManagerRef->InventoryWidget)
	{
		// 인벤토리 열기 전에 데이터를 전달
		UIManagerRef->InventoryWidget->SetInventoryItems(DefaultInventory);
		UIManagerRef->InventoryWidget->ToggleInventory();
	}	
}


void ATPSPlayer::HandleInventoryToggled(bool bIsOpen)
{
	bIsInventoryOpen = 	bIsOpen;

	if(bIsInventoryOpen)
	{
		UE_LOG(LogTemp, Log,TEXT("Inventory Opened"));
	}
	else
	{
		UE_LOG(LogTemp, Log,TEXT("Inventory Closed"));
	}
}

/*
void ATPSPlayer::AddItemToInventory(FItemData& item)
{
	Inventory.Add(item);
}

void ATPSPlayer::RemoveItemFromInventory(int32 ItemID)
{
	//인벤토리를 한바퀴 for 검색
	for(int32 i = 0; i < Inventory.Num(); i++)
	{
		//아이템 ID가 맞는지 확인
		if (Inventory[i].ItemId == ItemID)
		{
			// 스택가능한 아이템인지 확인
			if(Inventory[i].bIsStackable)
			{
				// 수량을 - 1
				Inventory[i].MaxStackCount -= 1;

				if(Inventory[i].MaxStackCount <= 0)
				{
					Inventory.RemoveAt(i);
				}
				else
				{
					//예외처리 로그
				}
			}
			else // 스태형이 아니면 바로삭제 
			{
				Inventory.RemoveAt(i);
			}
		
			return;
		}		
	}
}*/
