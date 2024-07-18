#pragma once

#include "CoreMinimal.h"

#define CheckNull(P) {if(P == nullptr) return;}
#define CheckNullResult(P, result) {if(P == nullptr) return result;}

#define CheckTrue(P) {if(P == true) return;}
#define CheckTrueResult(P, result) {if(P == true) return result;}

#define CheckFalse(P) {if(P == false) return;}
#define CheckFalseResult(P, result) {if(P == false) return result;}

class THIRDPERSONCPP_API  CHelpers
{
public:
	template<typename T>
	static void GetAsset(T** OutAsset, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T>Asset(*InPath);
		ensureMsgf(Asset.Succeeded(), TEXT("Asset Not Find"));

		*OutAsset = Asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutAsset, FString InPath)
	{
		T* Object = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *InPath));
		ensureMsgf(Object, TEXT("Asset Not Found(Dynamic)"));

		*OutAsset = Object;
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> Class(*InPath);
		ensureMsgf(Class.Succeeded(), TEXT("Class Not Find"));
		*OutClass = Class.Class;
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	static void CreateSceneComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr)
	{
		*OutComponent = InActor-> CreateDefaultSubobject<T>(InName);

		if (InParent)
		{
			(*OutComponent)->SetupAttachment(InParent);
				return;
		}
		InActor->SetRootComponent(*OutComponent);
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}


};