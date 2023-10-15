// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WebSockets/Public/IWebSocket.h"
#include "KeDaSoundPlugin.generated.h"

class UAudioRecordSubSys;
/**
 * 
 */
UCLASS()
class KEDASOUND_API UKeDaSoundPlugin : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
    
public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
    
public:
	UFUNCTION(BlueprintCallable)
	void CreateSocket();
	UFUNCTION(BlueprintCallable)
    void CloseSocket();

	static void SendVoiceData(const float* InAudio, int32 NumSamples);

	UFUNCTION(BlueprintCallable)
	void StopSendData();

	//ToDo 视频还没有进行到的部分
	// void XGBeginSpeachToText(FXGBeginSpeachToTextDelegate InXGBeginSpeachToTextDelegate, FXGSpeachToTextDelegate InXGSpeachToTextDelegat);
	// void XGStopSpeachToText();
    
protected:

	FString ServerURL = TEXT("");
	FString ServerProtocal = TEXT("");

	FString Appid = TEXT("278c62b6");
	FString APIKey = TEXT("2184ab5f1435ffdc8001e50f87df80f5");

	// static TSharedPtr<IWebSocket> Socket;
	// static bool bSendingData;
	// static UAudioRecordSubSys* AudioRecordSubSys;
	
public:
    
	UFUNCTION(BlueprintCallable)
	void OnConnected();
    	
	UFUNCTION(BlueprintCallable)
	void OnConnectionError(const FString& Error);
    	
	UFUNCTION(BlueprintCallable)
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
    	
	UFUNCTION(BlueprintCallable)
	void OnMessage(const FString Message);
    	
	UFUNCTION(BlueprintCallable)
	void OnMessageSent(const FString MessageString);

	//ToDo
	// bool bSpeechToText = false;
	// FXGBeginSpeachToTextDelegate XGBeginSpeachToTextDelegate;
	// FXGSpeachToTextDelegate XGSpeachToTextDelegate;
	// TSharedPtr< FConsumeSoundRunnable> VoiceRunnable;

	public:
	static UAudioRecordSubSys* AudioRecordSubSystem;
};



