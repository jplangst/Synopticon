// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#pragma warning(push)
//#include "MacroUtility/UndefineMacros_UE_4.18.h"
//#include "AllowWindowsPlatformAtomics.h"
//Video Decoding
#define VPX_CODEC_DISABLE_COMPAT 1
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"
#define VPX_DECODE_INTERFACE (vpx_codec_vp8_dx())

//Video Encoding
#define VPX_CODEC_DISABLE_COMPAT 1
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#define VPX_ENCODE_INTERFACE (vpx_codec_vp8_cx())

//Image format conversion
#include "../../../SDKs/libyuv/include/libyuv.h"
//#include "HideWindowsPlatformAtomics.h"
//#include "MacroUtility/RedefineMacros_UE_4.18.h"
//#pragma warning(pop)

#include "CoreMinimal.h"
#include "Queue.h"

#include "Object.h"
#include "SystemClasses/SynOpticonCustomType.h"
#include "ImageProcessing/ImageDataStructures.h"

#include "SystemClasses/SynOpticonState.h"

#include "ThreadPoolTasks.generated.h"

class FDecodeImageTask;
class FEncodeImageTask;

DECLARE_DELEGATE_OneParam(FImageDecodeImageDelegate, ImageDataStructures::VP8EncodedImageData*);
DECLARE_DELEGATE_OneParam(FImageDecodedDelegate, UTexture2D*);

DECLARE_DELEGATE_OneParam(FImageEncodeImageDelegate, uint8*);
DECLARE_DELEGATE_OneParam(FImageEncodedDelegate, ImageDataStructures::VP8EncodedImageData*);

/**
 * 
 */
UCLASS()
class SYNOPTICON_API UThreadPoolTasks : public UObject
{
	GENERATED_BODY()

public:
	FAsyncTask<FDecodeImageTask>* CreateDecodeTask(FImageDecodedDelegate* ImageDecodedDelegate, FImageDecodeImageDelegate* DecodeImageDelegate);
	//FAsyncTask<FEncodeImageTask>* CreateEncodeTask(FImageEncodedDelegate* ImageEncodeDelegate, FImageEncodeImageDelegate* EncodeImageDelegate, URadiantWebViewRenderComponent* RenderComponent);
};

/*===========================================================================*
*									FImageDecodeTask						*
*===========================================================================*/
//For decoding images we want to reuse the task so that we don't have to allocate memory every time. 
//That means that we can only work on one image at a time unless we create more than one task.
class FDecodeImageTask : public FNonAbandonableTask
{
	friend class FAsyncTask<FDecodeImageTask>; //FAsyncTask change to FAutoDeleteAsyncTask in order to clean up the task automagically

private:
	ImageDataStructures::VP8EncodedImageData* EncodedImage; //The image to decode
	FImageDecodedDelegate* ImageDecodedDelegate;  //Callback for when the image has been decoded
	bool RecievedKeyframe; //Used to avoid some unneccessary updating of the UTexture when we have not recieved a keyframe yet
	bool FirstFrame;

	//VP Decoding
	vpx_codec_ctx_t* codec;
	uint8* RGBABuffer;

	EPixelFormat PixelFormat;

	int32 ImageWidth;
	int32 ImageHeight;
	int32 NumberOfChannels;

	bool DecoderInitialized;
	UTexture2D* DecodedTexture;
	FUpdateTextureRegion2D* DecodedTextureRegion;

	//Buffer to use for the decoded images
	uint8* DecodedImageBuffer;
	int32 DecodedBufferSize;

	void InitDecoder(int32 ImageWidth, int32 ImageHeight);
	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions,
		FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData); //TODO should move this function into the image data structures file or something. it is used in more than one class.

	bool DecodeImage();

	//Used when we are processing the data while seeking in the replay structure
	TQueue<ImageDataStructures::VP8EncodedImageData*> KeyframesToDecode; //TODO add cleanup logic for this one, even though it is supposed to be emptied in the processing step.
	ImageDataStructures::VP8EncodedImageData* Keyframe;
protected:
	//Called from the thread in the threadpool
	void DoWork();

	// This next section of code needs to be here.  
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FMyTaskName, STATGROUP_ThreadPoolAsyncTasks);
	}

public:
	//Takes a struct containing the information of the image that is to be decoded.
	FDecodeImageTask(FImageDecodedDelegate* ImageDecodedDelegate, FImageDecodeImageDelegate* DecodeImageDelegate, EPixelFormat PixelFormat);
	virtual ~FDecodeImageTask();

	//Takes a pre-bound delegate to provide a callback with the decoded image. Remember to add the task to the worker pool, otherwise no work will be done.
	void DecodeImage(ImageDataStructures::VP8EncodedImageData* EncodedImage);
};


/*===========================================================================*
*									FImageEncodeTask						*
*===========================================================================*/
//For decoding images we want to reuse the task so that we don't have to allocate memory every time. 
//That means that we can only work on one image at a time unless we create more than one task.
class FEncodeImageTask : public FNonAbandonableTask
{
	friend class FAsyncTask<FEncodeImageTask>; //FAsyncTask change to FAutoDeleteAsyncTask in order to clean up the task automagically

private:
	ImageDataStructures::VP8EncodedImageData* OutEncodedImage; //The encoded image
	FImageEncodedDelegate* ImageEncodedDelegate;  //Callback for when the image has been decoded
	bool IsWorking; //Used to make sure that we don't have two threads working on the same task

	//VP Encoding
	vpx_codec_ctx_t* Codec;
	int FPS;
	int KeyframeInterval;
	uint8* OutputBuffer;
	int32 FrameCount;

	uint8* OutYuvImage;
	uint8* RawTextureData;
	int32 RawTextureBufferSize;

	uint8* ImageToEncode;
	int32 ImageWidth;
	int32 ImageHeight;
	int32 NumberOfChannels;

	bool EncoderInitialized;
	bool ForceKeyframe;
	bool FirstFrame;

	bool InitEncoder(int32 ImageWidth, int32 ImageHeight);
	int EncodeImage(vpx_image_t *img, int frame_index, int flags);
	bool EncodeImage();

protected:
	//Called from the thread in the threadpool
	void DoWork();

	// This next section of code needs to be here.  
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FMyTaskName, STATGROUP_ThreadPoolAsyncTasks);
	}

public:
	//Takes a struct containing the information of the image that is to be decoded.
	//FEncodeImageTask(FImageEncodedDelegate* ImageEncodedDelegate, FImageEncodeImageDelegate* EncodeImageDelegate, URadiantWebViewRenderComponent* RenderComponent);
	virtual ~FEncodeImageTask();

	//Takes a pre-bound delegate to provide a callback with the encoded image. Remember to add the task to the worker pool, otherwise no work will be done.
	void EncodeImage(uint8* Image);
};
