//#include "SoundSource.hpp"
//
//SoundSource::SoundSource(EbonyAudio::AudioType type) : type(type)
//{
//	alGenSources(1, &source);
//	alSourcef(source, AL_PITCH, pitch);
//	alSourcef(source, AL_GAIN, gain);
//	alSource3f(source, AL_POSITION, positions[0], positions[1], positions[2]);
//	alSource3f(source, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
//	alSourcei(source, AL_LOOPING, loopSound);
//	alSourcei(source, AL_BUFFER, buffer);
//}
//
//void SoundSource::Delete()
//{
//	if (source != 0)
//	{
//		alDeleteSources(1, &source);
//	}
//}
//
//void SoundSource::Play(const ALuint newBuffer)
//{
//	if (newBuffer != buffer)
//	{
//		buffer = newBuffer;
//		alSourcei(source, AL_BUFFER, static_cast<ALint>(buffer));
//	}
//
//	alSourcePlay(source);
//}