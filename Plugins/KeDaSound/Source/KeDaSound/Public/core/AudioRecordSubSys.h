// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioCapture.h"
#include "Tickable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioRecordSubSys.generated.h"

/**
 * 
 */
UCLASS()
class KEDASOUND_API UAudioRecordSubSys : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	TStatId GetStatId() const override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;

	static void OnAudioGenerate(const float* InAudio, int32 NumSamples);

	UFUNCTION(BlueprintCallable, Category="AudioCapture")
	void StartCapturingAudio();

	UFUNCTION(BlueprintCallable, Category="AudioCapture")
	void StopCapturingAudio();

	UFUNCTION(BlueprintCallable, Category="AudioCapture")
	bool IsCapturingAudio();

	//ToDo
	UFUNCTION(BlueprintCallable, Category = "AudioCapture")
	bool GetAudioCaptureDeviceInfo(FAudioCaptureDeviceInfo& OutInfo);

	static float* KDAudio;
	static int32 KDNumSamples;
	static TArray<float*> InAudios;
	
	static	FCriticalSection KDAudioCriticalSection;
	static	TArray<float> AudioData;

protected:
	UPROPERTY(BlueprintReadWrite)
	UAudioCapture* KDAudioCapture;

	static float* ToSendData;
	static int32 IndexSend;
};
