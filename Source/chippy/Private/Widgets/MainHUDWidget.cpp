// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainHUDWidget.h"

#include <string>


void UMainHUDWidget::AddOrderCard(int OrderID, FProductInfo OrderInfo)
{
	if (UOrderCardWidget* OrderCardWidget = CreateWidget<UOrderCardWidget>(this, OrderCardWidgetClass))
	{
		if (!OrderCardWidget) return;
		OrderCardWidget->ProductInfo->SetText(FText::FromString(OrderInfo.Color.Name + " " + OrderInfo.Name));
		OrderCardWidget->ProductImage->SetBrushFromTexture(OrderInfo.Texture.LoadSynchronous());
		OrderCardWidget->ProductImage->SetBrushTintColor(OrderInfo.Color.RGBA);
		ActiveOrdersWidgets.Add(OrderID, OrderCardWidget);
		CardsVerticalBox->AddChild(OrderCardWidget);
	}
}

void UMainHUDWidget::UpdateAndRemoveOrder(int OrderID, float inOrderPay, float inTotalBudget)
{
	ActiveOrdersWidgets.FindRef(OrderID)->RemoveFromParent();
	ActiveOrdersWidgets.Remove(OrderID);
	OrderPayAnimation(inOrderPay);
	BudgetEditText->SetText(FText::FromString(FString::SanitizeFloat(inOrderPay)));
	CurrentBudgetText->SetText(FText::FromString(FString::SanitizeFloat(inTotalBudget)));
}

void UMainHUDWidget::OrderPayAnimation_Implementation(float inOrderPay)
{
	//blueprint native event
}
