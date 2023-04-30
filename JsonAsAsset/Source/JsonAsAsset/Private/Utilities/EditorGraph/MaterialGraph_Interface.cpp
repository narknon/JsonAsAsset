// Copyright Epic Games, Inc. All Rights Reserved.

#include "Utilities/EditorGraph/MaterialGraph_Interface.h"

#include "Curves/CurveLinearColorAtlas.h"
#include "VT/RuntimeVirtualTextureEnum.h"
#include "Dom/JsonObject.h"
#include "Utilities/MathUtilities.h"
#include "Materials/MaterialParameterCollection.h"
#include "LandscapeGrassType.h"

// Expressions
#include "Materials/MaterialExpressionAbs.h"
#include "Materials/MaterialExpressionAdd.h"
#include "Materials/MaterialExpressionAppendVector.h"
#include "Materials/MaterialExpressionCeil.h"
#include "Materials/MaterialExpressionVectorNoise.h"
#include "Materials/MaterialExpressionSceneDepth.h"
#include "Materials/MaterialExpressionClamp.h"
#include "Materials/MaterialExpressionComment.h"
#include "Materials/MaterialExpressionTextureSampleParameterSubUV.h"
#include "Materials/MaterialExpressionRuntimeVirtualTextureSample.h"
#include "Materials/MaterialExpressionRuntimeVirtualTextureSampleParameter.h"
#include "Materials/MaterialExpressionSkyLightEnvMapSample.h"
#include "Materials/MaterialExpressionLandscapeVisibilityMask.h"
#include "Materials/MaterialExpressionPreviousFrameSwitch.h"
#include "Materials/MaterialExpressionSign.h"
#include "Materials/MaterialExpressionVirtualTextureFeatureSwitch.h"
#include "Materials/MaterialExpressionDistanceFieldsRenderingSwitch.h"
#include "Materials/MaterialExpressionComponentMask.h"
#include "Materials/MaterialExpressionConstant.h"
#include "Materials/MaterialExpressionConstant2Vector.h"
#include "Materials/MaterialExpressionConstant3Vector.h"
#include "Materials/MaterialExpressionConstant4Vector.h"
#include "Materials/MaterialExpressionConstantBiasScale.h"
#include "Materials/MaterialExpressionShadowReplace.h"
#include "Materials/MaterialExpressionNaniteReplace.h"
#include "Materials/MaterialExpressionCosine.h"
#include "Materials/MaterialExpressionCurveAtlasRowParameter.h"
#include "Materials/MaterialExpressionCustom.h"
#include "Materials/MaterialExpressionLandscapeGrassOutput.h"
#include "Materials/MaterialExpressionLandscapeLayerSample.h"
#include "Materials/MaterialExpressionLandscapeLayerCoords.h"
#include "Materials/MaterialExpressionLandscapeLayerSwitch.h"
#include "Materials/MaterialExpressionLandscapeLayerBlend.h"
#include "Materials/MaterialExpressionLandscapeLayerWeight.h"
#include "Materials/MaterialExpressionLandscapePhysicalMaterialOutput.h"
#include "Materials/MaterialExpressionArcsine.h"
#include "Materials/MaterialExpressionArcsineFast.h"
#include "Materials/MaterialExpressionBumpOffset.h"
#include "Materials/MaterialExpressionFresnel.h"
#include "Materials/MaterialExpressionMaterialProxyReplace.h"
#include "Materials/MaterialExpressionSceneTexture.h"
#include "Materials/MaterialExpressionAbsorptionMediumMaterialOutput.h"
#include "Materials/MaterialExpressionSetMaterialAttributes.h"
#include "Materials/MaterialExpressionSquareRoot.h"
#include "Materials/MaterialExpressionSkyAtmosphereLightDirection.h"
#include "Materials/MaterialExpressionShaderStageSwitch.h"
#include "Materials/MaterialExpressionSkyAtmosphereLightIlluminance.h"
#include "Materials/MaterialExpressionStaticComponentMaskParameter.h"
#include "Materials/MaterialExpressionReflectionVectorWS.h"
#include "Materials/MaterialExpressionViewProperty.h"
#include "Materials/MaterialExpressionChannelMaskParameter.h"
#include "Materials/MaterialExpressionShadingModel.h"
#include "Materials/MaterialExpressionDesaturation.h"
#include "Materials/MaterialExpressionDistance.h"
#include "Materials/MaterialExpressionDivide.h"
#include "Materials/MaterialExpressionDDX.h"
#include "Materials/MaterialExpressionDDY.h"
#include "Materials/MaterialExpressionCrossProduct.h"
#include "Materials/MaterialExpressionDepthFade.h"
#include "Materials/MaterialExpressionRayTracingQualitySwitch.h"
#include "Materials/MaterialExpressionDeriveNormalZ.h"
#include "Materials/MaterialExpressionQualitySwitch.h"
#include "Materials/MaterialExpressionReflectionCapturePassSwitch.h"
#include "Materials/MaterialExpressionArctangent2Fast.h"
#include "Materials/MaterialExpressionArctangentFast.h"
#include "Materials/MaterialExpressionArctangent2.h"
#include "Materials/MaterialExpressionArctangent.h"
#include "Materials/MaterialExpressionRotateAboutAxis.h"
#include "Materials/MaterialExpressionMakeMaterialAttributes.h"
#include "Materials/MaterialExpressionMaterialAttributeLayers.h"
#include "Materials/MaterialExpressionShadingPathSwitch.h"
#include "Materials/MaterialExpressionTransform.h"
#include "Materials/MaterialExpressionVertexInterpolator.h"
#include "Materials/MaterialExpressionDotProduct.h"
#include "Materials/MaterialExpressionFloor.h"
#include "Materials/MaterialExpressionFmod.h"
#include "Materials/MaterialExpressionFrac.h"
#include "Materials/MaterialExpressionFunctionInput.h"
#include "Materials/MaterialExpressionFunctionOutput.h"
#include "Materials/MaterialExpressionIf.h"
#include "Materials/MaterialExpressionBlendMaterialAttributes.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"
#include "Materials/MaterialExpressionGetMaterialAttributes.h"
#include "Materials/MaterialExpressionBreakMaterialAttributes.h"
#include "Materials/MaterialExpressionMax.h"
#include "Materials/MaterialExpressionMin.h"
#include "Materials/MaterialExpressionMultiply.h"
#include "Materials/MaterialExpressionNamedReroute.h"
#include "Materials/MaterialExpressionOneMinus.h"
#include "Materials/MaterialExpressionPanner.h"
#include "Materials/MaterialExpressionPower.h"
#include "Materials/MaterialExpressionReroute.h"
#include "Materials/MaterialExpressionRotator.h"
#include "Materials/MaterialExpressionRound.h"
#include "Materials/MaterialExpressionSaturate.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionSine.h"
#include "Materials/MaterialExpressionSmoothStep.h"
#include "Materials/MaterialExpressionSphereMask.h"
#include "Materials/MaterialExpressionStaticBool.h"
#include "Materials/MaterialExpressionStaticSwitch.h"
#include "Materials/MaterialExpressionCollectionParameter.h"
#include "Materials/MaterialExpressionStaticSwitchParameter.h"
#include "Materials/MaterialExpressionStep.h"
#include "Materials/MaterialExpressionSubtract.h"
#include "Materials/MaterialExpressionTextureCoordinate.h"
#include "Materials/MaterialExpressionTextureObjectParameter.h"
#include "Materials/MaterialExpressionTextureProperty.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionTime.h"
#include "Materials/MaterialExpressionTransformPosition.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionDynamicParameter.h"
#include "Materials/MaterialExpressionFeatureLevelSwitch.h"
#include "Materials/MaterialExpressionNormalize.h"
#include "Materials/MaterialExpressionTruncate.h"
#include "Materials/MaterialExpressionWorldPosition.h"
#include "Materials/MaterialExpressionNoise.h"
#include <MaterialEditingLibrary.h>
#include <Editor/UnrealEd/Public/FileHelpers.h>
//#include <Import/Private/MaterialX/MaterialExpressions/MaterialExpressionLength.h>

TSharedPtr<FJsonObject> UMaterialGraph_Interface::FindEditorOnlyData(const FString& Type, const FString& Outer, TMap<FName, FImportData>& OutExports, TArray<FName>& ExpressionNames, bool bFilterByOuter) {
	TSharedPtr<FJsonObject> EditorOnlyData;

	for (const TSharedPtr<FJsonValue> Value : (bFilterByOuter ? FilterExportsByOuter(Outer) : AllJsonObjects)) {
		TSharedPtr<FJsonObject> Object = TSharedPtr(Value->AsObject());

		FString ExType = Object->GetStringField("Type");
		FString Name = Object->GetStringField("Name");

		if (ExType == Type + "EditorOnlyData") {
			EditorOnlyData = Object;
			continue;
		}

		ExpressionNames.Add(FName(Name));
		OutExports.Add(FName(Name), FImportData(ExType, Outer, Object));
	}

	return EditorOnlyData;
}

TMap<FName, UMaterialExpression*> UMaterialGraph_Interface::ConstructExpressions(UObject* Parent, const FString& Outer, TArray<FName>& ExpressionNames, TMap<FName, FImportData>& Exports) {
	TMap<FName, UMaterialExpression*> CreatedExpressionMap;

	for (FName Name : ExpressionNames) {
		FName Type;
		bool bFound = false;

		for (TTuple<FName, FImportData>& Key : Exports) {
			if (Key.Key == Name && Key.Value.Outer == FName(Outer)) {
				Type = Key.Value.Type;
				bFound = true;
				break;
			}
		}

		if (!bFound) continue;
		UMaterialExpression* Ex = CreateEmptyExpression(Parent, Name, Type);
		if (Ex == nullptr) continue;

		CreatedExpressionMap.Add(Name, Ex);
	}

	return CreatedExpressionMap;
}

FExpressionInput UMaterialGraph_Interface::CreateExpressionInput(TSharedPtr<FJsonObject> JsonProperties, TMap<FName, UMaterialExpression*>& CreatedExpressionMap, FString PropertyName) {
	// Find Expression Input by PropertyName
	if (const TSharedPtr<FJsonObject>* Ptr; JsonProperties->TryGetObjectField(PropertyName, Ptr)) {
		FJsonObject* AsObject = Ptr->Get();
		FName ExpressionName = GetExpressionName(AsObject);

		// If Expression Found
		if (CreatedExpressionMap.Contains(ExpressionName)) {
			FExpressionInput Input = PopulateExpressionInput(AsObject, *CreatedExpressionMap.Find(ExpressionName));

			return Input;
		}
	}

	return FExpressionInput();
}

