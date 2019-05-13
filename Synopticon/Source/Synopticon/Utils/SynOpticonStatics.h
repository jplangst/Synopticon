// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Engine.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

#include "Levmar.h"

#include "GeomTools.h"
#include "Engine/Polys.h"

#include "PlatformFilemanager.h"
#include "PlatformFile.h"
#include "IPlatformFileModule.h"
#include "Templates/SharedPointer.h"
#include "WidgetRenderer.h"
#include "UserWidget.h"
#include "WidgetRenderer.h"
#include "Engine/TextureRenderTarget2D.h"

#include "MediaTexture.h"
#include "MediaPlayer.h"

#include "SystemClasses/SynOpticonCustomType.h"

#include "SynOpticonStatics.generated.h"


//using namespace std;

/** The length of the glasses orientaton vector */
#define VECTOR_LENGTH 10000

#define FLOAT_PERCISION 10000

//Enum that can be used to control which sound effect is t be played
UENUM(BlueprintType)		
enum class ESoundEffectEnum : uint8
{
	SE_PointModelled 			UMETA(DisplayName = "PointModelledSound"),
	SE_PointRemoved 			UMETA(DisplayName = "PointRemovedSound")
};

USTRUCT(BlueprintType) 
struct FConvergenceStruct
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Convergence")
		FVector point;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Convergence")
		float distance;
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EFileDialogTypeEnum : uint8
{
	FT_Open_Settings 	UMETA(DisplayName = "OpenSettingsFile"),
	FT_Save_Settings 	UMETA(DisplayName = "SaveSettingsFile"),
	FT_Open_SOObjects 	UMETA(DisplayName = "OpenSynOpticonObjectsFile"),
	FT_Save_SOObjects 	UMETA(DisplayName = "SaveSynOpticonObjectsFile"),
	FT_Open_SOActors		UMETA(DisplayName = "OpenSynOpticonActorsFile"),
	FT_Save_SOActors		UMETA(DisplayName = "SaveSynOpticonActorsFile"),
	FT_Open_SOMovableObjects		UMETA(DisplayName = "OpenMovableObjectsFile"),
	FT_Save_SOMovableObjects		UMETA(DisplayName = "SaveMovableObjectsFile")
};

/**
* Query flags
* - COLLIDABLE_OBJECT: Collidable objects (e.g., table, screens)
* - NOT_COLLIDABLE_OBJECT: Not colliable objects (e.g., room, glasses)
*/
enum QueryFlags
{
	COLLIDABLE_OBJECT = 1 << 0,
	NOT_COLLIDABLE_OBJECT = 1 << 1,
	HIGHLIGHTAVLE_OBJECT = 1 << 2
};

USTRUCT(BlueprintType)
struct FFileDataStruct
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "File Browser")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "File Browser")
		bool IsFile;
};

UCLASS()
class USynOpticonStatics : public UObject
{
	GENERATED_UCLASS_BODY()
private:
		
	static FLinearColor GenerateColor(float h, float s, float v);

public:

	/** Point inside Polygon methods */

	//Checks if the given point is to the left|on|right of an infinite line
	static float PointIsLeft(FVector2D P0, FVector2D P1 , FVector2D P2);

	//Checks if a point is inside a polygon using the crossing number test
	//Returns true if it is inside
	UFUNCTION(BlueprintCallable, Category = "Point inside Polygon")
	static bool CrossingNumberTest(FVector2D Point, TArray<FVector2D> Polygon);

	//Checks if a point is inside a polygon using the winding number test
	//Returns true if the point is inside
	UFUNCTION(BlueprintCallable, Category = "Point inside Polygon")
	static bool WindingNumberTest(FVector2D Point, TArray<FVector2D> Polygon);
	//

	//Creates and returns a MediaTexture reference
	UFUNCTION(BlueprintCallable, Category = "MediaPlayer")
		static void CreateMediaPlayer(UObject* Owner, UMediaPlayer*& MediaPlayer, UMediaTexture*& MediaTexture);
	
	UFUNCTION(BlueprintCallable, Category = "CBluePrintLibrary")
		static TArray<UObject*>  LoadObjectLibrary(const FString& Path, TSubclassOf<UObject> ObjectClass);

	/** Convert integer number to FString */
	static FString ConvertNumberToString(int Number);

	/** Convert float number to FString */
	static FString ConvertNumberToString(float Number);

	/** Convert double number to FString */
	static FString ConvertNumberToString(double Number);

	/** Convert FString to Double */
	static double ConvertFStringToDouble(FString String);

	/** Convert FString to Boolean */
	static bool ConvertFStringToBoolean(FString input);

	/** Convert Boolean to FString */
	static FString ConvertBooleanToFString(bool input);

	/** Convert FString to Float */
	static float ConvertFStringToFloat(FString input);

	/** Convert FString to Integer */
	static int ConvertFStringToInt(FString input);

	/** Convert Tracker vector to FVector */
	//static FVector ConvertTrackerVectorToFVector(vrpn_float64 input[], float factor = 1);

