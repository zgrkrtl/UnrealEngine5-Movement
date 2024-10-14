// Mercenary Game


#include "Player/MercenaryPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Character/MercenaryCharacter.h"

AMercenaryPlayerController::AMercenaryPlayerController()
{
}

void AMercenaryPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AMercenaryPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(MercenaryContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)	Subsystem->AddMappingContext(MercenaryContext, 0);	
}

void AMercenaryPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMercenaryPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMercenaryPlayerController::Look);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMercenaryPlayerController::Crouch);


}

void AMercenaryPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AMercenaryPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y * -1);
	}
}

void AMercenaryPlayerController::Crouch(const FInputActionValue& InputActionValue)
{
	AMercenaryCharacter* MercenaryCharacter = Cast<AMercenaryCharacter>(GetPawn());
	if (MercenaryCharacter)
	{
		// Toggle between crouch and uncrouch
		if (MercenaryCharacter->bIsCrouched)
		{
			MercenaryCharacter->UnCrouch();
			MercenaryCharacter->SetSpeed(MercenaryCharacter->Speed);
		}
		else
		{
			MercenaryCharacter->Crouch();
			MercenaryCharacter->SetSpeed(MercenaryCharacter->CrouchSpeed);
		}
	}
}



void AMercenaryPlayerController::CursorTrace()
{

	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();
	
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}	
	}
	else 
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			
		}
	}
}
