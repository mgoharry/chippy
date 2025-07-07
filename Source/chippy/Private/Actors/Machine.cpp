// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Machine.h"
#include <Structs/SProductInfo.h>

#include "Actors/MachineButton.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMachine::AMachine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	CreatedProductSpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("Created Product Spawn Location"));
	CreatedProductSpawnLocation->SetupAttachment(RootComponent);

	ButtonsSpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("Buttons Spawn Location"));
	ButtonsSpawnLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMachine::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMachine::OnOverlapBegin);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMachine::OnOverlapEnd);
	}

	MainGameMode = GetWorld()->GetAuthGameMode<AMainGameMode>();

	if (MainGameMode)
	{
		ProductsDataTable = MainGameMode->ProductsDataTable;
		AvailableColors = MainGameMode->AvailableColors;
	}
}

void AMachine::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMachine, MachineCurrentState);
	DOREPLIFETIME(AMachine, CurrentProductRef);
}

void AMachine::SpawnButtonsOnMachine()
{
}


void AMachine::ModifyProduct(FProductInfo ProductToCreate)
{
	// This function is called when a machine button is pressed to create a product
	if (!CurrentProductRef)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Product is not set!"));
		return;
	}
}

void AMachine::ClearProduct()
{
	MeshRemovedDelegate.Broadcast();
	CurrentProductRef->ProductReleasedDelegate.Unbind();
	SetMachineState(EMachineState::EMS_Idle);
	CurrentProductRef = nullptr;
}


void AMachine::SetMachineState(EMachineState inState = EMachineState::EMS_Idle)
{
	MachineCurrentState = inState;
}

void AMachine::OnRep_MachineState()
{
	// Handle state changes on client
	if (MachineCurrentState == EMachineState::EMS_Occupied)
	{
		// Machine is now occupied, update visuals or trigger effects
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Machine is now occupied"));
	}
	else if (MachineCurrentState == EMachineState::EMS_Idle)
	{
		// Machine is now idle, update visuals or trigger effects
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Machine is now idle"));
	}
}

void AMachine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetMachineState() == EMachineState::EMS_Occupied) return;

	if (AProduct* OverlappedActor = Cast<AProduct>(OtherActor))
	{
		CurrentProductRef = OverlappedActor;
		SetMachineState(EMachineState::EMS_Occupied);
		MeshReceivedDelegate.Broadcast(CurrentProductRef->AssignedProduct);
		CurrentProductRef->ProductReleasedDelegate.BindUFunction(this, "ClearProduct");
		CurrentProductRef->GetSphereComponent()->SetSimulatePhysics(false);
		CurrentProductRef->SetActorTransform(CreatedProductSpawnLocation->GetComponentTransform());
	}
}

void AMachine::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex)
{
}