	/** Convert Tracker vector to FVector */
	//static FVector ConvertTrackerVectorToFVector(vrpn_float64 x, vrpn_float64 y, vrpn_float64 z, float factor = 1);

	/** Convert Tracker quatarnion to FQuat */
	//static FQuat ConvertTrackerQuatToFQuat(vrpn_float64 input[], float factor = 1);

	/**
	* Calculate the location of the fourth point and return the points in the correct order for rendering. 
	* The points are returned as TArray with 4 elements. 
	*/
	UFUNCTION(BlueprintCallable, Category = "Rigid Body Utility")
	static TArray<FVector> CalRigidBodyWH(FVector pMaker1, FVector pMaker2, FVector pMaker3);

	/** Get the signed angle between two vectors */
	static float GetSignedAngle(FVector v1, FVector v2);

	/** Convert from Position Tracking coordinate system to UE coordinate system */
	static FVector ConvertXYZtoXZYCoordinateSystem(const FVector vector);

	/** Convert from Position Tracking coordinate system to UE coordinate system */
	static FQuat ConvertXYZtoXZYCoordinateSystem(const FQuat quat);

	/** Convert from Eye Tracking coordinate system to UE coordinate system */
	static FVector ConvertNegXYZtoXZYCoordinateSystem(const FVector vector);

	/** Convert from Position Tracking coordinate system to UE coordinate system */
	static FVector ConvertFromMotiveToUnrealEngineCoordinateSystem(FVector MotiveVector, FVector Offset, bool Horizontal, float Factor = 1);

	/** Convert from Position Tracking coordinate system to UE coordinate system */
	static FQuat ConvertFromMotiveToUnrealEngineCoordinateSystem(FQuat MotiveQuat, bool Horizontal);

	/** Convert from Position Tracking coordinate system to UE coordinate system */
	static FQuat ConvertXYZtoZYnegXCoordinateSystem(const FQuat quat);

	/** Convert from Eye Tracking coordinate system to UE coordinate system */
	static FVector ConvertXYZtoZYnegXCoordinateSystem(const FVector vector);

	/** Find the shortest line that is perpendicular to two specified lines. 
	* @param
	* u - direction vector of the first line.
	* v - direction vector of the second line.
	* A - position of the fisrt line.
	* B - poistion of the second line.
	* @return 
	* two end points of that line */
	static TPair<FVector, FVector> TwoIntersectionPoints(FVector u, FVector v, FVector A, FVector B);

	/** Find the shortest distance between two lines.
	* @param
	* u - direction vector of the first line.
	* v - direction vector of the second line.
	* A - position of the fisrt line.
	* B - poistion of the second line. 
	*/
	static float DistanceBetweenSkewLines(FVector u, FVector v, FVector A, FVector B);

	/** Find the shortest line that is perpendicular to two specified lines.
	* @param
	* u - direction vector of the first line.
	* v - direction vector of the second line.
	* A - position of the fisrt line.
	* B - poistion of the second line.
	* @return 
	* the mid point of that line and the shortest distance */
	static TPair<FVector, float> ConvPointAndDist(FVector u, FVector v, FVector A, FVector B);

	UFUNCTION(BlueprintCallable, Category = "ChartHUDUtilities")
		static FString ConvertNameToURL(FString name);

	UFUNCTION(BlueprintCallable, Category = "URL Utility")
	static int IsValidURL(FString name);

	/** Convert from Pixels to Centimetres */
	static float PixelToCm(float pixels);

	static double func(double* params, int x, void* input);
	static double linear_func(double* params, int x, void* input);
	static void grad(double* g, double* params, int x, void* input);
	static void linear_grad(double* g, double* params, int x, void* input);


	static TArray<double> estimator(TArray<TArray<double>> data);
	//static bool levmar(void *func, void *jacobian);

	static bool CreateBoxGeometry(float width, float length, float height, TArray<FVector> &OutVerticies, TArray<int32> &OutTriangleIndices, TArray<FVector> &OutBoxVerticies, TArray<int32> &OutBoxTriangleIndices, TArray<FVector2D> &OutBoxUVs);

	//Check if a point is inside a triangle using UV coordinates.
	static bool PointInTriangle2DUVs(FVector2D p, FVector2D p0, FVector2D p1, FVector2D p2);

	UFUNCTION(BlueprintCallable, Category = "File Utility")
		static bool RenameFile(FString oldName, FString directory, FString newName);

	static UTexture2D* LoadTextureFromImage(FString Filepath);

	UFUNCTION(BlueprintCallable, Category = "Texture Loading")
	static UTexture2D* LoadTextureFromImage(FString FullFilePath, FString ImageFormat, bool& IsValid, int32& Width, int32& Height);

	UFUNCTION(BlueprintCallable, Category = "Texture Loading")
		static UTexture2D* TextureFromWidget(UUserWidget *const Widget, const FVector2D &DrawSize);

	UFUNCTION(BlueprintCallable, Category = "Geometry")
		static void EstimatingCenterOfSphere(TArray<FVector> Points, FVector& Center, float& Radius);