void UMaterialGraph_Interface::PropagateExpressions(UObject* Parent, TArray<FName>& ExpressionNames, TMap<FName, FImportData>& Exports, TMap<FName, UMaterialExpression*>& CreatedExpressionMap, bool bCheckOuter, bool bSubgraph) {
	for (FName Name : ExpressionNames) {
		FImportData* Type = Exports.Find(Name);

		TSharedPtr<FJsonObject> Properties = Type->Json->GetObjectField("Properties");

		// Find the expression from FName
		if (!CreatedExpressionMap.Contains(Name)) continue;
		UMaterialExpression* Expression = *CreatedExpressionMap.Find(Name);

		if (bCheckOuter) {
			FString Outer;
			if (Type->Json->TryGetStringField("Outer", Outer) && Outer != Parent->GetName())
				continue;
		}

		if (Type->Type == "MaterialExpressionFunctionOutput") {
			UMaterialExpressionFunctionOutput* FunctionOutput = Cast<UMaterialExpressionFunctionOutput>(Expression);

			if (FString OutputName; Properties->TryGetStringField("OutputName", OutputName)) 
				FunctionOutput->OutputName = FName(OutputName);
			if (FString FuncDescription; Properties->TryGetStringField("Description", FuncDescription)) 
				FunctionOutput->Description = FuncDescription;
			if (int SortPriority; Properties->TryGetNumberField("SortPriority", SortPriority)) 
				FunctionOutput->SortPriority = SortPriority;

			FunctionOutput->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			if (bool bLastPreviewed; Properties->TryGetBoolField("bLastPreviewed", bLastPreviewed)) 
				FunctionOutput->bLastPreviewed = bLastPreviewed;
			FunctionOutput->Id = FGuid(Properties->GetStringField("ID"));

			Expression = FunctionOutput;
		} else if (Type->Type == "MaterialExpressionStaticSwitchParameter") {
			UMaterialExpressionStaticSwitchParameter* StaticSwitchParameter = Cast<UMaterialExpressionStaticSwitchParameter>(Expression);

			StaticSwitchParameter->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			StaticSwitchParameter->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");
			
			if (bool DefaultValue; Properties->TryGetBoolField("DefaultValue", DefaultValue)) 
				StaticSwitchParameter->DefaultValue = DefaultValue;

			Expression = StaticSwitchParameter;
		} else if (Type->Type == "MaterialExpressionFunctionInput") {
			UMaterialExpressionFunctionInput* FunctionInput = Cast<UMaterialExpressionFunctionInput>(Expression);

			FunctionInput->Preview = CreateExpressionInput(Properties, CreatedExpressionMap, "Preview");
			if (FString InputName; Properties->TryGetStringField("InputName", InputName)) 
				FunctionInput->InputName = FName(InputName);
			if (FString FuncDescription; Properties->TryGetStringField("Description", FuncDescription)) 
				FunctionInput->Description = FuncDescription;
			FunctionInput->Id = FGuid(Properties->GetStringField("ID"));
			
			if (FString InputType; Properties->TryGetStringField("InputType", InputType))
				FunctionInput->InputType = static_cast<EFunctionInputType>(StaticEnum<EFunctionInputType>()->GetValueByNameString(InputType));

			if (const TSharedPtr<FJsonObject>* PreviewValue; Properties->TryGetObjectField("PreviewValue", PreviewValue)) 
				FunctionInput->PreviewValue = FMathUtilities::ObjectToVector4f(PreviewValue->Get());
			if (bool bUsePreviewValueAsDefault; Properties->TryGetBoolField("bUsePreviewValueAsDefault", bUsePreviewValueAsDefault)) 
				FunctionInput->bUsePreviewValueAsDefault = bUsePreviewValueAsDefault;
			if (int SortPriority; Properties->TryGetNumberField("SortPriority", SortPriority)) 
				FunctionInput->SortPriority = SortPriority;

			Expression = FunctionInput;
		} else if (Type->Type == "MaterialExpressionAbs") {
			UMaterialExpressionAbs* Abs = Cast<UMaterialExpressionAbs>(Expression);

			Abs->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			Expression = Abs;
		}/* else if (Type->Type == "MaterialExpressionLength") {
			UMaterialExpressionLength* Length = Cast<UMaterialExpressionLength>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);

				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					Length->Input = Input;
				}
			}

			Expression = Length;
		} */else if (Type->Type == "MaterialExpressionFrac") {
			UMaterialExpressionFrac* Frac = Cast<UMaterialExpressionFrac>(Expression);

			Frac->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			Expression = Frac;
		} else if (Type->Type == "MaterialExpressionArcsine") {
			UMaterialExpressionArcsine* Arcsine = Cast<UMaterialExpressionArcsine>(Expression);

			Arcsine->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			Expression = Arcsine;
		} else if (Type->Type == "MaterialExpressionSign") {
			UMaterialExpressionSign* Sign = static_cast<UMaterialExpressionSign*>(Expression);

			Sign->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			Expression = Sign;
		} else if (Type->Type == "MaterialExpressionArcsineFast") {
			UMaterialExpressionArcsineFast* ArcsineFast = static_cast<UMaterialExpressionArcsineFast*>(Expression);

			ArcsineFast->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			Expression = ArcsineFast;
		} else if (Type->Type == "MaterialExpressionConstant") {
			UMaterialExpressionConstant* Constant = Cast<UMaterialExpressionConstant>(Expression);

			if (float R; Properties->TryGetNumberField("R", R))
				Constant->R = R;
			Expression = Constant;
		} else if (Type->Type == "MaterialExpressionAdd") {
			UMaterialExpressionAdd* Add = Cast<UMaterialExpressionAdd>(Expression);

			Add->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			Add->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");

			if (float ConstA; Properties->TryGetNumberField("ConstA", ConstA)) 
				Add->ConstA = ConstA;
			if (float ConstB; Properties->TryGetNumberField("ConstB", ConstB)) 
				Add->ConstB = ConstB;

			Expression = Add;
		} else if (Type->Type == "MaterialExpressionLinearInterpolate") {
			UMaterialExpressionLinearInterpolate* LinearInterpolate = Cast<UMaterialExpressionLinearInterpolate>(Expression);

			// Create Expression Inputs
			LinearInterpolate->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			LinearInterpolate->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");
			LinearInterpolate->Alpha = CreateExpressionInput(Properties, CreatedExpressionMap, "Alpha");

			if (float ConstA; Properties->TryGetNumberField("ConstA", ConstA)) 
				LinearInterpolate->ConstA = ConstA;
			if (float ConstB; Properties->TryGetNumberField("ConstB", ConstB)) 
				LinearInterpolate->ConstB = ConstB;
			if (float ConstAlpha; Properties->TryGetNumberField("ConstAlpha", ConstAlpha)) 
				LinearInterpolate->ConstAlpha = ConstAlpha;

			Expression = LinearInterpolate;
		} else if (Type->Type == "MaterialExpressionAbsorptionMediumMaterialOutput") {
			UMaterialExpressionAbsorptionMediumMaterialOutput* AbsorptionMediumMaterialOutput = Cast<UMaterialExpressionAbsorptionMediumMaterialOutput>(Expression);

			AbsorptionMediumMaterialOutput->TransmittanceColor = CreateExpressionInput(Properties, CreatedExpressionMap, "TransmittanceColor");
			Expression = AbsorptionMediumMaterialOutput;
		} else if (Type->Type == "MaterialExpressionComponentMask") {
			UMaterialExpressionComponentMask* ComponentMask = Cast<UMaterialExpressionComponentMask>(Expression);

			ComponentMask->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");

			if (bool R; Properties->TryGetBoolField("R", R)) 
				ComponentMask->R = R;
			if (bool G; Properties->TryGetBoolField("G", G)) 
				ComponentMask->G = G;
			if (bool B; Properties->TryGetBoolField("B", B)) 
				ComponentMask->B = B;
			if (bool A; Properties->TryGetBoolField("A", A)) 
				ComponentMask->A = A;

			Expression = ComponentMask;
		} else if (Type->Type == "MaterialExpressionConstant2Vector") {
			UMaterialExpressionConstant2Vector* Vector2 = Cast<UMaterialExpressionConstant2Vector>(Expression);

			if (float R; Properties->TryGetNumberField("R", R)) 
				Vector2->R = R;
			if (float G; Properties->TryGetNumberField("G", G)) 
				Vector2->G = G;

			Expression = Vector2;
		} else if (Type->Type == "MaterialExpressionConstant3Vector") {
			UMaterialExpressionConstant3Vector* Vector3 = Cast<UMaterialExpressionConstant3Vector>(Expression);

			if (const TSharedPtr<FJsonObject>* Constant; Properties->TryGetObjectField("Constant", Constant))
				Vector3->Constant = FMathUtilities::ObjectToLinearColor(Constant->Get());

			Expression = Vector3;
		} else if (Type->Type == "MaterialExpressionConstant4Vector") {
			UMaterialExpressionConstant4Vector* Vector4 = Cast<UMaterialExpressionConstant4Vector>(Expression);

			if (const TSharedPtr<FJsonObject>* Constant; Properties->TryGetObjectField("Constant", Constant))
				Vector4->Constant = FMathUtilities::ObjectToLinearColor(Constant->Get());

			Expression = Vector4;
		} else if (Type->Type == "MaterialExpressionConstantBiasScale") {
			UMaterialExpressionConstantBiasScale* ConstantBiasScale = Cast<UMaterialExpressionConstantBiasScale>(Expression);

			ConstantBiasScale->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			if (float Bias; Properties->TryGetNumberField("Bias", Bias)) 
				ConstantBiasScale->Bias = Bias;
			if (float Scale; Properties->TryGetNumberField("Scale", Scale)) 
				ConstantBiasScale->Scale = Scale;

			Expression = ConstantBiasScale;
		} else if (Type->Type == "MaterialExpressionOneMinus") {
			UMaterialExpressionOneMinus* OneMinus = Cast<UMaterialExpressionOneMinus>(Expression);

			OneMinus->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");

			Expression = OneMinus;
		} else if (Type->Type == "MaterialExpressionMultiply") {
			UMaterialExpressionMultiply* Multiply = Cast<UMaterialExpressionMultiply>(Expression);

			Multiply->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			Multiply->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");

			if (float ConstA; Properties->TryGetNumberField("ConstA", ConstA)) 
				Multiply->ConstA = ConstA;
			if (float ConstB; Properties->TryGetNumberField("ConstB", ConstB)) 
				Multiply->ConstB = ConstB;

			Expression = Multiply;
		}
		if (Type->Type == "MaterialExpressionRuntimeVirtualTextureSample" || Type->Type == "MaterialExpressionRuntimeVirtualTextureSampleParameter") {
			UMaterialExpressionRuntimeVirtualTextureSample* RuntimeVirtualTextureSample = Cast<UMaterialExpressionRuntimeVirtualTextureSample>(Expression);

			RuntimeVirtualTextureSample->Coordinates = CreateExpressionInput(Properties, CreatedExpressionMap, "Coordinates");
			RuntimeVirtualTextureSample->WorldPosition = CreateExpressionInput(Properties, CreatedExpressionMap, "WorldPosition");
			RuntimeVirtualTextureSample->MipValue = CreateExpressionInput(Properties, CreatedExpressionMap, "MipValue");

			if (const TSharedPtr<FJsonObject>* VirtualTexture; Properties->TryGetObjectField("VirtualTexture", VirtualTexture)) {
				LoadObject(VirtualTexture, RuntimeVirtualTextureSample->VirtualTexture);

				if (RuntimeVirtualTextureSample->VirtualTexture == nullptr) {
					FString ObjectPath;
					VirtualTexture->Get()->GetStringField("ObjectPath").Split(".", &ObjectPath, nullptr);

					AppendNotification(FText::FromString("Virtual Texture Sample Missing: " + ObjectPath), FText::FromString("Material Graph"), 2.0f, SNotificationItem::CS_Fail, true);
				}
			}
			
			if (FString MaterialType; Properties->TryGetStringField("MaterialType", MaterialType))
				RuntimeVirtualTextureSample->MaterialType = static_cast<ERuntimeVirtualTextureMaterialType>(StaticEnum<ERuntimeVirtualTextureMaterialType>()->GetValueByNameString(MaterialType));
			if (bool bSinglePhysicalSpace; Properties->TryGetBoolField("bSinglePhysicalSpace", bSinglePhysicalSpace)) RuntimeVirtualTextureSample->bSinglePhysicalSpace = bSinglePhysicalSpace;
			if (bool bAdaptive; Properties->TryGetBoolField("bAdaptive", bAdaptive)) RuntimeVirtualTextureSample->bAdaptive = bAdaptive;
			if (FString MipValueMode; Properties->TryGetStringField("MipValueMode", MipValueMode))
				RuntimeVirtualTextureSample->MipValueMode = static_cast<ERuntimeVirtualTextureMipValueMode>(StaticEnum<ERuntimeVirtualTextureMipValueMode>()->GetValueByNameString(MipValueMode));
			if (FString TextureAddressMode; Properties->TryGetStringField("TextureAddressMode", TextureAddressMode))
				RuntimeVirtualTextureSample->TextureAddressMode = static_cast<ERuntimeVirtualTextureTextureAddressMode>(StaticEnum<ERuntimeVirtualTextureTextureAddressMode>()->GetValueByNameString(TextureAddressMode));

			Expression = RuntimeVirtualTextureSample;
		}
		if (Type->Type == "MaterialExpressionRuntimeVirtualTextureSampleParameter") {
			UMaterialExpressionRuntimeVirtualTextureSampleParameter* RuntimeVirtualTextureSampleParameter = Cast<UMaterialExpressionRuntimeVirtualTextureSampleParameter>(Expression);

			if (FString ExpressionGUID; Properties->TryGetStringField("ExpressionGUID", ExpressionGUID)) 
				RuntimeVirtualTextureSampleParameter->ExpressionGUID = FGuid(ExpressionGUID);
			if (FString ParameterName; Properties->TryGetStringField("ParameterName", ParameterName)) 
				RuntimeVirtualTextureSampleParameter->ParameterName = FName(ParameterName);
			if (FString Group; Properties->TryGetStringField("Group", Group)) 
				RuntimeVirtualTextureSampleParameter->Group = FName(Group);
			if (int SortPriority; Properties->TryGetNumberField("SortPriority", SortPriority)) 
				RuntimeVirtualTextureSampleParameter->SortPriority = SortPriority;

			Expression = RuntimeVirtualTextureSampleParameter;
		}
		if (Type->Type == "MaterialExpressionVectorParameter" || Type->Type == "MaterialExpressionChannelMaskParameter") {
			UMaterialExpressionVectorParameter* VectorParameter = Cast<UMaterialExpressionVectorParameter>(Expression);

			if (const TSharedPtr<FJsonObject>* DefaultValue; Properties->TryGetObjectField("DefaultValue", DefaultValue)) 
				VectorParameter->DefaultValue = FMathUtilities::ObjectToLinearColor(DefaultValue->Get());
			if (bool bUseCustomPrimitiveData; Properties->TryGetBoolField("bUseCustomPrimitiveData", bUseCustomPrimitiveData)) 
				VectorParameter->bUseCustomPrimitiveData = bUseCustomPrimitiveData;
			if (uint8 PrimitiveDataIndex; Properties->TryGetNumberField("PrimitiveDataIndex", PrimitiveDataIndex)) 
				VectorParameter->PrimitiveDataIndex = PrimitiveDataIndex;

			if (const TSharedPtr<FJsonObject>* ChannelNames; Properties->TryGetObjectField("ChannelNames", ChannelNames)) {
				if (const TSharedPtr<FJsonObject>* R; ChannelNames->Get()->TryGetObjectField("R", R))
					VectorParameter->ChannelNames.R = FText::FromString(R->Get()->GetStringField("SourceString"));
				if (const TSharedPtr<FJsonObject>* G; ChannelNames->Get()->TryGetObjectField("G", G))
					VectorParameter->ChannelNames.G = FText::FromString(G->Get()->GetStringField("SourceString"));
				if (const TSharedPtr<FJsonObject>* B; ChannelNames->Get()->TryGetObjectField("B", B))
					VectorParameter->ChannelNames.B = FText::FromString(B->Get()->GetStringField("SourceString"));
				if (const TSharedPtr<FJsonObject>* A; ChannelNames->Get()->TryGetObjectField("A", A))
					VectorParameter->ChannelNames.A = FText::FromString(A->Get()->GetStringField("SourceString"));
			}

			Expression = VectorParameter;
		} else if (Type->Type == "MaterialExpressionMaterialFunctionCall") {
			UMaterialExpressionMaterialFunctionCall* MaterialFunctionCall = Cast<UMaterialExpressionMaterialFunctionCall>(Expression);

			if (const TSharedPtr<FJsonObject>* MaterialFunctionPtr; Properties->TryGetObjectField("MaterialFunction", MaterialFunctionPtr)) {
				LoadObject(MaterialFunctionPtr, MaterialFunctionCall->MaterialFunction);

				// Notify material function is missing
				if (MaterialFunctionCall->MaterialFunction == nullptr) {
					FString ObjectPath;
					MaterialFunctionPtr->Get()->GetStringField("ObjectPath").Split(".", &ObjectPath, nullptr);
					if (!HandleReference(ObjectPath)) AppendNotification(FText::FromString("Material Function Missing: " + ObjectPath), FText::FromString("Material Graph"), 2.0f, SNotificationItem::CS_Fail, true);
					else LoadObject(MaterialFunctionPtr, MaterialFunctionCall->MaterialFunction);
				}
			}

			if (const TArray<TSharedPtr<FJsonValue>>* FunctionInputsPtr; Properties->TryGetArrayField("FunctionInputs", FunctionInputsPtr)) {
				TArray<FFunctionExpressionInput> FunctionInputs;
				for (const TSharedPtr<FJsonValue> FunctionInputValue : *FunctionInputsPtr) {
					TSharedPtr<FJsonObject> FunctionInputObject = FunctionInputValue->AsObject();

					FunctionInputs.Add(PopulateFuncExpressionInput(FunctionInputObject, CreatedExpressionMap));
				}

				MaterialFunctionCall->FunctionInputs = FunctionInputs;
			}

			if (const TArray<TSharedPtr<FJsonValue>>* FunctionOutputsPtr; Properties->TryGetArrayField("FunctionOutputs", FunctionOutputsPtr)) {
				TArray<FFunctionExpressionOutput> FunctionOutputs;
				for (const TSharedPtr<FJsonValue> FunctionOutputValue : *FunctionOutputsPtr) {
					TSharedPtr<FJsonObject> FunctionOutputObject = FunctionOutputValue->AsObject();
					FunctionOutputs.Add(PopulateFuncExpressionOutput(FunctionOutputObject));
				}

				MaterialFunctionCall->FunctionOutputs = FunctionOutputs;
			}

			Expression = MaterialFunctionCall;
		} else if (Type->Type == "MaterialExpressionMax") {
			UMaterialExpressionMax* Max = Cast<UMaterialExpressionMax>(Expression);

			Max->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			Max->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");

			if (float ConstA; Properties->TryGetNumberField("ConstA", ConstA)) 
				Max->ConstA = ConstA;
			if (float ConstB; Properties->TryGetNumberField("ConstB", ConstB)) 
				Max->ConstB = ConstB;

			Expression = Max;
		} else if (Type->Type == "MaterialExpressionTextureCoordinate") {
			UMaterialExpressionTextureCoordinate* TextureCoordinate = Cast<UMaterialExpressionTextureCoordinate>(Expression);

			if (int CoordinateIndex; Properties->TryGetNumberField("CoordinateIndex", CoordinateIndex)) 
				TextureCoordinate->CoordinateIndex = CoordinateIndex;
			if (float UTiling; Properties->TryGetNumberField("UTiling", UTiling)) 
				TextureCoordinate->UTiling = UTiling;
			if (float VTiling; Properties->TryGetNumberField("VTiling", VTiling)) 
				TextureCoordinate->VTiling = VTiling;

			if (bool UnMirrorU; Properties->TryGetBoolField("UnMirrorU", UnMirrorU)) 
				TextureCoordinate->UnMirrorU = UnMirrorU;
			if (bool UnMirrorV; Properties->TryGetBoolField("UnMirrorV", UnMirrorV)) 
				TextureCoordinate->UnMirrorV = UnMirrorV;

			Expression = TextureCoordinate;
		} else if (Type->Type == "MaterialExpressionTime") {
			UMaterialExpressionTime* Time = Cast<UMaterialExpressionTime>(Expression);

			if (bool bIgnorePause; Properties->TryGetBoolField("bIgnorePause", bIgnorePause)) 
				Time->bIgnorePause = bIgnorePause;
			if (bool bOverride_Period; Properties->TryGetBoolField("bOverride_Period", bOverride_Period)) 
				Time->bOverride_Period = bOverride_Period;
			if (float Period; Properties->TryGetNumberField("Period", Period)) 
				Time->Period = Period;

			Expression = Time;
		} else if (Type->Type == "MaterialExpressionScalarParameter") {
			UMaterialExpressionScalarParameter* ScalarParameter = Cast<UMaterialExpressionScalarParameter>(Expression);

			if (float DefaultValue; Properties->TryGetNumberField("DefaultValue", DefaultValue)) ScalarParameter->DefaultValue = DefaultValue;
			
			if (bool bUseCustomPrimitiveData; Properties->TryGetBoolField("bUseCustomPrimitiveData", bUseCustomPrimitiveData)) 
				ScalarParameter->bUseCustomPrimitiveData = bUseCustomPrimitiveData;
			if (uint8 PrimitiveDataIndex; Properties->TryGetNumberField("PrimitiveDataIndex", PrimitiveDataIndex)) 
				ScalarParameter->PrimitiveDataIndex = PrimitiveDataIndex;
			
			if (float SliderMin; Properties->TryGetNumberField("SliderMin", SliderMin)) 
				ScalarParameter->SliderMin = SliderMin;
			if (float SliderMax; Properties->TryGetNumberField("SliderMax", SliderMax)) 
				ScalarParameter->SliderMax = SliderMax;

			Expression = ScalarParameter;
		} else if (Type->Type == "MaterialExpressionPanner") {
			UMaterialExpressionPanner* Panner = Cast<UMaterialExpressionPanner>(Expression);

			Panner->Coordinate = CreateExpressionInput(Properties, CreatedExpressionMap, "Coordinate");
			Panner->Time = CreateExpressionInput(Properties, CreatedExpressionMap, "Time");
			Panner->Speed = CreateExpressionInput(Properties, CreatedExpressionMap, "Speed");

			if (float SpeedX; Properties->TryGetNumberField("SpeedX", SpeedX)) 
				Panner->SpeedX = SpeedX;
			if (float SpeedY; Properties->TryGetNumberField("SpeedY", SpeedY)) 
				Panner->SpeedY = SpeedY;
			if (int ConstCoordinate; Properties->TryGetNumberField("ConstCoordinate", ConstCoordinate)) 
				Panner->ConstCoordinate = ConstCoordinate;
			if (bool bFractionalPart; Properties->TryGetBoolField("bFractionalPart", bFractionalPart)) 
				Panner->bFractionalPart = bFractionalPart;

			Expression = Panner;
		} else if (Type->Type == "MaterialExpressionNamedRerouteDeclaration") {
			UMaterialExpressionNamedRerouteDeclaration* NamedRerouteDeclaration = Cast<UMaterialExpressionNamedRerouteDeclaration>(Expression);

			NamedRerouteDeclaration->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");

			if (FString VarName; Properties->TryGetStringField("Name", VarName))
				NamedRerouteDeclaration->Name = FName(VarName);
			if (const TSharedPtr<FJsonObject>* NodeColor; Properties->TryGetObjectField("NodeColor", NodeColor))
				NamedRerouteDeclaration->NodeColor = FMathUtilities::ObjectToLinearColor(NodeColor->Get());
			if (FString VariableGuid; Properties->TryGetStringField("VariableGuid", VariableGuid)) 
				NamedRerouteDeclaration->VariableGuid = FGuid(VariableGuid);

			Expression = NamedRerouteDeclaration;
		} else if (Type->Type == "MaterialExpressionSceneTexture") {
			UMaterialExpressionSceneTexture* SceneTexture = static_cast<UMaterialExpressionSceneTexture*>(Expression);

			SceneTexture->Coordinates = CreateExpressionInput(Properties, CreatedExpressionMap, "Coordinates");
			if (bool bFiltered; Properties->TryGetBoolField("bFiltered", bFiltered)) 
				SceneTexture->bFiltered = bFiltered;

			if (FString SceneTextureId; Properties->TryGetStringField("SceneTextureId", SceneTextureId))
				SceneTexture->SceneTextureId = static_cast<ESceneTextureId>(StaticEnum<ESceneTextureId>()->GetValueByNameString(SceneTextureId));

			Expression = SceneTexture;
		} else if (Type->Type == "MaterialExpressionReroute") {
			UMaterialExpressionReroute* Reroute = Cast<UMaterialExpressionReroute>(Expression);

			Reroute->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			Expression = Reroute;
		} else if (Type->Type == "MaterialExpressionDDX") {
			UMaterialExpressionDDX* ExpressionDDX = Cast<UMaterialExpressionDDX>(Expression);

			ExpressionDDX->Value = CreateExpressionInput(Properties, CreatedExpressionMap, "Value");
			Expression = ExpressionDDX;
		} else if (Type->Type == "MaterialExpressionDDY") {
			UMaterialExpressionDDY* ExpressionDDY = Cast<UMaterialExpressionDDY>(Expression);

			ExpressionDDY->Value = CreateExpressionInput(Properties, CreatedExpressionMap, "Value");
			Expression = ExpressionDDY;
		} else if (Type->Type == "MaterialExpressionSubtract") {
			UMaterialExpressionSubtract* Subtract = Cast<UMaterialExpressionSubtract>(Expression);

			Subtract->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			Subtract->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");

			if (float ConstA; Properties->TryGetNumberField("ConstA", ConstA)) Subtract->ConstA = ConstA;
			if (float ConstB; Properties->TryGetNumberField("ConstB", ConstB)) Subtract->ConstB = ConstB;

			Expression = Subtract;
		} else if (Type->Type == "MaterialExpressionSaturate") {
			UMaterialExpressionSaturate* Saturate = Cast<UMaterialExpressionSaturate>(Expression);

			Saturate->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			Expression = Saturate;
		} else if (Type->Type == "MaterialExpressionRotator") {
			UMaterialExpressionRotator* Rotator = static_cast<UMaterialExpressionRotator*>(Expression);

			Rotator->Coordinate = CreateExpressionInput(Properties, CreatedExpressionMap, "Coordinate");
			Rotator->Time = CreateExpressionInput(Properties, CreatedExpressionMap, "Time");

			if (float CenterX; Properties->TryGetNumberField("CenterX", CenterX)) 
				Rotator->CenterX = CenterX;
			if (float CenterY; Properties->TryGetNumberField("CenterY", CenterY)) 
				Rotator->CenterY = CenterY;
			if (float Speed; Properties->TryGetNumberField("Speed", Speed)) 
				Rotator->Speed = Speed;
			if (int ConstCoordinate; Properties->TryGetNumberField("ConstCoordinate", ConstCoordinate)) 
				Rotator->ConstCoordinate = ConstCoordinate;

			Expression = Rotator;
		} else if (Type->Type == "MaterialExpressionMin") {
			UMaterialExpressionMin* Min = Cast<UMaterialExpressionMin>(Expression);

			Min->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			Min->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");
			
			if (float ConstA; Properties->TryGetNumberField("ConstA", ConstA)) 
				Min->ConstA = ConstA;
			if (float ConstB; Properties->TryGetNumberField("ConstB", ConstB)) 
				Min->ConstB = ConstB;

			Expression = Min;
		} else if (Type->Type == "MaterialExpressionNaniteReplace") {
			UMaterialExpressionNaniteReplace* NaniteReplace = static_cast<UMaterialExpressionNaniteReplace*>(Expression);

			NaniteReplace->Default = CreateExpressionInput(Properties, CreatedExpressionMap, "Default");
			NaniteReplace->Nanite = CreateExpressionInput(Properties, CreatedExpressionMap, "Nanite");
			Expression = NaniteReplace;
		} else if (Type->Type == "MaterialExpressionNamedRerouteUsage") {
			UMaterialExpressionNamedRerouteUsage* NamedRerouteUsage = Cast<UMaterialExpressionNamedRerouteUsage>(Expression);

			if (const TSharedPtr<FJsonObject>* DeclarationPtr; Properties->TryGetObjectField("Declaration", DeclarationPtr)) {
				LoadObject(DeclarationPtr, NamedRerouteUsage->Declaration);
			}

			if (FString DeclarationGuid; Properties->TryGetStringField("DeclarationGuid", DeclarationGuid)) 
				NamedRerouteUsage->DeclarationGuid = FGuid(DeclarationGuid);

			Expression = NamedRerouteUsage;
		} else if (Type->Type == "MaterialExpressionCollectionParameter") {
			UMaterialExpressionCollectionParameter* CollectionParameter = Cast<UMaterialExpressionCollectionParameter>(Expression);

			const TSharedPtr<FJsonObject>* Collection = nullptr;
			if (Properties->TryGetObjectField("Collection", Collection) && Collection != nullptr) {
				LoadObject(Collection, CollectionParameter->Collection);

				if (CollectionParameter->Collection == nullptr) {
					FString ObjectPath;
					Collection->Get()->GetStringField("ObjectPath").Split(".", &ObjectPath, nullptr);

					if (!HandleReference(ObjectPath)) AppendNotification(FText::FromString("Material Collection Missing: " + ObjectPath), FText::FromString("Material Graph"), 2.0f, SNotificationItem::CS_Fail, true);
					else LoadObject(Collection, CollectionParameter->Collection);
				}
			}

			if (FString ParameterName; Properties->TryGetStringField("ParameterName", ParameterName)) 
				CollectionParameter->ParameterName = FName(ParameterName);
			if (FString ParameterId; Properties->TryGetStringField("ParameterId", ParameterId)) 
				CollectionParameter->ParameterId = FGuid(ParameterId);

			Expression = CollectionParameter;
		} else if (Type->Type == "MaterialExpressionLandscapeVisibilityMask") {
			UMaterialExpressionLandscapeVisibilityMask* LandscapeVisibilityMask = Cast<UMaterialExpressionLandscapeVisibilityMask>(Expression);
			
			if (FString ParameterName; Properties->TryGetStringField("ParameterName", ParameterName)) 
				LandscapeVisibilityMask->ParameterName = FName(ParameterName);

			Expression = LandscapeVisibilityMask;
		} else if (Type->Type == "MaterialExpressionSine") {
			UMaterialExpressionSine* Sine = Cast<UMaterialExpressionSine>(Expression);

			Sine->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			if (float Period; Properties->TryGetNumberField("Period", Period)) 
				Sine->Period = Period;

			Expression = Sine;
		} else if (Type->Type == "MaterialExpressionSmoothStep") {
			UMaterialExpressionSmoothStep* SmoothStep = Cast<UMaterialExpressionSmoothStep>(Expression);

			SmoothStep->Min = CreateExpressionInput(Properties, CreatedExpressionMap, "Min");
			if (SmoothStep->Min.Expression == nullptr)
				SmoothStep->Min = CreateExpressionInput(Properties, CreatedExpressionMap, "min");

			SmoothStep->Max = CreateExpressionInput(Properties, CreatedExpressionMap, "Max");
			if (SmoothStep->Max.Expression == nullptr)
				SmoothStep->Max = CreateExpressionInput(Properties, CreatedExpressionMap, "max");

			SmoothStep->Value = CreateExpressionInput(Properties, CreatedExpressionMap, "Value");

			if (float ConstMin; Properties->TryGetNumberField("ConstMin", ConstMin)) 
				SmoothStep->ConstMin = ConstMin;
			if (float ConstMax; Properties->TryGetNumberField("ConstMax", ConstMax)) 
				SmoothStep->ConstMax = ConstMax;
			if (float ConstValue; Properties->TryGetNumberField("ConstValue", ConstValue)) 
				SmoothStep->ConstValue = ConstValue;

			Expression = SmoothStep;
		} else if (Type->Type == "MaterialExpressionAppendVector") {
			UMaterialExpressionAppendVector* AppendVector = Cast<UMaterialExpressionAppendVector>(Expression);

			AppendVector->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			AppendVector->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");
			Expression = AppendVector;
		} else if (Type->Type == "MaterialExpressionDivide") {
			UMaterialExpressionDivide* Divide = Cast<UMaterialExpressionDivide>(Expression);

			Divide->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			Divide->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");

			if (float ConstA; Properties->TryGetNumberField("ConstA", ConstA)) 
				Divide->ConstA = ConstA;
			if (float ConstB; Properties->TryGetNumberField("ConstB", ConstB)) 
				Divide->ConstB = ConstB;

			Expression = Divide;
		} else if (Type->Type == "MaterialExpressionDistance") {
			UMaterialExpressionDistance* Distance = Cast<UMaterialExpressionDistance>(Expression);

			Distance->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			Distance->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");
			Expression = Distance;
		} else if (Type->Type == "MaterialExpressionVectorNoise") {
			UMaterialExpressionVectorNoise* VectorNoise = Cast<UMaterialExpressionVectorNoise>(Expression);

			VectorNoise->Position = CreateExpressionInput(Properties, CreatedExpressionMap, "Position");
			if (FString NoiseFunction; Properties->TryGetStringField("NoiseFunction", NoiseFunction)) 
				VectorNoise->NoiseFunction = static_cast<EVectorNoiseFunction>(StaticEnum<EVectorNoiseFunction>()->GetValueByNameString(NoiseFunction));
			if (int Quality; Properties->TryGetNumberField("Quality", Quality)) 
				VectorNoise->Quality = Quality;
			if (bool bTiling; Properties->TryGetBoolField("bTiling", bTiling)) 
				VectorNoise->bTiling = bTiling;
			if (int TileSize; Properties->TryGetNumberField("TileSize", TileSize)) 
				VectorNoise->TileSize = TileSize;

			Expression = VectorNoise;
		} else if (Type->Type == "MaterialExpressionCrossProduct") {
			UMaterialExpressionCrossProduct* CrossProduct = Cast<UMaterialExpressionCrossProduct>(Expression);

			CrossProduct->A = CreateExpressionInput(Properties, CreatedExpressionMap, "A");
			CrossProduct->B = CreateExpressionInput(Properties, CreatedExpressionMap, "B");

			Expression = CrossProduct;
		} else if (Type->Type == "MaterialExpressionTransform") {
			UMaterialExpressionTransform* Transform = Cast<UMaterialExpressionTransform>(Expression);

			if (FString TransformSourceType; Properties->TryGetStringField("TransformSourceType", TransformSourceType))
				Transform->TransformSourceType = static_cast<EMaterialVectorCoordTransformSource>(StaticEnum<EMaterialVectorCoordTransformSource>()->GetValueByNameString(TransformSourceType));
			if (FString TransformType; Properties->TryGetStringField("TransformType", TransformType))
				Transform->TransformType = static_cast<EMaterialVectorCoordTransform>(StaticEnum<EMaterialVectorCoordTransform>()->GetValueByNameString(TransformType));

			Transform->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			Expression = Transform;
		} else if (Type->Type == "MaterialExpressionVertexInterpolator") {
			UMaterialExpressionVertexInterpolator* VertexInterpolator = Cast<UMaterialExpressionVertexInterpolator>(Expression);

			VertexInterpolator->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			Expression = VertexInterpolator;
		} else if (Type->Type == "MaterialExpressionDepthFade") {
			UMaterialExpressionDepthFade* DepthFade = static_cast<UMaterialExpressionDepthFade*>(Expression);

			DepthFade->InOpacity = CreateExpressionInput(Properties, CreatedExpressionMap, "InOpacity");
			DepthFade->FadeDistance = CreateExpressionInput(Properties, CreatedExpressionMap, "FadeDistance");
			Expression = DepthFade;
		} else if (Type->Type == "MaterialExpressionSceneDepth") {
			UMaterialExpressionSceneDepth* SceneDepth = static_cast<UMaterialExpressionSceneDepth*>(Expression);

			SceneDepth->Input = CreateExpressionInput(Properties, CreatedExpressionMap, "Input");
			if (FString InputMode; Properties->TryGetStringField("InputMode", InputMode))
				SceneDepth->InputMode = static_cast<EMaterialSceneAttributeInputMode::Type>(StaticEnum<EMaterialSceneAttributeInputMode::Type>()->GetValueByNameString(InputMode));

			if (const TSharedPtr<FJsonObject>* ConstInput; Properties->TryGetObjectField("ConstInput", ConstInput))
				SceneDepth->ConstInput = FVector2D(ConstInput->Get()->GetNumberField("X"), ConstInput->Get()->GetNumberField("Y"));

			Expression = SceneDepth;
		} else if (Type->Type == "MaterialExpressionDeriveNormalZ") {
			UMaterialExpressionDeriveNormalZ* DeriveNormalZ = static_cast<UMaterialExpressionDeriveNormalZ*>(Expression);

			DeriveNormalZ->InXY = CreateExpressionInput(Properties, CreatedExpressionMap, "InXY");
			Expression = DeriveNormalZ;
		} else if (Type->Type == "MaterialExpressionQualitySwitch") {
			// TODO: Add different qualities
			UMaterialExpressionQualitySwitch* QualitySwitch = Cast<UMaterialExpressionQualitySwitch>(Expression);

			QualitySwitch->Default = CreateExpressionInput(Properties, CreatedExpressionMap, "Default");
			const TArray<TSharedPtr<FJsonValue>>* InputsPtr;
			if (Properties->TryGetArrayField("Inputs", InputsPtr)) {
				int i = 0;
				for (const TSharedPtr<FJsonValue> InputValue : *InputsPtr) {
					FJsonObject* InputObject = InputValue->AsObject().Get();
					FName InputExpressionName = GetExpressionName(InputObject);
					if (CreatedExpressionMap.Contains(InputExpressionName)) {
						FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
						QualitySwitch->Inputs[i] = Input;
					}
					i++;
				}
			}

			Expression = QualitySwitch;
		} else if (Type->Type == "MaterialExpressionReflectionCapturePassSwitch") {
			UMaterialExpressionReflectionCapturePassSwitch* ReflectionCapturePassSwitch = static_cast<UMaterialExpressionReflectionCapturePassSwitch*>(Expression);

			ReflectionCapturePassSwitch->Default = CreateExpressionInput(Properties, CreatedExpressionMap, "Default");
			ReflectionCapturePassSwitch->Reflection = CreateExpressionInput(Properties, CreatedExpressionMap, "Reflection");
			Expression = ReflectionCapturePassSwitch;
		} else if (Type->Type == "MaterialExpressionRotateAboutAxis") {
			UMaterialExpressionRotateAboutAxis* RotateAboutAxis = Cast<UMaterialExpressionRotateAboutAxis>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("NormalizedRotationAxis", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					RotateAboutAxis->NormalizedRotationAxis = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("RotationAngle", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					RotateAboutAxis->RotationAngle = B;
				}
			}

			const TSharedPtr<FJsonObject>* CPtr = nullptr;
			if (Properties->TryGetObjectField("PivotPoint", CPtr) && CPtr != nullptr) {
				FJsonObject* CObject = CPtr->Get();
				FName CExpressionName = GetExpressionName(CObject);
				if (CreatedExpressionMap.Contains(CExpressionName)) {
					FExpressionInput C = PopulateExpressionInput(CObject, *CreatedExpressionMap.Find(CExpressionName));
					RotateAboutAxis->PivotPoint = C;
				}
			}

			const TSharedPtr<FJsonObject>* DPtr = nullptr;
			if (Properties->TryGetObjectField("Position", DPtr) && DPtr != nullptr) {
				FJsonObject* DObject = DPtr->Get();
				FName DExpressionName = GetExpressionName(DObject);
				if (CreatedExpressionMap.Contains(DExpressionName)) {
					FExpressionInput D = PopulateExpressionInput(DObject, *CreatedExpressionMap.Find(DExpressionName));
					RotateAboutAxis->Position = D;
				}
			}

			Expression = RotateAboutAxis;
		} else if (Type->Type == "MaterialExpressionNoise") {
			UMaterialExpressionNoise* Noise = Cast<UMaterialExpressionNoise>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Position", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					Noise->Position = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("FilterWidth", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					Noise->FilterWidth = B;
				}
			}

			float Scale;
			if (Properties->TryGetNumberField("Scale", Scale)) Noise->Scale = Scale;
			int Quality;
			if (Properties->TryGetNumberField("Quality", Quality)) Noise->Quality = Quality;

			FString NoiseFunction;
			if (Properties->TryGetStringField("NoiseFunction", NoiseFunction)) {
				Noise->NoiseFunction = static_cast<ENoiseFunction>(StaticEnum<ENoiseFunction>()->GetValueByNameString(NoiseFunction));
			}

			bool bTurbulence;
			if (Properties->TryGetBoolField("bTurbulence", bTurbulence)) Noise->bTurbulence = bTurbulence;
			int Levels;
			if (Properties->TryGetNumberField("Levels", Levels)) Noise->Levels = Levels;
			float OutputMin;
			if (Properties->TryGetNumberField("OutputMin", OutputMin)) Noise->OutputMin = OutputMin;
			float OutputMax;
			if (Properties->TryGetNumberField("OutputMax", OutputMax)) Noise->OutputMax = OutputMax;
			float LevelScale;
			if (Properties->TryGetNumberField("LevelScale", LevelScale)) Noise->LevelScale = LevelScale;
			bool bTiling;
			if (Properties->TryGetBoolField("bTiling", bTiling)) Noise->bTiling = bTiling;
			bool RepeatSize;
			if (Properties->TryGetBoolField("RepeatSize", RepeatSize)) Noise->RepeatSize = RepeatSize;

			Expression = Noise;
		} else if (Type->Type == "MaterialExpressionBumpOffset") {
			UMaterialExpressionBumpOffset* BumpOffset = Cast<UMaterialExpressionBumpOffset>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Coordinate", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					BumpOffset->Coordinate = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("Height", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					BumpOffset->Height = B;
				}
			}

			const TSharedPtr<FJsonObject>* CPtr = nullptr;
			if (Properties->TryGetObjectField("HeightRatioInput", CPtr) && CPtr != nullptr) {
				FJsonObject* CObject = CPtr->Get();
				FName CExpressionName = GetExpressionName(CObject);
				if (CreatedExpressionMap.Contains(CExpressionName)) {
					FExpressionInput C = PopulateExpressionInput(CObject, *CreatedExpressionMap.Find(CExpressionName));
					BumpOffset->HeightRatioInput = C;
				}
			}

			Expression = BumpOffset;
		} else if (Type->Type == "MaterialExpressionSquareRoot") {
			UMaterialExpressionSquareRoot* SquareRoot = Cast<UMaterialExpressionSquareRoot>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Input", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					SquareRoot->Input = A;
				}
			}

			Expression = SquareRoot;
		} else if (Type->Type == "MaterialExpressionFresnel") {
			UMaterialExpressionFresnel* Frensel = Cast<UMaterialExpressionFresnel>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("ExponentIn", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					Frensel->ExponentIn = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("BaseReflectFractionIn", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					Frensel->BaseReflectFractionIn = B;
				}
			}

			const TSharedPtr<FJsonObject>* CPtr = nullptr;
			if (Properties->TryGetObjectField("Normal", CPtr) && CPtr != nullptr) {
				FJsonObject* CObject = CPtr->Get();
				FName CExpressionName = GetExpressionName(CObject);
				if (CreatedExpressionMap.Contains(CExpressionName)) {
					FExpressionInput C = PopulateExpressionInput(CObject, *CreatedExpressionMap.Find(CExpressionName));
					Frensel->Normal = C;
				}
			}

			float Exponent;
			if (Properties->TryGetNumberField("Exponent", Exponent)) Frensel->Exponent = Exponent;
			float BaseReflectFraction;
			if (Properties->TryGetNumberField("BaseReflectFraction", BaseReflectFraction)) Frensel->BaseReflectFraction = BaseReflectFraction;

			Expression = Frensel;
		} else if (Type->Type == "MaterialExpressionRayTracingQualitySwitch") {
			UMaterialExpressionRayTracingQualitySwitch* RayTracingQualitySwitch = static_cast<UMaterialExpressionRayTracingQualitySwitch*>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Normal", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					RayTracingQualitySwitch->Normal = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("RayTraced", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					RayTracingQualitySwitch->RayTraced = B;
				}
			}

			Expression = RayTracingQualitySwitch;
		} else if (Type->Type == "MaterialExpressionMaterialProxyReplace") {
			UMaterialExpressionMaterialProxyReplace* MaterialProxyReplace = static_cast<UMaterialExpressionMaterialProxyReplace*>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Realtime", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					MaterialProxyReplace->Realtime = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("MaterialProxy", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					MaterialProxyReplace->MaterialProxy = B;
				}
			}

			Expression = MaterialProxyReplace;
		} else if (Type->Type == "MaterialExpressionShaderStageSwitch") {
			UMaterialExpressionShaderStageSwitch* ShaderStageSwitch = Cast<UMaterialExpressionShaderStageSwitch>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("PixelShader", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					ShaderStageSwitch->PixelShader = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("VertexShader", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					ShaderStageSwitch->VertexShader = B;
				}
			}

			Expression = ShaderStageSwitch;
		} else if (Type->Type == "MaterialExpressionVirtualTextureFeatureSwitch") {
			UMaterialExpressionVirtualTextureFeatureSwitch* VirtualTextureFeatureSwitch = static_cast<UMaterialExpressionVirtualTextureFeatureSwitch*>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("No", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					VirtualTextureFeatureSwitch->No = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("Yes", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					VirtualTextureFeatureSwitch->Yes = B;
				}
			}

			Expression = VirtualTextureFeatureSwitch;
		} else if (Type->Type == "MaterialExpressionPreviousFrameSwitch") {
			UMaterialExpressionPreviousFrameSwitch* PreviousFrameSwitch = static_cast<UMaterialExpressionPreviousFrameSwitch*>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("CurrentFrame", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					PreviousFrameSwitch->CurrentFrame = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("PreviousFrame", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					PreviousFrameSwitch->PreviousFrame = B;
				}
			}

			Expression = PreviousFrameSwitch;
		} else if (Type->Type == "MaterialExpressionShadowReplace") {
			UMaterialExpressionShadowReplace* ShadowReplace = static_cast<UMaterialExpressionShadowReplace*>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Default", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					ShadowReplace->Default = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("Shadow", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					ShadowReplace->Shadow = B;
				}
			}

			Expression = ShadowReplace;
		} else if (Type->Type == "MaterialExpressionDistanceFieldsRenderingSwitch") {
			UMaterialExpressionDistanceFieldsRenderingSwitch* DistanceFieldsRenderingSwitch = static_cast<UMaterialExpressionDistanceFieldsRenderingSwitch*>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("No", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					DistanceFieldsRenderingSwitch->No = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("Yes", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					DistanceFieldsRenderingSwitch->Yes = B;
				}
			}

			Expression = DistanceFieldsRenderingSwitch;
		} else if (Type->Type == "MaterialExpressionMaterialAttributeLayers") {
			UMaterialExpressionMaterialAttributeLayers* MaterialAttributeLayers = static_cast<UMaterialExpressionMaterialAttributeLayers*>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Input", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput ExpressionInput = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					FMaterialAttributesInput* A = reinterpret_cast<FMaterialAttributesInput*>(&ExpressionInput);
					MaterialAttributeLayers->Input = *A;
				}
			}

			// TODO
			if (const TSharedPtr<FJsonObject>* DefaultLayersPtr; Properties->TryGetObjectField("DefaultLayers", DefaultLayersPtr) && DefaultLayersPtr != nullptr) {
				FJsonObject* DefaultLayersPtrObject = DefaultLayersPtr->Get();

				int i = 0;
				for (const TSharedPtr<FJsonValue> Layer : DefaultLayersPtrObject->GetArrayField("Layers")) {
					TSharedPtr<FJsonObject> LayerObj = Layer->AsObject();
					MaterialAttributeLayers->DefaultLayers.Layers.Add(nullptr);
					i++;
				}
			}

			Expression = MaterialAttributeLayers;
		} else if (Type->Type == "MaterialExpressionSkyLightEnvMapSample") {
			UMaterialExpressionSkyLightEnvMapSample* SkyLightEnvMapSample = static_cast<UMaterialExpressionSkyLightEnvMapSample*>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Direction", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					SkyLightEnvMapSample->Direction = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("Roughness", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					SkyLightEnvMapSample->Roughness = B;
				}
			}

			Expression = SkyLightEnvMapSample;
		} else if (Type->Type == "MaterialExpressionReflectionVectorWS") {
			UMaterialExpressionReflectionVectorWS* ReflectionVectorWS = Cast<UMaterialExpressionReflectionVectorWS>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("CustomWorldNormal", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					ReflectionVectorWS->CustomWorldNormal = A;
				}
			}

			bool bNormalizeCustomWorldNormal;
			if (Properties->TryGetBoolField("bNormalizeCustomWorldNormal", bNormalizeCustomWorldNormal)) ReflectionVectorWS->bNormalizeCustomWorldNormal = bNormalizeCustomWorldNormal;

			Expression = ReflectionVectorWS;
		} else if (Type->Type == "MaterialExpressionGetMaterialAttributes") {
			UMaterialExpressionGetMaterialAttributes* GetMaterialAttributes = Cast<UMaterialExpressionGetMaterialAttributes>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("MaterialAttributes", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput ExpressionInput = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					FMaterialAttributesInput* A = reinterpret_cast<FMaterialAttributesInput*>(&ExpressionInput);
					GetMaterialAttributes->MaterialAttributes = *A;
				}
			}

			const TArray<TSharedPtr<FJsonValue>>* AttributeGetTypesPtr;
			if (Properties->TryGetArrayField("AttributeGetTypes", AttributeGetTypesPtr)) {
				int i = 0;
				for (const TSharedPtr<FJsonValue> AttributeGetTypeValue : *AttributeGetTypesPtr) {
					FString AttributeGetType = AttributeGetTypeValue->AsString();
					GetMaterialAttributes->AttributeGetTypes.Add(FGuid(AttributeGetType));
					i++;
				}
			}

			const TArray<TSharedPtr<FJsonValue>>* OutputsPtr;
			if (Properties->TryGetArrayField("Outputs", OutputsPtr)) {
				TArray<FExpressionOutput> Outputs;
				for (const TSharedPtr<FJsonValue> FunctionOutputValue : *OutputsPtr) {
					Outputs.Add(PopulateExpressionOutput(FunctionOutputValue->AsObject().Get()));
				}

				GetMaterialAttributes->Outputs = Outputs;
			}

			Expression = GetMaterialAttributes;
		} else if (Type->Type == "MaterialExpressionBreakMaterialAttributes") {
			UMaterialExpressionBreakMaterialAttributes* BreakMaterialAttributes = Cast<UMaterialExpressionBreakMaterialAttributes>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("MaterialAttributes", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput ExpressionInput = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					FMaterialAttributesInput* A = reinterpret_cast<FMaterialAttributesInput*>(&ExpressionInput);
					BreakMaterialAttributes->MaterialAttributes = *A;
				}
			}

			Expression = BreakMaterialAttributes;
		} else if (Type->Type == "MaterialExpressionBlendMaterialAttributes") {
			UMaterialExpressionBlendMaterialAttributes* BlendMaterialAttributes = Cast<UMaterialExpressionBlendMaterialAttributes>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("A", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput ExpressionInput = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					FMaterialAttributesInput* A = reinterpret_cast<FMaterialAttributesInput*>(&ExpressionInput);
					BlendMaterialAttributes->A = *A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("B", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput ExpressionInput = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					FMaterialAttributesInput* B = reinterpret_cast<FMaterialAttributesInput*>(&ExpressionInput);
					BlendMaterialAttributes->B = *B;
				}
			}

			const TSharedPtr<FJsonObject>* CPtr = nullptr;
			if (Properties->TryGetObjectField("Alpha", CPtr) && CPtr != nullptr) {
				FJsonObject* CObject = CPtr->Get();
				FName CExpressionName = GetExpressionName(CObject);
				if (CreatedExpressionMap.Contains(CExpressionName)) {
					FExpressionInput C = PopulateExpressionInput(CObject, *CreatedExpressionMap.Find(CExpressionName));
					BlendMaterialAttributes->Alpha = C;
				}
			}

			FString PixelAttributeBlendType;
			if (Properties->TryGetStringField("PixelAttributeBlendType", PixelAttributeBlendType)) {
				BlendMaterialAttributes->PixelAttributeBlendType = static_cast<EMaterialAttributeBlend::Type>(StaticEnum<EMaterialAttributeBlend::Type>()->GetValueByNameString(PixelAttributeBlendType));
			}

			FString VertexAttributeBlendType;
			if (Properties->TryGetStringField("VertexAttributeBlendType", VertexAttributeBlendType)) {
				BlendMaterialAttributes->VertexAttributeBlendType = static_cast<EMaterialAttributeBlend::Type>(StaticEnum<EMaterialAttributeBlend::Type>()->GetValueByNameString(VertexAttributeBlendType));
			}

			Expression = BlendMaterialAttributes;
		} else if (Type->Type == "MaterialExpressionMakeMaterialAttributes") {
			UMaterialExpressionMakeMaterialAttributes* MakeMaterialAttributes = Cast<UMaterialExpressionMakeMaterialAttributes>(Expression);

			const TSharedPtr<FJsonObject>* BaseColorPtr = nullptr;
			if (Properties->TryGetObjectField("BaseColor", BaseColorPtr) && BaseColorPtr != nullptr) {
				FJsonObject* BaseColorObject = BaseColorPtr->Get();
				FName BaseColorExpressionName = GetExpressionName(BaseColorObject);
				if (CreatedExpressionMap.Contains(BaseColorExpressionName)) {
					FExpressionInput BaseColor = PopulateExpressionInput(BaseColorObject, *CreatedExpressionMap.Find(BaseColorExpressionName));
					MakeMaterialAttributes->BaseColor = BaseColor;
				}
			}

			const TSharedPtr<FJsonObject>* MetallicPtr = nullptr;
			if (Properties->TryGetObjectField("Metallic", MetallicPtr) && MetallicPtr != nullptr) {
				FJsonObject* MetallicObject = MetallicPtr->Get();
				FName MetallicExpressionName = GetExpressionName(MetallicObject);
				if (CreatedExpressionMap.Contains(MetallicExpressionName)) {
					FExpressionInput Metallic = PopulateExpressionInput(MetallicObject, *CreatedExpressionMap.Find(MetallicExpressionName));
					MakeMaterialAttributes->Metallic = Metallic;
				}
			}

			const TSharedPtr<FJsonObject>* SpecularPtr = nullptr;
			if (Properties->TryGetObjectField("Specular", SpecularPtr) && SpecularPtr != nullptr) {
				FJsonObject* SpecularObject = SpecularPtr->Get();
				FName SpecularExpressionName = GetExpressionName(SpecularObject);
				if (CreatedExpressionMap.Contains(SpecularExpressionName)) {
					FExpressionInput Specular = PopulateExpressionInput(SpecularObject, *CreatedExpressionMap.Find(SpecularExpressionName));
					MakeMaterialAttributes->Specular = Specular;
				}
			}

			const TSharedPtr<FJsonObject>* RoughnessPtr = nullptr;
			if (Properties->TryGetObjectField("Roughness", RoughnessPtr) && RoughnessPtr != nullptr) {
				FJsonObject* RoughnessObject = RoughnessPtr->Get();
				FName RoughnessExpressionName = GetExpressionName(RoughnessObject);
				if (CreatedExpressionMap.Contains(RoughnessExpressionName)) {
					FExpressionInput Roughness = PopulateExpressionInput(RoughnessObject, *CreatedExpressionMap.Find(RoughnessExpressionName));
					MakeMaterialAttributes->Roughness = Roughness;
				}
			}

			const TSharedPtr<FJsonObject>* AnisotropyPtr = nullptr;
			if (Properties->TryGetObjectField("Anisotropy", AnisotropyPtr) && AnisotropyPtr != nullptr) {
				FJsonObject* AnisotropyObject = AnisotropyPtr->Get();
				FName AnisotropyExpressionName = GetExpressionName(AnisotropyObject);
				if (CreatedExpressionMap.Contains(AnisotropyExpressionName)) {
					FExpressionInput Anisotropy = PopulateExpressionInput(AnisotropyObject, *CreatedExpressionMap.Find(AnisotropyExpressionName));
					MakeMaterialAttributes->Anisotropy = Anisotropy;
				}
			}

			const TSharedPtr<FJsonObject>* EmissiveColorPtr = nullptr;
			if (Properties->TryGetObjectField("EmissiveColor", EmissiveColorPtr) && EmissiveColorPtr != nullptr) {
				FJsonObject* EmissiveColorObject = EmissiveColorPtr->Get();
				FName EmissiveColorExpressionName = GetExpressionName(EmissiveColorObject);
				if (CreatedExpressionMap.Contains(EmissiveColorExpressionName)) {
					FExpressionInput EmissiveColor = PopulateExpressionInput(EmissiveColorObject, *CreatedExpressionMap.Find(EmissiveColorExpressionName));
					MakeMaterialAttributes->EmissiveColor = EmissiveColor;
				}
			}

			const TSharedPtr<FJsonObject>* OpacityPtr = nullptr;
			if (Properties->TryGetObjectField("Opacity", OpacityPtr) && OpacityPtr != nullptr) {
				FJsonObject* OpacityObject = OpacityPtr->Get();
				FName OpacityExpressionName = GetExpressionName(OpacityObject);
				if (CreatedExpressionMap.Contains(OpacityExpressionName)) {
					FExpressionInput Opacity = PopulateExpressionInput(OpacityObject, *CreatedExpressionMap.Find(OpacityExpressionName));
					MakeMaterialAttributes->Opacity = Opacity;
				}
			}

			const TSharedPtr<FJsonObject>* OpacityMaskPtr = nullptr;
			if (Properties->TryGetObjectField("OpacityMask", OpacityMaskPtr) && OpacityMaskPtr != nullptr) {
				FJsonObject* OpacityMaskObject = OpacityMaskPtr->Get();
				FName OpacityMaskExpressionName = GetExpressionName(OpacityMaskObject);
				if (CreatedExpressionMap.Contains(OpacityMaskExpressionName)) {
					FExpressionInput OpacityMask = PopulateExpressionInput(OpacityMaskObject, *CreatedExpressionMap.Find(OpacityMaskExpressionName));
					MakeMaterialAttributes->OpacityMask = OpacityMask;
				}
			}

			const TSharedPtr<FJsonObject>* NormalPtr = nullptr;
			if (Properties->TryGetObjectField("Normal", NormalPtr) && NormalPtr != nullptr) {
				FJsonObject* NormalObject = NormalPtr->Get();
				FName NormalExpressionName = GetExpressionName(NormalObject);
				if (CreatedExpressionMap.Contains(NormalExpressionName)) {
					FExpressionInput Normal = PopulateExpressionInput(NormalObject, *CreatedExpressionMap.Find(NormalExpressionName));
					MakeMaterialAttributes->Normal = Normal;
				}
			}

			const TSharedPtr<FJsonObject>* TangentPtr = nullptr;
			if (Properties->TryGetObjectField("Tangent", TangentPtr) && TangentPtr != nullptr) {
				FJsonObject* TangentObject = TangentPtr->Get();
				FName TangentExpressionName = GetExpressionName(TangentObject);
				if (CreatedExpressionMap.Contains(TangentExpressionName)) {
					FExpressionInput Tangent = PopulateExpressionInput(TangentObject, *CreatedExpressionMap.Find(TangentExpressionName));
					MakeMaterialAttributes->Tangent = Tangent;
				}
			}

			const TSharedPtr<FJsonObject>* WorldPositionOffsetPtr = nullptr;
			if (Properties->TryGetObjectField("WorldPositionOffset", WorldPositionOffsetPtr) && WorldPositionOffsetPtr != nullptr) {
				FJsonObject* WorldPositionOffsetObject = WorldPositionOffsetPtr->Get();
				FName WorldPositionOffsetExpressionName = GetExpressionName(WorldPositionOffsetObject);
				if (CreatedExpressionMap.Contains(WorldPositionOffsetExpressionName)) {
					FExpressionInput WorldPositionOffset = PopulateExpressionInput(WorldPositionOffsetObject, *CreatedExpressionMap.Find(WorldPositionOffsetExpressionName));
					MakeMaterialAttributes->WorldPositionOffset = WorldPositionOffset;
				}
			}

			const TSharedPtr<FJsonObject>* SubsurfaceColorPtr = nullptr;
			if (Properties->TryGetObjectField("SubsurfaceColor", SubsurfaceColorPtr) && SubsurfaceColorPtr != nullptr) {
				FJsonObject* SubsurfaceColorObject = SubsurfaceColorPtr->Get();
				FName SubsurfaceColorExpressionName = GetExpressionName(SubsurfaceColorObject);
				if (CreatedExpressionMap.Contains(SubsurfaceColorExpressionName)) {
					FExpressionInput SubsurfaceColor = PopulateExpressionInput(SubsurfaceColorObject, *CreatedExpressionMap.Find(SubsurfaceColorExpressionName));
					MakeMaterialAttributes->SubsurfaceColor = SubsurfaceColor;
				}
			}

			const TSharedPtr<FJsonObject>* ClearCoatPtr = nullptr;
			if (Properties->TryGetObjectField("ClearCoat", ClearCoatPtr) && ClearCoatPtr != nullptr) {
				FJsonObject* ClearCoatObject = ClearCoatPtr->Get();
				FName ClearCoatExpressionName = GetExpressionName(ClearCoatObject);
				if (CreatedExpressionMap.Contains(ClearCoatExpressionName)) {
					FExpressionInput ClearCoat = PopulateExpressionInput(ClearCoatObject, *CreatedExpressionMap.Find(ClearCoatExpressionName));
					MakeMaterialAttributes->ClearCoat = ClearCoat;
				}
			}

			const TSharedPtr<FJsonObject>* ClearCoatRoughnessPtr = nullptr;
			if (Properties->TryGetObjectField("ClearCoatRoughness", ClearCoatRoughnessPtr) && ClearCoatRoughnessPtr != nullptr) {
				FJsonObject* ClearCoatRoughnessObject = ClearCoatRoughnessPtr->Get();
				FName ClearCoatRoughnessExpressionName = GetExpressionName(ClearCoatRoughnessObject);
				if (CreatedExpressionMap.Contains(ClearCoatRoughnessExpressionName)) {
					FExpressionInput ClearCoatRoughness = PopulateExpressionInput(ClearCoatRoughnessObject, *CreatedExpressionMap.Find(ClearCoatRoughnessExpressionName));
					MakeMaterialAttributes->ClearCoatRoughness = ClearCoatRoughness;
				}
			}

			const TSharedPtr<FJsonObject>* AmbientOcclusionPtr = nullptr;
			if (Properties->TryGetObjectField("AmbientOcclusion", AmbientOcclusionPtr) && AmbientOcclusionPtr != nullptr) {
				FJsonObject* AmbientOcclusionObject = AmbientOcclusionPtr->Get();
				FName AmbientOcclusionExpressionName = GetExpressionName(AmbientOcclusionObject);
				if (CreatedExpressionMap.Contains(AmbientOcclusionExpressionName)) {
					FExpressionInput AmbientOcclusion = PopulateExpressionInput(AmbientOcclusionObject, *CreatedExpressionMap.Find(AmbientOcclusionExpressionName));
					MakeMaterialAttributes->AmbientOcclusion = AmbientOcclusion;
				}
			}

			const TSharedPtr<FJsonObject>* RefractionPtr = nullptr;
			if (Properties->TryGetObjectField("Refraction", RefractionPtr) && RefractionPtr != nullptr) {
				FJsonObject* RefractionObject = RefractionPtr->Get();
				FName RefractionExpressionName = GetExpressionName(RefractionObject);
				if (CreatedExpressionMap.Contains(RefractionExpressionName)) {
					FExpressionInput Refraction = PopulateExpressionInput(RefractionObject, *CreatedExpressionMap.Find(RefractionExpressionName));
					MakeMaterialAttributes->Refraction = Refraction;
				}
			}

			// TODO: Add Customized UVs

			const TSharedPtr<FJsonObject>* PixelDepthOffsetPtr = nullptr;
			if (Properties->TryGetObjectField("PixelDepthOffset", PixelDepthOffsetPtr) && PixelDepthOffsetPtr != nullptr) {
				FJsonObject* PixelDepthOffsetObject = PixelDepthOffsetPtr->Get();
				FName PixelDepthOffsetExpressionName = GetExpressionName(PixelDepthOffsetObject);
				if (CreatedExpressionMap.Contains(PixelDepthOffsetExpressionName)) {
					FExpressionInput PixelDepthOffset = PopulateExpressionInput(PixelDepthOffsetObject, *CreatedExpressionMap.Find(PixelDepthOffsetExpressionName));
					MakeMaterialAttributes->PixelDepthOffset = PixelDepthOffset;
				}
			}

			const TSharedPtr<FJsonObject>* ShadingModelPtr = nullptr;
			if (Properties->TryGetObjectField("ShadingModel", ShadingModelPtr) && ShadingModelPtr != nullptr) {
				FJsonObject* ShadingModelObject = ShadingModelPtr->Get();
				FName ShadingModelExpressionName = GetExpressionName(ShadingModelObject);
				if (CreatedExpressionMap.Contains(ShadingModelExpressionName)) {
					FExpressionInput ShadingModel = PopulateExpressionInput(ShadingModelObject, *CreatedExpressionMap.Find(ShadingModelExpressionName));
					MakeMaterialAttributes->ShadingModel = ShadingModel;
				}
			}

			Expression = MakeMaterialAttributes;
		}
		if (Type->Type == "MaterialExpressionChannelMaskParameter") {
			UMaterialExpressionChannelMaskParameter* ChannelMaskParameter = Cast<UMaterialExpressionChannelMaskParameter>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Input", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					ChannelMaskParameter->Input = A;
				}
			}

			FString MaskChannel;
			if (Properties->TryGetStringField("MaskChannel", MaskChannel)) {
				ChannelMaskParameter->MaskChannel = static_cast<EChannelMaskParameterColor::Type>(StaticEnum<EChannelMaskParameterColor::Type>()->GetValueByNameString(MaskChannel));
			}

			Expression = ChannelMaskParameter;
		} else if (Type->Type == "MaterialExpressionStaticComponentMaskParameter") {
			UMaterialExpressionStaticComponentMaskParameter* StaticComponentMaskParameter = Cast<UMaterialExpressionStaticComponentMaskParameter>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Input", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					StaticComponentMaskParameter->Input = A;
				}
			}

			// Reset to defaults
			StaticComponentMaskParameter->DefaultR = false;
			StaticComponentMaskParameter->DefaultG = false;
			StaticComponentMaskParameter->DefaultB = false;
			StaticComponentMaskParameter->DefaultA = false;

			bool DefaultR;
			if (Properties->TryGetBoolField("DefaultR", DefaultR)) StaticComponentMaskParameter->DefaultR = DefaultR;
			bool DefaultG;
			if (Properties->TryGetBoolField("DefaultG", DefaultG)) StaticComponentMaskParameter->DefaultG = DefaultG;
			bool DefaultB;
			if (Properties->TryGetBoolField("DefaultB", DefaultB)) StaticComponentMaskParameter->DefaultB = DefaultB;
			bool DefaultA;
			if (Properties->TryGetBoolField("DefaultA", DefaultA)) StaticComponentMaskParameter->DefaultA = DefaultA;

			Expression = StaticComponentMaskParameter;
		} else if (Type->Type == "MaterialExpressionShadingModel") {
			UMaterialExpressionShadingModel* ShadingModelObj = Cast<UMaterialExpressionShadingModel>(Expression);

			FString ShadingModel;
			if (Properties->TryGetStringField("ShadingModel", ShadingModel)) {
				ShadingModelObj->ShadingModel = static_cast<EMaterialShadingModel>(StaticEnum<EMaterialShadingModel>()->GetValueByNameString(ShadingModel));
			}

			Expression = ShadingModelObj;
		} else if (Type->Type == "MaterialExpressionViewProperty") {
			UMaterialExpressionViewProperty* ViewProperty = Cast<UMaterialExpressionViewProperty>(Expression);

			FString Property;
			if (Properties->TryGetStringField("Property", Property)) {
				ViewProperty->Property = static_cast<EMaterialExposedViewProperty>(StaticEnum<EMaterialExposedViewProperty>()->GetValueByNameString(Property));
			}

			Expression = ViewProperty;
		} else if (Type->Type == "MaterialExpressionSetMaterialAttributes") {
			UMaterialExpressionSetMaterialAttributes* SetMaterialAttributes = Cast<UMaterialExpressionSetMaterialAttributes>(Expression);

			const TArray<TSharedPtr<FJsonValue>>* InputsPtr;
			if (Properties->TryGetArrayField("Inputs", InputsPtr)) {
				int i = 0;
				SetMaterialAttributes->Inputs.SetNumZeroed(InputsPtr->Num());
				for (const TSharedPtr<FJsonValue> InputValue : *InputsPtr) {
					FJsonObject* InputObject = InputValue->AsObject().Get();
					FName InputExpressionName = GetExpressionName(InputObject);
					if (CreatedExpressionMap.Contains(InputExpressionName)) {
						FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
						SetMaterialAttributes->Inputs[i] = Input;
					}
					i++;
				}
			}

			const TArray<TSharedPtr<FJsonValue>>* AttributeSetTypesPtr;
			if (Properties->TryGetArrayField("AttributeSetTypes", AttributeSetTypesPtr)) {
				int i = 0;
				SetMaterialAttributes->AttributeSetTypes.SetNumZeroed(AttributeSetTypesPtr->Num());
				for (const TSharedPtr<FJsonValue> AttributeSetTypeValue : *AttributeSetTypesPtr) {
					FString AttributeSetType = AttributeSetTypeValue->AsString();
					SetMaterialAttributes->AttributeSetTypes[i] = FGuid(AttributeSetType);
					i++;
				}
			}

			Expression = SetMaterialAttributes;
		} else if (Type->Type == "MaterialExpressionStaticBool") {
			UMaterialExpressionStaticBool* StaticBool = Cast<UMaterialExpressionStaticBool>(Expression);

			bool Value;
			if (Properties->TryGetBoolField("Value", Value)) StaticBool->Value = Value;

			Expression = StaticBool;
		} else if (Type->Type == "MaterialExpressionStep") {
			UMaterialExpressionStep* Step = Cast<UMaterialExpressionStep>(Expression);

			const TSharedPtr<FJsonObject>* YPtr = nullptr;
			if (Properties->TryGetObjectField("Y", YPtr) && YPtr != nullptr) {
				FJsonObject* YObject = YPtr->Get();
				FName YExpressionName = GetExpressionName(YObject);
				if (CreatedExpressionMap.Contains(YExpressionName)) {
					FExpressionInput Y = PopulateExpressionInput(YObject, *CreatedExpressionMap.Find(YExpressionName));
					Step->Y = Y;
				}
			}

			const TSharedPtr<FJsonObject>* XPtr = nullptr;
			if (Properties->TryGetObjectField("X", XPtr) && XPtr != nullptr) {
				FJsonObject* XObject = XPtr->Get();
				FName XExpressionName = GetExpressionName(XObject);
				if (CreatedExpressionMap.Contains(XExpressionName)) {
					FExpressionInput X = PopulateExpressionInput(XObject, *CreatedExpressionMap.Find(XExpressionName));
					Step->X = X;
				}
			}

			float ConstY;
			if (Properties->TryGetNumberField("ConstY", ConstY)) Step->ConstY = ConstY;
			float ConstX;
			if (Properties->TryGetNumberField("ConstX", ConstX)) Step->ConstX = ConstX;

			Expression = Step;
		} else if (Type->Type == "MaterialExpressionDotProduct") {
			UMaterialExpressionDotProduct* DotProduct = Cast<UMaterialExpressionDotProduct>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("A", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					DotProduct->A = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("B", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					DotProduct->B = B;
				}
			}

			Expression = DotProduct;
		} else if (Type->Type == "MaterialExpressionArctangent2Fast") {
			UMaterialExpressionArctangent2Fast* Arctangent2Fast = static_cast<UMaterialExpressionArctangent2Fast*>(Expression);

			const TSharedPtr<FJsonObject>* YPtr = nullptr;
			if (Properties->TryGetObjectField("Y", YPtr) && YPtr != nullptr) {
				FJsonObject* YObject = YPtr->Get();
				FName YExpressionName = GetExpressionName(YObject);
				if (CreatedExpressionMap.Contains(YExpressionName)) {
					FExpressionInput Y = PopulateExpressionInput(YObject, *CreatedExpressionMap.Find(YExpressionName));
					Arctangent2Fast->Y = Y;
				}
			}

			const TSharedPtr<FJsonObject>* XPtr = nullptr;
			if (Properties->TryGetObjectField("X", XPtr) && XPtr != nullptr) {
				FJsonObject* XObject = XPtr->Get();
				FName XExpressionName = GetExpressionName(XObject);
				if (CreatedExpressionMap.Contains(XExpressionName)) {
					FExpressionInput X = PopulateExpressionInput(XObject, *CreatedExpressionMap.Find(XExpressionName));
					Arctangent2Fast->X = X;
				}
			}

			Expression = Arctangent2Fast;
		} else if (Type->Type == "MaterialExpressionArctangent2") {
			UMaterialExpressionArctangent2* Arctangent2 = Cast<UMaterialExpressionArctangent2>(Expression);

			const TSharedPtr<FJsonObject>* YPtr = nullptr;
			if (Properties->TryGetObjectField("Y", YPtr) && YPtr != nullptr) {
				FJsonObject* YObject = YPtr->Get();
				FName YExpressionName = GetExpressionName(YObject);
				if (CreatedExpressionMap.Contains(YExpressionName)) {
					FExpressionInput Y = PopulateExpressionInput(YObject, *CreatedExpressionMap.Find(YExpressionName));
					Arctangent2->Y = Y;
				}
			}

			const TSharedPtr<FJsonObject>* XPtr = nullptr;
			if (Properties->TryGetObjectField("X", XPtr) && XPtr != nullptr) {
				FJsonObject* XObject = XPtr->Get();
				FName XExpressionName = GetExpressionName(XObject);
				if (CreatedExpressionMap.Contains(XExpressionName)) {
					FExpressionInput X = PopulateExpressionInput(XObject, *CreatedExpressionMap.Find(XExpressionName));
					Arctangent2->X = X;
				}
			}

			Expression = Arctangent2;
		} else if (Type->Type == "MaterialExpressionArctangentFast") {
			UMaterialExpressionArctangentFast* ArctangentFast = static_cast<UMaterialExpressionArctangentFast*>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);
				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					ArctangentFast->Input = Input;
				}
			}

			Expression = ArctangentFast;
		} else if (Type->Type == "MaterialExpressionArctangent") {
			UMaterialExpressionArctangent* Arctangent = Cast<UMaterialExpressionArctangent>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);
				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					Arctangent->Input = Input;
				}
			}

			Expression = Arctangent;
		} else if (Type->Type == "MaterialExpressionStaticSwitch") {
			UMaterialExpressionStaticSwitch* StaticSwitch = Cast<UMaterialExpressionStaticSwitch>(Expression);

			bool DefaultValue;
			if (Properties->TryGetBoolField("DefaultValue", DefaultValue)) StaticSwitch->DefaultValue = DefaultValue;

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("A", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					StaticSwitch->A = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("B", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					StaticSwitch->B = B;
				}
			}

			const TSharedPtr<FJsonObject>* ValuePtr = nullptr;
			if (Properties->TryGetObjectField("Value", ValuePtr) && ValuePtr != nullptr) {
				FJsonObject* ValueObject = ValuePtr->Get();
				FName ValueExpressionName = GetExpressionName(ValueObject);
				if (CreatedExpressionMap.Contains(ValueExpressionName)) {
					FExpressionInput Value = PopulateExpressionInput(ValueObject, *CreatedExpressionMap.Find(ValueExpressionName));
					StaticSwitch->Value = Value;
				}
			}

			Expression = StaticSwitch;
		} else if (Type->Type == "MaterialExpressionPower") {
			UMaterialExpressionPower* Power = Cast<UMaterialExpressionPower>(Expression);

			const TSharedPtr<FJsonObject>* BasePtr = nullptr;
			if (Properties->TryGetObjectField("Base", BasePtr) && BasePtr != nullptr) {
				FJsonObject* BaseObject = BasePtr->Get();
				FName BaseExpressionName = GetExpressionName(BaseObject);
				if (CreatedExpressionMap.Contains(BaseExpressionName)) {
					FExpressionInput Base = PopulateExpressionInput(BaseObject, *CreatedExpressionMap.Find(BaseExpressionName));
					Power->Base = Base;
				}
			}

			const TSharedPtr<FJsonObject>* ExponentPtr = nullptr;
			if (Properties->TryGetObjectField("Exponent", ExponentPtr) && ExponentPtr != nullptr) {
				FJsonObject* ExponentObject = ExponentPtr->Get();
				FName ExponentExpressionName = GetExpressionName(ExponentObject);
				if (CreatedExpressionMap.Contains(ExponentExpressionName)) {
					FExpressionInput Exponent = PopulateExpressionInput(ExponentObject, *CreatedExpressionMap.Find(ExponentExpressionName));
					Power->Exponent = Exponent;
				}
			}

			float ConstExponent;
			if (Properties->TryGetNumberField("ConstExponent", ConstExponent)) Power->ConstExponent = ConstExponent;

			Expression = Power;
		} else if (Type->Type == "MaterialExpressionRound") {
			UMaterialExpressionRound* Round = static_cast<UMaterialExpressionRound*>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);
				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					Round->Input = Input;
				}
			}

			Expression = Round;
		} else if (Type->Type == "MaterialExpressionFloor") {
			UMaterialExpressionFloor* Floor = Cast<UMaterialExpressionFloor>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);
				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					Floor->Input = Input;
				}
			}

			Expression = Floor;
		} else if (Type->Type == "MaterialExpressionCustom") {
			UMaterialExpressionCustom* Custom = Cast<UMaterialExpressionCustom>(Expression);

			FString Code;
			if (Properties->TryGetStringField("Code", Code)) Custom->Code = Code;

			FString OutputType;
			if (Properties->TryGetStringField("OutputType", OutputType)) {
				Custom->OutputType = static_cast<ECustomMaterialOutputType>(StaticEnum<ECustomMaterialOutputType>()->GetValueByNameString(OutputType));
			}

			FString FuncDescription;
			if (Properties->TryGetStringField("Description", FuncDescription)) Custom->Description = FuncDescription;

			const TArray<TSharedPtr<FJsonValue>>* StringInputs;
			if (Properties->TryGetArrayField("Inputs", StringInputs)) {
				TArray<FCustomInput> Inputs;

				for (const TSharedPtr<FJsonValue> StringInput : *StringInputs) {
					if (StringInput->IsNull()) continue;

					FCustomInput Input;
					const TSharedPtr<FJsonObject>* InputPtr = nullptr;
					if (StringInput->AsObject()->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
						FJsonObject* InputObject = InputPtr->Get();
						FName InputExpressionName = GetExpressionName(InputObject);

						if (CreatedExpressionMap.Contains(InputExpressionName)) {
							FExpressionInput ExInput = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
							Input.Input = ExInput;
						}
					}

					FString InputName;
					if (StringInput->AsObject()->TryGetStringField("InputName", InputName)) Input.InputName = FName(InputName);

					Inputs.Add(Input);
				}

				Custom->Inputs = Inputs;
			}

			const TArray<TSharedPtr<FJsonValue>>* StringAdditionalOutputs;
			if (Properties->TryGetArrayField("AdditionalOutputs", StringAdditionalOutputs)) {
				TArray<FCustomOutput> AdditionalOutputs;

				for (const TSharedPtr<FJsonValue> StringOutput : *StringAdditionalOutputs) {
					if (StringOutput->IsNull()) continue;

					FCustomOutput Output;

					FString OutputType1;
					if (StringOutput->AsObject()->TryGetStringField("OutputType", OutputType1)) {
						Output.OutputType = static_cast<ECustomMaterialOutputType>(StaticEnum<ECustomMaterialOutputType>()->GetValueByNameString(OutputType1));
					}

					FString OutputName;
					if (StringOutput->AsObject()->TryGetStringField("OutputName", OutputName)) Output.OutputName = FName(OutputName);

					AdditionalOutputs.Add(Output);
				}

				Custom->AdditionalOutputs = AdditionalOutputs;
			}

			const TArray<TSharedPtr<FJsonValue>>* StringAdditionalDefines;
			if (Properties->TryGetArrayField("AdditionalDefines", StringAdditionalDefines)) {
				TArray<FCustomDefine> AdditionalDefines;

				for (const TSharedPtr<FJsonValue> StringDefine : *StringAdditionalDefines) {
					if (StringDefine->IsNull()) continue;

					FCustomDefine Define;

					FString DefineName;
					if (StringDefine->AsObject()->TryGetStringField("DefineName", DefineName)) Define.DefineName = DefineName;
					FString DefineValue;
					if (StringDefine->AsObject()->TryGetStringField("DefineValue", DefineValue)) Define.DefineValue = DefineValue;

					AdditionalDefines.Add(Define);
				}

				Custom->AdditionalDefines = AdditionalDefines;
			}

			const TArray<TSharedPtr<FJsonValue>>* IncludeFilePathsPtr;
			if (Properties->TryGetArrayField("IncludeFilePaths", IncludeFilePathsPtr)) {
				Custom->IncludeFilePaths.Empty();

				for (const TSharedPtr<FJsonValue> IncludeFilePathsValue : *IncludeFilePathsPtr) {
					Custom->IncludeFilePaths.Add(IncludeFilePathsValue->AsString());
				}
			}

			Expression = Custom;
		} else if (Type->Type == "MaterialExpressionCeil") {
			UMaterialExpressionCeil* Ceil = Cast<UMaterialExpressionCeil>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);
				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					Ceil->Input = Input;
				}
			}

			Expression = Ceil;
		} else if (Type->Type == "MaterialExpressionIf") {
			UMaterialExpressionIf* If = Cast<UMaterialExpressionIf>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("A", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					If->A = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("B", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					If->B = B;
				}
			}

			const TSharedPtr<FJsonObject>* AGreaterThanBPtr = nullptr;
			if (Properties->TryGetObjectField("AGreaterThanB", AGreaterThanBPtr) && AGreaterThanBPtr != nullptr) {
				FJsonObject* AGreaterThanBObject = AGreaterThanBPtr->Get();
				FName AGreaterThanBExpressionName = GetExpressionName(AGreaterThanBObject);
				if (CreatedExpressionMap.Contains(AGreaterThanBExpressionName)) {
					FExpressionInput AGreaterThanB = PopulateExpressionInput(AGreaterThanBObject, *CreatedExpressionMap.Find(AGreaterThanBExpressionName));
					If->AGreaterThanB = AGreaterThanB;
				}
			}

			const TSharedPtr<FJsonObject>* AEqualsBPtr = nullptr;
			if (Properties->TryGetObjectField("AEqualsB", AEqualsBPtr) && AEqualsBPtr != nullptr) {
				FJsonObject* AEqualsBObject = AEqualsBPtr->Get();
				FName AEqualsBExpressionName = GetExpressionName(AEqualsBObject);
				if (CreatedExpressionMap.Contains(AEqualsBExpressionName)) {
					FExpressionInput AEqualsB = PopulateExpressionInput(AEqualsBObject, *CreatedExpressionMap.Find(AEqualsBExpressionName));
					If->AEqualsB = AEqualsB;
				}
			}

			const TSharedPtr<FJsonObject>* ALessThanBPtr = nullptr;
			if (Properties->TryGetObjectField("ALessThanB", ALessThanBPtr) && ALessThanBPtr != nullptr) {
				FJsonObject* ALessThanBObject = ALessThanBPtr->Get();
				FName ALessThanBExpressionName = GetExpressionName(ALessThanBObject);
				if (CreatedExpressionMap.Contains(ALessThanBExpressionName)) {
					FExpressionInput ALessThanB = PopulateExpressionInput(ALessThanBObject, *CreatedExpressionMap.Find(ALessThanBExpressionName));
					If->ALessThanB = ALessThanB;
				}
			}

			float EqualsThreshold;
			if (Properties->TryGetNumberField("EqualsThreshold", EqualsThreshold)) If->EqualsThreshold = EqualsThreshold;
			float ConstB;
			if (Properties->TryGetNumberField("ConstB", ConstB)) If->ConstB = ConstB;

			Expression = If;
		} else if (Type->Type == "MaterialExpressionCosine") {
			UMaterialExpressionCosine* Cosine = Cast<UMaterialExpressionCosine>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);
				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					Cosine->Input = Input;
				}
			}

			float Period;
			if (Properties->TryGetNumberField("Period", Period)) Cosine->Period = Period;

			Expression = Cosine;
		} else if (Type->Type == "MaterialExpressionDesaturation") {
			UMaterialExpressionDesaturation* Desaturation = Cast<UMaterialExpressionDesaturation>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);
				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					Desaturation->Input = Input;
				}
			}

			const TSharedPtr<FJsonObject>* FractionPtr = nullptr;
			if (Properties->TryGetObjectField("Fraction", FractionPtr) && FractionPtr != nullptr) {
				FJsonObject* FractionObject = FractionPtr->Get();
				FName FractionExpressionName = GetExpressionName(FractionObject);
				if (CreatedExpressionMap.Contains(FractionExpressionName)) {
					FExpressionInput Fraction = PopulateExpressionInput(FractionObject, *CreatedExpressionMap.Find(FractionExpressionName));
					Desaturation->Fraction = Fraction;
				}
			}

			const TSharedPtr<FJsonObject>* LuminanceFactors;
			if (Properties->TryGetObjectField("LuminanceFactors", LuminanceFactors)) Desaturation->LuminanceFactors = FMathUtilities::ObjectToLinearColor(LuminanceFactors->Get());

			Expression = Desaturation;
		} else if (Type->Type == "MaterialExpressionClamp") {
			UMaterialExpressionClamp* Clamp = Cast<UMaterialExpressionClamp>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);
				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					Clamp->Input = Input;
				}
			}

			const TSharedPtr<FJsonObject>* MinPtr = nullptr;
			if ((Properties->TryGetObjectField("min", MinPtr) || Properties->TryGetObjectField("Min", MinPtr)) && MinPtr != nullptr) {
				FJsonObject* MinObject = MinPtr->Get();
				FName MinExpressionName = GetExpressionName(MinObject);
				if (CreatedExpressionMap.Contains(MinExpressionName)) {
					FExpressionInput Min = PopulateExpressionInput(MinObject, *CreatedExpressionMap.Find(MinExpressionName));
					Clamp->Min = Min;
				}
			}

			const TSharedPtr<FJsonObject>* MaxPtr = nullptr;
			if ((Properties->TryGetObjectField("max", MaxPtr) || Properties->TryGetObjectField("Max", MaxPtr)) && MaxPtr != nullptr) {
				FJsonObject* MaxObject = MaxPtr->Get();
				FName MaxExpressionName = GetExpressionName(MaxObject);
				if (CreatedExpressionMap.Contains(MaxExpressionName)) {
					FExpressionInput Max = PopulateExpressionInput(MaxObject, *CreatedExpressionMap.Find(MaxExpressionName));
					Clamp->Max = Max;
				}
			}

			FString ClampModeString;
			if (Properties->TryGetStringField("ClampMode", ClampModeString)) {
				EClampMode ClampMode = Clamp->ClampMode;

				if (ClampModeString.EndsWith("CMODE_Clamp")) ClampMode = CMODE_Clamp;
				else if (ClampModeString.EndsWith("CMODE_ClampMin")) ClampMode = CMODE_ClampMin;
				else if (ClampModeString.EndsWith("CMODE_ClampMax")) ClampMode = CMODE_ClampMax;

				Clamp->ClampMode = ClampMode;
			}

			float MinDefault;
			if (Properties->TryGetNumberField("MinDefault", MinDefault)) Clamp->MinDefault = MinDefault;
			float MaxDefault;
			if (Properties->TryGetNumberField("MaxDefault", MaxDefault)) Clamp->MaxDefault = MaxDefault;

			Expression = Clamp;
		} else if (Type->Type == "MaterialExpressionTransformPosition") {
			UMaterialExpressionTransformPosition* TransformPosition = Cast<UMaterialExpressionTransformPosition>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);
				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					TransformPosition->Input = Input;
				}
			}

			FString TransformSourceType;
			if (Properties->TryGetStringField("TransformSourceType", TransformSourceType)) {
				TransformPosition->TransformSourceType = static_cast<EMaterialPositionTransformSource>(StaticEnum<EMaterialPositionTransformSource>()->GetValueByNameString(TransformSourceType));
			}

			FString TransformType;
			if (Properties->TryGetStringField("TransformType", TransformType)) {
				TransformPosition->TransformType = static_cast<EMaterialPositionTransformSource>(StaticEnum<EMaterialPositionTransformSource>()->GetValueByNameString(TransformType));
			}

			Expression = TransformPosition;
		} else if (Type->Type == "MaterialExpressionSphereMask") {
			UMaterialExpressionSphereMask* SphereMask = Cast<UMaterialExpressionSphereMask>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("A", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					SphereMask->A = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("B", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					SphereMask->B = B;
				}
			}

			const TSharedPtr<FJsonObject>* RadiusPtr = nullptr;
			if (Properties->TryGetObjectField("Radius", RadiusPtr) && RadiusPtr != nullptr) {
				FJsonObject* RadiusObject = RadiusPtr->Get();
				FName RadiusExpressionName = GetExpressionName(RadiusObject);
				if (CreatedExpressionMap.Contains(RadiusExpressionName)) {
					FExpressionInput Radius = PopulateExpressionInput(RadiusObject, *CreatedExpressionMap.Find(RadiusExpressionName));
					SphereMask->Radius = Radius;
				}
			}

			const TSharedPtr<FJsonObject>* HardnessPtr = nullptr;
			if (Properties->TryGetObjectField("Hardness", HardnessPtr) && HardnessPtr != nullptr) {
				FJsonObject* HardnessObject = HardnessPtr->Get();
				FName HardnessExpressionName = GetExpressionName(HardnessObject);
				if (CreatedExpressionMap.Contains(HardnessExpressionName)) {
					FExpressionInput Hardness = PopulateExpressionInput(HardnessObject, *CreatedExpressionMap.Find(HardnessExpressionName));
					SphereMask->Hardness = Hardness;
				}
			}

			float AttenuationRadius;
			if (Properties->TryGetNumberField("AttenuationRadius", AttenuationRadius)) SphereMask->AttenuationRadius = AttenuationRadius;
			float HardnessPercent;
			if (Properties->TryGetNumberField("HardnessPercent", HardnessPercent)) SphereMask->HardnessPercent = HardnessPercent;

			Expression = SphereMask;
		} else if (Type->Type == "MaterialExpressionCurveAtlasRowParameter") {
			UMaterialExpressionCurveAtlasRowParameter* CurveAtlasRowParameter = Cast<UMaterialExpressionCurveAtlasRowParameter>(Expression);

			const TSharedPtr<FJsonObject>* CurvePtr = nullptr;
			if (Properties->TryGetObjectField("Curve", CurvePtr) && CurvePtr != nullptr) {
				LoadObject(CurvePtr, CurveAtlasRowParameter->Curve);
			}

			const TSharedPtr<FJsonObject>* AtlasPtr = nullptr;
			if (Properties->TryGetObjectField("Atlas", AtlasPtr) && AtlasPtr != nullptr) {
				LoadObject(AtlasPtr, CurveAtlasRowParameter->Atlas);
			}

			const TSharedPtr<FJsonObject>* InputTimePtr = nullptr;
			if (Properties->TryGetObjectField("InputTime", InputTimePtr) && InputTimePtr != nullptr) {
				FJsonObject* InputTimeObject = InputTimePtr->Get();
				FName InputTimeExpressionName = GetExpressionName(InputTimeObject);
				if (CreatedExpressionMap.Contains(InputTimeExpressionName)) {
					FExpressionInput InputTime = PopulateExpressionInput(InputTimeObject, *CreatedExpressionMap.Find(InputTimeExpressionName));
					CurveAtlasRowParameter->InputTime = InputTime;
				}
			}

			Expression = CurveAtlasRowParameter;
		} else if (Type->Type == "MaterialExpressionFmod") {
			UMaterialExpressionFmod* Fmod = Cast<UMaterialExpressionFmod>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("A", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					Fmod->A = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("B", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					Fmod->B = B;
				}
			}

			Expression = Fmod;
		} else if (Type->Type == "MaterialExpressionTextureProperty") {
			UMaterialExpressionTextureProperty* TextureProperty = Cast<UMaterialExpressionTextureProperty>(Expression);

			const TSharedPtr<FJsonObject>* TextureObjectPtr = nullptr;
			if (Properties->TryGetObjectField("TextureObject", TextureObjectPtr) && TextureObjectPtr != nullptr) {
				FJsonObject* TextureObjectObject = TextureObjectPtr->Get();
				FName TextureObjectExpressionName = GetExpressionName(TextureObjectObject);
				if (CreatedExpressionMap.Contains(TextureObjectExpressionName)) {
					FExpressionInput TextureObject = PopulateExpressionInput(TextureObjectObject, *CreatedExpressionMap.Find(TextureObjectExpressionName));
					TextureProperty->TextureObject = TextureObject;
				}
			}

			FString PropertyString;
			if (Properties->TryGetStringField("Property", PropertyString)) {
				EMaterialExposedTextureProperty Property = TextureProperty->Property;

				if (PropertyString.EndsWith("TMTM_TextureSize")) Property = TMTM_TextureSize;
				else if (PropertyString.EndsWith("TMTM_TexelSize")) Property = TMTM_TexelSize;

				TextureProperty->Property = Property;
			}

			Expression = TextureProperty;
		} else if (Type->Type == "MaterialExpressionWorldPosition") {
			UMaterialExpressionWorldPosition* WorldPosition = Cast<UMaterialExpressionWorldPosition>(Expression);

			FString WorldPositionShaderOffset;
			if (Properties->TryGetStringField("WorldPositionShaderOffset", WorldPositionShaderOffset)) {
				WorldPosition->WorldPositionShaderOffset = static_cast<EWorldPositionIncludedOffsets>(StaticEnum<EWorldPositionIncludedOffsets>()->GetValueByNameString(WorldPositionShaderOffset));
			}

			Expression = WorldPosition;
		} else if (Type->Type == "MaterialExpressionNormalize") {
			UMaterialExpressionNormalize* Normalize = Cast<UMaterialExpressionNormalize>(Expression);

			const TSharedPtr<FJsonObject>* VectorInputPtr = nullptr;
			if (Properties->TryGetObjectField("VectorInput", VectorInputPtr) && VectorInputPtr != nullptr) {
				FJsonObject* VectorInputObject = VectorInputPtr->Get();
				FName VectorInputExpressionName = GetExpressionName(VectorInputObject);
				if (CreatedExpressionMap.Contains(VectorInputExpressionName)) {
					FExpressionInput VectorInput = PopulateExpressionInput(VectorInputObject, *CreatedExpressionMap.Find(VectorInputExpressionName));
					Normalize->VectorInput = VectorInput;
				}
			}

			Expression = Normalize;
		} else if (Type->Type == "MaterialExpressionDynamicParameter") {
			UMaterialExpressionDynamicParameter* DynamicParameter = Cast<UMaterialExpressionDynamicParameter>(Expression);

			const TArray<TSharedPtr<FJsonValue>>* ParamNamesPtr;
			if (Properties->TryGetArrayField("ParamNames", ParamNamesPtr)) {
				DynamicParameter->ParamNames.Empty();

				for (const TSharedPtr<FJsonValue> ReroutePinsValue : *ParamNamesPtr) {
					DynamicParameter->ParamNames.Add(ReroutePinsValue->AsString());
				}
			}

			const TSharedPtr<FJsonObject>* DefaultValue;
			if (Properties->TryGetObjectField("DefaultValue", DefaultValue)) DynamicParameter->DefaultValue = FMathUtilities::ObjectToLinearColor(DefaultValue->Get());

			int ParameterIndex;
			if (Properties->TryGetNumberField("ParameterIndex", ParameterIndex)) DynamicParameter->ParameterIndex = ParameterIndex;

			Expression = DynamicParameter;
		} else if (Type->Type == "MaterialExpressionFeatureLevelSwitch") {
			UMaterialExpressionFeatureLevelSwitch* FeatureLevelSwitch = Cast<UMaterialExpressionFeatureLevelSwitch>(Expression);

			const TSharedPtr<FJsonObject>* DefaultPtr = nullptr;
			if (Properties->TryGetObjectField("Default", DefaultPtr) && DefaultPtr != nullptr) {
				FJsonObject* DefaultObject = DefaultPtr->Get();
				FName DefaultExpressionName = GetExpressionName(DefaultObject);
				if (CreatedExpressionMap.Contains(DefaultExpressionName)) {
					FExpressionInput Default = PopulateExpressionInput(DefaultObject, *CreatedExpressionMap.Find(DefaultExpressionName));
					FeatureLevelSwitch->Default = Default;
				}
			}

			const TArray<TSharedPtr<FJsonValue>>* InputsPtr;
			if (Properties->TryGetArrayField("Inputs", InputsPtr)) {
				int i = 0;
				for (const TSharedPtr<FJsonValue> InputValue : *InputsPtr) {
					FJsonObject* InputObject = InputValue->AsObject().Get();
					FName InputExpressionName = GetExpressionName(InputObject);
					if (CreatedExpressionMap.Contains(InputExpressionName)) {
						FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
						FeatureLevelSwitch->Inputs[i] = Input;
					}
					i++;
				}
			}

			Expression = FeatureLevelSwitch;
		} else if (Type->Type == "MaterialExpressionShadingPathSwitch") {
			UMaterialExpressionShadingPathSwitch* ShadingPathSwitch = Cast<UMaterialExpressionShadingPathSwitch>(Expression);

			const TSharedPtr<FJsonObject>* DefaultPtr = nullptr;
			if (Properties->TryGetObjectField("Default", DefaultPtr) && DefaultPtr != nullptr) {
				FJsonObject* DefaultObject = DefaultPtr->Get();
				FName DefaultExpressionName = GetExpressionName(DefaultObject);
				if (CreatedExpressionMap.Contains(DefaultExpressionName)) {
					FExpressionInput Default = PopulateExpressionInput(DefaultObject, *CreatedExpressionMap.Find(DefaultExpressionName));
					ShadingPathSwitch->Default = Default;
				}
			}

			const TArray<TSharedPtr<FJsonValue>>* InputsPtr;
			if (Properties->TryGetArrayField("Inputs", InputsPtr)) {
				int i = 0;
				for (const TSharedPtr<FJsonValue> InputValue : *InputsPtr) {
					FJsonObject* InputObject = InputValue->AsObject().Get();
					FName InputExpressionName = GetExpressionName(InputObject);
					if (CreatedExpressionMap.Contains(InputExpressionName)) {
						FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
						ShadingPathSwitch->Inputs[i] = Input;
					}
					i++;
				}
			}

			Expression = ShadingPathSwitch;
		} else if (Type->Type == "MaterialExpressionSkyAtmosphereLightDirection") {
			UMaterialExpressionSkyAtmosphereLightDirection* SkyAtmosphereLightDirection = static_cast<UMaterialExpressionSkyAtmosphereLightDirection*>(Expression);

			int LightIndex;
			if (Properties->TryGetNumberField("LightIndex", LightIndex)) SkyAtmosphereLightDirection->LightIndex = LightIndex;

			Expression = SkyAtmosphereLightDirection;
		} else if (Type->Type == "MaterialExpressionStaticBoolParameter") {
			UMaterialExpressionStaticBoolParameter* StaticBoolParameter = static_cast<UMaterialExpressionStaticBoolParameter*>(Expression);

			bool DefaultValue;
			if (Properties->TryGetBoolField("DefaultValue", DefaultValue)) StaticBoolParameter->DefaultValue = DefaultValue;

			Expression = StaticBoolParameter;
		} else if (Type->Type == "MaterialExpressionSkyAtmosphereLightDiskLuminance") {
			UMaterialExpressionSkyAtmosphereLightDiskLuminance* SkyAtmosphereLightDiskLuminance = static_cast<UMaterialExpressionSkyAtmosphereLightDiskLuminance*>(Expression);

			int LightIndex;
			if (Properties->TryGetNumberField("LightIndex", LightIndex)) SkyAtmosphereLightDiskLuminance->LightIndex = LightIndex;

			Expression = SkyAtmosphereLightDiskLuminance;
		} else if (Type->Type == "MaterialExpressionSkyAtmosphereAerialPerspective") {
			UMaterialExpressionSkyAtmosphereAerialPerspective* AerialPerspective = static_cast<UMaterialExpressionSkyAtmosphereAerialPerspective*>(Expression);

			const TSharedPtr<FJsonObject>* DefaultPtr = nullptr;
			if (Properties->TryGetObjectField("WorldPosition", DefaultPtr) && DefaultPtr != nullptr) {
				FJsonObject* DefaultObject = DefaultPtr->Get();
				FName DefaultExpressionName = GetExpressionName(DefaultObject);
				if (CreatedExpressionMap.Contains(DefaultExpressionName)) {
					FExpressionInput Default = PopulateExpressionInput(DefaultObject, *CreatedExpressionMap.Find(DefaultExpressionName));
					AerialPerspective->WorldPosition = Default;
				}
			}

			Expression = AerialPerspective;
		} else if (Type->Type == "MaterialExpressionSkyAtmosphereLightIlluminance") {
			UMaterialExpressionSkyAtmosphereLightIlluminance* LightIlluminance = static_cast<UMaterialExpressionSkyAtmosphereLightIlluminance*>(Expression);

			const TSharedPtr<FJsonObject>* DefaultPtr = nullptr;
			if (Properties->TryGetObjectField("WorldPosition", DefaultPtr) && DefaultPtr != nullptr) {
				FJsonObject* DefaultObject = DefaultPtr->Get();
				FName DefaultExpressionName = GetExpressionName(DefaultObject);
				if (CreatedExpressionMap.Contains(DefaultExpressionName)) {
					FExpressionInput Default = PopulateExpressionInput(DefaultObject, *CreatedExpressionMap.Find(DefaultExpressionName));
					LightIlluminance->WorldPosition = Default;
				}
			}

			Expression = LightIlluminance;
		} else if (Type->Type == "MaterialExpressionTruncate") {
			UMaterialExpressionTruncate* Truncate = static_cast<UMaterialExpressionTruncate*>(Expression);

			const TSharedPtr<FJsonObject>* InputPtr = nullptr;
			if (Properties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
				FJsonObject* InputObject = InputPtr->Get();
				FName InputExpressionName = GetExpressionName(InputObject);
				if (CreatedExpressionMap.Contains(InputExpressionName)) {
					FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
					Truncate->Input = Input;
				}
			}

			Expression = Truncate;
		} else if (Type->Type == "MaterialExpressionLandscapeGrassOutput") {
			UMaterialExpressionLandscapeGrassOutput* LandscapeGrassOutput = static_cast<UMaterialExpressionLandscapeGrassOutput*>(Expression);

			const TArray<TSharedPtr<FJsonValue>>* GrassTypes;
			if (Properties->TryGetArrayField("GrassTypes", GrassTypes)) {
				for (const TSharedPtr<FJsonValue> _GrassType : *GrassTypes) {
					if (_GrassType->IsNull()) continue;
					TSharedPtr<FJsonObject> GrassType = _GrassType.Get()->AsObject();
					FGrassInput GrassInput = FGrassInput(FName(GrassType->GetStringField("Name")));

					// Grass Type Property
					const TSharedPtr<FJsonObject>* GrassAsset = nullptr;
					if (GrassType->TryGetObjectField("GrassAsset", GrassAsset) && GrassAsset != nullptr) {
						LoadObject(GrassAsset, GrassInput.GrassType);
					}

					const TSharedPtr<FJsonObject>* InputPtr = nullptr;
					if (GrassType->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
						FJsonObject* InputObject = InputPtr->Get();
						FName InputExpressionName = GetExpressionName(InputObject);

						if (CreatedExpressionMap.Contains(InputExpressionName)) {
							FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
							GrassInput.Input = Input;
						}
					}

					LandscapeGrassOutput->GrassTypes.Add(GrassInput);
				}
			}

			Expression = LandscapeGrassOutput;
		} else if (Type->Type == "MaterialExpressionLandscapeLayerSample") {
			UMaterialExpressionLandscapeLayerSample* LandscapeLayerSample = static_cast<UMaterialExpressionLandscapeLayerSample*>(Expression);

			FString ParameterName;
			if (Properties->TryGetStringField("ParameterName", ParameterName)) LandscapeLayerSample->ParameterName = FName(ParameterName);
			float PreviewWeight;
			if (Properties->TryGetNumberField("PreviewWeight", PreviewWeight)) LandscapeLayerSample->PreviewWeight = PreviewWeight;

			Expression = LandscapeLayerSample;
		} else if (Type->Type == "MaterialExpressionLandscapePhysicalMaterialOutput") {
			UMaterialExpressionLandscapePhysicalMaterialOutput* LandscapePhysicalMaterialOutput = static_cast<UMaterialExpressionLandscapePhysicalMaterialOutput*>(Expression);

			if (const TArray<TSharedPtr<FJsonValue>>* Inputs; Properties->TryGetArrayField("Inputs", Inputs))
				for (const TSharedPtr<FJsonValue> InputPtr : *Inputs) {
					TSharedPtr<FJsonObject> Input_Object = InputPtr->AsObject();
					FPhysicalMaterialInput PhysicalMaterialInput = FPhysicalMaterialInput();

					if (const TSharedPtr<FJsonObject>* PhysicalMaterial; Input_Object->TryGetObjectField("PhysicalMaterial", PhysicalMaterial)) LoadObject(PhysicalMaterial, PhysicalMaterialInput.PhysicalMaterial);
					if (const TSharedPtr<FJsonObject>* _InputPtr; Input_Object->TryGetObjectField("Input", _InputPtr)) {
						FJsonObject* InputObject = _InputPtr->Get();
						FName InputExpressionName = GetExpressionName(InputObject);
						if (CreatedExpressionMap.Contains(InputExpressionName)) {
							FExpressionInput Input = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
							PhysicalMaterialInput.Input = Input;
						}
					}

					LandscapePhysicalMaterialOutput->Inputs.Add(PhysicalMaterialInput);
				}

			Expression = LandscapePhysicalMaterialOutput;
		} else if (Type->Type == "MaterialExpressionLandscapeLayerCoords") {
			UMaterialExpressionLandscapeLayerCoords* LandscapeLayerCoords = static_cast<UMaterialExpressionLandscapeLayerCoords*>(Expression);

			FString MappingType;
			if (Properties->TryGetStringField("MappingType", MappingType)) {
				LandscapeLayerCoords->MappingType = static_cast<ETerrainCoordMappingType>(StaticEnum<ETerrainCoordMappingType>()->GetValueByNameString(MappingType));
			}

			FString CustomUVType;
			if (Properties->TryGetStringField("CustomUVType", CustomUVType)) {
				LandscapeLayerCoords->CustomUVType = static_cast<ELandscapeCustomizedCoordType>(StaticEnum<ELandscapeCustomizedCoordType>()->GetValueByNameString(CustomUVType));
			}

			float MappingScale;
			if (Properties->TryGetNumberField("MappingScale", MappingScale)) LandscapeLayerCoords->MappingScale = MappingScale;
			float MappingRotation;
			if (Properties->TryGetNumberField("MappingRotation", MappingRotation)) LandscapeLayerCoords->MappingRotation = MappingRotation;
			float MappingPanU;
			if (Properties->TryGetNumberField("MappingPanU", MappingPanU)) LandscapeLayerCoords->MappingPanU = MappingPanU;
			float MappingPanV;
			if (Properties->TryGetNumberField("MappingPanV", MappingPanV)) LandscapeLayerCoords->MappingPanV = MappingPanV;

			Expression = LandscapeLayerCoords;
		} else if (Type->Type == "MaterialExpressionLandscapeLayerSwitch") {
			UMaterialExpressionLandscapeLayerSwitch* LandscapeLayerSwitch = static_cast<UMaterialExpressionLandscapeLayerSwitch*>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("LayerUsed", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					LandscapeLayerSwitch->LayerUsed = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("LayerNotUsed", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					LandscapeLayerSwitch->LayerNotUsed = B;
				}
			}

			FString ParameterName;
			if (Properties->TryGetStringField("ParameterName", ParameterName)) LandscapeLayerSwitch->ParameterName = FName(ParameterName);
			bool PreviewUsed;
			if (Properties->TryGetBoolField("PreviewUsed", PreviewUsed)) LandscapeLayerSwitch->PreviewUsed = PreviewUsed;

			Expression = LandscapeLayerSwitch;
		} else if (Type->Type == "MaterialExpressionLandscapeLayerWeight") {
			UMaterialExpressionLandscapeLayerWeight* LandscapeLayerWeight = static_cast<UMaterialExpressionLandscapeLayerWeight*>(Expression);

			const TSharedPtr<FJsonObject>* APtr = nullptr;
			if (Properties->TryGetObjectField("Base", APtr) && APtr != nullptr) {
				FJsonObject* AObject = APtr->Get();
				FName AExpressionName = GetExpressionName(AObject);
				if (CreatedExpressionMap.Contains(AExpressionName)) {
					FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
					LandscapeLayerWeight->Base = A;
				}
			}

			const TSharedPtr<FJsonObject>* BPtr = nullptr;
			if (Properties->TryGetObjectField("Layer", BPtr) && BPtr != nullptr) {
				FJsonObject* BObject = BPtr->Get();
				FName BExpressionName = GetExpressionName(BObject);
				if (CreatedExpressionMap.Contains(BExpressionName)) {
					FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
					LandscapeLayerWeight->Layer = B;
				}
			}

			FString ParameterName;
			if (Properties->TryGetStringField("ParameterName", ParameterName)) LandscapeLayerWeight->ParameterName = FName(ParameterName);
			float PreviewWeight;
			if (Properties->TryGetNumberField("PreviewWeight", PreviewWeight)) LandscapeLayerWeight->PreviewWeight = PreviewWeight;
			const TSharedPtr<FJsonObject>* ConstBase;
			if (Properties->TryGetObjectField("ConstBase", ConstBase)) LandscapeLayerWeight->ConstBase = FMathUtilities::ObjectToVector(ConstBase->Get());

			Expression = LandscapeLayerWeight;
		} else if (Type->Type == "MaterialExpressionLandscapeLayerBlend") {
			UMaterialExpressionLandscapeLayerBlend* LandscapeLayerBlend = static_cast<UMaterialExpressionLandscapeLayerBlend*>(Expression);

			const TArray<TSharedPtr<FJsonValue>>* LayersObject;
			if (Properties->TryGetArrayField("Layers", LayersObject)) {
				for (const TSharedPtr<FJsonValue> _Layers : *LayersObject) {
					if (_Layers->IsNull()) continue;
					TSharedPtr<FJsonObject> Layers = _Layers.Get()->AsObject();
					FLayerBlendInput LayerBlendInput = FLayerBlendInput();

					FString LayerName;
					if (Layers->TryGetStringField("LayerName", LayerName)) LayerBlendInput.LayerName = FName(LayerName);

					FString BlendType;
					if (Layers->TryGetStringField("BlendType", BlendType)) {
						LayerBlendInput.BlendType = static_cast<ELandscapeLayerBlendType>(StaticEnum<ELandscapeLayerBlendType>()->GetValueByNameString(BlendType));
					}

					const TSharedPtr<FJsonObject>* BPtr = nullptr;
					if (Layers->TryGetObjectField("LayerInput", BPtr) && BPtr != nullptr) {
						FJsonObject* BObject = BPtr->Get();
						FName BExpressionName = GetExpressionName(BObject);
						if (CreatedExpressionMap.Contains(BExpressionName)) {
							FExpressionInput B = PopulateExpressionInput(BObject, *CreatedExpressionMap.Find(BExpressionName));
							LayerBlendInput.LayerInput = B;
						}
					}

					const TSharedPtr<FJsonObject>* APtr = nullptr;
					if (Layers->TryGetObjectField("HeightInput", APtr) && APtr != nullptr) {
						FJsonObject* AObject = APtr->Get();
						FName AExpressionName = GetExpressionName(AObject);
						if (CreatedExpressionMap.Contains(AExpressionName)) {
							FExpressionInput A = PopulateExpressionInput(AObject, *CreatedExpressionMap.Find(AExpressionName));
							LayerBlendInput.HeightInput = A;
						}
					}

					float PreviewWeight;
					if (Layers->TryGetNumberField("PreviewWeight", PreviewWeight)) LayerBlendInput.PreviewWeight = PreviewWeight;
					const TSharedPtr<FJsonObject>* ConstLayerInput;
					if (Layers->TryGetObjectField("ConstLayerInput", ConstLayerInput)) LayerBlendInput.ConstLayerInput = FMathUtilities::ObjectToVector(ConstLayerInput->Get());
					float ConstHeightInput;
					if (Layers->TryGetNumberField("ConstHeightInput", ConstHeightInput)) LayerBlendInput.ConstHeightInput = ConstHeightInput;

					LandscapeLayerBlend->Layers.Add(LayerBlendInput);
				}
			}

			Expression = LandscapeLayerBlend;
		} else if (Type->Type == "MaterialExpressionTextureSampleParameterSubUV") {
			UMaterialExpressionTextureSampleParameterSubUV* TextureSampleParameterSubUV = static_cast<UMaterialExpressionTextureSampleParameterSubUV*>(Expression);

			if (bool bBlend; Properties->TryGetBoolField("bBlend", bBlend)) TextureSampleParameterSubUV->bBlend = bBlend;

			Expression = TextureSampleParameterSubUV;
		}

		// -------------------- Parent Classes -------------------- //

		// >             Texture Sample             //
		if (Cast<UMaterialExpressionTextureSample>(Expression)) {
			UMaterialExpressionTextureSample* TextureSample = Cast<UMaterialExpressionTextureSample>(Expression);

			if (FString MipValueMode; Properties->TryGetStringField("MipValueMode", MipValueMode)) TextureSample->MipValueMode = static_cast<ETextureMipValueMode>(StaticEnum<ETextureMipValueMode>()->GetValueByNameString(MipValueMode));
			if (FString SamplerSource; Properties->TryGetStringField("SamplerSource", SamplerSource)) TextureSample->SamplerSource = static_cast<ESamplerSourceMode>(StaticEnum<ESamplerSourceMode>()->GetValueByNameString(SamplerSource));
			if (bool AutomaticViewMipBias; Properties->TryGetBoolField("AutomaticViewMipBias", AutomaticViewMipBias)) TextureSample->AutomaticViewMipBias = AutomaticViewMipBias;

			if (int ConstCoordinate; Properties->TryGetNumberField("ConstCoordinate", ConstCoordinate)) TextureSample->ConstCoordinate = ConstCoordinate;
			if (int ConstMipValue; Properties->TryGetNumberField("ConstMipValue", ConstMipValue)) TextureSample->ConstMipValue = ConstMipValue;

			if (const TSharedPtr<FJsonObject>* CoordinatesPtr = nullptr; Properties->TryGetObjectField("Coordinates", CoordinatesPtr)) {
				FJsonObject* CoordinatesObject = CoordinatesPtr->Get();
				FName CoordinatesExpressionName = GetExpressionName(CoordinatesObject);
				if (CreatedExpressionMap.Contains(CoordinatesExpressionName)) {
					FExpressionInput Coordinates = PopulateExpressionInput(CoordinatesObject, *CreatedExpressionMap.Find(CoordinatesExpressionName));
					TextureSample->Coordinates = Coordinates;
				}
			}

			if (const TSharedPtr<FJsonObject>* TextureObjectPtr; Properties->TryGetObjectField("TextureObject", TextureObjectPtr)) {
				FJsonObject* TextureObjectObject = TextureObjectPtr->Get();
				FName TextureObjectExpressionName = GetExpressionName(TextureObjectObject);
				if (CreatedExpressionMap.Contains(TextureObjectExpressionName)) {
					FExpressionInput TextureObject = PopulateExpressionInput(TextureObjectObject, *CreatedExpressionMap.Find(TextureObjectExpressionName));
					TextureSample->TextureObject = TextureObject;
				}
			}

			if (const TSharedPtr<FJsonObject>* MipValuePtr; Properties->TryGetObjectField("MipValue", MipValuePtr)) {
				FJsonObject* MipValueObject = MipValuePtr->Get();
				FName MipValueExpressionName = GetExpressionName(MipValueObject);
				if (CreatedExpressionMap.Contains(MipValueExpressionName)) {
					FExpressionInput MipValue = PopulateExpressionInput(MipValueObject, *CreatedExpressionMap.Find(MipValueExpressionName));
					TextureSample->MipValue = MipValue;
				}
			}

			if (const TSharedPtr<FJsonObject>* CoordinatesDXPtr; Properties->TryGetObjectField("CoordinatesDX", CoordinatesDXPtr)) {
				FJsonObject* CoordinatesDXObject = CoordinatesDXPtr->Get();
				FName CoordinatesDXExpressionName = GetExpressionName(CoordinatesDXObject);
				if (CreatedExpressionMap.Contains(CoordinatesDXExpressionName)) {
					FExpressionInput CoordinatesDX = PopulateExpressionInput(CoordinatesDXObject, *CreatedExpressionMap.Find(CoordinatesDXExpressionName));
					TextureSample->CoordinatesDX = CoordinatesDX;
				}
			}

			if (const TSharedPtr<FJsonObject>* CoordinatesDYPtr; Properties->TryGetObjectField("CoordinatesDY", CoordinatesDYPtr)) {
				FJsonObject* CoordinatesDYObject = CoordinatesDYPtr->Get();
				FName CoordinatesDYExpressionName = GetExpressionName(CoordinatesDYObject);
				if (CreatedExpressionMap.Contains(CoordinatesDYExpressionName)) {
					FExpressionInput CoordinatesDY = PopulateExpressionInput(CoordinatesDYObject, *CreatedExpressionMap.Find(CoordinatesDYExpressionName));
					TextureSample->CoordinatesDY = CoordinatesDY;
				}
			}

			if (const TSharedPtr<FJsonObject>* AutomaticViewMipBiasValuePtr; Properties->TryGetObjectField("AutomaticViewMipBiasValue", AutomaticViewMipBiasValuePtr)) {
				FJsonObject* AutomaticViewMipBiasValueObject = AutomaticViewMipBiasValuePtr->Get();
				FName AutomaticViewMipBiasValueExpressionName = GetExpressionName(AutomaticViewMipBiasValueObject);
				if (CreatedExpressionMap.Contains(AutomaticViewMipBiasValueExpressionName)) {
					FExpressionInput AutomaticViewMipBiasValue = PopulateExpressionInput(AutomaticViewMipBiasValueObject, *CreatedExpressionMap.Find(AutomaticViewMipBiasValueExpressionName));
					TextureSample->AutomaticViewMipBiasValue = AutomaticViewMipBiasValue;
				}
			}
		}

		// >             Texture Sample Parameter
		// (every single texture sample parameter inherits this)
		if (Cast<UMaterialExpressionTextureSampleParameter>(Expression)) {
			UMaterialExpressionTextureSampleParameter* TextureSampleParameter = Cast<UMaterialExpressionTextureSampleParameter>(Expression);

			if (FString ParameterName; Properties->TryGetStringField("ParameterName", ParameterName)) TextureSampleParameter->ParameterName = FName(ParameterName);
			if (FString ExpressionGUID; Properties->TryGetStringField("ExpressionGUID", ExpressionGUID)) TextureSampleParameter->ExpressionGUID = FGuid(ExpressionGUID);
			if (FString Group; Properties->TryGetStringField("Group", Group)) TextureSampleParameter->Group = FName(Group);
			if (int SortPriority; Properties->TryGetNumberField("SortPriority", SortPriority)) TextureSampleParameter->SortPriority = SortPriority;

			if (const TSharedPtr<FJsonObject>* ChannelNames; Properties->TryGetObjectField("ChannelNames", ChannelNames)) {
				// Used to describe each channel's name
				const TSharedPtr<FJsonObject>* R;
				if (ChannelNames->Get()->TryGetObjectField("R", R)) {
					TextureSampleParameter->ChannelNames.R = FText::FromString(R->Get()->GetStringField("SourceString"));
				}
				const TSharedPtr<FJsonObject>* G;
				if (ChannelNames->Get()->TryGetObjectField("G", G)) {
					TextureSampleParameter->ChannelNames.G = FText::FromString(G->Get()->GetStringField("SourceString"));
				}
				const TSharedPtr<FJsonObject>* B;
				if (ChannelNames->Get()->TryGetObjectField("B", B)) {
					TextureSampleParameter->ChannelNames.B = FText::FromString(B->Get()->GetStringField("SourceString"));
				}
				const TSharedPtr<FJsonObject>* A;
				if (ChannelNames->Get()->TryGetObjectField("A", A)) {
					TextureSampleParameter->ChannelNames.A = FText::FromString(A->Get()->GetStringField("SourceString"));
				}
			}
		}

		if (!bSubgraph) {
			if (UMaterial* MatCasted = Cast<UMaterial>(Parent)) MatCasted->GetEditorOnlyData()->ExpressionCollection.Expressions.Add(Expression);

			if (UMaterialFunction* FuncCasted = Cast<UMaterialFunction>(Parent)) FuncCasted->GetExpressionCollection().AddExpression(Expression);
			//else if (UMaterial* MatCasted = Cast<UMaterial>(Parent)) MatCasted->GetExpressionCollection().AddExpression(Expression);
		}

		MaterialGraphNode_ExpressionWrapper(Parent, Expression, Properties);
	}
}

