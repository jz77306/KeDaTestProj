// Fill out your copyright notice in the Description page of Project Settings.


#include "core/KeDaSoundPlugin.h"
#include "WebSocketsModule.h"
#include "core/AudioRecordSubSys.h"
#include "Misc/Base64.h"
#include "Modules/ModuleManager.h"
#include "Serialization/JsonSerializer.h"

static TSharedPtr<IWebSocket> Socket = {};
static bool bSendingData = false;
static UAudioRecordSubSys* AudioRecordSubSys = nullptr;

bool UKeDaSoundPlugin::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UKeDaSoundPlugin::Initialize(FSubsystemCollectionBase& Collection) 
{
	Super::Initialize(Collection);
}

void UKeDaSoundPlugin::Deinitialize()
{
	Super::Deinitialize();
}

void UKeDaSoundPlugin::Tick(float DeltaTime)
{
	
}

bool UKeDaSoundPlugin::IsTickable() const
{
	//return FTickableGameObject::IsTickable();
	return !IsTemplate();
}

TStatId UKeDaSoundPlugin::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UXGLoginSystem, STATGROUP_Tickables);
}

void UKeDaSoundPlugin::CreateSocket()
{
	FModuleManager::Get().LoadModuleChecked("WebSockets");

	FString BaseString = TEXT("");
	FString TS;
	TS = FString::FromInt(FDateTime::Now().UtcNow().ToUnixTimestamp());
	BaseString = Appid + TS;

	FString MD5BaseString = FMD5::HashAnsiString(*BaseString);

	FSHAHash Hash;
	FSHA1 SHA1;
	SHA1.HMACBuffer(TCHAR_TO_ANSI(*APIKey), APIKey.Len(), TCHAR_TO_ANSI(*MD5BaseString), MD5BaseString.Len(), Hash.Hash);

	FString WrongSigna = Hash.ToString();
	FString Signa = FBase64::Encode(Hash.Hash, 20);
	Signa.ReplaceInline(TEXT("="), TEXT("%3D"));
	
	ServerProtocal = TEXT("ws");
	ServerURL = TEXT("ws://rtasr.xfyun.cn/v1/ws?");
	ServerURL += TEXT("appid=");
	ServerURL += Appid;
	
	ServerURL += TEXT("&");
	ServerURL += TEXT("ts=");
	ServerURL += TS;

	ServerURL += TEXT("&");
	
	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocal);

	Socket->OnConnected().AddUObject(this, &UKeDaSoundPlugin::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UKeDaSoundPlugin::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UKeDaSoundPlugin::OnClosed);
	//Socket->OnMessage().AddUObject(this, &UKeDaSoundPlugin::OnMessage);
	//Socket->OnMessageSent().AddUObject(this, &UKeDaSoundPlugin::OnMessageSent);

	Socket->Connect();

	bSendingData = true;
}

void UKeDaSoundPlugin::CloseSocket()
{
	bSendingData = false;
	if(Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}

}

void UKeDaSoundPlugin::SendVoiceData(const float* InAudio, int32 NumSamples)
{
	if(Socket.IsValid() && Socket->IsConnected() && bSendingData)
	{
		TArray<int16> ToChangeAuidoData;
		TArray<uint8> BinaryDataToSend;
		int32 i = 0;
		for (; i < 1024;)
		{	//-1.0~1.0
			ToChangeAuidoData.Add((int16)FMath::Clamp<int32>(FMath::FloorToInt(32767.0f * InAudio[i]), -32768, 32767));

			uint8 Bytes[2];

			Bytes[0] = (uint8)(ToChangeAuidoData[i] & 0xFF);

			Bytes[1] = (uint8)((ToChangeAuidoData[i] >> 8) & 0xFF);

			BinaryDataToSend.Add(Bytes[0]);

			BinaryDataToSend.Add(Bytes[1]);

			i++;
		}

		Socket->Send(BinaryDataToSend.GetData(), BinaryDataToSend.Num(), true);
	}
}

void UKeDaSoundPlugin::StopSendData()
{
	bSendingData = false;
}

void UKeDaSoundPlugin::OnConnected()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
}

void UKeDaSoundPlugin::OnConnectionError(const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("%s ERROR:%s"), *FString(__FUNCTION__), *Error);
}

void UKeDaSoundPlugin::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogTemp, Warning, TEXT("%s StatusCode:%d Reason:%s bWasClean:%d"), *FString(__FUNCTION__), StatusCode, *Reason, bWasClean);
	bSendingData = false;
}

void UKeDaSoundPlugin::OnMessage(const FString Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Message:%s"), *FString(__FUNCTION__), *Message);
	if (!Message.IsEmpty())
	{
		TSharedPtr<FJsonObject> ResultObj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);
		FJsonSerializer::Deserialize(Reader, ResultObj);

		FString MessageAction;
		
		if (ResultObj->TryGetStringField("action", MessageAction))
		{
			
			if (MessageAction.Equals(TEXT("started")))
			{
				bSendingData = true;
				//bSpeechToText = true;

				AudioRecordSubSys->StartCapturingAudio();
				// VoiceRunnable = MakeShareable(  new FConsumeSoundRunnable(TEXT("VoiceRunnable")));
				// FRunnableThread* RunnableThread1 = FRunnableThread::Create(VoiceRunnable.Get(), *VoiceRunnable->MyThreadName);
			}


			if (MessageAction.Equals(TEXT("error")))
			{
				bSendingData = false;
				//bSpeechToText = false;

				// XGBeginSpeachToTextDelegate.ExecuteIfBound(false);
				// XGBeginSpeachToTextDelegate.Clear();
				// XGSpeachToTextDelegate.Clear();
				
			}
			
			if (MessageAction.Equals(TEXT("result")))
			{
				FString OutText;
				if (ResultObj->TryGetStringField("data", OutText))
				{
					if (!OutText.IsEmpty())
					{
						TSharedPtr<FJsonObject> DataObj;
						TSharedRef<TJsonReader<>> Reader2 = TJsonReaderFactory<>::Create(OutText);
						FJsonSerializer::Deserialize(Reader2, DataObj);

						FString dst;
						FString src;
						FString end;

						bool bDest = DataObj->TryGetStringField(TEXT("dst"), dst);
						bool bSrc = DataObj->TryGetStringField(TEXT("src"), src);
						bool bEnd = DataObj->TryGetStringField(TEXT("ed"), end);
						int32 EndNum = FCString::Atoi(*end);
						bEnd = EndNum > 0 ? true : false;

						if (bDest && bSrc&& bEnd)
						{
							UE_LOG(LogTemp, Warning, TEXT("dst :%s Message [src]:%s"), *dst, *src);
							//XGSpeachToTextDelegate.ExecuteIfBound(src,dst);
						}
					}
				}
			}
		}
	}
}

void UKeDaSoundPlugin::OnMessageSent(const FString MessageString)
{
	UE_LOG(LogTemp, Warning, TEXT("%s MessageString:%s"), *FString(__FUNCTION__), *MessageString);
}

