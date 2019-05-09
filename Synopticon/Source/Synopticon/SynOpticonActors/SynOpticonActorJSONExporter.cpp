// Fill out your copyright notice in the Description page of Project Settings.

#include "SynOpticonActorJSONExporter.h"
//#include "Synopticon.h"


USynOpticonActorJSONExporter::USynOpticonActorJSONExporter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

TSharedPtr<FJsonObject> USynOpticonActorJSONExporter::SaveSynOpticonActorData(FSynOpticonActorStruct SynOpticonActorStruct)
{
	TSharedPtr<FJsonObject> SynOpticonActorJson = MakeShareable(new FJsonObject);

	SynOpticonActorJson->SetStringField("ActorName", SynOpticonActorStruct.ActorName);
	SynOpticonActorJson->SetStringField("NatNetName", SynOpticonActorStruct.NatNetName);
	SynOpticonActorJson->SetObjectField("EyeTrackerStruct", SaveEyeTrackerStructData(SynOpticonActorStruct.EyeTrackerStruct));
	SynOpticonActorJson->SetObjectField("RemoteEyeTrackerStruct", SaveRemoteEyeTrackerStructData(SynOpticonActorStruct.RemoteEyeTrackerStruct));
	
	TArray<TSharedPtr<FJsonValue>> Shimmers;
	for (FShimmerComponentStruct S : SynOpticonActorStruct.ShimmerComponents)
	{
		Shimmers.Add(MakeShareable(new FJsonValueObject(SaveShimmerComponentStructData(S))));
	}
	SynOpticonActorJson->SetArrayField("ShimmerComponents", Shimmers);

	TArray<TSharedPtr<FJsonValue>> Hands;
	for (FHandStruct H : SynOpticonActorStruct.HandComponents)
	{
		Hands.Add(MakeShareable(new FJsonValueObject(SaveHandStructData(H))));
	}
	SynOpticonActorJson->SetArrayField("HandComponents", Hands);

	SynOpticonActorJson->SetObjectField("VisualComponentData", SaveVisionStructData(SynOpticonActorStruct.VisualComponentData));
	SynOpticonActorJson->SetObjectField("GazeLogComponentData", SaveLoggingStructData(SynOpticonActorStruct.GazeLogComponentData));
	SynOpticonActorJson->SetObjectField("LeftHandLogComponentData", SaveLoggingStructData(SynOpticonActorStruct.LeftHandLogComponentData));
	SynOpticonActorJson->SetObjectField("RightHandLogComponentData", SaveLoggingStructData(SynOpticonActorStruct.RightHandLogComponentData));

	TArray<TSharedPtr<FJsonValue>> GazeData;
	for (FFixationGroupStruct G : SynOpticonActorStruct.GazeData)
	{
		GazeData.Add(MakeShareable(new FJsonValueObject(SaveFixationGroupData(G))));
	}
	SynOpticonActorJson->SetArrayField("GazeData", GazeData);
	
	SynOpticonActorJson->SetBoolField("HasMyo", SynOpticonActorStruct.HasMyo);
	SynOpticonActorJson->SetStringField("MarkersConfiguration", SaveMarkersConfigurationAsString(SynOpticonActorStruct.MarkersConfiguration));

	return SynOpticonActorJson;
}

TSharedPtr<FJsonObject> USynOpticonActorJSONExporter::SaveEyeTrackerStructData(FEyeTrackerStruct EyeTrackerStruct)
{
	TSharedPtr<FJsonObject> EyeTrackerJson = MakeShareable(new FJsonObject);
	EyeTrackerJson->SetStringField("GlassesID", EyeTrackerStruct.GlassesID);
	return EyeTrackerJson;
}

TSharedPtr<FJsonObject> USynOpticonActorJSONExporter::SaveRemoteEyeTrackerStructData(FRemoteEyeTrackerStruct RemoteEyeTrackerStruct)
{
	TSharedPtr<FJsonObject> RemoteEyeTrackerJson = MakeShareable(new FJsonObject);

	RemoteEyeTrackerJson->SetStringField("RemoteTrackerID", RemoteEyeTrackerStruct.RemoteTrackerID);
	RemoteEyeTrackerJson->SetObjectField("PointToLookAt", UJSONExporter::SaveFVectorAsJSON(RemoteEyeTrackerStruct.PointToLookAt));
	RemoteEyeTrackerJson->SetObjectField("RemoteTrackerPositionRight", UJSONExporter::SaveFVectorAsJSON(RemoteEyeTrackerStruct.RemoteTrackerPositionRight));
	RemoteEyeTrackerJson->SetObjectField("RemoteTrackerPositionLeft", UJSONExporter::SaveFVectorAsJSON(RemoteEyeTrackerStruct.RemoteTrackerPositionLeft));
	RemoteEyeTrackerJson->SetStringField("AttachedScreenName", RemoteEyeTrackerStruct.AttachedScreenName);

	return RemoteEyeTrackerJson;
}

