// Mercenary Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MercenaryUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MERCENARY_API UMercenaryUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