void UMaterialGraph_Interface::MaterialGraphNode_ConstructComments(UObject* Parent, const TSharedPtr<FJsonObject>& Json, TMap<FName, FImportData>& Exports) {
	const TArray<TSharedPtr<FJsonValue>>* StringExpressionComments;
	if (Json->TryGetArrayField("EditorComments", StringExpressionComments)) {
		for (const TSharedPtr<FJsonValue> ExpressionComment : *StringExpressionComments) {
			if (ExpressionComment->IsNull()) continue;
			FName ExportName = GetExportNameOfSubobject(ExpressionComment.Get()->AsObject()->GetStringField("ObjectName"));

			const TSharedPtr<FJsonObject> Comment = Exports.Find(ExportName)->Json->GetObjectField("Properties");
			UMaterialExpressionComment* MatComment = NewObject<UMaterialExpressionComment>(Parent, UMaterialExpressionComment::StaticClass(), ExportName, RF_Transactional);

			int SizeX;
			if (Comment->TryGetNumberField("SizeX", SizeX)) MatComment->SizeX = SizeX;
			int SizeY;
			if (Comment->TryGetNumberField("SizeY", SizeY)) MatComment->SizeY = SizeY;
			FString Text;
			if (Comment->TryGetStringField("Text", Text)) MatComment->Text = Text;
			const TSharedPtr<FJsonObject>* CommentColor;
			if (Comment->TryGetObjectField("CommentColor", CommentColor)) MatComment->CommentColor = FMathUtilities::ObjectToLinearColor(CommentColor->Get());
			int FontSize;
			if (Comment->TryGetNumberField("FontSize", FontSize)) MatComment->FontSize = FontSize;
			MaterialGraphNode_ExpressionWrapper(Parent, MatComment, Comment);

			if (UMaterialFunction* FuncCasted = Cast<UMaterialFunction>(Parent)) FuncCasted->GetExpressionCollection().AddComment(MatComment);
			else if (UMaterial* MatCasted = Cast<UMaterial>(Parent)) MatCasted->GetExpressionCollection().AddComment(MatComment);
		}
	}
}

