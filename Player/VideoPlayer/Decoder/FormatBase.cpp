#include "stdafx.h"
#include "FormatBase.h"
extern "C"
{
#include "libavformat/avformat.h"
}
#include "AudioDecoder.h"
#include "Packet.h"
#include "VideoDecoder.h"


using namespace std;
FormatBase::FormatBase(const std::string& strStreamFile)
	:mpFormatCtx(nullptr)
	, mstrStreamFile(strStreamFile)
	, mpAudioDecoder(nullptr)
	, mpVideoDecoder(nullptr)
	, maudioStream(-1)
	, mvideoStream(-1)
{
	static bool init_ffmpeg = false;
	if (!init_ffmpeg)
	{
		init_ffmpeg = true;
		av_register_all();//��ʼ��FFMPEG  ��������������������ñ������ͽ�����
	}

	//FFMPEG���еĲ�����Ҫͨ��AVFormatContext������
	mpFormatCtx = avformat_alloc_context();
}

FormatBase::~FormatBase()
{
	avformat_close_input(&mpFormatCtx);

	delete mpAudioDecoder;
	mpAudioDecoder = nullptr;

	delete mpVideoDecoder;
	mpVideoDecoder = nullptr;
}

bool FormatBase::init()
{
	//����Ƶ�ļ�
	if (0 != avformat_open_input(&mpFormatCtx, mstrStreamFile.c_str(), NULL, NULL))
	{
		return false;
	}

	if (avformat_find_stream_info(mpFormatCtx, NULL) < 0)
	{
		return false;
	}
	av_dump_format(mpFormatCtx, 0, mstrStreamFile.c_str(), 0); //�����Ƶ��Ϣ
	return true;
}

AudioDecoder* FormatBase::findAudioDecoder()
{
	for (int i = 0; i < mpFormatCtx->nb_streams; ++i)
	{
		if (AVMEDIA_TYPE_AUDIO == mpFormatCtx->streams[i]->codec->codec_type)
		{
			maudioStream = i;
		}
	}

	//�ж��Ƿ��ҵ���Ƶ��
	if (-1 == maudioStream)
	{
		return nullptr;
	}

	AVCodecContext* pACodecCtx = mpFormatCtx->streams[maudioStream]->codec;



	mpAudioDecoder = new AudioDecoder(pACodecCtx);
	if (!mpAudioDecoder->init())
	{
		delete mpAudioDecoder;
		mpAudioDecoder = nullptr;
	}
	return mpAudioDecoder;
}

VideoDecoder* FormatBase::findVideoDecoder()
{
	for (int i = 0; i < mpFormatCtx->nb_streams; ++i)
	{
		if (AVMEDIA_TYPE_VIDEO == mpFormatCtx->streams[i]->codec->codec_type)
		{
			mvideoStream = i;
		}
	}

	//�ж��Ƿ��ҵ���Ƶ��
	if (-1 == mvideoStream)
	{
		return nullptr;
	}

	AVCodecContext* pVCodecCtx = mpFormatCtx->streams[mvideoStream]->codec;
	double timeBase = av_q2d(mpFormatCtx->streams[mvideoStream]->time_base);

	mpVideoDecoder = new VideoDecoder(pVCodecCtx, timeBase);
	if (!mpVideoDecoder->init())
	{
		delete mpVideoDecoder;
		mpVideoDecoder = nullptr;
	}
	return mpVideoDecoder;
}

bool FormatBase::readFrame()
{
	AVPacket pkt;
	av_init_packet(&pkt);
	for (; av_read_frame(mpFormatCtx,&pkt) >= 0;)
	{
		if (pkt.stream_index == maudioStream)
		{
			mpAudioDecoder->addPacket(Packet(pkt));
		} 
		else if(pkt.stream_index == mvideoStream)
		{
			mpVideoDecoder->addPacket(Packet(pkt));
		}
		av_packet_unref(&pkt);
	}
	return true;
}