TSharedPtr<FJsonObject> USynOpticonActorJSONExporter::SaveShimmerComponentStructData(FShimmerComponentStruct ShimmerComponentStruct)
{
	TSharedPtr<FJsonObject> ShimmerComponentJson = MakeShareable(new FJsonObject);
	
	ShimmerComponentJson->SetStringField("DeviceName", ShimmerComponentStruct.DeviceName);
	ShimmerComponentJson->SetBoolField("HasHeartRate", ShimmerComponentStruct.HasHeartRate);
	ShimmerComponentJson->SetBoolField("HasGalvanicSkinResponse", ShimmerComponentStruct.HasGalvanicSkinResponse);
	ShimmerComponentJson->SetNumberField("ShimmerIndex", ShimmerComponentStruct.ShimmerIndex);

	return ShimmerComponentJson;
}

TSharedPtr<FJsonObject> USynOpticonActorJSONExporter::SaveHandStructData(FHandStruct HandStruct)
{
	TSharedPtr<FJsonObject> HandJson = MakeShareable(new FJsonObject);

	HandJson->SetStringField("NatNetName", HandStruct.NatNetName);
	HandJson->SetStringField("MarkersConfiguration", SaveMarkersConfigurationAsString(HandStruct.MarkersConfiguration));
	HandJson->SetBoolField("LeftHand", HandStruct.LeftHand);

	return HandJson;
}

TSharedPtr<FJsonObject> USynOpticonActorJSONExporter::SaveVisionStructData(FOpenFaceStruct VisionStruct)
{
	TSharedPtr<FJsonObject> VisionJson = MakeShareable(new FJsonObject);

	VisionJson->SetStringField("OpenFaceID", VisionStruct.OpenFaceID); 
	VisionJson->SetObjectField("CameraTrackerPositionRight", UJSONExporter::SaveFVectorAsJSON(VisionStruct.CameraTrackerPositionRight));
	VisionJson->SetObjectField("CameraTrackerPositionLeft", UJSONExporter::SaveFVectorAsJSON(VisionStruct.CameraTrackerPositionLeft));
	VisionJson->SetStringField("AttachedScreenName", VisionStruct.AttachedScreenName);
	VisionJson->SetBoolField("IsTrackingHead", VisionStruct.IsTrackingHead);
	VisionJson->SetBoolField("IsTrackingEyes", VisionStruct.IsTrackingEyes);

	return VisionJson;
}

TSharedPtr<FJsonObject> USynOpticonActorJSONExporter::SaveLoggingStructData(FSynOpticonActorLoggingStruct SynOpticonActorLoggingStruct)
{
	TSharedPtr<FJsonObject> LoggingJson = MakeShareable(new FJsonObject);

	LoggingJson->SetNumberField("SynOpticonActorID", SynOpticonActorLoggingStruct.SynOpticonActorID);
	LoggingJson->SetNumberField("TotalTime", SynOpticonActorLoggingStruct.TotalTime);

	TArray<TSharedPtr<FJsonValue>> MapContent;
	for (TPair<int32, FObjectLoggingInfoStruct> Pair : SynOpticonActorLoggingStruct.Objects)
	{
		TSharedPtr<FJsonObject> PairObject = MakeShareable(new FJsonObject);
		PairObject->SetNumberField("Key", Pair.Key);
		PairObject->SetObjectField("Value", SaveObjectLoggingInfoData(Pair.Value));
		MapContent.Add(MakeShareable(new FJsonValueObject(PairObject)));
	}

	LoggingJson->SetArrayField("Objects", MapContent);

	TArray<TSharedPtr<FJsonValue>> GazePaths;
	for (FGazePathLoggingInfoStruct P : SynOpticonActorLoggingStruct.GazePaths)
	{
		GazePaths.Add(MakeShareable(new FJsonValueObject(SavePathLoggingInfoData(P))));
	}
	LoggingJson->SetArrayField("GazePaths", GazePaths);

	return LoggingJson;
}