void UMaterialGraph_Interface::MaterialGraphNode_ExpressionWrapper(UObject* Parent, UMaterialExpression* Expression, const TSharedPtr<FJsonObject>& Json) {
	int MaterialExpressionEditorX;
	int MaterialExpressionEditorY;
	FString Desc;
	bool bCommentBubbleVisible;
	bool bCollapsed;
	bool bRealtimePreview;
	bool bShowOutputNameOnPin;

	if (Json->TryGetNumberField("MaterialExpressionEditorX", MaterialExpressionEditorX)) Expression->MaterialExpressionEditorX = MaterialExpressionEditorX;
	if (Json->TryGetNumberField("MaterialExpressionEditorY", MaterialExpressionEditorY)) Expression->MaterialExpressionEditorY = MaterialExpressionEditorY;

	FString MaterialExpressionGuid;
	if (Json->TryGetStringField("MaterialExpressionGuid", MaterialExpressionGuid)) Expression->MaterialExpressionGuid = FGuid(MaterialExpressionGuid);
	if (UMaterialFunction* FuncCasted = Cast<UMaterialFunction>(Parent)) Expression->Function = FuncCasted;
	else if (UMaterial* MatCasted = Cast<UMaterial>(Parent)) Expression->Material = MatCasted;
	if (Json->TryGetStringField("Desc", Desc)) Expression->Desc = Desc;
	if (Json->TryGetBoolField("bCommentBubbleVisible", bCommentBubbleVisible)) Expression->bCommentBubbleVisible = bCommentBubbleVisible;
	if (Json->TryGetBoolField("bCollapsed", bCollapsed)) Expression->bCollapsed = bCollapsed;
	if (Json->TryGetBoolField("bRealtimePreview", bRealtimePreview)) Expression->bRealtimePreview = bRealtimePreview;
	if (Json->TryGetBoolField("bShowOutputNameOnPin", bShowOutputNameOnPin)) Expression->bShowOutputNameOnPin = bShowOutputNameOnPin;

	const TArray<TSharedPtr<FJsonValue>>* OutputsPtr;
	if (Json->TryGetArrayField("Outputs", OutputsPtr)) {
		TArray<FExpressionOutput> Outputs;
		for (const TSharedPtr<FJsonValue> FunctionOutputValue : *OutputsPtr) {
			Outputs.Add(PopulateExpressionOutput(FunctionOutputValue->AsObject().Get()));
		}

		Expression->Outputs = Outputs;
	}

	if (UMaterialExpressionParameter* Parameter = Cast<UMaterialExpressionParameter>(Expression)) {
		FString ExpressionGUID;
		if (Json->TryGetStringField("ExpressionGUID", ExpressionGUID)) Parameter->ExpressionGUID = FGuid(ExpressionGUID);
		FString ParameterName;
		if (Json->TryGetStringField("ParameterName", ParameterName)) Parameter->ParameterName = FName(ParameterName);
		FString Group;
		if (Json->TryGetStringField("Group", Group)) Parameter->Group = FName(Group);
		int SortPriority;
		if (Json->TryGetNumberField("SortPriority", SortPriority)) Parameter->SortPriority = SortPriority;
	}

	if (UMaterialExpressionTextureBase* TextureBase = Cast<UMaterialExpressionTextureBase>(Expression)) {
		FString SamplerType;
		if (Json->TryGetStringField("SamplerType", SamplerType)) {
			FPropertyChangedEvent SamplerTypeChangeEvent(UMaterialExpressionTextureBase::StaticClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UMaterialExpressionTextureBase, SamplerType)));

			TextureBase->SamplerType = static_cast<EMaterialSamplerType>(StaticEnum<EMaterialSamplerType>()->GetValueByNameString(SamplerType));

			Expression->Modify();
			Expression->PostEditChangeProperty(SamplerTypeChangeEvent);
		}

		if (const TSharedPtr<FJsonObject>* TexturePtr; Json->TryGetObjectField("Texture", TexturePtr)) {
			FPropertyChangedEvent TextureChangeEvent(UMaterialExpressionTextureBase::StaticClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UMaterialExpressionTextureBase, Texture)));

			LoadObject(TexturePtr, TextureBase->Texture);

			if (UMaterial* MatCasted = Cast<UMaterial>(Parent)) {
				UMaterialEditingLibrary::RecompileMaterial(MatCasted);
				FEditorFileUtils::PromptForCheckoutAndSave({ MatCasted->GetOutermost() }, false, false);
			}

			Expression->Modify();
			Expression->PostEditChangeProperty(TextureChangeEvent);
		}

		bool IsDefaultMeshpaintTexture;
		if (Json->TryGetBoolField("IsDefaultMeshpaintTexture", IsDefaultMeshpaintTexture)) TextureBase->IsDefaultMeshpaintTexture = IsDefaultMeshpaintTexture;

		if (UMaterial* MatCasted = Cast<UMaterial>(Parent)) {
			MatCasted->RecacheUniformExpressions(true);
			MatCasted->UpdateCachedExpressionData();
		}
	}
}

