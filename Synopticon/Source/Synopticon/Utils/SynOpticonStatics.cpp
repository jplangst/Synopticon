// Fill out your copyright notice in the Description page of Project Settings.

#include "SynOpticonStatics.h"
//#include "Synopticon.h"


USynOpticonStatics::USynOpticonStatics(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

float USynOpticonStatics::PointIsLeft(FVector2D P0, FVector2D P1, FVector2D P2) {
	return ((P1.X - P0.X)*(P2.Y - P0.Y) - (P2.X - P0.X)*(P1.Y-P0.Y));
}

bool USynOpticonStatics::CrossingNumberTest(FVector2D Point, TArray<FVector2D> Polygon) {
	int CN = 0; //The crossing number counter

	//Iterate over all the edges in the Polygon
	for (int il = 0; il<Polygon.Num(); il++) { 
		int i = il%(Polygon.Num()-1);
		if (((Polygon[i].Y <= Point.Y) && (Polygon[i + 1].Y > Point.Y))       // an upward crossing
			|| ((Polygon[i].Y > Point.Y) && (Polygon[i + 1].Y <= Point.Y))) { // a downward crossing
			
		    // compute  the actual edge-ray intersect x-coordinate
			float vt = (float)(Point.Y - Polygon[i].Y) / (Polygon[i + 1].Y - Polygon[i].Y);

			if (Point.X < Polygon[i].X + vt * (Polygon[i + 1].X - Polygon[i].X)) { // P.x < intersect
				CN++;   // a valid crossing of y=Point.Y right of Point.X
			}
		}
	}

	//Bitwise AND: 0 if even (out), and 1 if  odd (in) 
	return (CN & 1) == 1;
}

bool USynOpticonStatics::WindingNumberTest(FVector2D Point, TArray<FVector2D> Polygon) {
	int WN = 0; // the  winding number counter

	//Iterate through all edges of the polygon
	for (int il = 0; il<Polygon.Num(); il++) {
		int i = il%(Polygon.Num()-1);
		if (Polygon[i].Y <= Point.Y) {            // start y <= P.y
			if (Polygon[i + 1].Y  > Point.Y)      // an upward crossing
				if (USynOpticonStatics::PointIsLeft(Polygon[i], Polygon[i + 1], Point) > 0)  // P left of  edge
					WN++; // have  a valid up intersect
		}
		else { // start Y > Point.Y (no test needed)
			if (Polygon[i + 1].Y <= Point.Y)     // a downward crossing
				if (USynOpticonStatics::PointIsLeft(Polygon[i], Polygon[i + 1], Point) < 0)  // P right of  edge
					WN--;            // have  a valid down intersect
		}
	}

	//Outside only if WN == 0
	return WN != 0;
}

int32 USynOpticonStatics::Triangulate(TArray<FVector> vertices, TArray<int32> &OutTriangles)
{
	FPoly newPolygon;
	for (int i = 0; i < vertices.Num(); i++)
	{
		newPolygon.InsertVertex(i, vertices[i]);
	}

	TArray<FPoly> outPolys;
	int numTriangles = TriangulatePoly(nullptr, outPolys, vertices, newPolygon);

	for (int i = 0; i < outPolys.Num(); i++)
	{
		int Index0;
		vertices.Find(outPolys[i].Vertices[2], Index0);
		OutTriangles.Add(Index0);

		int Index1;
		vertices.Find(outPolys[i].Vertices[1], Index1);
		OutTriangles.Add(Index1);

		int Index2;
		vertices.Find(outPolys[i].Vertices[0], Index2);
		OutTriangles.Add(Index2);
	}
	return numTriangles;
}

int32 USynOpticonStatics::TriangulatePoly(ABrush* InOwnerBrush, TArray<FPoly>& OutTriangles, TArray<FVector> Vertices, FPoly InitialPoly)
{
	if (Vertices.Num() < 3)
	{
		return 0;
	}

	FClipSMPolygon Polygon(0);

	for (int32 v = 0; v < Vertices.Num(); ++v)
	{
		FClipSMVertex vtx;
		vtx.Pos = Vertices[v];

		// Init other data so that VertsAreEqual won't compare garbage
		vtx.TangentX = FVector::ZeroVector;
		vtx.TangentY = FVector::ZeroVector;
		vtx.TangentZ = FVector::ZeroVector;
		vtx.Color = FColor(0, 0, 0);
		for (int32 uvIndex = 0; uvIndex<ARRAY_COUNT(vtx.UVs); ++uvIndex)
		{
			vtx.UVs[uvIndex] = FVector2D(0.f, 0.f);
		}


		Polygon.Vertices.Add(vtx);
	}

	Polygon.FaceNormal = InitialPoly.Normal;

	// Attempt to triangulate this polygon
	TArray<FClipSMTriangle> Triangles;
	if (FGeomTools::TriangulatePoly(Triangles, Polygon))
	{
		// Create a set of FPolys from the triangles
		OutTriangles.Empty();
		FPoly TrianglePoly;

		for (int32 p = 0; p < Triangles.Num(); ++p)
		{
			FClipSMTriangle* tri = &(Triangles[p]);

			TrianglePoly.Init();
			TrianglePoly.Base = tri->Vertices[0].Pos;

			TrianglePoly.Vertices.Add(tri->Vertices[0].Pos);
			TrianglePoly.Vertices.Add(tri->Vertices[1].Pos);
			TrianglePoly.Vertices.Add(tri->Vertices[2].Pos);

			OutTriangles.Add(TrianglePoly);
		}
	}
	return OutTriangles.Num();
}

FVector2D USynOpticonStatics::MapFromUnitSquareToUnitCircle(FVector2D UnitSquareCoordinate)
{
	return FVector2D(UnitSquareCoordinate.X * FMath::Sqrt(1 - UnitSquareCoordinate.Y * UnitSquareCoordinate.Y / 2),
		UnitSquareCoordinate.Y * FMath::Sqrt(1 - UnitSquareCoordinate.X * UnitSquareCoordinate.X / 2));
}

double USynOpticonStatics::ConvertFStringToDouble(FString string)
{
	return FCString::Atof(*string);
}

FString USynOpticonStatics::ConvertNumberToString(int number)
{
	FString Str = FString::FromInt(number);
	FString str(TCHAR_TO_UTF8(*Str));
	return str;
}

bool USynOpticonStatics::CopyFileToDestination(FString Filepath, FString Destination) {
	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	bool Result = PlatformFile.CopyFile(*Destination, *Filepath);

	return Result;
}

FString USynOpticonStatics::GetTextureFolder() {
	return FPaths::ProjectDir() + "Resources/Textures/";
}

FString USynOpticonStatics::ConvertNumberToString(float number)
{
	FString Str = FString::SanitizeFloat(number);
	FString str(TCHAR_TO_UTF8(*Str));
	return str;
}

FString USynOpticonStatics::ConvertNumberToString(double number)
{
	FString Str = FString::SanitizeFloat(number);
	FString str(TCHAR_TO_UTF8(*Str));
	return str;
}

bool USynOpticonStatics::ConvertFStringToBoolean(FString input)
{
	if (input.Compare("true") == 0) return true;
	return false;
}

FString USynOpticonStatics::ConvertBooleanToFString(bool input)
{
	if (input)
		return "true";
	return "false";
}

float USynOpticonStatics::ConvertFStringToFloat(FString input)
{
	return FCString::Atof(*input);
}

int USynOpticonStatics::ConvertFStringToInt(FString input)
{
	return FCString::Atoi(*input);
}

float USynOpticonStatics::GetSignedAngle(FVector v1, FVector  v2)
{
	v1.Normalize(); v2.Normalize();
	return FMath::Acos(FVector::DotProduct(v1, v2));
}

TArray<FVector> USynOpticonStatics::CalRigidBodyWH(FVector pMaker1, FVector pMaker2, FVector pMaker3)
{
	TArray<FVector> points;
	FVector vMainMarker, vTempMaker1, vTempMaker2;
	FVector v21 = pMaker2 - pMaker1;
	FVector v31 = pMaker3 - pMaker1;
	float rA1 = GetSignedAngle(v21, v31);
	if (rA1 > 180) rA1 = rA1 - 180;

	FVector v12 = pMaker1 - pMaker2;
	FVector v32 = pMaker3 - pMaker2;
	float rA2 = GetSignedAngle(v12, v32);
	if (rA2 > 180) rA2 = rA2 - 180;

	FVector v13 = pMaker1 - pMaker3;
	FVector v23 = pMaker2 - pMaker3;
	float rA3 = GetSignedAngle(v13, v23);
	if (rA3 > 180) rA3 = rA3 - 180;

	if (rA1 > rA2 && rA1 > rA3)
	{
		vMainMarker = pMaker1;
		vTempMaker1 = pMaker2;
		vTempMaker2 = pMaker3;
	}

	else if (rA2 > rA1 && rA2 > rA3)
	{
		vMainMarker = pMaker2;
		vTempMaker1 = pMaker1;
		vTempMaker2 = pMaker3;
	}

	else if (rA3 > rA1 && rA3 > rA2)
	{
		vMainMarker = pMaker3;
		vTempMaker1 = pMaker1;
		vTempMaker2 = pMaker2;
	}

	if (vTempMaker1.Z > vTempMaker2.Z)
	{
		FVector vTempMarker = vTempMaker1;
		vTempMaker1 = vTempMaker2;
		vTempMaker2 = vTempMarker;
	}

	FVector vTempMaker3 = (vTempMaker1 + vTempMaker2) - vMainMarker;
	points.Add(vTempMaker1);
	points.Add(vMainMarker);
	points.Add(vTempMaker2);
	points.Add(vTempMaker3); 

	return points;
}

FVector USynOpticonStatics::ConvertXYZtoXZYCoordinateSystem(const FVector vector)
{
	return FVector(vector.X, vector.Z, vector.Y);
}

FQuat USynOpticonStatics::ConvertXYZtoXZYCoordinateSystem(const FQuat quat)
{
	FVector axis;
	float angle;
	quat.ToAxisAndAngle(axis, angle);
	float temp = axis.Y;
	axis.Y = axis.Z;
	axis.Z = -temp;
	return FQuat(axis, angle) * FQuat::MakeFromEuler(FVector(0, 0, 180)); //TODO might not need the rotation anymore
}

FVector USynOpticonStatics::ConvertNegXYZtoXZYCoordinateSystem(const FVector vector)
{
	return FVector(vector.Z, -vector.X, vector.Y);
}

FQuat USynOpticonStatics::ConvertXYZtoZYnegXCoordinateSystem(const FQuat quat)
{
	FVector axis;
	float angle;
	quat.ToAxisAndAngle(axis, angle);
	axis.Y = -axis.Y;
	return FQuat(axis, angle) * FQuat::MakeFromEuler(FVector(0, 0, 180)); //TODO might not need the rotation anymore
}

/** Convert from Eye Tracking coordinate system to UE coordinate system */
FVector USynOpticonStatics::ConvertXYZtoZYnegXCoordinateSystem(const FVector vector) //Vertical ground plane
{
	return FVector(vector.X, -vector.Y, vector.Z);
}

FVector USynOpticonStatics::ConvertFromMotiveToUnrealEngineCoordinateSystem(FVector MotiveVector, FVector Offset, bool Horizontal, float Factor)
{
	FVector ConvertVector = MotiveVector * Factor + Offset;
	if (Horizontal)
	{
		return ConvertXYZtoXZYCoordinateSystem(ConvertVector);
	}
	else {
		return ConvertXYZtoZYnegXCoordinateSystem(ConvertVector);
	}
}

FQuat USynOpticonStatics::ConvertFromMotiveToUnrealEngineCoordinateSystem(FQuat MotiveQuat, bool Horizontal)
{
	//return ConvertXYZtoXZYCoordinateSystem(MotiveQuat).Inverse();
	if (Horizontal)
	{
		return ConvertXYZtoXZYCoordinateSystem(MotiveQuat);
	}
	else
	{
		//return MotiveQuat;
		return ConvertXYZtoZYnegXCoordinateSystem(MotiveQuat);
	}
}

TPair<FVector, FVector> USynOpticonStatics::TwoIntersectionPoints(FVector u, FVector v, FVector A, FVector B)
{
	//find the shortest line between two input lines.
	FVector n = FVector::CrossProduct(u, v);
	FVector q = FVector::CrossProduct(n, u);
	float t_c = (q.X*(A.X - B.X) + q.Y*(A.Y - B.Y) + q.Z*(A.Z - B.Z)) / (q.X*v.X + q.Y*v.Y + q.Z*v.Z);
	FVector C = B + v*t_c;

	FVector r = FVector::CrossProduct(n, v);
	float t_d = (r.X*(B.X - A.X) + r.Y*(B.Y - A.Y) + r.Z*(B.Z - A.Z)) / (r.X*u.X + r.Y*u.Y + r.Z*u.Z);
	FVector D = A + u*t_d;
	return TPair<FVector, FVector>(C, D);
}

float USynOpticonStatics::DistanceBetweenSkewLines(FVector u, FVector v, FVector A, FVector B)
{
	FVector AB = B - A;
	FVector n = FVector::CrossProduct(u, v);
	return FVector::DotProduct(AB, n) / n.Size();
}

TPair<FVector, float> USynOpticonStatics::ConvPointAndDist(FVector u, FVector v, FVector A, FVector B)
{
	//if u and v is parallel then we don't have convergence point
	if (FVector::Parallel(u, v))
		return TPair<FVector, float>(FVector(-1, -1, -1), -1);
	else
	{
		TPair<FVector, FVector> CD = USynOpticonStatics::TwoIntersectionPoints(u, v, A, B);
		float d = FVector::Dist(CD.Key, CD.Value);
		if (d == 0)
			return TPair<FVector, float>(CD.Key, 0);
		else
			return TPair<FVector, float>((CD.Key + CD.Value) / 2, d);
	}
}

float USynOpticonStatics::PixelToCm(float pixels)
{
	float RATIO = 0.026458;
	return pixels*RATIO;
}

double USynOpticonStatics::func(double* params, int x, void* data)
{
	double** input = (double**)data;
	return (params[0] - input[x][0])*(params[0] - input[x][0])
		+ (params[1] - input[x][1])*(params[1] - input[x][1]);
}

void USynOpticonStatics::grad(double* g, double* params, int x, void* data)
{
	double** input = (double**)data;
	g[0] = 2 * (params[0] - input[x][0]);
	g[1] = 2 * (params[1] - input[x][1]);
}

double USynOpticonStatics::linear_func(double* params, int x, void* data)
{
	double** input = (double**)data;
	return (params[0] * input[x][0])
		+ (params[1] * input[x][1])
		+ (params[2] * input[x][2])
		+ params[4];
}

void USynOpticonStatics::linear_grad(double* g, double* params, int x, void* data)
{
	double** input = (double**)data;
	g[0] = (params[0]);
	g[1] = (params[1]);
	g[2] = (params[2]);
}

TArray<double> USynOpticonStatics::estimator(TArray<TArray<double>> data)
{
	TArray<double> resultAngles;
	LMstat* lmstat = new LMstat();
	Levmar::levmarq_init(lmstat);
	int npar = 2;
	double* par = new double[npar];
	par[0] = par[1] = 0;
	double** fdata = new double*[data.Num()];
	for (int count = 0; count < data.Num(); count++)
	{
		fdata[count] = new double[npar];
		fdata[count][0] = data[count][0];
		fdata[count][1] = data[count][1];
	}

	int ny = data.Num();
	double* y = new double[ny];
	for (int count = 0; count < ny; count++)
	{
		y[count] = 0;
	}
	int result = Levmar::levmarq(npar, par, ny, y, NULL,
		func,
		grad,
		fdata, lmstat);

	resultAngles.Add(par[0]);
	resultAngles.Add(par[1]);

	delete lmstat;
	delete[] par;
	for (int count = 0; count < data.Num(); count++)
	{
		delete[] fdata[count];
	}
	delete[] fdata;
	delete[] y;

	return resultAngles;
}

FString USynOpticonStatics::ConvertNameToURL(FString name)
{	
	bool flag = false;
	while(IsValidURL(name) != -1)
	{
		name.RemoveAt(IsValidURL(name));
	} 

	return name;
}

int USynOpticonStatics::IsValidURL(FString name)
{
	const FRegexPattern allowableLetters(TEXT("[^(A-Z)(a-z)(0-9)(\\-)(\\.)(\\_)(\\~)(\\:)(\\/)(\\?)(\\#)(\\[)(\\])(\\@)(\\!)(\\$)(\\&)(\\')(\\()(\\))(\\*)(\\+)(\\,)(\\;)(\\=)(\\%)]"));

	FRegexMatcher urlMatcher(allowableLetters, name);
	if (urlMatcher.FindNext())
		return urlMatcher.GetMatchBeginning();

	return -1;
}

FString USynOpticonStatics::GetValidFileName(FString filePath, FString name, FString ext)
{
	if (ext == "")
	{
		ext = ".*";
	}
	else
	{
		ext = (ext.Left(1) == ".") ? ext : "." + ext;
	}
	FString fileName = FPaths::Combine(*filePath, *(name + ext));
	if (!FPaths::FileExists(fileName))
		return name + ext;
	else
	{
		int index = 0;
		do
		{
			index++;
			fileName = FPaths::Combine(*filePath, *(name + USynOpticonStatics::ConvertNumberToString(index) + ext));
		} while (FPaths::FileExists(fileName));
		return name + USynOpticonStatics::ConvertNumberToString(index) + ext;
	}
}

bool USynOpticonStatics::CreateBoxGeometry(float width, float length, float height, TArray<FVector> &OutVerticies, TArray<int32> &OutTriangleIndices, TArray<FVector> &OutBoxVerticies, TArray<int32> &OutBoxTriangleIndices, TArray<FVector2D> &OutBoxUVs)
{
	//Right Face
	FVector RightFaceLowerLeftCorner(-width / 2, length / 2, -height / 2);
	FVector RightFaceLowerRightCorner(width / 2, length / 2, -height / 2);
	FVector RightFaceUpperRightCorner(width / 2, length / 2, height / 2);
	FVector RightFaceUpperLeftCorner(-width / 2, length / 2, height / 2);

	OutBoxVerticies.Add(RightFaceLowerLeftCorner);
	OutBoxVerticies.Add(RightFaceLowerRightCorner);
	OutBoxVerticies.Add(RightFaceUpperRightCorner);
	OutBoxVerticies.Add(RightFaceUpperLeftCorner);
	
	OutBoxUVs.Add(FVector2D(RightFaceLowerLeftCorner.X, RightFaceLowerLeftCorner.Z));
	OutBoxUVs.Add(FVector2D(RightFaceLowerRightCorner.X, RightFaceLowerRightCorner.Z));
	OutBoxUVs.Add(FVector2D(RightFaceUpperRightCorner.X, RightFaceUpperRightCorner.Z));
	OutBoxUVs.Add(FVector2D(RightFaceUpperLeftCorner.X, RightFaceUpperLeftCorner.Z));

	//Forward Face
	FVector ForwardFaceLowerLeftCorner(width / 2, -length / 2, -height / 2);
	FVector ForwardFaceLowerRightCorner(width / 2, length / 2, -height / 2);
	FVector ForwardFaceUpperRightCorner(width / 2, length / 2, height / 2);
	FVector ForwardFaceUpperLeftCorner(width / 2, -length / 2, height / 2);

	OutBoxVerticies.Add(ForwardFaceLowerLeftCorner);
	OutBoxVerticies.Add(ForwardFaceLowerRightCorner);
	OutBoxVerticies.Add(ForwardFaceUpperRightCorner);
	OutBoxVerticies.Add(ForwardFaceUpperLeftCorner);

	OutBoxUVs.Add(FVector2D(ForwardFaceLowerLeftCorner.Y, ForwardFaceLowerLeftCorner.Z));
	OutBoxUVs.Add(FVector2D(ForwardFaceLowerRightCorner.Y, ForwardFaceLowerRightCorner.Z));
	OutBoxUVs.Add(FVector2D(ForwardFaceUpperRightCorner.Y, ForwardFaceUpperRightCorner.Z));
	OutBoxUVs.Add(FVector2D(ForwardFaceUpperLeftCorner.Y, ForwardFaceUpperLeftCorner.Z));

	//Left face
	FVector LeftFaceLowerLeftCorner(-width / 2, -length / 2, -height / 2);
	FVector LeftFaceLowerRightCorner(width / 2, -length / 2, -height / 2);
	FVector LeftFaceUpperRightCorner(width / 2, -length / 2, height / 2);
	FVector LeftFaceUpperLeftCorner(-width / 2, -length / 2, height / 2);

	OutBoxVerticies.Add(LeftFaceLowerLeftCorner); 
	OutBoxVerticies.Add(LeftFaceLowerRightCorner); 
	OutBoxVerticies.Add(LeftFaceUpperRightCorner); 
	OutBoxVerticies.Add(LeftFaceUpperLeftCorner);

	//Flip UVS due to negative normal
	OutBoxUVs.Add(FVector2D(LeftFaceUpperLeftCorner.X, LeftFaceUpperLeftCorner.Z));
	OutBoxUVs.Add(FVector2D(LeftFaceUpperRightCorner.X, LeftFaceUpperRightCorner.Z));
	OutBoxUVs.Add(FVector2D(LeftFaceLowerRightCorner.X, LeftFaceLowerRightCorner.Z));
	OutBoxUVs.Add(FVector2D(LeftFaceLowerLeftCorner.X, LeftFaceLowerLeftCorner.Z));

	//Back face
	FVector BackFaceLowerLeftCorner(-width / 2, -length / 2, -height / 2);
	FVector BackFaceLowerRightCorner(-width / 2, length / 2, -height / 2);
	FVector BackFaceUpperRightCorner(-width / 2, length / 2, height / 2);
	FVector BackFaceUpperLeftCorner(-width / 2, -length / 2, height / 2);

	OutBoxVerticies.Add(BackFaceLowerLeftCorner); 
	OutBoxVerticies.Add(BackFaceLowerRightCorner); 
	OutBoxVerticies.Add(BackFaceUpperRightCorner); 
	OutBoxVerticies.Add(BackFaceUpperLeftCorner); 

	//Flip UVS due to negative normal
	OutBoxUVs.Add(FVector2D(BackFaceUpperLeftCorner.Y, BackFaceUpperLeftCorner.Z));
	OutBoxUVs.Add(FVector2D(BackFaceUpperRightCorner.Y, BackFaceUpperRightCorner.Z));
	OutBoxUVs.Add(FVector2D(BackFaceLowerRightCorner.Y, BackFaceLowerRightCorner.Z));
	OutBoxUVs.Add(FVector2D(BackFaceLowerLeftCorner.Y, BackFaceLowerLeftCorner.Z));

	//Bottom face
	FVector BottomFaceLowerLeftCorner(-width / 2, -length / 2, -height / 2);
	FVector BottomFaceLowerRightCorner(width / 2, -length / 2, -height / 2);
	FVector BottomFaceUpperRightCorner(width / 2, length / 2, -height / 2);
	FVector BottomFaceUpperLeftCorner(-width / 2, length / 2, -height / 2);

	OutBoxVerticies.Add(BottomFaceLowerLeftCorner); 
	OutBoxVerticies.Add(BottomFaceLowerRightCorner); 
	OutBoxVerticies.Add(BottomFaceUpperRightCorner); 
	OutBoxVerticies.Add(BottomFaceUpperLeftCorner); 

	//Flip UVS due to negative normal
	OutBoxUVs.Add(FVector2D(BottomFaceUpperLeftCorner.X, BottomFaceUpperLeftCorner.Y));
	OutBoxUVs.Add(FVector2D(BottomFaceUpperRightCorner.X, BottomFaceUpperRightCorner.Y));
	OutBoxUVs.Add(FVector2D(BottomFaceLowerRightCorner.X, BottomFaceLowerRightCorner.Y));
	OutBoxUVs.Add(FVector2D(BottomFaceLowerLeftCorner.X, BottomFaceLowerLeftCorner.Y));

	//upper
	OutVerticies.Add(FVector(width / 2, length / 2, height / 2));  //TODO check that the screen is facing forwards (minus x)
	OutVerticies.Add(FVector(-width / 2, length / 2, height / 2));
	OutVerticies.Add(FVector(-width / 2, -length / 2, height / 2));
	OutVerticies.Add(FVector(width / 2, -length / 2, height / 2));

	int32 OutsideTriangleIndices[] = {
		0,  1,  2,  0,  2,  3,   //Right face
		6,  5,  4,  7,  6,  4,   //Forward face
		10,  9,  8, 11,  10, 8,  //Left face
		12, 13, 14, 12, 14, 15,  //Back face
		16, 17, 18, 16, 18, 19 }; //Bottom face

	int32 InteractionFaceTriangleIndices[] = {
		2, 1, 0, 3, 2, 0 };  //upper

	OutTriangleIndices.Append(InteractionFaceTriangleIndices, ARRAY_COUNT(InteractionFaceTriangleIndices));

	OutBoxTriangleIndices.Append(OutsideTriangleIndices, ARRAY_COUNT(OutsideTriangleIndices));

	return true;
}

bool USynOpticonStatics::PointInTriangle2DUVs(FVector2D p, FVector2D p0, FVector2D p1, FVector2D p2)
{
	float s = p0.Y * p2.X - p0.X * p2.Y + (p2.Y - p0.Y) * p.X + (p0.X - p2.X) * p.Y;
	float t = p0.X * p1.Y - p0.Y * p1.X + (p0.Y - p1.Y) * p.X + (p1.X - p0.X) * p.Y;

	if ((s < 0) != (t < 0))
		return false;

	float A = -p1.Y * p2.X + p0.Y * (p2.X - p1.X) + p0.X * (p1.Y - p2.Y) + p1.X * p2.Y;
	if (A < 0.0)
	{
		s = -s;
		t = -t;
		A = -A;
	}
	return s > 0 && t > 0 && (s + t) <= A;
}

bool USynOpticonStatics::RenameFile(FString oldName, FString directory, FString newName)
{
	return false;
}

UTexture2D* USynOpticonStatics::LoadTextureFromImage(FString Filepath) {
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Filepath));
}