TSharedPtr<FJsonObject> USynOpticonActorJSONExporter::SaveFixationGroupData(FFixationGroupStruct FixationGroupStruct)
{
	TSharedPtr<FJsonObject> FixationGroupJson = MakeShareable(new FJsonObject);

	FixationGroupJson->SetNumberField("GazeActorId", FixationGroupStruct.GazeActorId);
	FixationGroupJson->SetStringField("SessionId", FixationGroupStruct.SessionId);
	FixationGroupJson->SetBoolField("bPopup", FixationGroupStruct.bPopup);
	FixationGroupJson->SetObjectField("Location", UJSONExporter::SaveFVectorAsJSON(FixationGroupStruct.Location));
	FixationGroupJson->SetNumberField("Duration", FixationGroupStruct.Duration);
	FixationGroupJson->SetNumberField("Timestamp", FixationGroupStruct.Timestamp.ToUnixTimestamp());
	FixationGroupJson->SetObjectField("Normal", UJSONExporter::SaveFVectorAsJSON(FixationGroupStruct.Normal));
	FixationGroupJson->SetObjectField("HitUV", UJSONExporter::SaveFVector2DAsJSON(FixationGroupStruct.HitUV));
	
	return FixationGroupJson;
}

TSharedPtr<FJsonObject> USynOpticonActorJSONExporter::SavePathLoggingInfoData(FGazePathLoggingInfoStruct GazePathLoggingInfoStruct)
{
	TSharedPtr<FJsonObject> GazePathLoggingInfoJson = MakeShareable(new FJsonObject);

	GazePathLoggingInfoJson->SetNumberField("GazeActorID", GazePathLoggingInfoStruct.GazeActorID);
	GazePathLoggingInfoJson->SetNumberField("Timestamp", GazePathLoggingInfoStruct.TimeStamp.ToUnixTimestamp());
	GazePathLoggingInfoJson->SetNumberField("Duration", GazePathLoggingInfoStruct.Duration);

	return GazePathLoggingInfoJson;
}

TSharedPtr<FJsonObject> USynOpticonActorJSONExporter::SaveObjectLoggingInfoData(FObjectLoggingInfoStruct ObjectLoggingInfoStruct)
{
	TSharedPtr<FJsonObject> ObjectLoggingInfoJson = MakeShareable(new FJsonObject);

	ObjectLoggingInfoJson->SetNumberField("GazeActorID", ObjectLoggingInfoStruct.GazeActorID);
	ObjectLoggingInfoJson->SetNumberField("Time", ObjectLoggingInfoStruct.Time);
	ObjectLoggingInfoJson->SetNumberField("Count", ObjectLoggingInfoStruct.Count);
	ObjectLoggingInfoJson->SetNumberField("Percentage", ObjectLoggingInfoStruct.Percentage);

	return ObjectLoggingInfoJson;
}

FString USynOpticonActorJSONExporter::SaveMarkersConfigurationAsString(MarkersConfigurationEnum MarkersConfiguration)
{
	switch (MarkersConfiguration)
	{
	case MarkersConfigurationEnum::VE_Glasses_Right: return "Glasses Right";
	case MarkersConfigurationEnum::VE_Glasses_Left: return "Glasses Left";
	case MarkersConfigurationEnum::VE_Position: return "Position";
	case MarkersConfigurationEnum::VE_Laser: return	"Laser Tool";
	case MarkersConfigurationEnum::VE_ManualTool: return "Manual Tool";
	case MarkersConfigurationEnum::VE_CalibrationTool: return "Calibration Tool";
	case MarkersConfigurationEnum::VE_PositionCap: return "Cap";
	default: return "Position Only";
	}
}