UMaterialExpression* UMaterialGraph_Interface::CreateEmptyExpression(UObject* Parent, const FName Name, const FName Type) const {
	if (IgnoredExpressions.Contains(Type.ToString())) return nullptr;

	// Handle different module expressions
	const FString Engine_Class = "/Script/Engine." + Type.ToString();
	const FString Landscape_Class = "/Script/Landscape." + Type.ToString();
	const FString InterchangeImport_Class = "/Script/InterchangeImport." + Type.ToString();

	// Let user know that a expression is not added yet
	if (!Expressions.Contains(Type.ToString())) {
		UE_LOG(LogJson, Warning, TEXT("Missing support for expression type: \"%s\""), *Type.ToString());
		if (Type.ToString() != FString("")) {
			const FText DialogText = FText::FromString("Missing support for expression type: " + Type.ToString() + ", please modify source to allow properties to be set.");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		}
	}

	// Try to find using Engine
	UClass* ExpressionClass = FindObject<UClass>(nullptr, *Engine_Class);
	if (ExpressionClass == nullptr) ExpressionClass = FindObject<UClass>(nullptr, *Landscape_Class);
	if (ExpressionClass == nullptr) ExpressionClass = FindObject<UClass>(nullptr, *InterchangeImport_Class);

	// Return a new expression
	return NewObject<UMaterialExpression>(Parent, ExpressionClass, Name, RF_Transactional);
}

