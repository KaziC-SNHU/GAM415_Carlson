// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDMIMod.h"
#include "GAM415_CarlsonCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACubeDMIMod::ACubeDMIMod()
{
	ACubeDMIMod::BaseMaterial = nullptr;
	ACubeDMIMod::DynamicMaterialInstance = nullptr;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));

	RootComponent = BoxComponent;
	CubeMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACubeDMIMod::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIMod::OnOverlapBegin);

	if (BaseMaterial)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	}
	
	if (CubeMesh)
	{
		CubeMesh->SetMaterial(0, DynamicMaterialInstance);
	}
}

// Called every frame
void ACubeDMIMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeDMIMod::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGAM415_CarlsonCharacter* overlappingCharacter = Cast<AGAM415_CarlsonCharacter>(OtherActor);

	if (overlappingCharacter)
	{
		// Generate colors and frame
		float randomRed = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
		float randomGreen = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
		float randomBlue = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);

		FVector4 randomColor = FVector4(randomRed, randomGreen, randomBlue, 1.0f);
		if (DynamicMaterialInstance)
		{
			// Set parameters
			DynamicMaterialInstance->SetVectorParameterValue("Color", randomColor);
			DynamicMaterialInstance->SetScalarParameterValue("Darkness", randomRed);
			DynamicMaterialInstance->SetScalarParameterValue("Opacity", randomGreen);
		}
	}
}

