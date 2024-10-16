// Mercenary Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MercenaryPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class MERCENARY_API AMercenaryPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMercenaryPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	bool bIsShooting = false;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> MercenaryContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> RunAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShootAction;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Crouch(const FInputActionValue& InputActionValue);
	void Run(bool bIsRunning);
	void StartRunning();
	void StopRunning();
	void Jump();
	void StopJump();
	void Shoot();
	void StopShooting();

	void CursorTrace();
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;

};
