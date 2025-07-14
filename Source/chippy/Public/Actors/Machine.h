// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MachineButton.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameModes/MainGameMode.h"
#include "NiagaraComponent.h"
#include "Product.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "Machine.generated.h"

//possible states for the machine
UENUM(BlueprintType)
enum class EMachineState : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"), // the machine is clear and ready to process a product
	EMS_Occupied UMETA(DisplayName = "Occupied"), // the machine is occupied with a product
	EMS_Working UMETA(DisplayName = "Working"), // the machine is working on a product
	EMS_Done UMETA(DisplayName = "Done"), // the machine is done working on a product
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMeshReceived, FProductInfo);
DECLARE_MULTICAST_DELEGATE(FOnMeshRemoved);

UCLASS()
class CHIPPY_API AMachine : public AActor
{
	GENERATED_BODY()

public:
	AMachine();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	//Text to show the machine state
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UTextRenderComponent* MachineStateText;

	// Location to set the product in
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UArrowComponent* CreatedProductSpawnLocation;

	// Location for the buttons to spawn in
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UArrowComponent* ButtonsSpawnLocation;

protected:
	virtual void BeginPlay() override;

	// Spawns interactive buttons on the machine for product modification
	virtual void SpawnButtonsOnMachine();

	// Applies modifications to the current product based on provided info
	UFUNCTION()
	virtual void ModifyProduct(FProductInfo ProductToCreate);

	// start a timer to begin working on the assigned product
	UFUNCTION()
	virtual void StartWorkingOnProduct(FProductInfo ProductToCreate);

	// Removes the current product from the machine
	UFUNCTION()
	virtual void ClearProduct();

	//Reference to the product currently being processed
	UPROPERTY(Replicated)
	AProduct* CurrentProductRef;

	// Class of buttons to spawn on the machine
	UPROPERTY(EditDefaultsOnly, Category = "Machine")
	TSubclassOf<AMachineButton> MachineButtonClass;

	// Spacing between machine buttons
	UPROPERTY(EditDefaultsOnly, Category = "Machine")
	float ButtonsPadding = 100;

	// Reference to the main game mode
	UPROPERTY()
	AMainGameMode* MainGameMode;

	// Table containing available product data
	UPROPERTY()
	UDataTable* ProductsDataTable;

	// List of colors available for product modification
	TArray<FColorInfo> AvailableColors;

	//Timer handle for the machine working timer
	FTimerHandle StartProcessTimerHandle;
	// Current state of the machine
	UPROPERTY(ReplicatedUsing= "OnRep_MachineState")
	EMachineState MachineCurrentState;

	// Updates the machine's current state and triggers related events on clients sides
	UFUNCTION()
	void OnRep_MachineState();

	// Delegates for mesh-related events
	FOnMeshReceived MeshReceivedDelegate;
	FOnMeshRemoved MeshRemovedDelegate;

	// Updates the machine's current state and triggers related events
	UFUNCTION(Server, Unreliable)
	void SetMachineState(EMachineState inState);

	// Returns the current machine state
	UFUNCTION()
	EMachineState GetMachineState() const { return MachineCurrentState; }

	// Handle actor beginning overlap with machine
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// visual effects component for machine 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Effects");
	UNiagaraComponent* MachineVFXComponent;
	// set time for products to be finished in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Machine");
	float ProductProcessTime = 2.f;

	//Sound for when a product is assigned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* AssignSoundEffect;
	//Sound for when a product is finished
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* FinishedSoundEffect;
};
