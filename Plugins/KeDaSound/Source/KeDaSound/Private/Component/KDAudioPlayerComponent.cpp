// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/KDAudioPlayerComponent.h"
#include "core/AudioRecordSubSys.h"

bool UKDAudioPlayerComponent::Init(int32& SampleRate)
{
	NumChannels = 1;

#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Initialize the DSP objects
	Osc.Init(SampleRate);
	Osc.SetFrequency(440.0f);
	Osc.Start();
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED

	return true;
}

int32 UKDAudioPlayerComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
	int i = 0;
	int j = 0;

	for(; i < 1024; )
	{
		OutAudio[i] = UAudioRecordSubSys::InAudios[SoundIndex][j];

		i++;
		j= j+2;
	}

	SoundIndex++;
	return 1024;
}

void UKDAudioPlayerComponent::SetFrequency(const float InFrequencyHz)
{
#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
	{
		Osc.SetFrequency(InFrequencyHz);
		Osc.Update();
	});
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
}