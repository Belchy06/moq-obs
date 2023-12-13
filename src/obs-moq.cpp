// Copyright William Belcher. All Rights Reserved.

#include <obs-module.h>
#include <plugin-support.h>

#include "moq-output.h"
#include "moq-service.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

bool obs_module_load(void)
{
	register_moq_output();
	register_moq_service();

	obs_log(LOG_INFO, "%s plugin loaded successfully (version %s)",
		PLUGIN_NAME, PLUGIN_VERSION);
	return true;
}

void obs_module_unload(void)
{
	obs_log(LOG_INFO, "%s plugin unloaded", PLUGIN_NAME);
}
