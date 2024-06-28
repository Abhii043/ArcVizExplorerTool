
#include "InteriorDesignActor.h"

AInteriorDesignActor::AInteriorDesignActor()
{
	PrimaryActorTick.bCanEverTick = true;
	InteriorSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("InteriorSceneComponent"));
	RootComponent = InteriorSceneComponent;
}

void AInteriorDesignActor::SetInteriorMesh(UStaticMesh* InMesh)
{
	if (InteriorMeshComponent) {
		InteriorMeshComponent->DestroyComponent();
		InteriorMeshComponent = nullptr;
	}
	InteriorMeshComponent = NewObject<UStaticMeshComponent>(this);
	if (InteriorMeshComponent) {
		InteriorMeshComponent->SetupAttachment(InteriorSceneComponent);
		InteriorMeshComponent->RegisterComponentWithWorld(GetWorld());
		if (InMesh) {
			InteriorMesh = InMesh;
			InteriorMeshComponent->SetStaticMesh(InMesh);
		}
	}
}

void AInteriorDesignActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteriorDesignActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