FSynOpticonActorStruct USynOpticonActorJSONExporter::GetSynOpticonActorData(TSharedPtr<FJsonObject> SynOpticonActorStructObj)
{
	FSynOpticonActorStruct SynOpticonActorStruct;
	SynOpticonActorStruct.ActorName = SynOpticonActorStructObj->GetStringField("ActorName");
	SynOpticonActorStruct.NatNetName = SynOpticonActorStructObj->GetStringField("NatNetName");
	SynOpticonActorStruct.EyeTrackerStruct = GetEyeTrackerStructData(SynOpticonActorStructObj->GetObjectField("EyeTrackerStruct"));
	SynOpticonActorStruct.RemoteEyeTrackerStruct = GetRemoteEyeTrackerStructData(SynOpticonActorStructObj->GetObjectField("RemoteEyeTrackerStruct"));
	
	TArray<TSharedPtr<FJsonValue>> ShimmerComponents = SynOpticonActorStructObj->GetArrayField("ShimmerComponents");
	for (TSharedPtr<FJsonValue> Shimmer : ShimmerComponents)
	{
		TSharedPtr<FJsonObject> ShimmerObj = Shimmer->AsObject();
		SynOpticonActorStruct.ShimmerComponents.Add(GetShimmerComponentStructData(ShimmerObj));
	}

	TArray<TSharedPtr<FJsonValue>> HandComponents = SynOpticonActorStructObj->GetArrayField("HandComponents");
	for (TSharedPtr<FJsonValue> Hand : HandComponents)
	{
		TSharedPtr<FJsonObject> HandObj = Hand->AsObject();
		SynOpticonActorStruct.HandComponents.Add(GetHandStructData(HandObj));
	}

	const TSharedPtr< FJsonObject >* Ptr;
	if (SynOpticonActorStructObj->TryGetObjectField("VisualComponentData", Ptr))
	{
		SynOpticonActorStruct.VisualComponentData = GetVisionStructData(*Ptr);
	}
	
	SynOpticonActorStruct.GazeLogComponentData = GetLoggingStructData(SynOpticonActorStructObj->GetObjectField("GazeLogComponentData"));
	SynOpticonActorStruct.LeftHandLogComponentData = GetLoggingStructData(SynOpticonActorStructObj->GetObjectField("LeftHandLogComponentData"));
	SynOpticonActorStruct.RightHandLogComponentData = GetLoggingStructData(SynOpticonActorStructObj->GetObjectField("RightHandLogComponentData"));

	TArray<TSharedPtr<FJsonValue>> GazeData = SynOpticonActorStructObj->GetArrayField("GazeData");
	for (TSharedPtr<FJsonValue> GD : GazeData)
	{
		TSharedPtr<FJsonObject> GDObj = GD->AsObject();
		SynOpticonActorStruct.GazeData.Add(GetFixationGroupData(GDObj));
	}
	
	SynOpticonActorStruct.HasMyo = SynOpticonActorStructObj->GetBoolField("HasMyo");
	SynOpticonActorStruct.MarkersConfiguration = GetMarkersConfigurationFromString(
		SynOpticonActorStructObj->GetStringField("MarkersConfiguration"));

	return SynOpticonActorStruct;
}

FEyeTrackerStruct USynOpticonActorJSONExporter::GetEyeTrackerStructData(TSharedPtr<FJsonObject> EyeTrackerStructObj)
{
	FEyeTrackerStruct EyeTrackerStruct;

	EyeTrackerStruct.GlassesID = EyeTrackerStructObj->GetStringField("GlassesID");

	return EyeTrackerStruct;
}

FRemoteEyeTrackerStruct USynOpticonActorJSONExporter::GetRemoteEyeTrackerStructData(TSharedPtr<FJsonObject> RemoteEyeTrackerStructObj)
{
	FRemoteEyeTrackerStruct RemoteEyeTrackerStruct;

	RemoteEyeTrackerStruct.RemoteTrackerID = RemoteEyeTrackerStructObj->GetStringField("RemoteTrackerID");
	RemoteEyeTrackerStruct.PointToLookAt = UJSONExporter::GetFVectorFromJSON(RemoteEyeTrackerStructObj->GetObjectField("PointToLookAt"));
	RemoteEyeTrackerStruct.RemoteTrackerPositionLeft = UJSONExporter::GetFVectorFromJSON(RemoteEyeTrackerStructObj->GetObjectField("RemoteTrackerPositionLeft"));
	RemoteEyeTrackerStruct.RemoteTrackerPositionRight = UJSONExporter::GetFVectorFromJSON(RemoteEyeTrackerStructObj->GetObjectField("RemoteTrackerPositionRight"));
	RemoteEyeTrackerStruct.AttachedScreenName = RemoteEyeTrackerStructObj->GetStringField("AttachedScreenName");

	return RemoteEyeTrackerStruct;
}

