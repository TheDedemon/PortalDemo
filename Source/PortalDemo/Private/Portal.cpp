// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCapture");
	SceneCapture->SetupAttachment(RootComponent);

	Screen = CreateDefaultSubobject<UStaticMeshComponent>("Screen");
	Screen->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	RuntimeSceneCaptureTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, ViewportSize.X * ScreenQuality, ViewportSize.Y * ScreenQuality);

	RuntimeScreenMaterial = UMaterialInstanceDynamic::Create(DefaultScreenMaterial, this);

	SceneCapture->TextureTarget = RuntimeSceneCaptureTarget;
	Screen->SetMaterial(0, RuntimeScreenMaterial);

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &APortal::OnWorldBeginPlay);
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::Link(APortal* Other)
{
	LinkedPortal = Other;
	RuntimeScreenMaterial->SetTextureParameterValue("RenderTarget", Other->RuntimeSceneCaptureTarget);
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

