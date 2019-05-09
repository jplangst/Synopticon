// Fill out your copyright notice in the Description page of Project Settings.

#include "ThreadPoolTasks.h"
//#include "Synopticon.h"


//Note that to use libyuv and get a pixel format that is supported in the UTexture B8G8R8A8 format we have to use the To and From ARGB function!

FAsyncTask<FDecodeImageTask>* UThreadPoolTasks::CreateDecodeTask(FImageDecodedDelegate* ImageDecodedDelegate, FImageDecodeImageDelegate* DecodeImageDelegate) {
	return (new FAsyncTask<FDecodeImageTask>(ImageDecodedDelegate, DecodeImageDelegate, EPixelFormat::PF_R8G8B8A8));
}

//FAsyncTask<FEncodeImageTask>* UThreadPoolTasks::CreateEncodeTask(FImageEncodedDelegate* ImageEncodedDelegate, FImageEncodeImageDelegate* EncodeImageDelegate, URadiantWebViewRenderComponent* RenderComponent) {
//	return (new FAsyncTask<FEncodeImageTask>(ImageEncodedDelegate, EncodeImageDelegate, RenderComponent));
//}

/********************************************************************
*							FImageDecodeTask					    *
*********************************************************************/
FDecodeImageTask::FDecodeImageTask(FImageDecodedDelegate* _ImageDecodedDelegate, FImageDecodeImageDelegate* DecodeImageDelegate, EPixelFormat _PixelFormat) {
	ImageDecodedDelegate = _ImageDecodedDelegate;
	PixelFormat = _PixelFormat;

	FirstFrame = true;

	DecoderInitialized = false;
	DecodedTexture = nullptr;
	codec = nullptr;
	RecievedKeyframe = false;

	if (PixelFormat == EPixelFormat::PF_R5G6B5_UNORM) {
		NumberOfChannels = 2; //The number of color channels of the scene camera image. (8bits*channels per pixel)
	}
	else if (PixelFormat == EPixelFormat::PF_ATC_RGB) {
		NumberOfChannels = 3;
	}
	else 
	{
		NumberOfChannels = 4; //The number of color channels of the scene camera image. (8bits*channels per pixel)
	}

	RGBABuffer = nullptr;
	DecodeImageDelegate->BindRaw(this, &FDecodeImageTask::DecodeImage);
	Keyframe = nullptr;
}

FDecodeImageTask::~FDecodeImageTask() {
	//printf("Processed %d frames.\n", frame_cnt);
	if (codec) {
		if (vpx_codec_destroy(codec)) {
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "Failed to destroy codec");
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, "");
			}

			//die_codec(&codec, "Failed to destroy codec");
		}
	}
	if (RGBABuffer) {
		FMemory::Free(RGBABuffer);
	}
	DecodedTexture = nullptr;
	ImageDecodedDelegate = nullptr;
}

void FDecodeImageTask::DoWork() {
	bool result = DecodeImage();

	if (result) {
		if (FirstFrame) {
			FirstFrame = false;
			//Call the delegate with the decoded texture
			ImageDecodedDelegate->ExecuteIfBound(DecodedTexture); 
		}				
	}
}

/** Scene Texture  */
void FDecodeImageTask::InitDecoder(int32 _ImageWidth, int32 _ImageHeight) {
	ImageWidth = _ImageWidth;
	ImageHeight = _ImageHeight;

	DecodedBufferSize = ImageWidth * ImageHeight * NumberOfChannels;
	DecodedImageBuffer = (uint8*)FMemory::Malloc(DecodedBufferSize);
	//EPixelFormat::PF_R8G8B8A8
	//EPixelFormat::PF_B8G8R8A8
	//EPixelFormat::PF_R5G6B5_UNORM
	
	DecodedTexture = UTexture2D::CreateTransient(ImageWidth, ImageHeight, PixelFormat);
	//DecodedTexture->MipGenSettings = TMGS_NoMipmaps;
	DecodedTexture->SRGB = false;
	DecodedTexture->CompressionSettings = TC_VectorDisplacementmap;
	DecodedTexture->UpdateResource();
	DecodedTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, ImageWidth, ImageHeight);
	DecoderInitialized = true;

	RGBABuffer = new uint8[ImageWidth*ImageHeight*NumberOfChannels];

	codec = new vpx_codec_ctx_t;
	vpx_codec_dec_cfg cfg;
	cfg.threads = 1;
	cfg.w = ImageWidth;
	cfg.h = ImageHeight;
	if (vpx_codec_dec_init(codec, VPX_DECODE_INTERFACE, &cfg, 0)) {
		//Failed to initialize decoder
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "Failed to initialize decoder");
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, "");
		}
	}
}