UTexture2D* USynOpticonStatics::LoadTextureFromImage(FString FullFilePath, FString ImageFormat, bool& OutIsValid, int32& OutWidth, int32& OutHeight)
{
	OutIsValid = false;
	UTexture2D* LoadedT2D = NULL;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

	EImageFormat Format = ImageFormat.Equals("png", ESearchCase::Type::IgnoreCase) ? EImageFormat::PNG : EImageFormat::JPEG;

	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);

	//Load From File
	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath))
	{
		return NULL;
	}


	//Create T2D!
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		const TArray<uint8>* UncompressedBGRA = NULL;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{
			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			//Valid?
			if (!LoadedT2D)
			{
				return NULL;
			}

			//Out!
			OutWidth = ImageWrapper->GetWidth();
			OutHeight = ImageWrapper->GetHeight();

			//Copy!
			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

			//Update!
			LoadedT2D->UpdateResource();
		}
	}

	// Success!
	OutIsValid = true;
	return LoadedT2D;
}

void USynOpticonStatics::EstimatingCenterOfSphere(TArray<FVector> Points, FVector& Center, float& Radius)
{
	int N = Points.Num();

	double meanX = 0;
	double meanY = 0;
	double meanZ = 0;

	double XXYYZZ = 0;
	for (FVector p : Points)
	{
		meanX += p.X;
		meanY += p.Y;
		meanZ += p.Z;
		XXYYZZ += (p.X*p.X + p.Y*p.Y + p.Z*p.Z);
	}

	meanX /= N;
	meanY /= N;
	meanZ /= N;

	double delXX = 0; double delXY = 0; double delXZ = 0;
	double delYY = 0; double delYZ = 0;
	double delZZ = 0;

	double sqrtX = 0;
	double sqrtY = 0;
	double sqrtZ = 0;

	for (FVector p : Points)
	{
		delXX += p.X*(p.X - meanX); delXY += p.X*(p.Y - meanY); delXZ += p.X*(p.Z - meanZ);
		delYY += p.Y*(p.Y - meanY); delYZ += p.Y*(p.Z - meanZ);
		delZZ += p.Z*(p.Z - meanZ);

		sqrtX += ((p.X*p.X + p.Y*p.Y + p.Z*p.Z)*(p.X - meanX));
		sqrtY += ((p.X*p.X + p.Y*p.Y + p.Z*p.Z)*(p.Y - meanY));
		sqrtZ += ((p.X*p.X + p.Y*p.Y + p.Z*p.Z)*(p.Z - meanZ));
	}

	delXX /= N; delXY /= N; delXZ /= N;
	delYY /= N; delYZ /= N; 
	delZZ /= N;

	sqrtX /= N;
	sqrtY /= N;
	sqrtZ /= N;

	FMatrix A = FMatrix(FPlane(delXX, 2*delXY, 2*delXZ, 0),
						FPlane(0, delYY, 2*delYZ, 0),
						FPlane(0, 0, delZZ, 0),
						FPlane(0, 0, 0, 1));
	A = A + A.GetTransposed();

	FMatrix B = FMatrix(FPlane(sqrtX, 0, 0, 0),
						FPlane(sqrtY, 0, 0, 0),
						FPlane(sqrtZ, 0, 0, 0),
						FPlane(0, 0, 0, 1));


	FMatrix C = FMatrix(FPlane(0, 0, 0, 0),
				FPlane(0, 0, 0, 0),
				FPlane(0, 0, 0, 0),
				FPlane(0, 0, 0, -1));
	A = A + C;
	FMatrix CenterMatrix = A.Inverse()*B;
	Center.X = CenterMatrix.GetColumn(0).X;
	Center.Y = CenterMatrix.GetColumn(0).Y;
	Center.Z = CenterMatrix.GetColumn(0).Z;

	Radius = 0;
}