	UFUNCTION(BlueprintCallable, Category = "Geometry")
		static void CalculateCircle(FVector P1, FVector P2, FVector P3, float& a, float& b, float& r);

	UFUNCTION(BlueprintCallable, Category = "Geometry")
		static float CircularFormula(float a, float b, float r, float x);

	static void SolveSystemOf3Equations(FMatrix A, FPlane B, FVector& outX);
	static void PlaneEquationGiven3Points(FVector P1, FVector P2, FVector P3, FVector& Normal, float& D);
	static void CircleFromCuttingSphereUsingPlane(FVector P1, FVector P2, FVector P3, FPlane& Plane, FVector& Center, float& Radius);
	UFUNCTION(BlueprintPure, Category = "Geometry")
		static TArray<FVector> CreateArcShape3D(TArray<FVector> Points, int Subdivisions);
	
	UFUNCTION(BlueprintPure, Category = "Geometry")
		static TArray<FVector> CreatePyramidShape(TArray<FVector> Points, int Subdivisions);

	UFUNCTION(BlueprintPure, Category = "Geometry")
		static TArray<FVector> CreateArcShape3DV2(TArray<FVector> Points, int Subdivisions, bool bFullCircle);

	UFUNCTION(BlueprintCallable, Category = "Geometry")
		static TArray<FVector2D> CreateUVsForCurvedScreen(TArray<FVector> Verticies);

	/**
	*	4 points define the intersection circle between a sphere and a plane
	*	@param Plane: A(x-x0) + B(y-y0) + C(z-z0) = D : first using 3 given points to find the plane
	*	@param Center: (x-xC^)^2 + (y-yC)^2 + (z-zC)^2 = R^2 : in which (xC, yC, zC) is the center of the sphere, R is the radisu.
		applying 3 given points to the sphere equation gives us 3 equations
		combining with the plane equation => solve a system of 4 equations to find 4 variables: xC, yC, zC, R
	*	@param Radius
	*	@param inX: apply the given X to the equation of the plane and the sphere, solve the system of 2 equations to find other 2 variables: y & z
	*	@out outY
	*	@out outZ
	*/
	static void CircularYAndZGivenX(FPlane Plane, FVector Center, float Radius, float inX, FVector2D& outY, FVector2D& outZ);
	static bool SolveQuadraticEquation(double a, double b, double c, double& out1, double& out2);
	static bool CheckIfPointsOnSameSide(FVector P1, FVector P2, FVector P3, FVector P);

	static void CalculateBoundingBox(const TArray<FVector> &vertices, FVector &Min, FVector &Max);

	UFUNCTION(BlueprintCallable, Category = "Sequence Object List")
		static void SortSequenceObjects(UPARAM(ref) TArray<FSequenceObjectData> &SequenceObjectList);

	UFUNCTION(BlueprintCallable, Category = "Generate Colors")
		static TArray<FLinearColor> GenerateChartColors(float NmbColors);

	UFUNCTION(BlueprintCallable, Category = "UVs")
		static FVector2D FindUVInTriangle(TArray<FVector> TriangleVerticies, TArray<FVector2D> TriangleUVs, FVector HitLocation);

	//================ File Browser ================================
	UFUNCTION(BlueprintCallable, Category = "File Browser")
		static TArray<FFileDataStruct> GetListOfFileInDirectory(FString path, FString ext);
	UFUNCTION(BlueprintPure, Category = "File Browser")
		static FString GetGameDirectoryPath();
	UFUNCTION(BlueprintCallable, Category = "File Browser")
		static FString GetValidFileName(FString path, FString name, FString ext);
	UFUNCTION(BlueprintCallable, Category = "File Browser")
		static FString GetValidFolderName(FString path, FString name);
	UFUNCTION(BlueprintCallable, Category = "File Browser")
		static bool VerifyDirectory(FString path);
	UFUNCTION(BlueprintCallable, Category = "File Browser")
		static bool CreateNewDirectory(FString path);
	UFUNCTION(BlueprintCallable, Category = "File Browser")
		static bool DeleteFileOrDirectory(FString path, bool isFile);
	UFUNCTION(BlueprintCallable, Category = "File Browser")
		static bool CopyFileToDestination(FString Filepath, FString Destination);

	UFUNCTION(BlueprintPure, Category = "File Browser")
	static FString GetTextureFolder();

	UFUNCTION(BlueprintCallable, Category = "Mesh Triangulation")
	static int32 Triangulate(TArray<FVector> vertices, TArray<int32> &OutTriangles);

	static int32 TriangulatePoly(ABrush* InOwnerBrush, TArray<FPoly>& OutTriangles, TArray<FVector> Vertices, FPoly InitialPoly);

	static FVector2D MapFromUnitSquareToUnitCircle(FVector2D UnitSquareCoordinate);
};

static FORCEINLINE FString GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero = true)
{
	//Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	if (FMath::Abs(TheFloat - Rounded) < FMath::Pow(10, -1 * Precision))
	{
		TheFloat = Rounded;
	}
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	return FText::AsNumber(TheFloat, &NumberFormat).ToString();
}

