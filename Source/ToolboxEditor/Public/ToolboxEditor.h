// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolboxEditorModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;
};
