// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "PBullet.h"


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
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

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
	//시작점 
    FVector _StartPoint  = GetActorLocation();
	//끝점
	FVector _EndPoint = _StartPoint + GetActorForwardVector() * 2000.f;
	//Trace 결과 값 Struct
	//FHitResult _HitOut;

	//결과값을 Array 복수형태로 
	TArray<FHitResult> _HitResults;

	
	FCollisionQueryParams _TraceParams;

	//GetWorld()->LineTraceSingleByChannel(_HitOut,_StartPoint,_EndPoint,ECC_Visibility,_TraceParams);

	GetWorld()->LineTraceMultiByChannel(_HitResults,_StartPoint,_EndPoint,ECC_GameTraceChannel1,_TraceParams);
    //ECC_GameTraceChannel1 -> Collision Profile 셋팅에 직접 만든 첫번째 채널

	//DrawDebugLine(GetWorld(),_StartPoint,_EndPoint, FColor::Green,true,10.f);

	//_HitResults 반복문을 통해서 확인
	for(FHitResult& Hit: _HitResults)
	{
		FVector HitLocation = Hit.ImpactPoint;
		//FVector Debug 

		//트레이스 쏜, 시작 지점에서 , 충돌한 지점까지 그린 debug 선 
		DrawDebugLine(GetWorld(), _StartPoint,HitLocation,FColor::Red,true,10.f,0,5.f);

		DrawDebugSphere(GetWorld(), HitLocation,10.f,12,FColor::Blue,true,5.f);

		// 
		_StartPoint = HitLocation;
	}
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