bool FDecodeImageTask::DecodeImage() {
	bool result = false;

	while (!KeyframesToDecode.IsEmpty()) {
		if (KeyframesToDecode.Dequeue(Keyframe)) {
			if (Keyframe->IsKeyFrame) {
				RecievedKeyframe = true;
			}

			vpx_codec_err_t res = vpx_codec_decode(codec, Keyframe->ImageBuffer, Keyframe->ImageDataBufferSize, nullptr, 1); //1 = realtime

			//If that was the last frame to process we update the texture 
			if (KeyframesToDecode.IsEmpty()) {
				vpx_codec_iter_t  iter = NULL;
				vpx_image_t      *img; //TODO should this be freed?

									   // Get a YV12 image frame from the decoder
				img = vpx_codec_get_frame(codec, &iter);

				if (!img) {
					return result;
				}

				//Optimise by not updating the texture until a keyframe has been encounted
				if (ASynOpticonState::IsProcessingReplayData && !RecievedKeyframe) {
					delete Keyframe;
					Keyframe = nullptr;
					continue;
				}
				else {
					RecievedKeyframe = false;
				}

				const uint8_t* y_plane = img->planes[VPX_PLANE_Y];
				const uint8_t* u_plane = img->planes[VPX_PLANE_U];
				const uint8_t* v_plane = img->planes[VPX_PLANE_V];

				if (PixelFormat == EPixelFormat::PF_R5G6B5_UNORM) {
					libyuv::I420ToRGB565(y_plane, img->stride[VPX_PLANE_Y],
						u_plane, img->stride[VPX_PLANE_U],
						v_plane, img->stride[VPX_PLANE_V],
						RGBABuffer, ImageWidth * NumberOfChannels,
						ImageWidth, ImageHeight);
				}
				else if (PixelFormat == EPixelFormat::PF_ATC_RGB) {
					libyuv::I420ToRGB565(y_plane, img->stride[VPX_PLANE_Y],
						u_plane, img->stride[VPX_PLANE_U],
						v_plane, img->stride[VPX_PLANE_V],
						RGBABuffer, ImageWidth * NumberOfChannels,
						ImageWidth, ImageHeight);
				}
				else {
					libyuv::I420ToARGB(y_plane, img->stride[VPX_PLANE_Y],
						u_plane, img->stride[VPX_PLANE_U],
						v_plane, img->stride[VPX_PLANE_V],
						RGBABuffer, ImageWidth * NumberOfChannels,
						ImageWidth, ImageHeight);
				}

				if(!ASynOpticonState::IsProcessingReplayData)
				UpdateTextureRegions(DecodedTexture, (int32)0, (uint32)1, DecodedTextureRegion, (uint32)(ImageWidth * NumberOfChannels), (uint32)4, RGBABuffer, false);
				result = true;
			}
			//If we have more frames we get the decoded frames from the decoder but don't show them
			else 
			{
				vpx_codec_iter_t  iter = NULL;
				while (vpx_codec_get_frame(codec, &iter)) {

				}
			}

			delete Keyframe;
			Keyframe = nullptr;
		} 
	}

	return result;
}

void FDecodeImageTask::DecodeImage(ImageDataStructures::VP8EncodedImageData* _EncodedImage) {
	if (!DecoderInitialized && _EncodedImage) {
		InitDecoder(_EncodedImage->ImageWidth, _EncodedImage->ImageHeight);
	}

	if (_EncodedImage) {
		//Make a copy of the data and add it to the processing queue
		ImageDataStructures::VP8EncodedImageData* InKeyframe = new ImageDataStructures::VP8EncodedImageData();
		FMemory::Memcpy(InKeyframe->ImageBuffer, _EncodedImage->ImageBuffer, _EncodedImage->ImageDataBufferSize);
		InKeyframe->ImageDataBufferSize = _EncodedImage->ImageDataBufferSize;
		KeyframesToDecode.Enqueue(InKeyframe);
	}
}

void FDecodeImageTask::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
							);
					}
				}
		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}

