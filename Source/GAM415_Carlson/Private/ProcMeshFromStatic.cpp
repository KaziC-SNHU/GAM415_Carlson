// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcMeshFromStatic.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
AProcMeshFromStatic::AProcMeshFromStatic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the Procedural Mesh Component
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));

	// Set the Procedural Mesh as the Root Component
	RootComponent = ProcMesh;
	BaseMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AProcMeshFromStatic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called when actor is created (in editor or during gameplay)
void AProcMeshFromStatic::PostActorCreated()
{
	Super::PostActorCreated();
	GetMeshData();
	CreateMesh();
}

// Called after the C++ constructor and after the properties have been initialized
void AProcMeshFromStatic::PostLoad()
{
	Super::PostLoad();
	GetMeshData();
	CreateMesh();
}

// Get the mesh data from the Static Mesh Component
void AProcMeshFromStatic::GetMeshData()
{
	UStaticMesh* mesh = BaseMesh->GetStaticMesh();
	if (mesh)
	{
		UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(mesh, 0, 0, Vertices, Triangles, Normals, UVs, Tangents);
		ProcMesh->UpdateMeshSection(0, Vertices, Normals, UVs, UpVertexColors, Tangents);
		CreateMesh();
	}
}

// Create the procedural mesh section
void AProcMeshFromStatic::CreateMesh()
{
	ProcMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, UpVertexColors, Tangents, true);
}

// Called every frame
void AProcMeshFromStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

