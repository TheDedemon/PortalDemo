// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FirstPersonCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class APortal;

UCLASS()
class PORTALDEMO_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	double MaxRaycastDistance = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMapping = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* LookAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* JumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* FirePortalAAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* FirePortalBAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APortal> PortalTemplate = nullptr;

	APortal* PortalA = nullptr;
	APortal* PortalB = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called for movement input
	void Move(const FInputActionValue& Value);
	// Called for looking input
	void Look(const FInputActionValue& Value);
	// Called for firing portal A
	void FirePortalA(const FInputActionValue& Value);
	// Called for firing portal B
	void FirePortalB(const FInputActionValue& Value);
	void FirePortal(const bool firePortalA);
};