/********************************************************************
*							FImageEncodeTask					    *
*********************************************************************/
//FEncodeImageTask::FEncodeImageTask(FImageEncodedDelegate* _ImageEncodedDelegate, FImageEncodeImageDelegate* _EncodeImageDelegate, URadiantWebViewRenderComponent* _RenderComponent) {
//	ImageEncodedDelegate = _ImageEncodedDelegate;
//	RenderComponent = _RenderComponent;
//
//	EncoderInitialized = false;
//	KeyframeInterval = ASynOpticonState::GetScreenSaveRateFPS() * 1;
//	ForceKeyframe = false;
//
//	Codec = nullptr;
//	OutEncodedImage = new ImageDataStructures::VP8EncodedImageData();
//	OutputBuffer = nullptr;
//	ImageToEncode = nullptr;
//	RawTextureData = nullptr;
//	RawTextureBufferSize = 0;
//
//	FrameCount = 0;
//	OutYuvImage = nullptr;
//
//	IsWorking = false;
//	_EncodeImageDelegate->BindRaw(this, &FEncodeImageTask::EncodeImage);
//}

FEncodeImageTask::~FEncodeImageTask() {
	if (EncoderInitialized) {
		// Flush encoder. 
		/*while (encode_frame(codec, NULL, -1, 0)) {  //TODO might need to flush the codec after every frame?
		}*/

		delete OutEncodedImage;

		delete[] OutputBuffer;
		delete[] OutYuvImage;
		delete[] RawTextureData;

		if (Codec && vpx_codec_destroy(Codec)) {
			//die_codec(&codec, "Failed to destroy codec.");
		}
	}
}

bool FEncodeImageTask::InitEncoder(int32 _ImageWidth, int32 _ImageHeight) {
	ImageWidth = _ImageWidth;
	ImageHeight = _ImageHeight;
	NumberOfChannels = 4; //TODO make sure this value is consistent with the selected texture format. 2 == EPixelFormat::PF_R5G6B5_UNORM (5+6+5 = 16/8 = 2)

	FirstFrame = true;

	const vpx_codec_iface_t* Iface = VPX_ENCODE_INTERFACE;
	if (!Iface) {
		//Could not initialize interface
		return false;
	}

	vpx_codec_enc_cfg_t Config;
	vpx_codec_err_t Res = vpx_codec_enc_config_default(Iface, &Config, 0);
	if (Res) {
		//Could not configure codec
		return false;
	}

	Config.rc_target_bitrate = ImageWidth * ImageHeight *
		Config.rc_target_bitrate / Config.g_w / Config.g_h;

	Config.g_w = ImageWidth;
	Config.g_h = ImageHeight;
	Config.g_threads = 1;
	//Config.rc_target_bitrate = 2000;

	Codec = new vpx_codec_ctx_t;

	Res = vpx_codec_enc_init(Codec, Iface, &Config, 0);
	if (Res)
	{
		delete Codec;
		Codec = nullptr;
		return false;
		//Failed to initialise codec
		//THROW("failed initialize codec: %s", vpx_codec_err_to_string(res));
	}

	RawTextureBufferSize = ImageWidth * ImageHeight * NumberOfChannels;

	OutputBuffer = new uint8_t[ImageDataStructures::gOutputBufferSize];
	OutYuvImage = new uint8_t[ImageWidth * ImageHeight * 3 / 2];
	RawTextureData = new uint8_t[RawTextureBufferSize];
	EncoderInitialized = true;
	return EncoderInitialized;
}

