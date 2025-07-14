// MainHUDWidget.cpp
// Manages the main HUD interface for displaying and updating order cards, including budget information
// and order payment animations

#include "Widgets/MainHUDWidget.h"


/**
 * Creates and adds a new order card widget to the HUD
 * @param OrderID - Unique ID for the order
 * @param OrderInfo - Product information to display on the card
 */
void UMainHUDWidget::AddOrderCard(int OrderID, FProductInfo OrderInfo)
{
	if (!CardsVerticalBox) return;

	//Create the order card widget
	UOrderCardWidget* OrderCardWidget = CreateWidget<UOrderCardWidget>(this, OrderCardWidgetClass);
	if (!OrderCardWidget) return;

	//Assign data to order card widget and displayng it

	OrderCardWidget->ProductInfo->SetText(FText::FromString(OrderInfo.Color.Name + " " + OrderInfo.Name));
	OrderCardWidget->ProductImage->SetBrushFromTexture(OrderInfo.Texture.LoadSynchronous());
	OrderCardWidget->ProductImage->SetBrushTintColor(OrderInfo.Color.RGBA);

	//Adding order in current active widgets array for later access
	ActiveOrdersWidgets.Add(OrderID, OrderCardWidget);

	CardsVerticalBox->AddChild(OrderCardWidget);
}

/**
 * Updates the HUD when an order is completed and removes the associated order card
 * @param OrderID - ID of the order to remove
 * @param inOrderPay - Payment amount for the completed order
 * @param inTotalBudget - Updated total budget after payment
 */
void UMainHUDWidget::UpdateAndRemoveOrder(int OrderID, float inOrderPay, float inTotalBudget)
{
	if (UOrderCardWidget* CompletedOrderWidget = ActiveOrdersWidgets.FindRef(OrderID))
	{
		//Remove completed order from array and vertical box
		CompletedOrderWidget->BP_RemoveCard();
		ActiveOrdersWidgets.Remove(OrderID);
		OrderPayAnimation(inOrderPay);

		// Payment amount for the completed order
		if (BudgetEditText)
			BudgetEditText->SetText(FText::FromString(FString::SanitizeFloat(inOrderPay)));

		//Updated total budget after payment
		if (CurrentBudgetText)
			CurrentBudgetText->SetText(FText::FromString(FString::SanitizeFloat(inTotalBudget)));
	}
}

/**
 * Blueprint native event for playing order wrong animations
 */
void UMainHUDWidget::WrongOrderAnimation_Implementation()
{
	//blueprint native event
}

/**
 * Blueprint native event for playing order payment animations
 * @param inOrderPay - payment amount to animate
 */
void UMainHUDWidget::OrderPayAnimation_Implementation(float inOrderPay)
{
	//blueprint native event
}
