// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Portal.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
}

void AFirstPersonCharacter::Move(const FInputActionValue& Value)
{
	if (GetController())
	{
		const FVector2D axisValues = Value.Get<FVector2D>();
		AddMovementInput(GetActorRightVector(), axisValues.X);
		AddMovementInput(GetActorForwardVector(), axisValues.Y);
	}
}

void AFirstPersonCharacter::Look(const FInputActionValue& Value)
{
	if (GetController())
	{
		const FVector2D axisValues = Value.Get<FVector2D>();
		AddControllerPitchInput(axisValues.Y);
		AddControllerYawInput(axisValues.X);
	}
}

void AFirstPersonCharacter::FirePortalA(const FInputActionValue& Value)
{
	FirePortal(true);
}

void AFirstPersonCharacter::FirePortalB(const FInputActionValue& Value)
{
	FirePortal(false);
}

void AFirstPersonCharacter::FirePortal(const bool firePortalA)
{
	FHitResult hitResult;
	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.AddIgnoredActor(this);

	FVector start = Camera->GetComponentLocation();
	FVector end = start + Camera->GetForwardVector() * MaxRaycastDistance;

	DrawDebugLine(GetWorld(), Camera->GetComponentLocation(), end, FColor::Red, false, 10.0f);

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Pawn, collisionQueryParams))
	{
		if (firePortalA)
		{
			if (PortalA == nullptr)
			{
				PortalA = GetWorld()->SpawnActor<APortal>(PortalTemplate);
			}

			PortalA->SetActorLocationAndRotation(hitResult.Location, hitResult.ImpactNormal.Rotation());
		}
		else
		{
			if (PortalB == nullptr)
			{
				PortalB = GetWorld()->SpawnActor<APortal>(PortalTemplate);
			}

			PortalB->SetActorLocationAndRotation(hitResult.Location, hitResult.ImpactNormal.Rotation());
		}

		if (PortalA != nullptr && PortalB != nullptr)
		{
			PortalA->Link(PortalB);
			PortalB->Link(PortalA);
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, hitResult.GetActor()->GetName());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Didn't hit"));
	}
}


// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::Jump);
		EnhancedInputComponent->BindAction(FirePortalAAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::FirePortalA);
		EnhancedInputComponent->BindAction(FirePortalBAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::FirePortalB);
	}
}

