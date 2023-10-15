// Copyright Epic Games, Inc. All Rights Reserved.

#include "KeDaSoundBPLibrary.h"
#include "KeDaSound.h"
#include "Core/KeDaSoundPlugin.h"
#include "Kismet/GameplayStatics.h"

UKeDaSoundBPLibrary::UKeDaSoundBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UKeDaSoundBPLibrary::KDBeginSpeachToText(const UObject* WorldContextObject, FKDBeginSpeachToTextDelegate KDBeginSpeachToTextDelegate, FKDSpeachToTextDelegate KDSpeachToTextDelegat)
{
	if (!WorldContextObject)
	{
		return;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UKeDaSoundPlugin* SocketSystem = GameInstance->GetSubsystem<UKeDaSoundPlugin>();

	SocketSystem->KDBeginSpeachToText(KDBeginSpeachToTextDelegate, KDSpeachToTextDelegat);

}

void UKeDaSoundBPLibrary::KDStopSpeachToText(const UObject* WorldContextObject)
{

	FString ChineseTest1 = TEXT("你好");

	if (!WorldContextObject)
	{
		return;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UKeDaSoundPlugin* SocketSystem = GameInstance->GetSubsystem<UKeDaSoundPlugin>();
	SocketSystem->KDStopSpeachToText();

}