FExpressionInput UMaterialGraph_Interface::PopulateExpressionInput(const FJsonObject* JsonProperties, UMaterialExpression* Expression, const FString& Type) {
	FExpressionInput Input;
	Input.Expression = Expression;

	// Each Mask input/output
	int OutputIndex;
	if (JsonProperties->TryGetNumberField("OutputIndex", OutputIndex)) Input.OutputIndex = OutputIndex;
	FString InputName;
	if (JsonProperties->TryGetStringField("InputName", InputName)) Input.InputName = FName(InputName);
	int Mask;
	if (JsonProperties->TryGetNumberField("Mask", Mask)) Input.Mask = Mask;
	int MaskR;
	if (JsonProperties->TryGetNumberField("MaskR", MaskR)) Input.MaskR = MaskR;
	int MaskG;
	if (JsonProperties->TryGetNumberField("MaskG", MaskG)) Input.MaskG = MaskG;
	int MaskB;
	if (JsonProperties->TryGetNumberField("MaskB", MaskB)) Input.MaskB = MaskB;
	int MaskA;
	if (JsonProperties->TryGetNumberField("MaskA", MaskA)) Input.MaskA = MaskA;

	if (Type == "Color") {
		if (FColorMaterialInput* ColorInput = reinterpret_cast<FColorMaterialInput*>(&Input)) {
			bool UseConstant;
			if (JsonProperties->TryGetBoolField("UseConstant", UseConstant)) ColorInput->UseConstant = UseConstant;
			const TSharedPtr<FJsonObject>* Constant;
			if (JsonProperties->TryGetObjectField("Constant", Constant)) ColorInput->Constant = FMathUtilities::ObjectToLinearColor(Constant->Get()).ToFColor(true);
			Input = FExpressionInput(*ColorInput);
		}
	} else if (Type == "Scalar") {
		if (FScalarMaterialInput* ScalarInput = reinterpret_cast<FScalarMaterialInput*>(&Input)) {
			bool UseConstant;
			if (JsonProperties->TryGetBoolField("UseConstant", UseConstant)) ScalarInput->UseConstant = UseConstant;
			float Constant;
			if (JsonProperties->TryGetNumberField("Constant", Constant)) ScalarInput->Constant = Constant;
			Input = FExpressionInput(*ScalarInput);
		}
	} else if (Type == "Vector") {
		if (FVectorMaterialInput* VectorInput = reinterpret_cast<FVectorMaterialInput*>(&Input)) {
			bool UseConstant;
			if (JsonProperties->TryGetBoolField("UseConstant", UseConstant)) VectorInput->UseConstant = UseConstant;
			const TSharedPtr<FJsonObject>* Constant;
			if (JsonProperties->TryGetObjectField("Constant", Constant)) VectorInput->Constant = FMathUtilities::ObjectToVector3f(Constant->Get());
			Input = FExpressionInput(*VectorInput);
		}
	}

	return Input;
}

