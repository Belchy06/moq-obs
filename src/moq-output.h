// Copyright William Belcher. All Rights Reserved.

#pragma once

#include <obs-module.h>

#include <atomic>
#include <mutex>
#include <string>

class MOQOutput {
public:
	MOQOutput(obs_data_t *settings, obs_output_t *output);
	~MOQOutput();

	bool Start();
	void Stop();
	void Data(struct encoder_packet *packet);

	inline size_t GetTotalBytes() { return total_bytes_sent; }
	inline int GetConnectTime() { return connect_time_ms; }

private:
	obs_output_t *output;

	std::atomic<bool> running;
	std::atomic<size_t> total_bytes_sent;
	std::atomic<int> connect_time_ms;
	std::mutex mutex;
	std::string endpoint_url;
};

void register_moq_output();