int FEncodeImageTask::EncodeImage(vpx_image_t *Image, int FrameIndex, int Flags) {

	if (ForceKeyframe) {
		Flags = VPX_EFLAG_FORCE_KF;
		ForceKeyframe = false;
	}

	const vpx_codec_err_t Res = vpx_codec_encode(Codec, Image, FrameIndex, 1, Flags, VPX_DL_REALTIME);
	if (Res != VPX_CODEC_OK) {
		//die_codec(codec, "Failed to encode frame");
		//cerr << "Failed to encode frame";
		return 0;
	}

	int GotPkts = 0;
	vpx_codec_iter_t Iter = NULL;
	const vpx_codec_cx_pkt_t *Pkt = NULL; //Encoder output

	size_t BytesWritten = 0;
	int Keyframe = 0;
	while ((Pkt = vpx_codec_get_cx_data(Codec, &Iter)) != NULL) {
		if (Pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
			GotPkts = 1;
			Keyframe = (Pkt->data.frame.flags & VPX_FRAME_IS_KEY) != 0;
			//printf(keyframe ? "K" : ".");

			memcpy(OutputBuffer, Pkt->data.frame.buf, Pkt->data.frame.sz);

			OutputBuffer += Pkt->data.frame.sz;
			BytesWritten += Pkt->data.frame.sz;
		}
	}

	if (GotPkts) {
		OutputBuffer -= BytesWritten;

		if ((FirstFrame && FrameIndex == 0) || BytesWritten > 100000) {
			//if (Keyframe) {
				ForceKeyframe = true;
				FirstFrame = false;
			//}
				FrameCount--;
			return false;
		}

		OutEncodedImage->ImageDataBufferSize = BytesWritten;
		FMemory::Memcpy(OutEncodedImage->ImageBuffer, OutputBuffer, BytesWritten);
		OutEncodedImage->ImageWidth = ImageWidth;
		OutEncodedImage->ImageHeight = ImageHeight;
		OutEncodedImage->IsKeyFrame = Keyframe;

		//if (GEngine)
		//{
		//	FColor msgColor;
		//	if (BytesWritten >= 100000) {
		//		msgColor = FColor::Red;
		//	}
		//	else {
		//		msgColor = FColor::Cyan;
		//	}

		//	FString msg = "Frame Encoded - keyframe " + FString::FromInt(Keyframe) + " size: " + FString::FromInt(BytesWritten);
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, msgColor, msg);
		//}
	}

	return GotPkts;
}

void FEncodeImageTask::EncodeImage(uint8* Image) {
	if (!IsWorking) {
		IsWorking = true;
		ImageToEncode = Image;
	}
}

bool FEncodeImageTask::EncodeImage() {
	//if (ImageToEncode)
	//{
	//	//Use this if getting the data from the RadiantUI
	//	//if (RenderComponent)
	//	//{
	//	//	bool LockAcquired = RenderComponent->EnterCriticalSection();

	//	//	if (!LockAcquired)
	//	//	{
	//	//		return false;
	//	//	}
	//	//	if (ImageToEncode)
	//	//	{
	//	//		FMemory::Memcpy(RawTextureData, ImageToEncode, RawTextureBufferSize);
	//	//		RenderComponent->LeaveCriticalSection();			
	//	//	}
	//	//}

	//	//Use this if getting the data from a UTexture2D
	//	//FTexture2DMipMap& readMip = ImageToEncode->PlatformData->Mips[0];
	//	//readMip.BulkData.GetCopy((void**)&RawTextureData);

	//	//Remove our reference to the UTexture2D
	//	ImageToEncode = nullptr;
	//	 
	//	int Flags = 0;
	//	if (KeyframeInterval > 0 && (FrameCount % KeyframeInterval == 0)) {
	//		Flags = VPX_EFLAG_FORCE_KF;
	//	}

	//	uint8_t* YPlane = OutYuvImage;
	//	uint8_t* UPlane = YPlane + ImageWidth * ImageHeight;
	//	uint8_t* VPlane = UPlane + (ImageWidth * ImageHeight / 4);

	//	int Result = 0;

	//	Result = libyuv::ARGBToI420(RawTextureData, ImageWidth*NumberOfChannels,
	//		YPlane, ImageWidth, UPlane, (int32)ImageWidth / 2, VPlane, (int32)ImageWidth / 2,
	//		ImageWidth, ImageHeight);
	//										  
	//	vpx_image_t ImageWrapper;
	//	vpx_img_wrap(&ImageWrapper, VPX_IMG_FMT_I420, ImageWidth, ImageHeight, 1, OutYuvImage);

	//	if (EncodeImage(&ImageWrapper, FrameCount++, Flags)) {
	//		//frames_encoded++;
	//		return true;
	//	}	
	//}
	return false;
}

void FEncodeImageTask::DoWork() {
	//if (ImageToEncode) {
	//	bool Result = false;
	//	if (EncoderInitialized) {
	//		//Result = EncodeImage();
	//	}
	//	else {
	//		//FVector2D TextureSize = RenderComponent->GetTextureSize();
	//		//InitEncoder(ImageToEncode->PlatformData->SizeX, ImageToEncode->PlatformData->SizeY); Use this if using a UTexture directly
	//		//InitEncoder(TextureSize.X, TextureSize.Y); //Use this if using the Radiant UI version...
	//		//Result = EncodeImage();
	//	}

	//	if (Result) {
	//		//Call the delegate with the decoded texture
	//		//ImageEncodedDelegate->ExecuteIfBound(OutEncodedImage);
	//	}
	//}

	IsWorking = false;
}