void USynOpticonStatics::CalculateCircle(FVector P1, FVector P2, FVector P3, float& a, float& b, float& r)
{
	//we disregard Z, consider P1, P2, P3 are on the same plane
	//solve this equation with three inputs: (x - a)^2 + (y - b)^2 = r^2
	float M1 = (P1.X*P1.X - P2.X*P2.X) + (P1.Y*P1.Y - P2.Y*P2.Y);
	float M2 = (P1.X*P1.X - P3.X*P3.X) + (P1.Y*P1.Y - P3.Y*P3.Y);

	a = (M1*(P1.Y - P3.Y) - M2*(P1.Y - P2.Y)) / (2 * ((P1.X - P2.X)*(P1.Y - P3.Y) - (P1.X - P3.X)*(P1.Y - P2.Y)));
	b = (M1 - 2 * a*(P1.X - P2.X)) / (2 * (P1.Y - P2.Y));
	r = FMath::Sqrt(FMath::Pow(P1.X - a, 2) + FMath::Pow(P1.Y - b, 2));
}

float USynOpticonStatics::CircularFormula(float a, float b, float r, float x)
{
	return FMath::Sqrt(FMath::Pow(r, 2) - FMath::Pow(x - a, 2)) + b;
}

void USynOpticonStatics::SolveSystemOf3Equations(FMatrix A, FPlane B, FVector& outX)
{
	//A.X = B => X = A^(-1).B
	FMatrix C = FMatrix(FPlane(B.X, 0, 0, 0),
		FPlane(B.Y, 0, 0, 0),
		FPlane(B.Z, 0, 0, 0),
		FPlane(0, 0, 0, 1));
	FMatrix X = A.Inverse()*C;
	outX.X = X.GetColumn(0).X;
	outX.Y = X.GetColumn(0).Y;
	outX.Z = X.GetColumn(0).Z;
}