FExpressionOutput UMaterialGraph_Interface::PopulateExpressionOutput(const FJsonObject* JsonProperties) {
	FExpressionOutput Output;

	FString OutputName;
	if (JsonProperties->TryGetStringField("OutputName", OutputName)) Output.OutputName = FName(OutputName);
	int Mask;
	if (JsonProperties->TryGetNumberField("Mask", Mask)) Output.Mask = Mask;
	int MaskR;
	if (JsonProperties->TryGetNumberField("MaskR", MaskR)) Output.MaskR = MaskR;
	int MaskG;
	if (JsonProperties->TryGetNumberField("MaskG", MaskG)) Output.MaskG = MaskG;
	int MaskB;
	if (JsonProperties->TryGetNumberField("MaskB", MaskB)) Output.MaskB = MaskB;
	int MaskA;
	if (JsonProperties->TryGetNumberField("MaskA", MaskA)) Output.MaskA = MaskA;

	return Output;
}

FName UMaterialGraph_Interface::GetExpressionName(const FJsonObject* JsonProperties, FString OverrideParameterName) {
	const TSharedPtr<FJsonValue> ExpressionField = JsonProperties->TryGetField(OverrideParameterName);

	if (ExpressionField == nullptr || ExpressionField->IsNull()) {
		// Must be from < 4.25
		return FName(JsonProperties->GetStringField("ExpressionName"));
	}

	const TSharedPtr<FJsonObject> ExpressionObject = ExpressionField->AsObject();
	FString ObjectName;
	if (ExpressionObject->TryGetStringField("ObjectName", ObjectName)) {
		return GetExportNameOfSubobject(ObjectName);
	}

	return NAME_None;
}

