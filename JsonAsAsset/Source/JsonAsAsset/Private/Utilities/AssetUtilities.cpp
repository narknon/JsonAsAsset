// Copyright Epic Games, Inc. All Rights Reserved.

#include "Utilities/AssetUtilities.h"

#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Interfaces/IPluginManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Settings/JsonAsAssetSettings.h"
#include "Dom/JsonObject.h"

#include "HttpModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Importers/TextureImporters.h"
#include "Importers/MaterialParameterCollectionImporter.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Utilities/AssetUtilities.h"
#include "Utilities/RemoteUtilities.h"

UPackage* FAssetUtilities::CreateAssetPackage(const FString& FullPath) {
	UPackage* Package = CreatePackage(*FullPath);
	UPackage* _ = Package->GetOutermost();
	Package->FullyLoad();

	return Package;
}

UPackage* FAssetUtilities::CreateAssetPackage(const FString& Name, const FString& OutputPath) {
	UPackage* Ignore = nullptr;
	return CreateAssetPackage(Name, OutputPath, Ignore);
}

UPackage* FAssetUtilities::CreateAssetPackage(const FString& Name, const FString& OutputPath, UPackage*& OutOutermostPkg) {
	const UJsonAsAssetSettings* Settings = GetDefault<UJsonAsAssetSettings>();
	FString ModifiablePath;

	// References Automatically Formatted
	if (!OutputPath.StartsWith("/Game/") && !OutputPath.StartsWith("/Plugins/")) {
		OutputPath.Split(*(Settings->ExportDirectory.Path + "/"), nullptr, &ModifiablePath, ESearchCase::IgnoreCase, ESearchDir::FromStart);
		ModifiablePath.Split("/", nullptr, &ModifiablePath, ESearchCase::IgnoreCase, ESearchDir::FromStart);
		ModifiablePath.Split("/", &ModifiablePath, nullptr, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		// Ex: RestPath: Plugins/ContentLibraries/EpicBaseTextures
		// Ex: RestPath: Content/Athena
		bool bIsPlugin = ModifiablePath.StartsWith("Plugins");

		// Plugins/ContentLibraries/EpicBaseTextures -> ContentLibraries/EpicBaseTextures
		if (bIsPlugin) ModifiablePath = ModifiablePath.Replace(TEXT("Plugins/"), TEXT("")).Replace(TEXT("GameFeatures/"), TEXT("")).Replace(TEXT("Content/"), TEXT(""));
		// Content/Athena -> Game/Athena
		else ModifiablePath = ModifiablePath.Replace(TEXT("Content"), TEXT("Game"));

		// ContentLibraries/EpicBaseTextures -> /ContentLibraries/EpicBaseTextures/
		ModifiablePath = "/" + ModifiablePath + "/";

		// Check if plugin exists
		if (bIsPlugin) {
			FString PluginName;
			ModifiablePath.Split("/", nullptr, &PluginName, ESearchCase::IgnoreCase, ESearchDir::FromStart);
			PluginName.Split("/", &PluginName, nullptr, ESearchCase::IgnoreCase, ESearchDir::FromStart);

			if (IPluginManager::Get().FindPlugin(PluginName) == nullptr) {
				#define LOCTEXT_NAMESPACE "UMG"
				#if WITH_EDITOR
				// Setup notification's arguments
				FFormatNamedArguments Args;
				Args.Add(TEXT("PluginName"), FText::FromString(PluginName));

				// Create notification
				FNotificationInfo Info(FText::Format(LOCTEXT("NeedPlugin", "Plugin Missing: {PluginName}"), Args));
				Info.ExpireDuration = 10.0f;
				Info.bUseLargeFont = true;
				Info.bUseSuccessFailIcons = true;
				Info.WidthOverride = FOptionalSize(350);
				Info.SubText = FText::FromString(FString("Asset will be placed in Content Folder"));

				TSharedPtr<SNotificationItem> NotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);
				NotificationPtr->SetCompletionState(SNotificationItem::CS_Fail);
				#endif
				#undef LOCTEXT_NAMESPACE

				ModifiablePath = FString(TEXT("/Game/"));
			}
		}
	} else {
		ModifiablePath = OutputPath;
		ModifiablePath.Split("/", &ModifiablePath, nullptr, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		ModifiablePath = ModifiablePath + "/";
	}

	const FString PathWithGame = ModifiablePath + Name;
	UPackage* Package = CreatePackage(*PathWithGame);
	OutOutermostPkg = Package->GetOutermost();
	Package->FullyLoad();

	return Package;
}

UObject* FAssetUtilities::GetSelectedAsset() {
	const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	if (SelectedAssets.Num() == 0) {
		GLog->Log("JsonAsAsset: [GetSelectedAsset] None selected, returning nullptr.");

		const FText DialogText = FText::FromString(TEXT("A function to find a selected asset failed, please select a asset to go further."));
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);

		// None found, therefore we need to return nullptr
		return nullptr;
	}

	// Return only the first selected asset
	return SelectedAssets[0].GetAsset();
}