void USynOpticonStatics::PlaneEquationGiven3Points(FVector P1, FVector P2, FVector P3, FVector& Normal, float& D)
{
	//Plane equation: Ax + By + Cz = D
	//normal vector: crossproduct (P2 - P1) & (P23 - P1)
	Normal = FVector::CrossProduct(P2 - P1, P3 - P1);
	Normal.Normalize();
	D = Normal.X*P1.X + Normal.Y*P1.Y + Normal.Z*P1.Z;
}

void USynOpticonStatics::CircleFromCuttingSphereUsingPlane(FVector P1, FVector P2, FVector P3, FPlane& Plane, FVector& Center, float& Radius)
{
	//Sphere equation: (x - a)^2 + (y - b)^2 + (z - c)^2 = R^2
	FVector PlaneNormal;
	float D;
	PlaneEquationGiven3Points(P1, P2, P3, PlaneNormal, D); //find the cutting plane
	Plane = FPlane(PlaneNormal.X, PlaneNormal.Y, PlaneNormal.Z, D);
	FMatrix A = FMatrix(FPlane(2 * (P1.X - P2.X), 2 * (P1.Y - P2.Y), 2 * (P1.Z - P2.Z), 0),
						FPlane(2 * (P1.X - P3.X), 2 * (P1.Y - P3.Y), 2 * (P1.Z - P3.Z), 0),
						FPlane(PlaneNormal.X, PlaneNormal.Y, PlaneNormal.Z, 0),
						FPlane(0, 0, 0, 1));

	FPlane B = FPlane(P1.X*P1.X - P2.X*P2.X + P1.Y*P1.Y - P2.Y*P2.Y + P1.Z*P1.Z - P2.Z*P2.Z,
					P1.X*P1.X - P3.X*P3.X + P1.Y*P1.Y - P3.Y*P3.Y + P1.Z*P1.Z - P3.Z*P3.Z,
					D,
					1);

	SolveSystemOf3Equations(A, B, Center);
	Radius = FMath::Sqrt(FMath::Pow(P1.X - Center.X, 2) + FMath::Pow(P1.Y - Center.Y, 2) + FMath::Pow(P1.Z - Center.Z, 2));
}

