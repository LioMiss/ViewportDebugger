#pragma once
#include "Json.h"

struct FViewportInfoDump
{
	FVector Location;
	FRotator Rotation;
	float FOV;

	FVector PawnLocation;
	FRotator PawnRotation;
	FString MapName;
	//FString ScreenShotDescription;

	bool Serialize(FString& OutputStr) const
	{
		const TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		JsonObject->SetStringField(TEXT("Location"), Location.ToString());
		JsonObject->SetStringField(TEXT("Rotation"), Rotation.ToString());
		JsonObject->SetNumberField(TEXT("FOV"), FOV);
		JsonObject->SetStringField(TEXT("PawnLocation"), PawnLocation.ToString());
		JsonObject->SetStringField(TEXT("PawnRotation"), PawnRotation.ToString());
		JsonObject->SetStringField(TEXT("MapName"), MapName);
		//JsonObject->SetStringField(TEXT("ScreenShotDescription"), ScreenShotDescription);
		
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputStr);
		if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
		{
			return true;
		}
		return false;
	}

	bool Deserialize(const FString& JsonStr)
	{
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonStr);
		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			Location.InitFromString(JsonObject->GetStringField(TEXT("Location")));
			Rotation.InitFromString(JsonObject->GetStringField(TEXT("Rotation")));
			FOV = JsonObject->GetNumberField(TEXT("FOV"));
			PawnLocation.InitFromString(JsonObject->GetStringField(TEXT("PawnLocation")));
			PawnRotation.InitFromString(JsonObject->GetStringField(TEXT("PawnRotation")));
			MapName =JsonObject->GetStringField(TEXT("MapName"));
			//ScreenShotDescription =JsonObject->GetStringField(TEXT("ScreenShotDescription"));
			return true;
		}
		return false;
	}

	static void GenTeleportCommandStr(const FVector& Position, const FRotator& Rotation, FString& TeleportCommand)
	{
		TeleportCommand = FString::Printf(TEXT("rcon BugItGo %f %f %f %f %f %f"), Position.X, Position.Y, Position.Z,
			Rotation.Pitch, Rotation.Yaw, Rotation.Roll);
		//"rcon BugItGo -4615.750488 12616.836914 2163.401855 -7.331664 102.705376 -0.000051";
	}
};


struct FDebugInfoDump
{
	TArray<FViewportInfoDump> CameraInfoDumps;
};