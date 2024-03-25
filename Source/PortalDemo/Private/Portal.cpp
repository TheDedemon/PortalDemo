// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCapture");
	SceneCapture->SetupAttachment(RootComponent);
	SceneCapture->bOverride_CustomNearClippingPlane = true;

	Screen = CreateDefaultSubobject<UStaticMeshComponent>("Screen");
	Screen->SetupAttachment(RootComponent);

	BackFacingScene = CreateDefaultSubobject<USceneComponent>("BackFacingScene");
	BackFacingScene->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	RuntimeSceneCaptureTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, ViewportSize.X * ScreenQuality, ViewportSize.Y * ScreenQuality, RTF_RGBA32f);
	SceneCapture->TextureTarget = RuntimeSceneCaptureTarget;

	RuntimeScreenMaterial = UMaterialInstanceDynamic::Create(DefaultScreenMaterial, this);
	Screen->SetMaterial(0, RuntimeScreenMaterial);

	RuntimePlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &APortal::OnWorldBeginPlay);
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LinkedPortal)
	{
		FTransform relativeTransform = RuntimePlayerCameraManager->GetTransform().GetRelativeTransform(BackFacingScene->GetComponentTransform());
		LinkedPortal->SceneCapture->SetRelativeLocationAndRotation(relativeTransform.GetLocation(), relativeTransform.GetRotation());

		double distance = FVector::Distance(RuntimePlayerCameraManager->GetTransform().GetLocation(), GetActorLocation());
		LinkedPortal->SceneCapture->CustomNearClippingPlane = distance;
	}
}

void APortal::Link(APortal* Other)
{
	LinkedPortal = Other;
	RuntimeScreenMaterial->SetTextureParameterValue("RenderTarget", LinkedPortal->RuntimeSceneCaptureTarget);
}

void APortal::Unlink()
{
	LinkedPortal = nullptr;
	RuntimeScreenMaterial->SetTextureParameterValue("RenderTarget", nullptr);
}

void APortal::OnWorldBeginPlay()
{
	if (LinkedPortal)
	{
		Link(LinkedPortal);
	}
}

