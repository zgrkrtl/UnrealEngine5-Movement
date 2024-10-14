// Mercenary Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class MERCENARY_API AEnemyCharacter : public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
};
