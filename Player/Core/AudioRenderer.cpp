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
		alSourcePlay(Source);
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
		alGenSources(1, &Source);
		alSourcei(Source, AL_BUFFER, Buffer);
	}

}