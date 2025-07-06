// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MachineButton.h"
#include "Product.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Machine.generated.h"

UENUM(BlueprintType)
enum class EMachineState : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_Occupied UMETA(DisplayName = "Occupied")
};

UCLASS()
class CHIPPY_API AMachine : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMachine();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SpawnButtonsOnMachine();

	UFUNCTION()
	virtual void ModifyProduct(FProductInfo ProductToCreate);

	UFUNCTION()
	virtual void ClearProduct();

	UFUNCTION()
	void SetMachineState(EMachineState inState);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UArrowComponent* CreatedProductSpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UArrowComponent* ButtonsSpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Machine")
	TSubclassOf<AMachineButton> MachineButtonClass;

	UPROPERTY(Replicated)
	AProduct* CurrentProductRef;

	UPROPERTY(EditDefaultsOnly, Category = "Machine")
	float ButtonsPadding = 100;

	UPROPERTY(ReplicatedUsing = OnRep_MachineState)
	EMachineState MachineCurrentState;

	UFUNCTION()
	void OnRep_MachineState();

	UFUNCTION()
	EMachineState GetMachineState() const { return MachineCurrentState; }

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex);
};
