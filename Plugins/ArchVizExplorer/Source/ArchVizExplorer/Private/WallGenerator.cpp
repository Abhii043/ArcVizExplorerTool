#include "WallGenerator.h"

AWallGenerator::AWallGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	WallSceneComponent = CreateDefaultSubobject<USceneComponent>("WallSceneComponent");
	RootComponent = WallSceneComponent;
}

void AWallGenerator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWallGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallGenerator::DestroyWall()
{
	for (UStaticMeshComponent* Component : ComponentsArray) {
		if (Component) {
			Component->DestroyComponent();
			Component = nullptr;
		}
	}
}


void AWallGenerator::GenerateWall()
{
	DestroyWall();

	auto WallStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
	WallStaticMeshComponent->SetupAttachment(WallSceneComponent);
	WallStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
	if(WallStaticMesh){WallStaticMeshComponent->SetStaticMesh(WallStaticMesh);}

	ComponentsArray.Add(WallStaticMeshComponent);
}
