#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <string>
#include <fstream>
#include <time.h>
#include <vector>

// opencv
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

// neuro
#include "NCore.hpp"
#include "NBiometricClient.hpp"
#include "NBiometrics.hpp"
#include "NLicensing.hpp"
#include "NMedia.hpp"


using namespace Neurotec::Licensing;
using namespace Neurotec::Biometrics;
using namespace Neurotec::Biometrics::Client;
using namespace Neurotec::IO;
using namespace Neurotec::Images;


#define _VS_IMPORT

#ifdef _VS_IMPORT
	#define LINXDLL __declspec(dllexport)
#else
	#define LINXDLL __declspec(dllimport)
#endif

typedef struct VSAPI_DATA
{
	UINT8 *data;
	UINT32 length;
} VSAPI_DATA, *VSAPI_DATA_PTR;

class LinxFace
{
public:

	LINXDLL static LinxFace * getInstance();
	LINXDLL void onInitConfig(LPWSTR filePath);
	LINXDLL bool onInit();
	LINXDLL bool onExit();
	LINXDLL VSAPI_DATA onEnroll(VSAPI_DATA_PTR data);
	LINXDLL void onReleaseEnrolled(VSAPI_DATA enroll);
	LINXDLL bool onCompare(VSAPI_DATA_PTR processedTemplate, VSAPI_DATA_PTR storedTemplate);
	LINXDLL void writeLog(const std::string & tag, const std::string & msg);
private:
	static LinxFace * instance;
	LinxFace(void);
	~LinxFace(void);

	int getConfigInt(LPWSTR section, LPWSTR key, int defaultValue, LPWSTR file);
	void printConfig();
	float getConfigFloat(LPWSTR section, LPWSTR key, float defaultValue, LPWSTR file);
	bool getConfigBool(LPWSTR section, LPWSTR key, bool defaultValue, LPWSTR file);	
	void onInitSetting();

	Neurotec::NString components;
	Neurotec::NString additionalComponentSegmentation;
	Neurotec::NString additionalComponentMatching;

	// face
    int minInterOcular; //Faces.MinimalInterOcularDistance
    int confThreshold; // Faces.ConfidenceThreshold
    float maxRoll; // Faces.MaximalRoll
    float maxYaw; // Faces.MaximalYaw

    int qualityThreshold; // Faces.QualityThreshold

    bool useLivenessCheck; // Faces.UseLivenessCheck
    int livenessThresHold; // Faces.LivenessThreshold
    int livenessBlinkTimeOut; //Faces.LivenessBlinkTimeout

	NLivenessMode livenessMode; // Faces.LivenessMode
	NMatchingSpeed matchSpeed; // Faces.MatchingSpeed

    int tokenImageWidth; //Faces.TokenImageWidth
    int tokenQualityThreshold; // Faces.TokenQualityThreshold
    int sharpnessThreshold; // Faces.SharpnessThreshold

    int bgUniformityThreshold; // Faces.BackgroundUniformityThreshold

    int grayDensityThreshold; // Faces.GrayscaleDensityThreshold

	NTemplateSize templateSize; // Faces.TemplateSize
    bool detectAllFeature; //  Faces.DetectAllFeaturePoints

    // matching
    bool matchingWithDetail; // Matching.WithDetails
    int matchingMaxResult; // Matching.MaximalResultCount
    bool matchingFirstResult; // Matching.FirstResultOnly
    int matchingThreshold; // Matching.Threshold

	bool isLoop; // loop matching
};

