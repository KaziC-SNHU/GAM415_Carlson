// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "GAM415_CarlsonCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Intialize components
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalCollision"));
	PortalCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalCapture"));


	// Root component
	RootComponent = PortalCollision;
	// Attachments
	PortalMesh->SetupAttachment(RootComponent);
	PortalCapture->SetupAttachment(RootComponent);

	PortalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	// Set up Collision
	PortalCollision->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
	PortalMesh->SetHiddenInSceneCapture(true);

	// If PortalMaterial is set in editor, apply it to the mesh
	if (PortalMaterial)
	{
		PortalMesh->SetMaterial(0, PortalMaterial);
	}
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePortals();
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGAM415_CarlsonCharacter* playerChar = Cast<AGAM415_CarlsonCharacter>(OtherActor);

	// If player char valid, teleport them to the other portal
	if (playerChar)
	{
		if (OtherPortal)
		{
			if (!playerChar->bisTeleporting)
			{
				// Teleport player
				playerChar->bisTeleporting = true;
				FVector loc = OtherPortal->GetActorLocation();
				playerChar->SetActorLocation(loc);

				// Protect from immediate re-teleporting
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(this, FName("SetBool"), playerChar);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 1, false);
			}
		}
	}
}

void APortal::SetBool(AGAM415_CarlsonCharacter* playerChar)
{
	if (playerChar)
	{
		playerChar->bisTeleporting = false;
	}
}

void APortal::UpdatePortals()
{
	// Get Transforms
	FVector Location = this->GetActorLocation() - OtherPortal->GetActorLocation();
	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation();
	FRotator CameraRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentRotation();
	FVector CombinedLocation = Location + CameraLocation;

	// Set Capture Location and Rotation
	PortalCapture->SetWorldLocationAndRotation(CombinedLocation, CameraRotation);
}

