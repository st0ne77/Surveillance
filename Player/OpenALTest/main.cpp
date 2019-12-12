#include <conio.h>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <ALUT/alut.h>
#include <stdio.h>
/*
  ��ᷢ����OPENALͷ��OPENGLͷ�Ķ���������������֮�������ˡ�al.h","al
u.h","alut.h"�롱gl.h","glu.h","glut.h"���ƣ���������һ��"alc.h".ALC��
AUDIO LIBRARY CONTEXT����������ͨ����ͬ��ƽ̨����Ҳ�������ڼ��������¹�
���豸�Ļ�����
*/

// �洢��������
ALuint Buffer;

// ���ڲ�������
ALuint Source;

/*
  ���ǳ�����ṹ�ĳ�ʼ������OPENAL�����ֲ�ͬ�Ľṹ�����й����������ź�
�������ݴ洢��һ���ڴ��У�Դ��source����ָ��������Ŀռ䡣����Դ�Ƿǳ�
����Ҫ��Դֻ�����ڴ��еı����������ݡ�ԴҲ�����������������λ�ú��ٶȡ�
  ���������������ߣ��û�������Ψһ�����ߡ�������������Դ���ԣ��������
�����������磬��ͬλ�ý������������ٶȡ�
*/

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

/*
  ������Ĵ����У����Ƕ�����Դ�����߶����λ�ú��ٶȡ���Щ�����ǻ��ڵ�
���������ʸ�������ܺ������ýṹ��������ͬ�����顣
*/

ALboolean LoadALData()
{
    // �������.

    ALenum format;
    ALsizei size;
    ALvoid* data;
    ALsizei freq;
    ALboolean loop;
    //���������ǽ���һ���������ڴ�һ���ļ��������������ݡ��������ڴ洢�ʺ�
    //���ǵ�ALUT��Ϣ��

  // ����WAV����
    alGenBuffers(1, &Buffer);
    if (alGetError() != AL_NO_ERROR)
        return AL_FALSE;

    const char* path = "helloworld.wav";
    alutLoadWAVFile((signed char*)path, &format, &data, &size, &freq, &loop);
    alBufferData(Buffer, format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);
    /*
      ����alGenBufers���ڽ��������ڴ沢�����Ǵ洢�����Ƕ���ı����С�Ȼ���ж�
    �����Ƿ�洢��
      ALUT��Ϊ���Ǵ��ļ����ṩ���ǽ����ڴ��������Ϣ�����������ǹ�������
    ���ݵ��ڴ������������Щ���ݡ�
    */
    // ����Դ
    alGenSources(1, &Source);

    if (alGetError() != AL_NO_ERROR)
        return AL_FALSE;

    alSourcei(Source, AL_BUFFER, Buffer);
    alSourcef(Source, AL_PITCH, 1.0f);
    alSourcef(Source, AL_GAIN, 1.0f);
    alSourcefv(Source, AL_POSITION, SourcePos);
    alSourcefv(Source, AL_VELOCITY, SourceVel);
    alSourcei(Source, AL_LOOPING, loop);
    /*
      �����ý����ڴ����ķ���������Դ����Ȼ�����Ƕ���Դ��������¼�š�
    ����Ҫ�����������õ��ڴ档�����Դ����¼�š���ˣ�����ֻ����������ͬʱ��
    ����Ҳ���������Ƕ����Դλ�ú��ٶȡ�
    */
    // �������Ⲣ����
    if (alGetError() == AL_NO_ERROR)
        return AL_TRUE;

    return AL_FALSE;
    //�ں����Ľ�β�����ǽ�������ļ�⣬��ȷ��������ȷ��
}
void SetListenervalues()
{
    alListenerfv(AL_POSITION, ListenerPos);
    alListenerfv(AL_VELOCITY, ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);
}
//���ǽ���һ���������ڸ��������ٶȡ�
void KillALData()
{
    alDeleteBuffers(1, &Buffer);
    alDeleteSources(1, &Source);
    alutExit();
}
//����һ���رպ����������ͷ��ڴ����Ƶ�豸��
int main(int argc, char* argv[])
{
//     int (*func)(int) = [](int i)->int
//     {
//         return i;
//     };
//     printf("%d\n", func(8));
//     return 0;

    // ��ʼOPENAL��������ֽ�

    alutInit(&argc, argv);
    alGetError();
    /*
      ����alutInit����װALC��Ҫ�Ķ�����ALUTͨ��ALC��������Ϊ��ǰ����OPENAL
    ������������WINDOWSƽ̨�ϳ�ʼDIRECTSOUND��Ȼ���á�GLGETERROR��������
    */
    // ����WAV����
    if (LoadALData() == AL_FALSE)
        return -1;

    SetListenervalues();

    // �����˳�����
    atexit(KillALData);

    //���ǽ����WAV�ļ��Ƿ���ȷ���롣���û���˳�������ȷ�󣬸������߲���������˳���
    ALubyte c = ' ';

    while (c != 'q')
    {
        c = getche();

        switch (c)
        {
            // Pressing 'p' will begin playing the sample.
        case 'p': alSourcePlay(Source); break;

            // Pressing 's' will stop the sample from playing.
        case 's': alSourceStop(Source); break;

            // Pressing 'h' will pause (hold) the sample.
        case 'h': alSourcePause(Source); break;
        };
    }

    return 0;
}