void USynOpticonStatics::CircularYAndZGivenX(FPlane Plane, FVector Center, float Radius, float inX, FVector2D& outY, FVector2D& outZ)
{
	float t = (Plane.W - Plane.X*inX) / Plane.Y;
	double a = FMath::Pow(Plane.Z / Plane.Y, 2) + 1;
	double b = -2 * (Plane.Z / Plane.Y*(t - Center.Y) + Center.Z);
	double c = FMath::Pow(inX - Center.X, 2) + FMath::Pow(t - Center.Y, 2) + Center.Z*Center.Z - Radius*Radius;

	double outZ1; double outZ2;
	if (!SolveQuadraticEquation(a, b, c, outZ1, outZ2))
	{
		return;
	}

	outZ = FVector2D(outZ1, outZ2);

	outY = FVector2D(t - Plane.Z / Plane.Y*outZ1, t - Plane.Z / Plane.Y*outZ2);
}

bool USynOpticonStatics::SolveQuadraticEquation(double a, double b, double c, double& out1, double& out2)
{
	double delta = (b*b - 4*a*c);
	if (a == 0) {
		out1 = -c / b;
		out2 = out1;
		return true;
	}
	if (delta < 0) {
		return false;
	}
	
	out1 = (-b + FMath::Sqrt(delta)) / (2 * a);
	out2 = (-b - FMath::Sqrt(delta)) / (2 * a);
	return true;
}

