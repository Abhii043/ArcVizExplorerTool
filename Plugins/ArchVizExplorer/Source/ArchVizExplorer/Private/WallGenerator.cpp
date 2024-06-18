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
	ComponentsArray.Empty();
}


void AWallGenerator::GenerateWall(const int32& SegmentsNo)
{
	DestroyWall();

	float MeshLength = WallStaticMesh->GetBounds().GetBox().GetSize().X;

	for (int i{}; i < SegmentsNo; i++) {
		UStaticMeshComponent* WallStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		WallStaticMeshComponent->SetupAttachment(WallSceneComponent);
		WallStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		if (WallStaticMesh) {
			WallStaticMeshComponent->SetStaticMesh(WallStaticMesh);
		}
		WallStaticMeshComponent->SetRelativeLocation(FVector(i * MeshLength, WallStaticMeshComponent->GetRelativeLocation().Y, WallStaticMeshComponent->GetRelativeLocation().Z));
		ComponentsArray.Add(WallStaticMeshComponent);
	}
}
