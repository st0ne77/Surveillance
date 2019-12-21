#pragma once
#include "Base/StringPiece.h"
#include <memory>
#include <functional>
#include "AVThread.h"
#include "Base/BlockingQueue.h"
#include "Packet.h"
#include "AVParser.h"
#include "AVDemuxer.h"
#include "AudioDecoder.h"
#include "VideoDecoder.h"
#include "Base/Timer.h"

using std::shared_ptr;
using std::unique_ptr;

namespace balsampear
{
	class AVPlayer
	{
	public:
		enum class PlayStatus {Status_end, Status_playing, Status_pause, Status_prepare};
		enum class ClockMasterType {AudioMaster, VideoMaster, EXTERNAL_CLOCK};

		AVPlayer();
		~AVPlayer();
		bool load(const StringPiece& file);
		bool load();
		void unload();
		void setFile(const StringPiece& file);
		inline bool isLoaded() { return loaded; }
		void start();
		void seek(double pos);
		void pause();
		void stop();
		PlayStatus status();
		bool ended();


		//callbackֻ�������ݴ��ؽ��棬��������callback��ˢ��UI
		void setVideoRefreshCallback(std::function<void(std::weak_ptr<VideoFrame>)> f);
		void setSourceEndCallBack(std::function<void()> f);//������ɻص�
		void setProgressChangeCallBack(std::function<void(double)> f);//���Ž��ȱ仯�ص�
	private:
		void demux();
		void decodeAudio();
		void decodeVideo();
		void renderVideo();
		void renderAudio();

		void startAllTask();
		void stopAllTask();
		void clearAll();
		void wakeAllThread();//�������в������е��߳�

		void calcTime();

	private:
		StringPiece file_;
		AVThread demuxerThread_;
		AVThread aDecoderThread_;
		AVThread vDecoderThread_;
		AVThread audioRenderThread_;
		AVThread videoRenderThread_;
		Timer standardTimer;
		BlockingQueue<Packet> audioPackets;
		BlockingQueue<Packet> videoPackets;
		BlockingQueue<AudioFrame> audioFrames;
		BlockingQueue<VideoFrame> videoFrames;
		unique_ptr<AVParser> parser_;
		unique_ptr<AVDemuxer> demuxer_;
		unique_ptr<AudioDecoder> adecoder_;
		unique_ptr<VideoDecoder> vdecoder_;
		std::function<void(std::weak_ptr<VideoFrame>)> videoRefreshCallback_;
		std::function<void()> sourceEndCallBack_;
		std::function<void(double)> progressChangeCallBack_;
		bool loaded;
		PlayStatus state_;
		ClockMasterType clockmastertype_;
		uint64 clockmaster_msec_;
	};
}