bool USynOpticonStatics::CheckIfPointsOnSameSide(FVector P1, FVector P2, FVector P3, FVector P)
{
	FVector PlaneNormal = FVector::CrossProduct(FVector(0, 0, 1), P3 - P1);
	//A(x-x0) + B(y-y0)+ C(z - z0) = 0
	double P2Val = PlaneNormal.X*(P2.X - P1.X) + PlaneNormal.Y*(P2.Y - P1.Y) + PlaneNormal.Z*(P2.Z - P1.Z);
	double PVal = PlaneNormal.X*(P.X - P1.X) + PlaneNormal.Y*(P.Y - P1.Y) + PlaneNormal.Z*(P.Z - P1.Z);
	if (P2Val*PVal >= 0)
		return true;
	return false;
}

TArray<FVector> USynOpticonStatics::CreatePyramidShape(TArray<FVector> Points, int Subdivisions) {
	//The height of the screen
	FVector HeightVector = Points[1] - Points[0];
	//The vector between the start and the end of the curve
	FVector LengthVector = Points[3] - Points[1];
	//The vector from the middle of the curve to the heighest point of the curve
	FVector CentreVector = Points[2] - ((Points[1] + Points[3]) / 2);

	TArray<FVector> Result;

	//If there are no subdivisions we just add the points with their bottom part as well and return
	if (Subdivisions <= 0 && Points.Num() > 3)
	{
		Result.Add(Points[1]);
		Result.Add(Points[0]);

		for (int i = 2; i < Points.Num(); i++) {
			Result.Add(Points[i]);
			Result.Add(Points[i] + HeightVector);
		}
		return Result;
	}

	for (int i = 0; i <= Subdivisions; i++) {
		FVector OffsetVector;
		if (i < Subdivisions / 2) {
			OffsetVector = (CentreVector / (Subdivisions/2)) * i;
		}
		else if (i == Subdivisions / 2) {
			OffsetVector = CentreVector;
		}
		else{
			OffsetVector = (CentreVector / (Subdivisions/2)) * (Subdivisions-i);
		}

		Result.Add(Points[1] + ((LengthVector/Subdivisions) * i) + OffsetVector);
		Result.Add((Points[1] + HeightVector) + ((LengthVector / Subdivisions) * i) + OffsetVector);
	}

	return Result;
}

TArray<FVector> USynOpticonStatics::CreateArcShape3D(TArray<FVector> Points, int Subdivisions)
{
	TArray<FVector> Result;
	//Result.Add(Points[0]);
	if (Subdivisions < 0)
	{
		Subdivisions = 0;
	}

	float z = Points[1].Z;

	FVector Center;
	float Radius;
	FPlane Plane;
	CircleFromCuttingSphereUsingPlane(Points[1], Points[2], Points[3], Plane, Center, Radius);
	float Delta = 0;


	FVector Min;
	FVector Max;
	CalculateBoundingBox(Points, Min, Max);
	//TArray<FVector> UVWVertices = NormalizeBB(Min, Max, Points);

	//Choose unwrapping axis based on the dominant axis of the objects boundingbox
	FVector BoundsRanges = Max - Min;
	if (BoundsRanges.X < BoundsRanges.Z && BoundsRanges.Y < BoundsRanges.Z) {
		if (Subdivisions != 0)
		{
			Delta = (Points[3].Z - Points[1].Z) / Subdivisions;
		}
	}
	else if (BoundsRanges.Y < BoundsRanges.X && BoundsRanges.Z < BoundsRanges.X) {
		if (Subdivisions != 0)
		{
			Delta = (Points[3].X - Points[1].X) / Subdivisions;
		}
	}
	else { //(BoundsRanges.X > BoundsRanges.Y && BoundsRanges.Z > BoundsRanges.Y)
		if (Subdivisions != 0)
		{
			Delta = (Points[3].Y - Points[1].Y) / Subdivisions;
		}
	}

	//Result.Add(Points[1]);
	bool First = true;
	for (int i = 0; i <= Subdivisions; i++) {
		float x = Points[1].X + Delta*i;
		FVector2D Y;
		FVector2D Z;
		CircularYAndZGivenX(Plane, Center, Radius, x, Y, Z); //x given y and z, y given x and z, z given y and x

		FVector CalP1 = FVector(x, Y.X, Z.X);
		FVector CalP2 = FVector(x, Y.Y, Z.Y);
		int LastIndex = -1;
		if (CheckIfPointsOnSameSide(Points[1], Points[2], Points[3], CalP1))
		{
			LastIndex = Result.Add(CalP1);
		}
		else if (CheckIfPointsOnSameSide(Points[1], Points[2], Points[3], CalP2))
		{
			LastIndex = Result.Add(CalP2);
		}
		
		//LastIndex = Result.Add(FVector(x, Y.Y, Z.Y));
		FVector FourthMarker;
		if (First)
		{
			First = false;
			FourthMarker = Points[0];
		}
		else
		{
			if (LastIndex > 1)
				FourthMarker = (Result[LastIndex] + Result[LastIndex - 1] - Result[LastIndex - 2]);
		}

		Result.Add(FourthMarker);
	}

	return Result;
}

