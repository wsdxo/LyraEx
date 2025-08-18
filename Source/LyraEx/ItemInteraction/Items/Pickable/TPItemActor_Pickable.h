#pragma once
#include "LyraEx/ItemInteraction/Items/Core/TPItemActorBase.h"
#include "LyraEx/ItemInteraction/ItemInteractionType.h"
#include "TPItemActor_Pickable.generated.h"

UCLASS(Abstract)
class ATPItemActor_Pickable:public ATPItemActorBase
{
	GENERATED_BODY()

public:
	ATPItemActor_Pickable();
};

