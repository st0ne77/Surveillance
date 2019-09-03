#include "stdafx.h"
#include "AVDecoder.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

AVDecoder::AVDecoder(AVCodecContext *pAVCodecCtx)
	:mpAVCodecCtx(pAVCodecCtx)
{

}

AVDecoder::~AVDecoder()
{
	avcodec_close(mpAVCodecCtx);
}

bool AVDecoder::init()
{
	AVCodec* pACodec = avcodec_find_decoder(mpAVCodecCtx->codec_id);//���ݱ���ID�ҵ����ڽ���Ľṹ��
	if (nullptr == pACodec)
	{
		return false;
	}

	if (avcodec_open2(mpAVCodecCtx, pACodec, nullptr))
	{
		return false;
	}
	return true;
}

void AVDecoder::addPacket(const Packet& pkt)
{
	for (; !mPktQueue.put(pkt,10);)
	{
		usleep(1);
	}
	
}

Frame AVDecoder::popFrame()
{
	Frame frame;
	for (; !mFrameQueue.tack(frame, 10);)
	{
		sleep(1);
	}
	return frame;
}

void AVDecoder::run()
{
	for (;;)
	{
		decode();
		//sleep(10);
	}
}