TArray<FVector> USynOpticonStatics::CreateArcShape3DV2(TArray<FVector> Points, int Subdivisions, bool bFullCircle)
{
	TArray<FVector> Result;
	//Result.Add(Points[0]);
	if (Subdivisions < 0)
	{
		Subdivisions = 0;
	}

	if (bFullCircle)
	{
		Subdivisions = 360;
	}

	float z = Points[1].Z;

	FVector HeightVector = Points[1] - Points[0];

	FVector Center;
	float Radius;
	FPlane Plane;
	CircleFromCuttingSphereUsingPlane(Points[1], Points[2], Points[3], Plane, Center, Radius);

	//angle between CP1 & CP2
	FVector CP1 = Points[1] - Center;
	FVector CP3 = Points[3] - Center;
	FVector CP2 = Points[2] - Center;
	FQuat BetweenQuat = FQuat::FindBetweenVectors(CP3, CP1);
	FVector nAxis;
	float Angle;
	BetweenQuat.ToAxisAndAngle(nAxis, Angle);

	FQuat BetweenQuatCP1And2 = FQuat::FindBetweenVectors(CP2, CP1);
	FVector nAxis12;
	float Angle12;
	BetweenQuatCP1And2.ToAxisAndAngle(nAxis12, Angle12);

	FQuat BetweenQuatCP2And3 = FQuat::FindBetweenVectors(CP3, CP2);
	FVector nAxis23;
	float Angle23;
	BetweenQuatCP2And3.ToAxisAndAngle(nAxis23, Angle23);

	float CombinedAngle = Angle12 + Angle23;
	//float Delta = CombinedAngle / Subdivisions;
	float Delta = FMath::RadiansToDegrees((Angle12 + Angle23)) / Subdivisions;
	if (bFullCircle)
	{
		Delta = 1;
	}
	for (int i = 0; i <= Subdivisions; i++) {
		float RotDelta = Delta * i;
		FVector New = CP1.RotateAngleAxis(-RotDelta, nAxis12);

		Result.Add(New + Center);
		Result.Add(New + Center - HeightVector);
	}

	return Result;
}

TArray<FVector2D> USynOpticonStatics::CreateUVsForCurvedScreen(TArray<FVector> Verticies)
{
	TArray<FVector2D> UVs;
	float p = 0;
	for (int i = 2; i < (Verticies.Num() - 1); i++)
	{
		if ((i % 2) == 0)
		{
			p += FVector::Dist(Verticies[i], Verticies[i - 2]);
		}
	}
	UVs.Add(FVector2D(0, 0));
	UVs.Add(FVector2D(0, 1));
	float odd_d = 0;
	float even_d = 0;
	for (int i = 2; i < Verticies.Num(); i++)
	{
		if ((i % 2) == 0)
		{
			odd_d += FVector::Dist(Verticies[i], Verticies[i - 2]);
			UVs.Add(FVector2D(odd_d / p, 0));
		}
		else
		{
			even_d += FVector::Dist(Verticies[i], Verticies[i - 2]);
			UVs.Add(FVector2D(even_d / p, 1));
		}
	}

	return UVs;
}

void USynOpticonStatics::CalculateBoundingBox(const TArray<FVector> &vertices, FVector &Min, FVector &Max)
{
	//Find bounding box
	float maxX = vertices[0].X;
	float minX = maxX;
	float maxY = vertices[0].Y;
	float minY = maxY;
	float maxZ = vertices[0].Z;
	float minZ = maxZ;

	for (FVector v : vertices)
	{
		if (v.X > maxX) { maxX = v.X; }
		if (v.X < minX) { minX = v.X; }

		if (v.Y > maxY) { maxY = v.Y; }
		if (v.Y < minY) { minY = v.Y; }

		if (v.Z > maxZ) { maxZ = v.Z; }
		if (v.Z < minZ) { minZ = v.Z; }
	}

	Min = FVector(minX, minY, minZ);
	Max = FVector(maxX, maxY, maxZ);
}

//File manager
TArray<FFileDataStruct> USynOpticonStatics::GetListOfFileInDirectory(FString path, FString ext)
{
	//FString message = "";
	TArray<FFileDataStruct> ListOfFilesAndDirectories;
	if (ext == "")
	{
		ext = "*.*";
	}
	else
	{
		ext = (ext.Left(1) == ".") ? "*" + ext : "*." + ext;
	}

	IFileManager& FileManager = IFileManager::Get();
	TArray<FString> Files;
	TArray<FString> Folders;
	FString FinalFilePath = FPaths::Combine(*(path), *(ext));
	FString FinalFolderPath = FPaths::Combine(*(path), *(FString("*.*")));
	FileManager.FindFiles(Files, *FinalFilePath, true, false);
	FileManager.FindFiles(Folders, *FinalFolderPath, false, true);

	for (FString folder : Folders)
	{
		FFileDataStruct folderData;
		folderData.Name = folder;
		folderData.IsFile = false;
		ListOfFilesAndDirectories.Add(folderData);
		//message += folderData.Name + ", ";
	}

	for (FString file : Files)
	{
		FFileDataStruct fileData;
		fileData.Name = file;
		fileData.IsFile = true;
		ListOfFilesAndDirectories.Add(fileData);
	}

	

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, message);
	//}

	return ListOfFilesAndDirectories;
}

FString USynOpticonStatics::GetGameDirectoryPath()
{
	return FPaths::ProjectDir();
}

FString USynOpticonStatics::GetValidFolderName(FString path, FString name)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString finalName = FPaths::Combine(*path, *name);
	// Directory Exists?
	if (!PlatformFile.DirectoryExists(*finalName))
	{
		return name;
	}
	else
	{
		int index = 0;
		do
		{
			index++;
			finalName = FPaths::Combine(*path, *(name + USynOpticonStatics::ConvertNumberToString(index)));
		} while (PlatformFile.DirectoryExists(*finalName));
		return name + USynOpticonStatics::ConvertNumberToString(index);
	}
}

bool USynOpticonStatics::VerifyDirectory(FString path)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Directory Exists?
	if (!PlatformFile.DirectoryExists(*path))
	{
		return false;
	}
	return true;
}

bool USynOpticonStatics::CreateNewDirectory(FString path)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Directory Exists?
	if (!PlatformFile.DirectoryExists(*path))
	{
		PlatformFile.CreateDirectory(*path);

		if (!PlatformFile.DirectoryExists(*path))
		{
			return false;
		}
	}
	return true;
}

