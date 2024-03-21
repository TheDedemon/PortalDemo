// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class UStaticMeshComponent;
class USceneCaptureComponent2D;
class UMaterial;
class UMaterialInstanceDynamic;
class UTextureRenderTarget2D;

UCLASS()
class PORTALDEMO_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Link(APortal* Other);
	virtual void Unlink();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = "0.1", UIMax = "5", ClampMin = "0.1", ClampMax = "5"))
	double ScreenQuality = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneCaptureComponent2D* SceneCapture = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Screen = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterial* DefaultScreenMaterial = nullptr;

	// TEMPORARY
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APortal* LinkedPortal = nullptr;

	UTextureRenderTarget2D* RuntimeSceneCaptureTarget = nullptr;
	UMaterialInstanceDynamic* RuntimeScreenMaterial = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// TEMPORARY
	void OnWorldBeginPlay();
};
