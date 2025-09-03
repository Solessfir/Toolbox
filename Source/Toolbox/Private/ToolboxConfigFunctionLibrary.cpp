// Copyright Solessfir. All Rights Reserved.

#include "ToolboxConfigFunctionLibrary.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ToolboxConfigFunctionLibrary)

bool UToolboxConfigFunctionLibrary::WildcardFunctionTest(FString StringProperty, const int32& WCPinForInt,	const int32& WCPinForFloat, int32& WCSummOutput)
{
	// This will never be called, the exec version below will be hit instead
	checkNoEntry();
	return false;
}

DEFINE_FUNCTION(UToolboxConfigFunctionLibrary::execWildcardFunctionTest)
{
	// properties section begin:
	// Property order MATTERS!
	// function properties can be obtained this way:
	P_GET_OBJECT(FString, StringProperty); // FString is Class
	P_GET_PROPERTY(FIntProperty, IntValue);
	P_GET_PROPERTY(FFloatProperty, FloatValue);
	// get return value wildcard pin
	Stack.MostRecentProperty = nullptr;
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr); // move property pointer stack forward to 1, this is output pin, there is no value
	void* ResultValueAddr = Stack.MostRecentPropertyAddress;
	const FProperty* ResultWCProp = CastField<FProperty>(Stack.MostRecentProperty);
	P_FINISH; // properties section end
	// global values should be initialized before NATIVE section
	bool bResult = false;
	P_NATIVE_BEGIN;
	if (IntValue > 3 && FloatValue > 4.f) {
		bResult = true;
	}
	int res = IntValue + FMath::TruncToInt(FloatValue);
	ResultWCProp->CopySingleValueFromScriptVM(ResultValueAddr,&res);
	P_NATIVE_END;
	// Function Return Value
	*static_cast<bool*>(RESULT_PARAM) = bResult;
}
