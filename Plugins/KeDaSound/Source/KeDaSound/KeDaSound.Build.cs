// Some copyright should be here...

using UnrealBuildTool;

public class KeDaSound : ModuleRules
{
	public KeDaSound(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				"SignalProcessing",
				"AudioMixer",
				"Engine",
				"CoreUObject",
				"Slate",
				"SlateCore",
				
				
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "AudioCapture",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"WebSockets",
				"AudioMixer",
				"SignalProcessing",
				"AudioCapture",
				"Json",
				"DeveloperSettings",
				"JsonUtilities",
				"AudioCapture",
				"AudioCaptureEditor",
				"AudioCaptureRtAudio"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