bool USynOpticonStatics::DeleteFileOrDirectory(FString path, bool isFile)
{
	if (isFile)
	{
		if (!FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*path))
		{
			return false;
		}
		return true;
	}
	else
	{
		if (!FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*path))
		{
			return false;
		}
		return true;
	}
}

void USynOpticonStatics::SortSequenceObjects(TArray<FSequenceObjectData> &SequenceObjectList) {
	//Iterate as many times as there are elements in the list
	for (int i = 0; i < SequenceObjectList.Num(); i++) {
		//Flag to check if a swap should be done, this is so we can early stop when the list is already sorted 
		//(Most of the list will already be sorted from the previous iteration unless it is the first time we sort the list)
		bool ShouldSwap = false;
		int32 HighestDurationIndex = i; //The index of the current highest duration 
		//Iterate over the unsorted elements in the list
		for (int z = i; z < SequenceObjectList.Num(); z++) {
			//If the next element has a higher duration than the current one we swap them
			if (SequenceObjectList[z].ValidDuration > SequenceObjectList[HighestDurationIndex].ValidDuration) {
				HighestDurationIndex = z;
				ShouldSwap = true;
			}
		}

		//If we should swap we do so and update the correct YPositions
		if (ShouldSwap) {
			FSequenceObjectData Tmp = SequenceObjectList[i];
			float TmpYPos = Tmp.YPosition;
			SequenceObjectList[i] = SequenceObjectList[HighestDurationIndex];
			SequenceObjectList[HighestDurationIndex] = Tmp;
			SequenceObjectList[HighestDurationIndex].YPosition = SequenceObjectList[i].YPosition;
			SequenceObjectList[i].YPosition = TmpYPos;			
		}
	}
}

FVector2D USynOpticonStatics::FindUVInTriangle(TArray<FVector> TriangleVerticies, TArray<FVector2D> TriangleUVs, FVector HitLocation) {
	if (TriangleVerticies.Num() == 0 || TriangleUVs.Num() == 0) {
		return FVector2D(-1, -1);
	}

	// calculate vectors from point f to vertices p1, p2 and p3:
	FVector F1 = TriangleVerticies[0] - HitLocation;
	FVector F2 = TriangleVerticies[1] - HitLocation;
	FVector F3 = TriangleVerticies[2] - HitLocation;

	// calculate the areas and factors (order of parameters doesn't matter):
	float A = FVector::CrossProduct(TriangleVerticies[0] - TriangleVerticies[1], TriangleVerticies[0] - TriangleVerticies[2]).Size(); // main triangle area a
	float A1 = FVector::CrossProduct(F2, F3).Size() / A;      // p1's triangle area / a
	float A2 = FVector::CrossProduct(F3, F1).Size() / A;      // p2's triangle area / a 
	float A3 = FVector::CrossProduct(F1, F2).Size() / A;      // p3's triangle area / a
	
	// find the uv corresponding to point f (uv1/uv2/uv3 are associated to p1/p2/p3):
	FVector2D UV = TriangleUVs[0]*A1 + TriangleUVs[1]*A2 + TriangleUVs[2]*A3;

	return UV;
}

TArray<FLinearColor> USynOpticonStatics::GenerateChartColors(float NmbColors) {
	float s = 0.5; //0.99 gives Brighter colors, 0.5 and 0.3 gives mor pastel like colors
	float v = 0.95; 
	float golden_ratio_conjugate = 0.618033988749895;
	FMath::RandInit(11); //Hoa's seed
	float h = FMath::FRand();

	TArray<FLinearColor> GeneratedColors;
	for (int i = 0; i < NmbColors; i++) {
		h += golden_ratio_conjugate;
		h = FMath::Fmod(h,  1.0f);
		GeneratedColors.Add(GenerateColor(h, s, v));
	}

	return GeneratedColors;
}

//http://martin.ankerl.com/2009/12/09/how-to-create-random-colors-programmatically/
FLinearColor USynOpticonStatics::GenerateColor(float h, float s, float v) { //hsv to rgb
	// hsv values = 0 - 1, rgb values = 0 - 255

	int32 h_i = h * 6;
	float f = h * 6 - h_i;
	float p = v * (1 - s);
	float q = v * (1 - f*s);
	float t = v * (1 - (1 - f) * s);

	float r = 0;
	float g = 0;
	float b = 0;

	if (h_i == 0) {
		r = v;
		g = t;
		b = p;
	}
	else if (h_i == 1) {
		r = q;
		g = v;
		b = p;
	}
	else if (h_i == 2) {
		r = p;
		g = v;
		b = t;
	}
	else if (h_i == 3) {
		r = p;
		g = q;
		b = v;
	}
	else if (h_i == 4) {
		r = t;
		g = p;
		b = v;
	}
	else if (h_i == 5) {
		r = v;
		g = p;
		b = q;
	}

	return FLinearColor(r, g, b, 1);
};

UTexture2D* USynOpticonStatics::TextureFromWidget(UUserWidget *const Widget, const FVector2D &DrawSize)
{
	if (FSlateApplication::IsInitialized()
		&& Widget != NULL && Widget->IsValidLowLevel()
		&& DrawSize.X >= 1 && DrawSize.Y >= 1)
	{
		TSharedPtr<SWidget> SlateWidget(Widget->TakeWidget());
		if (!SlateWidget.IsValid()) return NULL;
		TSharedPtr<FWidgetRenderer> WidgetRenderer = MakeShareable(new FWidgetRenderer(true));
		if (!WidgetRenderer.IsValid()) return NULL;

		UTextureRenderTarget2D *TextureRenderTarget = WidgetRenderer->DrawWidget(SlateWidget.ToSharedRef(), DrawSize);
		// Creates Texture2D to store RenderTexture content
		UTexture2D *Texture = UTexture2D::CreateTransient(DrawSize.X, DrawSize.Y, PF_B8G8R8A8);
#if WITH_EDITORONLY_DATA
		Texture->MipGenSettings = TMGS_NoMipmaps;
#endif

		// Lock and copies the data between the textures
		TArray<FColor> SurfData;
		FRenderTarget *RenderTarget = TextureRenderTarget->GameThread_GetRenderTargetResource();
		RenderTarget->ReadPixels(SurfData);

		void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		const int32 TextureDataSize = SurfData.Num() * 4;
		FMemory::Memcpy(TextureData, SurfData.GetData(), TextureDataSize);
		Texture->PlatformData->Mips[0].BulkData.Unlock();
		Texture->UpdateResource();

		// Free resources
		SurfData.Empty();
		TextureRenderTarget->ConditionalBeginDestroy();
		SlateWidget.Reset();
		WidgetRenderer.Reset();

		return Texture;
	}
	return NULL;
}