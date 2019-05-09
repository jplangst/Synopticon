#pragma once

#include "CoreMinimal.h"

#include "SystemClasses/SynOpticonCustomType.h"

namespace ImageDataStructures {

	static const int32 gCompressedBufferSize = 700000; //TODO find the best value for the buffer

	static const int32 gOutputBufferSize = 700000; //TODO find the best value for the buffer (Could start at 200k and auto increase if we detect that the input buffer is bigger than the allocated buffer for memory optimisation)

	//A struct to hold an encoded image
	struct VP8EncodedImageData
	{
		//Scene Camera Data
		int32 BufferSize;
		int32 ImageDataBufferSize;
		uint8* ImageBuffer;
		uint8 IsKeyFrame;
		int32 ImageWidth;
		int32 ImageHeight;

		VP8EncodedImageData()
		{
			BufferSize = gOutputBufferSize;
			ImageDataBufferSize = 0;
			ImageBuffer = (uint8*)FMemory::Malloc(BufferSize);
		}

		~VP8EncodedImageData()
		{
			FMemory::Free(ImageBuffer);
		}
	};
}
