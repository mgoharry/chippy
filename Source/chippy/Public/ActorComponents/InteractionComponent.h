// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Product.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/Interactable.h"
#include "InteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHIPPY_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractionComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    /*** Interact logic ***/
    
    UFUNCTION()
    void PerformLineTrace();

    UFUNCTION()
    void Interact();

    UFUNCTION(Server, unreliable)
    void SER_Interact(AActor* inInteractableActor);

    /*** Carry logic ***/
    void CarryProduct(AProduct* Product);

    UPROPERTY(Replicated)
    bool isCarryingProduct = false;

    UPROPERTY(Replicated)
    AProduct* CarriedProduct;
    
    void DropProduct();
    UFUNCTION(Server, unreliable)
    void SER_DropProduct();
    
protected:
    UPROPERTY()
    AActor* OverlappingActor;

    UPROPERTY()
    TScriptInterface<IInteractable> InteractableInterface;

    UPROPERTY()
    AchippyCharacter* OwnerCharacter;

    UPROPERTY()
    UCameraComponent* CameraComponent;

    UPROPERTY(EditAnywhere, Category = "Interaction")
    float TraceDistance = 1500.0f;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);
};