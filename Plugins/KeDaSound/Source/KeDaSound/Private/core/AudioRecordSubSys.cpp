// Fill out your copyright notice in the Description page of Project Settings.


#include "core/AudioRecordSubSys.h"

#include "Tickable.h"
#include "core/KeDaSoundPlugin.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

float* UAudioRecordSubSys::KDAudio = nullptr;
int32 UAudioRecordSubSys::KDNumSamples = -1;
TArray<float*> UAudioRecordSubSys::InAudios = {};

float* UAudioRecordSubSys::ToSendData = nullptr;
int32 UAudioRecordSubSys::IndexSend = 0;

FCriticalSection UAudioRecordSubSys::KDAudioCriticalSection;
TArray<float> UAudioRecordSubSys::AudioData = {};

bool UAudioRecordSubSys::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UAudioRecordSubSys::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	KDAudioCapture = UAudioCaptureFunctionLibrary::CreateAudioCapture();
	KDAudioCapture->AddGeneratorDelegate(&UAudioRecordSubSys::OnAudioGenerate);
	
	if(InAudios.Num() > 0)
	{
		for(auto& SoundPtr : InAudios)
		{
			delete SoundPtr;
			SoundPtr = nullptr;
		}
		InAudios.Empty();
	}
}

void UAudioRecordSubSys::Deinitialize()
{
	Super::Deinitialize();
}

void UAudioRecordSubSys::Tick(float DeltaTime)
{
	
}

bool UAudioRecordSubSys::IsTickable() const
{
	return FTickableGameObject::IsTickable();
}

TStatId UAudioRecordSubSys::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UAudioRecordSubSys, STATGROUP_Tickables)
}

void UAudioRecordSubSys::OnAudioGenerate(const float* InAudio, int32 NumSamples)
{

	// GWorld->GetGameInstance();
	// //UGameInstance* MyGameInstance = UGameplayStatics::GetGameInstance();
	//
	// UGameInstance* MyGameInstance = GWorld->GetGameInstance();
	//
	// if (MyGameInstance)
	// {
	// 	UKeDaSoundPlugin* SocketSubsystem=	MyGameInstance->GetSubsystem<UKeDaSoundPlugin>();
	// 	SocketSubsystem->SendVoiceData(InAudio, NumSamples);
	// }
	 UE_LOG(LogTemp, Display, TEXT("OnAudioCapture %f----%d"), *InAudio, NumSamples);
	 KDAudio = const_cast<float*>(InAudio);
	 KDNumSamples = NumSamples;
	 KDAudio = new float[NumSamples];
	 for(int i = 0; i < NumSamples; i++)
	 {
	 	KDAudio[i] = InAudio[i];
	 }
	
	 FScopeLock Lock(&KDAudioCriticalSection);
	 int32 LeftIndex = 0;
	 if (IndexSend == 0)
	 {
	 	IndexSend++;
	 	for (int32 SendIndex = 0; SendIndex<341; SendIndex++)
	 	{
	 		AudioData.Add(InAudio[LeftIndex]);
	 		LeftIndex += 6;
	 	}
	
	 }
	 else if (IndexSend ==1)
	 {
	
	 	IndexSend++;
	 	for (int32 SendIndex = 341; SendIndex < 682; SendIndex++)
	 	{
	 		AudioData.Add(InAudio[LeftIndex]);
	 		LeftIndex += 6;
	 	}
	 }
	 else if (IndexSend ==2)
	 {
	
	 	IndexSend = 0;
	 	for (int32 SendIndex = 682; SendIndex < 1024; SendIndex++)
	 	{
	 		AudioData.Add(InAudio[LeftIndex]);
	 		LeftIndex += 6;
	 	}
			UKeDaSoundPlugin::SendVoiceData(ToSendData, NumSamples);
	}
}

void UAudioRecordSubSys::StartCapturingAudio()
{
	KDAudioCapture->StartCapturingAudio();
}

void UAudioRecordSubSys::StopCapturingAudio()
{
	KDAudioCapture->StopCapturingAudio();
}

bool UAudioRecordSubSys::IsCapturingAudio()
{
	return KDAudioCapture->IsCapturingAudio();
}

bool UAudioRecordSubSys::GetAudioCaptureDeviceInfo(FAudioCaptureDeviceInfo& OutInfo)
{
	bool bGet =	KDAudioCapture->GetAudioCaptureDeviceInfo(OutInfo);
	UE_LOG(LogTemp, Warning, TEXT("DeviceName:[%s]---NumInputChannels:[%d]----SampleRate:[%d]"), *(OutInfo.DeviceName.ToString()), OutInfo.NumInputChannels, OutInfo.SampleRate);
	return bGet;
}