FFunctionExpressionOutput UMaterialGraph_Interface::PopulateFuncExpressionOutput(const TSharedPtr<FJsonObject>& JsonProperties) {
	FFunctionExpressionOutput Output;

	FString ExpressionOutputId;
	if (JsonProperties->TryGetStringField("ExpressionOutputId", ExpressionOutputId)) Output.ExpressionOutputId = FGuid(ExpressionOutputId);

	const TSharedPtr<FJsonObject>* OutputPtr = nullptr;
	if (JsonProperties->TryGetObjectField("Output", OutputPtr) && OutputPtr != nullptr) {
		Output.Output = PopulateExpressionOutput(OutputPtr->Get());
	}

	return Output;
}

FFunctionExpressionInput UMaterialGraph_Interface::PopulateFuncExpressionInput(const TSharedPtr<FJsonObject>& JsonProperties, TMap<FName, UMaterialExpression*>& CreatedExpressionMap) {
	FFunctionExpressionInput Input;

	FString ExpressionInputId;
	if (JsonProperties->TryGetStringField("ExpressionInputId", ExpressionInputId)) Input.ExpressionInputId = FGuid(ExpressionInputId);

	const TSharedPtr<FJsonObject>* InputPtr = nullptr;
	if (JsonProperties->TryGetObjectField("Input", InputPtr) && InputPtr != nullptr) {
		const FJsonObject* InputObject = InputPtr->Get();
		const FName InputExpressionName = GetExpressionName(InputObject);

		if (CreatedExpressionMap.Contains(InputExpressionName)) {
			const FExpressionInput ExInput = PopulateExpressionInput(InputObject, *CreatedExpressionMap.Find(InputExpressionName));
			Input.Input = ExInput;
		}
	}

	return Input;
}
