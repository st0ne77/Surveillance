#include "AudioRenderer.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"
namespace balsampear
{

	AudioRenderer::AudioRenderer()
	{

	}

	AudioRenderer::~AudioRenderer()
	{

	}


	// Դ������λ��
	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };

	// Դ�������ٶ�
	ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

	// ���ߵ�λ��
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

	// ���ߵ��ٶ�
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

	// ���ߵķ��� (first 3 elements are "at", second 3 are "up")
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

	// �洢��������
	ALuint Buffer;

	// ���ڲ�������
	ALuint Source;

	void AudioRenderer::init()
	{
		//��device
		ALCdevice* m_Devicde = alcOpenDevice(nullptr);
		if (m_Devicde)
		{
			//���������ı�����
			ALCcontext *m_Context = alcCreateContext(m_Devicde, nullptr);

			//������Ϊ�ı�����
			alcMakeContextCurrent(m_Context);
		}
		// ����Դ
		alGenSources(1, &Source);

		if (alGetError() != AL_NO_ERROR)
			return ;

		
	
		alSourcef(Source, AL_PITCH, 1.0f);
		alSourcef(Source, AL_GAIN, 1.0f);
		alSourcefv(Source, AL_POSITION, SourcePos);
		alSourcefv(Source, AL_VELOCITY, SourceVel);
		alSourcei(Source, AL_LOOPING, AL_FALSE);
		

		// �������Ⲣ����
		if (alGetError() != AL_NO_ERROR)
			return ;

		alListenerfv(AL_POSITION, ListenerPos);
		alListenerfv(AL_VELOCITY, ListenerVel);
		alListenerfv(AL_ORIENTATION, ListenerOri);
		
	}

	void AudioRenderer::play()
	{
		int state;
		alGetSourcei(Source, AL_SOURCE_STATE, &state);
		if (state == AL_STOPPED || state == AL_INITIAL)
		{
			alSourcePlay(Source);
		}
	}

	void AudioRenderer::update(void* data)
	{
		
		alGenBuffers(1, &Buffer);
		if (alGetError() != AL_NO_ERROR)
			return;
		alBufferData(Buffer, AL_FORMAT_STEREO8, data, 2048, 44100);
		ALenum e = alGetError();
		if (e != AL_NO_ERROR)
			return;
		alSourceQueueBuffers(Source, 1, &Buffer);
	}

	size_t AudioRenderer::queuedFrameNum()
	{
		ALint processed = 0;
		alGetSourcei(Source, AL_BUFFERS_QUEUED, &processed);
		return processed;
	}

	void AudioRenderer::unqueue()
	{
		ALint processed = 0;
		alGetSourcei(Source, AL_BUFFERS_PROCESSED, &processed);
		while (processed > 0)
		{
			ALuint bufferID = 0;
			alSourceUnqueueBuffers(Source, 1, &bufferID);
			processed--;
		}
	}

}