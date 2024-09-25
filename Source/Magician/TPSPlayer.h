// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TPSPlayer.generated.h"


class UInputMappingContext;
class UInputAction;

UCLASS()
class MAGICIAN_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//Spring Arm 컴포넌트 생성
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArmComp;
	//Camera 컴포넌트 생성
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = EquipItems)
	class UStaticMeshComponent* WeaponMeshComp;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookUpIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TurnIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FireIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractionIA;

	void Move(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputFire(const FInputActionValue& Value);
	void InteractionPositive(const FInputActionValue& Value);

private:
	FVector MoveDircetion;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOptions)
	bool isInvertLookUp = false; // 이 데이터의 초기값이 false

	UPROPERTY(EditAnywhere, Category = Fire)
	TSubclassOf<class APBullet> magazine;

private:
	AActor* CachedInteractableActor; // 현재 상호작용 중인 액터를 저장 (캐싱 변수)
	void PerformInteractionTrace();
public:
	void SpawnBullet();

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackAnimMontage;


private:
	bool FireReady;
	float FireTimerTime;

public:

	UPROPERTY(EditAnywhere, Category = "Fire")
	float FireCoolTime;

	
	
protected:
	void FireCoolTimer(float Duration, float deltatTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int64 CurrentMoney = 9999;

	UFUNCTION(BlueprintCallable)
	void UpdateMoney(int64 inputVal);
/*
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	TArray<FItemData> Inventory;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(FItemData& item);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromInventory(int32 ItemID);*/
};
