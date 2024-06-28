#include "CEnenyNameWidget.h"
#include "Components/TextBlock.h"

void UCEnenyNameWidget::SetName(FString InControllerName, FString InPawnName)
{
	ControllerName->SetText(FText::FromString(InControllerName));
	PawnName->SetText(FText::FromString(InPawnName));
}