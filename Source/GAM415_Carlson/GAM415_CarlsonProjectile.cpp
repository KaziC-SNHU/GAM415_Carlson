// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAM415_CarlsonProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

AGAM415_CarlsonProjectile::AGAM415_CarlsonProjectile() 
{
	AGAM415_CarlsonProjectile::BaseMaterial = nullptr;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGAM415_CarlsonProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	// Set as root component
	RootComponent = CollisionComp;

	ProjectileMesh->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AGAM415_CarlsonProjectile::BeginPlay()
{
	Super::BeginPlay();
	RandomColor = FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f), UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f), UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f), 1.0f);

	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(ProjectileMat, this);
	ProjectileMesh->SetMaterial(0, DynamicMaterialInstance);

	DynamicMaterialInstance->SetVectorParameterValue("Color", RandomColor);
}

void AGAM415_CarlsonProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}

	if (OtherActor != nullptr)
	{
		// Generate frame
		float randomFrame = UKismetMathLibrary::RandomFloatInRange(0.0f, 3.0f);

		auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BaseMaterial, FVector(UKismetMathLibrary::RandomFloatInRange(20.0f, 40.0f)), Hit.Location, Hit.Normal.Rotation(), 0.0f);
		auto MatInstance = Decal->CreateDynamicMaterialInstance();

		// Set color and frame
		MatInstance->SetVectorParameterValue("Color", RandomColor);
		MatInstance->SetScalarParameterValue("Frame", randomFrame);
	}
}