FShimmerComponentStruct USynOpticonActorJSONExporter::GetShimmerComponentStructData(TSharedPtr<FJsonObject> ShimmerComponentStructObj)
{	
	FShimmerComponentStruct ShimmerComponentStruct;

	ShimmerComponentStruct.DeviceName = ShimmerComponentStructObj->GetStringField("DeviceName");
	ShimmerComponentStruct.HasHeartRate = ShimmerComponentStructObj->GetBoolField("HasHeartRate");
	ShimmerComponentStruct.HasGalvanicSkinResponse = ShimmerComponentStructObj->GetBoolField("HasGalvanicSkinResponse");
	ShimmerComponentStruct.ShimmerIndex = ShimmerComponentStructObj->GetIntegerField("ShimmerIndex");

	return ShimmerComponentStruct;
}

FHandStruct USynOpticonActorJSONExporter::GetHandStructData(TSharedPtr<FJsonObject> HandStructObj)
{
	FHandStruct HandStruct;

	HandStruct.NatNetName = HandStructObj->GetStringField("NatNetName");
	HandStruct.MarkersConfiguration = GetMarkersConfigurationFromString(HandStructObj->GetStringField("MarkersConfiguration"));
	HandStruct.LeftHand = HandStructObj->GetBoolField("LeftHand");

	return HandStruct;
}

FOpenFaceStruct USynOpticonActorJSONExporter::GetVisionStructData(TSharedPtr<FJsonObject> VisionStructObj)
{
	FOpenFaceStruct VisionStruct;

	if (!VisionStructObj->TryGetStringField("OpenFaceID", VisionStruct.OpenFaceID))
	{
		return VisionStruct;
	}
	VisionStruct.CameraTrackerPositionRight = UJSONExporter::GetFVectorFromJSON(VisionStructObj->GetObjectField("CameraTrackerPositionRight"));
	VisionStruct.CameraTrackerPositionLeft = UJSONExporter::GetFVectorFromJSON(VisionStructObj->GetObjectField("CameraTrackerPositionLeft"));
	VisionStruct.AttachedScreenName = VisionStructObj->GetStringField("AttachedScreenName");
	VisionStruct.IsTrackingHead = VisionStructObj->GetBoolField("IsTrackingHead");
	VisionStruct.IsTrackingEyes = VisionStructObj->GetBoolField("IsTrackingEyes");

	return VisionStruct;
}

FSynOpticonActorLoggingStruct USynOpticonActorJSONExporter::GetLoggingStructData(TSharedPtr<FJsonObject> SynOpticonActorLoggingStructObj)
{
	FSynOpticonActorLoggingStruct SynOpticonActorLoggingStruct;

	SynOpticonActorLoggingStruct.SynOpticonActorID = SynOpticonActorLoggingStructObj->GetIntegerField("SynOpticonActorID");
	SynOpticonActorLoggingStruct.TotalTime = SynOpticonActorLoggingStructObj->GetNumberField("TotalTime");
	TArray<TSharedPtr<FJsonValue>> Objects = SynOpticonActorLoggingStructObj->GetArrayField("Objects");
	for (TSharedPtr<FJsonValue> Object : Objects)
	{
		TSharedPtr<FJsonObject> Pair = Object->AsObject();
		TSharedPtr<FJsonObject> LoggingInfo = Pair->GetObjectField("Value");
		SynOpticonActorLoggingStruct.Objects.Add(Pair->GetIntegerField("Key"), GetObjectLoggingInfoData(LoggingInfo));
	}

	TArray<TSharedPtr<FJsonValue>> GazePaths = SynOpticonActorLoggingStructObj->GetArrayField("GazePaths");
	for (TSharedPtr<FJsonValue> GP : GazePaths)
	{
		TSharedPtr<FJsonObject> GPObj = GP->AsObject();
		SynOpticonActorLoggingStruct.GazePaths.Add(GetPathLoggingInfoData(GPObj));
	}
	
	return SynOpticonActorLoggingStruct;
}