// Constructing assets ect..
template <typename T>
bool FAssetUtilities::ConstructAsset(const FString& Path, const FString& Type, TObjectPtr<T>& OutObject, bool& bSuccess) {
	// Supported Assets
	if (Type == "Texture2D" ||
		Type == "TextureCube" ||
		Type == "TextureRenderTarget2D" ||
		Type == "MaterialParameterCollection" ||
		Type == "CurveFloat" ||
		Type == "CurveVector" ||
		Type == "CurveLinearColorAtlas" ||
		Type == "CurveLinearColor" ||
		Type == "PhysicalMaterial" ||
		Type == "SubsurfaceProfile" ||
		Type == "LandscapeGrassType" ||
		Type == "MaterialInstanceConstant" ||
		Type == "ReverbEffect" ||
		Type == "SoundAttenuation" ||
		Type == "SoundConcurrency" ||
		Type == "DataTable" ||
		Type == "SubsurfaceProfile" ||
		Type == "MaterialFunction"
		) {
		//		Manually supported asset types
		// (ex: textures have to be handled separately)
		if (Type ==
			"Texture2D" ||
			Type == "TextureRenderTarget2D" ||
			Type == "TextureCube"
			) {
			UTexture* Texture;

			bSuccess = Construct_TypeTexture(Path, Texture);
			if (bSuccess) OutObject = Cast<T>(Texture);

			return true;
		}
		else {
			const TArray<TSharedPtr<FJsonValue>> Response = API_RequestExports(Path);
			if (Response.IsEmpty()) return true;

			const TSharedPtr<FJsonObject> JsonObject = Response[0]->AsObject();
			FString PackagePath;
			FString AssetName;
			Path.Split(".", &PackagePath, &AssetName);

			if (JsonObject) {
				UPackage* OutermostPkg;
				UPackage* Package = CreatePackage(*PackagePath);
				OutermostPkg = Package->GetOutermost();
				Package->FullyLoad();

				// Import asset by IImporter
				IImporter* Importer = new IImporter();
				bSuccess = Importer->HandleExports(Response, PackagePath, true);

				// Define found object
				OutObject = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *Path));

				return true;
			}
		}
	}

	return false;
}

bool FAssetUtilities::Construct_TypeTexture(const FString& Path, UTexture*& OutTexture) {
	FHttpModule* HttpModule = &FHttpModule::Get();

	const TSharedRef<IHttpRequest> HttpRequest = HttpModule->CreateRequest();

	UE_LOG(LogTemp, Warning, TEXT("Test Path: %s"), *Path);

	HttpRequest->SetURL("http://localhost:1500/api/v1/export?path=" + Path);
	HttpRequest->SetHeader("content-type", "image/png");
	HttpRequest->SetVerb(TEXT("GET"));

	const TSharedPtr<IHttpResponse> HttpResponse = FRemoteUtilities::ExecuteRequestSync(HttpRequest);
	if (!HttpResponse.IsValid()) return false;

	const TArray<uint8> Data = HttpResponse->GetContent();

	FString PackagePath;
	FString AssetName;
	Path.Split(".", &PackagePath, &AssetName);

	const TSharedRef<IHttpRequest> NewRequest = HttpModule->CreateRequest();
	NewRequest->SetURL("http://localhost:1500/api/v1/export?raw=true&path=" + Path);
	NewRequest->SetVerb(TEXT("GET"));

	const TSharedPtr<IHttpResponse> NewResponse = FRemoteUtilities::ExecuteRequestSync(NewRequest);
	if (!NewResponse.IsValid()) return false;

	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(NewResponse->GetContentAsString());
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	if (FJsonSerializer::Deserialize(JsonReader, JsonArray)) {
		UPackage* OutermostPkg;
		UPackage* Package = CreatePackage(*PackagePath);
		OutermostPkg = Package->GetOutermost();
		Package->FullyLoad();

		const UTextureImporters* Importer = new UTextureImporters(AssetName, Path, JsonArray[0]->AsObject(), Package, OutermostPkg);

		if (JsonArray.IsEmpty())
			// No valid entries
			return false;

		TSharedPtr<FJsonObject> FinalJsonObject = JsonArray[0]->AsObject();
		UTexture* Texture = nullptr;

		// Texture 2D
		if (FinalJsonObject->GetStringField("Type") == "Texture2D")
			Importer->ImportTexture2D(Texture, Data, FinalJsonObject->GetObjectField("Properties"));
		// Texture Cube
		if (FinalJsonObject->GetStringField("Type") == "TextureCube")
			Importer->ImportTextureCube(Texture, Data, FinalJsonObject);
		// Texture Render Target 2D
		if (FinalJsonObject->GetStringField("Type") == "TextureRenderTarget2D")
			Importer->ImportRenderTarget2D(Texture, FinalJsonObject->GetObjectField("Properties"));

		// If it still wasn't imported
		if (Texture == nullptr) return false;

		FAssetRegistryModule::AssetCreated(Texture);
		if (!Texture->MarkPackageDirty()) return false;
		Package->SetDirtyFlag(true);
		Texture->PostEditChange();
		Texture->AddToRoot();

		OutTexture = Texture;
	}

	return true;
}

const TArray<TSharedPtr<FJsonValue>> FAssetUtilities::API_RequestExports(const FString& Path) {
	FHttpModule* HttpModule = &FHttpModule::Get();
	const TSharedRef<IHttpRequest> HttpRequest = HttpModule->CreateRequest();

	FString PackagePath;
	FString AssetName;
	Path.Split(".", &PackagePath, &AssetName);

	const TSharedRef<IHttpRequest> NewRequest = HttpModule->CreateRequest();
	NewRequest->SetURL("http://localhost:1500/api/v1/export?raw=true&path=" + Path);
	NewRequest->SetVerb(TEXT("GET"));

	const TSharedPtr<IHttpResponse> NewResponse = FRemoteUtilities::ExecuteRequestSync(NewRequest);
	if (!NewResponse.IsValid()) return TArray<TSharedPtr<FJsonValue>>();

	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(NewResponse->GetContentAsString());
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	if (FJsonSerializer::Deserialize(JsonReader, JsonArray)) {
		return JsonArray;
	}

	return TArray<TSharedPtr<FJsonValue>>();
}
