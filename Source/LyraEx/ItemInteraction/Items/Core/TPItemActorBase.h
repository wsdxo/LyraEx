#pragma once
#include "GameFramework/Actor.h"
#include "TPItemActorBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTPItemActor,Log,All);

UCLASS(Abstract)
class ATPItemActorBase : public AActor
{
	GENERATED_BODY()

public:
	ATPItemActorBase();
};
