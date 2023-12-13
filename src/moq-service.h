// Copyright William Belcher. All Rights Reserved.

#pragma once

#include <obs-module.h>

#include <string>

struct MOQService {
	std::string server;

	MOQService(obs_data_t *settings, obs_service_t *service);

	void Update(obs_data_t *settings);
	static obs_properties_t *Properties();
	static void ApplyEncoderSettings(obs_data_t *video_settings,
					 obs_data_t *audio_settings);
	bool CanTryToConnect();
	const char *GetConnectInfo(enum obs_service_connect_info type);
};

void register_moq_service();