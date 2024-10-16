// Mercenary Game


#include "Character/MercenaryCharacter.h"
#include "AbilitySystemComponent.h"
#include "Player/MercenaryPlayerController.h"
#include "Player/MercenaryPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/MercenaryHUD.h"
#include "AbilitySystem/MercenaryASC.h"
#include "Actor/Gun.h"

// Sets default values
AMercenaryCharacter::AMercenaryCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Speed = 300.f;       
	NormalSpeed = 300.f;
	RunSpeed = 600.f;
}

void AMercenaryCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);

}



void AMercenaryCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AMercenaryCharacter::OnRep_PlayerState()
{
	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

UAbilitySystemComponent* AMercenaryCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMercenaryCharacter::SetSpeed(float InSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InSpeed;
}

void AMercenaryCharacter::InitAbilityActorInfo()
{
	AMercenaryPlayerState* MercenaryPlayerState = GetPlayerState<AMercenaryPlayerState>();
	check(MercenaryPlayerState);
	MercenaryPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MercenaryPlayerState, this);
	AbilitySystemComponent = MercenaryPlayerState->GetAbilitySystemComponent();
	AttributeSet = MercenaryPlayerState->GetAttributeSet();

	// PLACE TO CALL INITOVERLAY
	if (AMercenaryPlayerController* MercenaryPlayerController = Cast<AMercenaryPlayerController>(GetController()))
	{
		if (AMercenaryHUD* MercenaryHUD = Cast<AMercenaryHUD>(MercenaryPlayerController->GetHUD()))
		{
			MercenaryHUD->InitOverlay(MercenaryPlayerController, MercenaryPlayerState, AbilitySystemComponent, AttributeSet);

		}
	}
}



