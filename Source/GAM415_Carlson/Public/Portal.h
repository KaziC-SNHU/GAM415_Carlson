// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/BoxComponent.h"
#include "GAM415_CarlsonCharacter.h"
#include "Portal.generated.h"


UCLASS()
class GAM415_CARLSON_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PortalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* PortalCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D* PortalRenderTarget;

	UPROPERTY(EditAnywhere)
	UBoxComponent* PortalCollision;
	
	UPROPERTY(EditAnywhere)
	APortal* OtherPortal;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* PortalMaterial;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetBool(AGAM415_CarlsonCharacter* playerChar);

	UFUNCTION()
	void UpdatePortals();

};
