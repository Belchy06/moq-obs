// Copyright William Belcher. All Rights Reserved.

#include "moq-output.h"

#include "plugin-support.h"

MOQOutput::MOQOutput(obs_data_t *, obs_output_t *output)
	: output(output),
	  total_bytes_sent(0),
	  connect_time_ms(0),
	  endpoint_url(),
	  mutex(),
	  running(false)
{
}

MOQOutput::~MOQOutput()
{
	Stop();
}

bool MOQOutput::Start()
{
	std::lock_guard<std::mutex> l(mutex);

	if (!obs_output_can_begin_data_capture(output, 0)) {
		return false;
	}

	if (!obs_output_initialize_encoders(output, 0)) {
		return false;
	}

	return true;
}

void MOQOutput::Stop()
{
	std::lock_guard<std::mutex> l(mutex);
}

void MOQOutput::Data(struct encoder_packet *packet)
{
	if (!packet) {
		Stop();
		obs_output_signal_stop(output, OBS_OUTPUT_ENCODE_ERROR);
		return;
	}

	// TODO (william.belcher): send that data

	obs_log(LOG_INFO, "Data:\nsize: %d\nkeyframe: %s", packet->size,
		packet->keyframe ? "true" : "false");
}

void register_moq_output()
{
	struct obs_output_info info = {};

	info.id = "moq_output";
	info.flags = OBS_OUTPUT_AV | OBS_OUTPUT_ENCODED | OBS_OUTPUT_SERVICE;
	info.get_name = [](void *) -> const char * {
		return obs_module_text("Output.Name");
	};
	info.create = [](obs_data_t *settings, obs_output_t *output) -> void * {
		return new MOQOutput(settings, output);
	};
	info.destroy = [](void *priv_data) {
		delete static_cast<MOQOutput *>(priv_data);
	};
	info.start = [](void *priv_data) -> bool {
		return static_cast<MOQOutput *>(priv_data)->Start();
	};
	info.stop = [](void *priv_data, uint64_t) {
		static_cast<MOQOutput *>(priv_data)->Stop();
	};
	info.encoded_packet = [](void *priv_data,
				 struct encoder_packet *packet) {
		static_cast<MOQOutput *>(priv_data)->Data(packet);
	};
	info.get_defaults = [](obs_data_t *) {};
	info.get_properties = [](void *) -> obs_properties_t * {
		return obs_properties_create();
	};
	info.get_total_bytes = [](void *priv_data) -> uint64_t {
		return (uint64_t) static_cast<MOQOutput *>(priv_data)
			->GetTotalBytes();
	};
	info.get_connect_time_ms = [](void *priv_data) -> int {
		return static_cast<MOQOutput *>(priv_data)->GetConnectTime();
	};
	info.encoded_video_codecs = "h264";
	info.encoded_audio_codecs = "aac";
	// TODO (william.belcher): obs doesn't support MOQ as a potential protocol:
	// https://github.dev/obsproject/obs-studio/blob/master/plugins/obs-outputs/CMakeLists.txt
	info.protocols = "WHIP";

	obs_register_output(&info);

	obs_log(LOG_INFO, "registered moq output");
}