FFixationGroupStruct USynOpticonActorJSONExporter::GetFixationGroupData(TSharedPtr<FJsonObject> FixationGroupStructObj)
{	
	FFixationGroupStruct FixationGroupStruct;

	FixationGroupStruct.GazeActorId = FixationGroupStructObj->GetIntegerField("GazeActorId");
	FixationGroupStruct.SessionId = FixationGroupStructObj->GetStringField("SessionId");
	FixationGroupStruct.bPopup = FixationGroupStructObj->GetBoolField("bPopup");
	FixationGroupStruct.Location = UJSONExporter::GetFVectorFromJSON(FixationGroupStructObj->GetObjectField("Location"));
	FixationGroupStruct.Duration = FixationGroupStructObj->GetNumberField("Duration");
	int64 TimestampUnix;
	if (FixationGroupStructObj->TryGetNumberField("Timestamp", TimestampUnix))
	{
		FixationGroupStruct.Timestamp = FDateTime::FromUnixTimestamp(TimestampUnix);
	}
	else
	{
		//FString TimestampS = FixationGroupStructObj->GetStringField("Timestamp");
		FDateTime::Parse(FixationGroupStructObj->GetStringField("Timestamp"), FixationGroupStruct.Timestamp); //for old versions that use string
	}

	FixationGroupStruct.Normal = UJSONExporter::GetFVectorFromJSON(FixationGroupStructObj->GetObjectField("Normal"));
	FixationGroupStruct.HitUV = UJSONExporter::GetFVector2DFromJSON(FixationGroupStructObj->GetObjectField("HitUV"));

	return FixationGroupStruct;
}

FGazePathLoggingInfoStruct USynOpticonActorJSONExporter::GetPathLoggingInfoData(TSharedPtr<FJsonObject> GazePathLoggingInfoStructObj)
{	
	FGazePathLoggingInfoStruct GazePathLoggingInfoStruct;

	GazePathLoggingInfoStruct.GazeActorID = GazePathLoggingInfoStructObj->GetNumberField("GazeActorID");
	int64 TimeStampUnix;
	if (GazePathLoggingInfoStructObj->TryGetNumberField("TimeStamp", TimeStampUnix))
	{
		GazePathLoggingInfoStruct.TimeStamp = FDateTime::FromUnixTimestamp(TimeStampUnix);
	}
	else
	{
		FDateTime::Parse(GazePathLoggingInfoStructObj->GetStringField("TimeStamp"), GazePathLoggingInfoStruct.TimeStamp);
	}
	
	GazePathLoggingInfoStruct.Duration = GazePathLoggingInfoStructObj->GetNumberField("Duration");

	return GazePathLoggingInfoStruct;
}

FObjectLoggingInfoStruct USynOpticonActorJSONExporter::GetObjectLoggingInfoData(TSharedPtr<FJsonObject> ObjectLoggingInfoStructObj)
{
	FObjectLoggingInfoStruct ObjectLoggingInfoStruct;

	ObjectLoggingInfoStruct.GazeActorID = ObjectLoggingInfoStructObj->GetIntegerField("GazeActorID");
	ObjectLoggingInfoStruct.Time = ObjectLoggingInfoStructObj->GetNumberField("Time");
	ObjectLoggingInfoStruct.Count = ObjectLoggingInfoStructObj->GetIntegerField("Count");
	ObjectLoggingInfoStruct.Percentage = ObjectLoggingInfoStructObj->GetNumberField("Percentage");

	return ObjectLoggingInfoStruct;
}

MarkersConfigurationEnum USynOpticonActorJSONExporter::GetMarkersConfigurationFromString(FString MarkersConfigurationStr)
{
	if (MarkersConfigurationStr == "Glasses Right")
		return MarkersConfigurationEnum::VE_Glasses_Right;
	if (MarkersConfigurationStr == "Glasses Left")
		return MarkersConfigurationEnum::VE_Glasses_Left;
	if (MarkersConfigurationStr == "Position")
		return MarkersConfigurationEnum::VE_Position;
	if (MarkersConfigurationStr == "Laser Tool")
		return MarkersConfigurationEnum::VE_Laser;
	if (MarkersConfigurationStr == "Manual Tool")
		return MarkersConfigurationEnum::VE_ManualTool;
	if (MarkersConfigurationStr == "Calibration Tool")
		return MarkersConfigurationEnum::VE_CalibrationTool;
	if (MarkersConfigurationStr == "Cap")
		return MarkersConfigurationEnum::VE_PositionCap;
	return MarkersConfigurationEnum::VE_PositionOnly;
}