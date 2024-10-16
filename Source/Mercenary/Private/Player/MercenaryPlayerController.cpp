// Mercenary Game


#include "Player/MercenaryPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Character/MercenaryCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Gun.h"

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
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AMercenaryPlayerController::StartRunning);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMercenaryPlayerController::StopRunning);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMercenaryPlayerController::Jump);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AMercenaryPlayerController::Shoot);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AMercenaryPlayerController::StopShooting);


}

void AMercenaryPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);



	// Check for conflicting forward and backward input
	if (FMath::Abs(InputAxisVector.Y) > 0.1f)  // Small tolerance for axis input sensitivity
	{
		if (InputAxisVector.Y > 0.1f && InputAxisVector.Y < -0.1f)  // Both forward and backward are pressed
		{
			// Option 1: Stop movement when both are pressed
			return;

		}
	}

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Only apply forward/backward movement if there's no conflicting input
		if (FMath::Abs(InputAxisVector.Y) > 0.1f)
		{
			ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		}
		// Left/right movement (no need to modify this part unless you want to apply similar logic)
		if (FMath::Abs(InputAxisVector.X) > 0.1f)
		{
			ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
		}
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
			MercenaryCharacter->SetSpeed(MercenaryCharacter->NormalSpeed);
		}
	}
}

void AMercenaryPlayerController::Run(bool bIsRunning)
{
	
	if (AMercenaryCharacter* MercenaryCharacter = Cast<AMercenaryCharacter>(GetPawn()))
	{
		if (bIsRunning)
		{
			MercenaryCharacter->SetSpeed(MercenaryCharacter->RunSpeed);
		}
		else
		{
			MercenaryCharacter->SetSpeed(MercenaryCharacter->NormalSpeed);
		}
	}
}

void AMercenaryPlayerController::StartRunning()
{
	Run(true);
}

void AMercenaryPlayerController::StopRunning()
{
	Run(false);
}

void AMercenaryPlayerController::Jump()
{
	if (AMercenaryCharacter* MercenaryCharacter = Cast<AMercenaryCharacter>(GetPawn()))
	{
		if (!MercenaryCharacter->GetMovementComponent()->IsFalling() && !MercenaryCharacter->bIsCrouched)
		{
			MercenaryCharacter->Jump();
		}
	}
}

void AMercenaryPlayerController::StopJump()
{
	if (AMercenaryCharacter* MercenaryCharacter = Cast<AMercenaryCharacter>(GetPawn()))
	{
		MercenaryCharacter->StopJumping();
	}
}

void AMercenaryPlayerController::Shoot()
{
	if (AMercenaryCharacter* MercenaryCharacter = Cast<AMercenaryCharacter>(GetPawn()))
	{
		MercenaryCharacter->Gun->PullTrigger();
		bIsShooting = true;
	}

}

void AMercenaryPlayerController::StopShooting()
{
	bIsShooting = false;

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
