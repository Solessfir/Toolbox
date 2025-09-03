// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolboxUncookedModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override;
	
    virtual void ShutdownModule() override;
};
