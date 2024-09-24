// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InteractableInterface.h"
#include "PBullet.h"
#include "InventoryActorComponent.h"


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

	InitializeInventoryComponent();
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 일정 주기로 Trace를 실행하는 타이머 설정 (1초마다 실행)
	FTimerHandle TraceTimerHandle;
	GetWorldTimerManager().SetTimer(TraceTimerHandle, this, &ATPSPlayer::PerformInteractionTrace, 0.2f, true);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsytem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PlayerController->GetLocalPlayer()))
		{
			Subsytem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
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
		EnhancedInputComponent->BindAction(FireIA, ETriggerEvent::Triggered, this, &ATPSPlayer::InputFire);
		EnhancedInputComponent->BindAction(InteractionIA, ETriggerEvent::Started, this, &ATPSPlayer::InteractionPositive);
	}
}

void ATPSPlayer::InitializeInventoryComponent()
{
	// DefaultInventoryClass가 설정되어 있는지 확인
	if (DefaultInventoryClass)
	{
		// 블루프린트 기반의 인벤토리 컴포넌트를 생성
		DefaultInventory = NewObject<UInventoryActorComponent>(this, DefaultInventoryClass);

		// 생성된 컴포넌트를 액터에 추가
		if (DefaultInventory)
		{
			DefaultInventory->RegisterComponent();
			UE_LOG(LogTemp, Log, TEXT("Inventory component created successfully!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create Inventory component!"));
		}
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
	if(CachedInteractableActor)
	{		
		IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(CachedInteractableActor);
		// 상호작용 수행
		InteractableActor->Interact();
	}
}

void ATPSPlayer::SpawnBullet()
{
	FTransform _firePosition = WeaponMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<APBullet>(magazine, _firePosition);
}

void ATPSPlayer::PerformInteractionTrace()
{
	FVector _Start = GetActorLocation();
	FVector _End = GetActorLocation() + GetActorForwardVector() * 2000.f;
	FHitResult _HitOut;	

	FCollisionQueryParams _TraceParams;
	_TraceParams.AddIgnoredActor(this);

	// LineTrace 실행
	bool bHit = GetWorld()->LineTraceSingleByChannel(_HitOut, _Start, _End, ECC_GameTraceChannel1, _TraceParams);
    
	if (bHit)
	{
		AActor* HitActor = _HitOut.GetActor();
        
		// 상호작용 가능한 액터가 IInteractableInterface를 구현했는지 확인
		if (HitActor)
		{
			IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(HitActor);
			if (InteractableActor)
			{
				// 새로운 상호작용 대상이 이전 상호작용 대상과 다르면
				if (CachedInteractableActor != HitActor)
				{
					// 이전 상호작용 대상이 있었다면 위젯을 숨김
					if (CachedInteractableActor)
					{
						IInteractableInterface* CachedInteractable = Cast<IInteractableInterface>(CachedInteractableActor);
						if (CachedInteractable)
						{
							CachedInteractable->HideInteractionWidget();
						}
					}

					// 새로운 액터를 캐시하고 위젯 표시
					CachedInteractableActor = HitActor;
					InteractableActor->DisplayInteractionWidget();
				}
			}
		}
	}
	else
	{
		// 트레이스가 실패하면 캐시된 상호작용 대상이 있을 경우 위젯을 숨김
		if (CachedInteractableActor)
		{
			IInteractableInterface* CachedInteractable = Cast<IInteractableInterface>(CachedInteractableActor);
			if (CachedInteractable)
			{
				CachedInteractable->HideInteractionWidget();
			}
			CachedInteractableActor = nullptr;  // 상호작용 대상 초기화
		}
	}

	// 디버그용 라인 및 메시지 (필요한 경우)
	//if (bHit)
	//{
	//	DrawDebugLine(GetWorld(), _Start, _HitOut.ImpactPoint, FColor::Red, false, 1.f, 0, 2.f);
	//	DrawDebugSphere(GetWorld(), _HitOut.ImpactPoint, 10.f, 12, FColor::Yellow, false, 1.f);
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Actor: %s"), *_HitOut.GetActor()->GetName()));
	//}
	//else
	//{
	//	DrawDebugLine(GetWorld(), _Start, _End, FColor::Blue, false, 1.f, 0, 2.f);
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Hit"));
	//}
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

/*
// ======================================
// Player Item Section
// ======================================
void ATPSPlayer::AddItemToInventory(const FItemData& NewItem)
{
	Inventory.Add(NewItem);
	UE_LOG(LogTemp, Warning, TEXT("Item added: %s"), *NewItem.ItemName.ToString());
}

void ATPSPlayer::RemoveItemFromInventory(int32 ItemID)
{
	// 인벤토리에서 아이템을 찾는다.
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		// 아이템 ID가 일치하는지 확인
		if (Inventory[i].ItemID == ItemID)
		{
			// 스택 가능한 아이템인지 확인
			if (Inventory[i].bIsStackable)
			{
				// 수량을 1 감소
				Inventory[i].MaxStackCount -= 1;

				// 수량이 0 이하가 되면 인벤토리에서 제거
				if (Inventory[i].MaxStackCount <= 0)
				{
					Inventory.RemoveAt(i);
					UE_LOG(LogTemp, Warning, TEXT("Item removed: %d"), ItemID);
				}
				else
				{
					// 수량이 0 이상이면 수량 감소를 로그로 남긴다.
					UE_LOG(LogTemp, Warning, TEXT("Item quantity decreased: %d, Remaining: %d"), ItemID, Inventory[i].MaxStackCount);
				}
			}
			else
			{
				// 스택 불가능한 아이템이면 바로 제거
				Inventory.RemoveAt(i);
				UE_LOG(LogTemp, Warning, TEXT("Non-stackable item removed: %d"), ItemID);
			}

			// 아이템을 처리한 후 함수를 종료
			return;
		}
	}	// 아이템이 인벤토리에 없을 경우 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Item not found in inventory: %d"), ItemID);
}*/

void ATPSPlayer::UpdateMoney(int64 inputVal)
{
	int64 _result;
	_result = CurrentMoney + inputVal;

	if(_result< 0 )
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f , FColor::Green, TEXT("Not Enough Money ! "));
	}
	else
	{
		CurrentMoney = _result;
	}
}
