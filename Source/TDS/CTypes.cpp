

#include "CTypes.h"
#include "TDS.h"
#include "TDS_IGameActor.h"
#include "WeaponActor.h"

void UCTypes::AddEffectBySurfaceType(AActor* TakeEffectActor, TSubclassOf<UTDS_StateEffect> AddEffectClass, EPhysicalSurface SurfaceType)
{
	if (SurfaceType != EPhysicalSurface::SurfaceType_Default && TakeEffectActor && AddEffectClass)
	{
		UTDS_StateEffect* myEffect = Cast<UTDS_StateEffect>(AddEffectClass->GetDefaultObject());
		if (myEffect)
		{
			bool bIsHavePossibleSurface = false;
			int8 i = 0;
			while (i < myEffect->PossibleInteractSurface.Num() && !bIsHavePossibleSurface)
			{
				if (myEffect->PossibleInteractSurface[i] == SurfaceType)
				{
					bIsHavePossibleSurface = true;
					bool bIsCanAddEffect = false;
					if (!myEffect->bIsStakable)
					{
						int8 j = 0;
						TArray<UTDS_StateEffect*> CurrentEffects;
						ITDS_IGameActor* myInterface = Cast<ITDS_IGameActor>(TakeEffectActor);
						if (myInterface)
						{
							CurrentEffects = myInterface->GetAllCurrentEffects();
						}

						if (CurrentEffects.Num() > 0)
						{
							while (j < CurrentEffects.Num() && !bIsCanAddEffect)
							{
								if (CurrentEffects[j]->GetClass() != AddEffectClass)
								{
									bIsCanAddEffect = true;
								}
								j++;
							}
						}
						else
						{
							bIsCanAddEffect = true;
						}

					}
					else
					{
						bIsCanAddEffect = true;
					}

					if (bIsCanAddEffect)
					{

						UTDS_StateEffect* NewEffect = NewObject<UTDS_StateEffect>(TakeEffectActor, AddEffectClass);
						if (NewEffect)
						{
							NewEffect->InitObject(TakeEffectActor);
						}
					}

				}
				i++;
			}
		}

	}
}