// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinProcTerrain.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
APerlinProcTerrain::APerlinProcTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	ProcMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APerlinProcTerrain::BeginPlay()
{
	Super::BeginPlay();

	CreateVertices();
	CreateTriangles();
	
	// Create the mesh section with the generated vertices and triangles
	ProcMesh->CreateMeshSection(SectionID, Vertices, Triangles, Normals, UVs, UpVertexColors, TArray<FProcMeshTangent>(), true);
	ProcMesh->SetMaterial(0, TerrainMaterial);
	
}

// Called every frame
void APerlinProcTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function to alter the mesh at a given impact point (from projectile hit)
void APerlinProcTerrain::AlterMesh(FVector impactpoint)
{
	for (int i = 0; i < Vertices.Num(); i++)
	{
		FVector tempVector = impactpoint - this->GetActorLocation();

		if (FVector(Vertices[i] - tempVector).Size() < radius)
		{
			Vertices[i] -= Depth;
			ProcMesh->UpdateMeshSection(SectionID, Vertices, Normals, UVs, UpVertexColors, TArray<FProcMeshTangent>());
		}
	}
}

// Function to create vertices using Perlin noise for height variation
void APerlinProcTerrain::CreateVertices()
{
	for (int x = 0; x <= XSize; x++)
	{
		for (int y = 0; y <= YSize; y++)
		{
			float Z = FMath::PerlinNoise2D(FVector2D(x * NoiseScale + 0.1, y * NoiseScale + 0.1)) * ZMultiplier;
			GEngine->AddOnScreenDebugMessage(-1, 999.0f, FColor::Yellow, FString::Printf(TEXT("Z: %f"), Z));
			Vertices.Add(FVector(x * Scale, y * Scale, Z));
			UVs.Add(FVector2D(x * UVScale, y * UVScale));
		}
	}
}

// Function to create triangles for the procedural mesh
void APerlinProcTerrain::CreateTriangles()
{
	int Vertex = 0;

	for (int x = 0; x < XSize; x++)
	{
		for (int y = 0; y < YSize; y++)
		{
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);
			Vertex++;
		}
		Vertex++;
	}
}

