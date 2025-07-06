// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/MainGameMode.h"

#include "Structs/SProductInfo.h"


AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainGameMode::GenerateOrder, 5.0f, true);
}

void AMainGameMode::GenerateOrder()
{
	TArray<FProductInfo*> Products;
	ProductsDataTable->GetAllRows("", Products);

	int RandomIndex = FMath::RandRange(0, Products.Num() - 1);
	FName Name = FName(Products[RandomIndex]->Name);
	UTexture2D* Texture = Products[RandomIndex]->Texture.LoadSynchronous();
	FColor Color = AvailableColors[FMath::RandRange(0, AvailableColors.Num() - 1)];

	for (AChippyPlayerController* PlayerController : PlayerControllers)
	{
		if (PlayerController)
		{
			PlayerController->AddOrderCard(Name, Texture, Color);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("im here")));
		}
	}
}

void AMainGameMode::CheckOrderValidity()
{
}

void AMainGameMode::UpdateBudget()
{
}
