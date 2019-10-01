// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereDeleterComp = CreateDefaultSubobject<USphereComponent>("SphereDeleterComp");
	SphereDeleterComp->SetSphereRadius(100);
	SphereDeleterComp->SetupAttachment(MeshComp);

	SphereDeleterComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapDeleterSphere);

	SphereActiveComp = CreateDefaultSubobject<USphereComponent>("SphereActiveComp");
	SphereActiveComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereActiveComp->SetSphereRadius(3000);
	SphereActiveComp->SetupAttachment(MeshComp);
	
	
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlackHole::OverlapDeleterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Find all overlapping components
	TArray<UPrimitiveComponent*> OverlappingComps;
	SphereActiveComp->GetOverlappingComponents(OverlappingComps);

	for (int32 i=0; i<OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComps[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			// The component we are looking for! It needs to be simulated in order to apply forces
			const float SphereRadius = SphereActiveComp->GetScaledSphereRadius();
			const float ForceStrength = -2000; //Negative pull towards origin

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}

}
