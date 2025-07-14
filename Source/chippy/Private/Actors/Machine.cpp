#include "Actors/Machine.h"
#include <Structs/SProductInfo.h>

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

	MachineStateText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Machine State Text"));
	MachineStateText->SetupAttachment(RootComponent);

	MachineVFXComponent = CreateDefaultSubobject<UNiagaraComponent>("Machine VFX Component");
	MachineVFXComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMachine::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		//assign begin overlap function on server
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMachine::OnOverlapBegin);
	}

	//assign game mode and retrieve available machine capabilities and info from it
	MainGameMode = GetWorld()->GetAuthGameMode<AMainGameMode>();

	if (MainGameMode)
	{
		// available products a chipping machine can make
		ProductsDataTable = MainGameMode->ProductsDataTable.LoadSynchronous();

		// available colors a coloring machine can paint
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


// Modifies the current product based on provided product information
void AMachine::ModifyProduct(FProductInfo ProductToCreate)
{
	// Play visual effects with owner character
	if (CurrentProductRef && ProductToCreate.OwnerCharacter)
		ProductToCreate.OwnerCharacter->ChippyPlayVFX(
			MachineVFXComponent);
}

// start a timer to begin working on the assigned product
void AMachine::StartWorkingOnProduct(FProductInfo ProductToCreate)
{
	// Validate that we have a product to modify
	if (!CurrentProductRef)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Product is not set!"));
		return;
	}
}

// Removes the current product from the machine
void AMachine::ClearProduct()
{
	//Invalidates timer if the machine was working on a product
	StartProcessTimerHandle.Invalidate();
	//broadcast to buttons that the product was cleared to do the necessary visual updates
	MeshRemovedDelegate.Broadcast();
	//unbind any events bound to the product
	CurrentProductRef->ProductReleasedDelegate.Unbind();
	//change the machine state to be available for other products
	SetMachineState(EMachineState::EMS_Idle);
	//clear the product reference
	CurrentProductRef = nullptr;
}

// Updates the machine's current state and triggers related events on clients sides
void AMachine::OnRep_MachineState()
{
	switch (MachineCurrentState)
	{
	case EMachineState::EMS_Idle:
		MachineStateText->SetText(FText::FromString("Idle"));
		break;
	case EMachineState::EMS_Occupied:
		MachineStateText->SetText(FText::FromString("Occupied"));
		break;
	case EMachineState::EMS_Working:
		MachineStateText->SetText(FText::FromString("Working"));
		break;
	case EMachineState::EMS_Done:
		MachineStateText->SetText(FText::FromString("Finished"));
	}
}

// Updates the machine's current state and triggers related events
void AMachine::SetMachineState_Implementation(EMachineState inState)
{
	MachineCurrentState = inState;

	switch (MachineCurrentState)
	{
	case EMachineState::EMS_Idle:
		MachineStateText->SetText(FText::FromString("Idle"));
		break;
	case EMachineState::EMS_Occupied:
		MachineStateText->SetText(FText::FromString("Occupied"));
		break;
	case EMachineState::EMS_Working:
		MachineStateText->SetText(FText::FromString("Working"));
		break;
	case EMachineState::EMS_Done:
		MachineStateText->SetText(FText::FromString("Finished"));
	}
}

// Handles begin overlap events with other actors
void AMachine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Skip if the machine is already processing a product
	if (GetMachineState() == EMachineState::EMS_Occupied) return;

	// Handle product overlap
	if (AProduct* OverlappedActor = Cast<AProduct>(OtherActor))
	{
		// Setup machine and product processing
		CurrentProductRef = OverlappedActor;
		SetMachineState(EMachineState::EMS_Occupied);
		MeshReceivedDelegate.Broadcast(CurrentProductRef->AssignedProduct);
		CurrentProductRef->ProductReleasedDelegate.BindUFunction(this, "ClearProduct");

		// Position the product
		CurrentProductRef->GetSphereComponent()->SetSimulatePhysics(false);
		CurrentProductRef->SetActorTransform(CreatedProductSpawnLocation->GetComponentTransform());
		CurrentProductRef->MC_PlaySoundEffect(AssignSoundEffect);
	}
}
