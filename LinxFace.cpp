#include "StdAfx.h"
#include "LinxFace.h"
LinxFace * LinxFace::instance = NULL;

LinxFace::LinxFace(void) 
	: components(N_T("Biometrics.FaceExtraction")),
		additionalComponentSegmentation(N_T("Biometrics.FaceSegmentsDetection")),
		additionalComponentMatching(N_T("Biometrics.FaceMatching"))

{
	
	onInitSetting();
	writeLog("VS", "=== INITIALIZED PARAMETERS ===");
	printConfig();
}


LinxFace::~LinxFace(void)
{

}

int LinxFace::getConfigInt(LPWSTR section, LPWSTR key, int defaultValue, LPWSTR file)
{
	return GetPrivateProfileInt(section, key, defaultValue, file);
}
float LinxFace::getConfigFloat(LPWSTR section, LPWSTR key, float defaultValue, LPWSTR file)
{
	TCHAR arr[255];
	std::stringstream ss;
	ss << defaultValue;
	std::string s = ss.str();
	std::wstring ws(s.begin(), s.end());
	GetPrivateProfileString(section, key, ws.c_str(), arr, 255, file);

	std::wstring arr_w(arr);
	std::string arr_s(arr_w.begin(), arr_w.end());
	return std::atof(arr_s.c_str());
}
bool LinxFace::getConfigBool(LPWSTR section, LPWSTR key, bool defaultValue, LPWSTR file)
{
	TCHAR arr[255];
	std::stringstream ss;
	if(defaultValue)
	{
		ss << "true";
	}
	else
	{
		ss << "false";
	}
	ss << defaultValue;
	std::string s = ss.str();
	std::wstring ws(s.begin(), s.end());
	GetPrivateProfileString(section, key, ws.c_str(), arr, 255, file);

	std::wstring arr_w(arr);
	std::string arr_s(arr_w.begin(), arr_w.end());

	bool value = false;
	if(arr_s == "true")
	{
		value = true;
	}
	return value;
}
void LinxFace::onInitSetting()
{
	NBiometricClient biometricClient;
	minInterOcular = biometricClient.GetFacesMinimalInterOcularDistance();
	confThreshold = biometricClient.GetFacesConfidenceThreshold();
	maxRoll = biometricClient.GetFacesMaximalRoll();
	maxYaw = biometricClient.GetFacesMaximalYaw();

	qualityThreshold = biometricClient.GetFacesQualityThreshold();

	// = 
	livenessThresHold = biometricClient.GetFacesLivenessThreshold();
	livenessBlinkTimeOut = biometricClient.GetFacesLivenessBlinkTimeout();

	livenessMode = biometricClient.GetFacesLivenessMode();
	matchSpeed = biometricClient.GetFacesMatchingSpeed();

	tokenImageWidth = biometricClient.GetFacesTokenImageWidth();
	tokenQualityThreshold = biometricClient.GetFacesTokenQualityThreshold();
	sharpnessThreshold = biometricClient.GetFacesSharpnessThreshold();

	bgUniformityThreshold = biometricClient.GetFacesBackgroundUniformityThreshold();

	grayDensityThreshold = biometricClient.GetFacesGrayscaleDensityThreshold();

	templateSize = biometricClient.GetFacesTemplateSize();
	detectAllFeature = biometricClient.GetFacesDetectAllFeaturePoints();

    // matching
	matchingWithDetail = biometricClient.GetMatchingWithDetails();
	matchingMaxResult = biometricClient.GetMatchingMaximalResultCount();
    //matchingFirstResult = biometricClient.getMatc
	matchingThreshold = biometricClient.GetMatchingThreshold();

	isLoop = false;
}
void LinxFace::printConfig()
{
	std::stringstream ss;
	ss << "MinimalInterOcularDistance:\t" << minInterOcular;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "ConfidenceThreshold:\t" << confThreshold;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "MaximalRoll:\t" << maxRoll;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "MaximalYaw:\t" << maxYaw;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "QualityThreshold:\t" << qualityThreshold;
	writeLog("VS", ss.str());


	ss.str("");
	ss << "UseLivenessCheck:\t" << useLivenessCheck;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "LivenessThreshold:\t" << livenessThresHold;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "LivenessBlinkTimeout:\t" << livenessBlinkTimeOut;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "LivenessMode:\t" << livenessMode;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "MatchingSpeed:\t" << matchSpeed;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "TokenImageWidth:\t" << tokenImageWidth;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "TokenQualityThreshold:\t" << tokenQualityThreshold;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "SharpnessThreshold:\t" << sharpnessThreshold;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "BackgroundUniformityThreshold:\t" << bgUniformityThreshold;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "GrayscaleDensityThreshold:\t" << grayDensityThreshold;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "TemplateSize:\t" << templateSize;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "DetectAllFeaturePoints:\t" << detectAllFeature;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "WithDetails:\t" << matchingWithDetail;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "MaximalResultCount:\t" << matchingMaxResult;
	writeLog("VS", ss.str());

	ss.str("");
	ss << "FirstResultOnly:\t" << matchingFirstResult;
	writeLog("VS", ss.str());

	ss.str("");	
	ss << "Threshold:\t" << matchingThreshold;
	writeLog("VS", ss.str());

	ss.str("");	
	ss << "loop:\t" << isLoop;
	writeLog("VS", ss.str());
}
LINXDLL void LinxFace::writeLog(const std::string & tag, const std::string & msg)
{
	std::ofstream log_file("log.txt", std::ios_base::out | std::ios_base::app);

    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	log_file << "[" << buf << "]" << "[" << tag << "] " << msg << std::endl;
	std::cout << "[" << buf << "]" << "[" << tag << "] " << msg << std::endl;
	
}
LINXDLL void LinxFace::onInitConfig(LPWSTR filePath)
{
	LPWSTR section = L"NEURO_CONFIG";
	minInterOcular = getConfigInt(section, L"MinimalInterOcularDistance", minInterOcular, filePath);
    confThreshold = getConfigInt(section, L"ConfidenceThreshold", confThreshold, filePath);
	maxRoll = getConfigFloat(section, L"MaximalRoll", maxRoll, filePath);
    maxYaw = getConfigFloat(section, L"MaximalYaw", maxYaw, filePath);

    qualityThreshold = getConfigInt(section, L"QualityThreshold", qualityThreshold, filePath);

	useLivenessCheck = getConfigBool(section, L"UseLivenessCheck", useLivenessCheck, filePath);
    livenessThresHold = getConfigInt(section, L"LivenessThreshold", livenessThresHold, filePath);
    livenessBlinkTimeOut = getConfigInt(section, L"LivenessBlinkTimeout", livenessBlinkTimeOut, filePath);

    int livenessModeInt = getConfigInt(section, L"LivenessMode", -1, filePath);
	switch(livenessModeInt)
	{
	case 0:
		livenessMode = nlmNone;
		break;
	case 1:
		livenessMode = nlmPassive;
		break;
	case 2:
		livenessMode = nlmActive;
		break;
	case 3:
		livenessMode = nlmPassiveAndActive;
		break;
	case 4:
		livenessMode = nlmSimple;
		break;
	}
    int matchSpeedInt = getConfigInt(section, L"MatchingSpeed", -1, filePath);
	switch(matchSpeedInt)
	{
	case 0:
		matchSpeed = nmsLow;
		break;
	case 128:
		matchSpeed = nmsMedium;
		break;
	case 256:
		matchSpeed = nmsHigh;
		break;
	}
    tokenImageWidth = getConfigInt(section, L"TokenImageWidth", tokenImageWidth, filePath);
    tokenQualityThreshold = getConfigInt(section, L"TokenQualityThreshold", tokenQualityThreshold, filePath);
    sharpnessThreshold = getConfigInt(section, L"SharpnessThreshold", sharpnessThreshold, filePath);

    bgUniformityThreshold = getConfigInt(section, L"BackgroundUniformityThreshold",bgUniformityThreshold, filePath);

    grayDensityThreshold = getConfigInt(section, L"GrayscaleDensityThreshold", grayDensityThreshold, filePath);

    int templateSizeInt = getConfigInt(section, L"TemplateSize", -1, filePath);
	switch(templateSizeInt)
	{
	case 0:
		templateSize = ntsCompact;
		break;
	case 64:
		templateSize = ntsSmall;
		break;
	case 128:
		templateSize = ntsMedium;
		break;
	case 256:
		templateSize = ntsLarge;
		break;
	}
    detectAllFeature = getConfigBool(section, L"DetectAllFeaturePoints", detectAllFeature, filePath);

    // matching
    matchingWithDetail = getConfigBool(section, L"WithDetails", matchingWithDetail, filePath);
    matchingMaxResult = getConfigInt(section, L"MaximalResultCount", matchingMaxResult, filePath);
    matchingFirstResult = getConfigBool(section, L"FirstResultOnly", matchingFirstResult, filePath);
    matchingThreshold = getConfigInt(section, L"Threshold", matchingThreshold, filePath);

	section = L"VS_CONFIG";
	isLoop = getConfigBool(section, L"loop", false, filePath);

	writeLog("VS", "=== SETTING PARAMETERS ===");
	printConfig();
}
LINXDLL LinxFace * LinxFace::getInstance()
{
	if(instance == NULL)
	{
		instance = new LinxFace();
	}
	return instance;
}
LINXDLL bool LinxFace::onInit()
{
		// check license for neuro
	Neurotec::NCore::OnStart();

	try
	{
		if(!NLicense::ObtainComponents(N_T("/local"), N_T("5000"), components))
		{
			Neurotec::NThrowException(Neurotec::NString::Format(N_T("Could not obtain licenses for components: {S}"), components.GetBuffer()));
		}
		if(!NLicense::ObtainComponents(N_T("/local"), N_T("5000"), additionalComponentSegmentation))
		{
			writeLog("VS", "Could not obtain licenses for Biometrics.FaceSegmentsDetection");
		}

		if(!NLicense::ObtainComponents(N_T("/local"), N_T("5000"), additionalComponentMatching))
		{
			Neurotec::NThrowException(Neurotec::NString::Format(N_T("Could not obtain licenses for additionalComponents: {S}"), additionalComponentMatching.GetBuffer()));
		}

	}
	catch(Neurotec::NError &ex)
	{
		Neurotec::NError::Report(ex.GetCode(), ex);
		writeLog("VS", "check license failed");
		return false;
	}
	writeLog("VS", "check license successfully");
	return true;
}
LINXDLL bool LinxFace::onExit()
{
	NLicense::ReleaseComponents(components);
	NLicense::ReleaseComponents(additionalComponentSegmentation);
	NLicense::ReleaseComponents(additionalComponentMatching);
	Neurotec::NCore::OnExit(false);

	return true;
}
//LINXDLL std::vector<UINT8> * LinxFace::onEnroll(VSAPI_DATA_PTR data)
//{
//	std::vector<UINT8> * v = new std::vector<UINT8>();
//
//	cv::Mat raw = cv::Mat(1, data->length, CV_8UC3, data->data);
//	cv::Mat img = cv::imdecode(raw, CV_LOAD_IMAGE_COLOR);
//	cv::cvtColor(img, img, CV_BGR2RGB);
//	if(img.data != NULL)
//	{
//		try
//		{
//			Neurotec::NSizeType srcPlane[] = {img.rows * img.cols * 3};
//			NImage nImage = NImage::FromData(NPF_RGB_8U, 0, img.cols, img.rows, img.cols * 3, (void * const *) &img.data, &srcPlane[0], 1, 0, 0, img.cols, img.rows, NI_SRC_ALPHA_CHANNEL_FIRST);	
//			//nImage.Save("E:/test.bmp");
//			//bool isAdditionalComponentActivated = NLicense::IsComponentActivated(additionalComponentMatching);
//			NBiometricClient biometricClient;
//			//biometricClient.SetFacesDetectAllFeaturePoints(isAdditionalComponentActivated);
//			biometricClient.SetFacesMinimalInterOcularDistance(minInterOcular);
//			biometricClient.SetFacesConfidenceThreshold(confThreshold);
//			biometricClient.SetFacesMaximalRoll(maxRoll);
//			biometricClient.SetFacesMaximalYaw(maxYaw);
//			biometricClient.SetFacesQualityThreshold(qualityThreshold);
//			biometricClient.SetFacesLivenessThreshold(livenessThresHold);
//			biometricClient.SetFacesLivenessBlinkTimeout(livenessBlinkTimeOut);
//			
//			biometricClient.SetFacesLivenessMode(livenessMode);
//			biometricClient.SetFacesMatchingSpeed(matchSpeed);
//			biometricClient.SetFacesTokenImageWidth(tokenImageWidth);
//			biometricClient.SetFacesTokenQualityThreshold(tokenQualityThreshold);
//			biometricClient.SetFacesSharpnessThreshold(sharpnessThreshold);
//			biometricClient.SetFacesBackgroundUniformityThreshold(bgUniformityThreshold);
//			biometricClient.SetFacesGrayscaleDensityThreshold(grayDensityThreshold);
//
//			biometricClient.SetFacesTemplateSize(templateSize);
//			
//
//			NFace face;
//			face.SetImage(nImage);
//			NSubject subject;
//			subject.GetFaces().Add(face);
//
//			NBiometricStatus status = biometricClient.CreateTemplate(subject);
//
//			if(status == nbsOk)
//			{
//				NBuffer buffer = subject.GetTemplateBuffer();
//
//				v->resize(buffer.GetSize());
//
//				buffer.CopyTo(0, &v[0][0], buffer.GetSize(), buffer.GetSize());
//				
//				writeLog("VS", "end enroll image successfully");
//			}
//			else
//			{
//				writeLog("VS", "end enroll image unsuccessfully");
//			}
//		}
//		catch(Neurotec::NError &ex)
//		{
//			writeLog("VS", "End enroll error");
//			Neurotec::NError::Report(ex.GetCode(), ex);
//		}
//
//		
//	} 
//	return v;
//}
LINXDLL void LinxFace::onReleaseEnrolled(VSAPI_DATA enroll)
{
	delete [] enroll.data;
}
LINXDLL VSAPI_DATA LinxFace::onEnroll(VSAPI_DATA_PTR data)
{
	VSAPI_DATA enroll;
	cv::Mat raw = cv::Mat(1, data->length, CV_8UC3, data->data);
	cv::Mat img = cv::imdecode(raw, CV_LOAD_IMAGE_COLOR);
	cv::cvtColor(img, img, CV_BGR2RGB);
	if(img.data != NULL)
	{
		try
		{
			Neurotec::NSizeType srcPlane[] = {img.rows * img.cols * 3};
			NImage nImage = NImage::FromData(NPF_RGB_8U, 0, img.cols, img.rows, img.cols * 3, (void * const *) &img.data, &srcPlane[0], 1, 0, 0, img.cols, img.rows, NI_SRC_ALPHA_CHANNEL_FIRST);	
			//nImage.Save("E:/test.bmp");
			//bool isAdditionalComponentActivated = NLicense::IsComponentActivated(additionalComponentMatching);
			NBiometricClient biometricClient;
			//biometricClient.SetFacesDetectAllFeaturePoints(isAdditionalComponentActivated);
			biometricClient.SetFacesMinimalInterOcularDistance(minInterOcular);
			biometricClient.SetFacesConfidenceThreshold(confThreshold);
			biometricClient.SetFacesMaximalRoll(maxRoll);
			biometricClient.SetFacesMaximalYaw(maxYaw);
			biometricClient.SetFacesQualityThreshold(qualityThreshold);
			biometricClient.SetFacesLivenessThreshold(livenessThresHold);
			biometricClient.SetFacesLivenessBlinkTimeout(livenessBlinkTimeOut);
			
			biometricClient.SetFacesLivenessMode(livenessMode);
			biometricClient.SetFacesMatchingSpeed(matchSpeed);
			biometricClient.SetFacesTokenImageWidth(tokenImageWidth);
			biometricClient.SetFacesTokenQualityThreshold(tokenQualityThreshold);
			biometricClient.SetFacesSharpnessThreshold(sharpnessThreshold);
			biometricClient.SetFacesBackgroundUniformityThreshold(bgUniformityThreshold);
			biometricClient.SetFacesGrayscaleDensityThreshold(grayDensityThreshold);

			biometricClient.SetFacesTemplateSize(templateSize);
			

			NFace face;
			face.SetImage(nImage);
			NSubject subject;
			subject.GetFaces().Add(face);

			NBiometricStatus status = biometricClient.CreateTemplate(subject);

			if(status == nbsOk)
			{
				NBuffer buffer = subject.GetTemplateBuffer();
				



				enroll.data = new UINT8[buffer.GetSize()];				
				enroll.length = buffer.GetSize();

				buffer.CopyTo(0, enroll.data, buffer.GetSize(), buffer.GetSize());

				writeLog("VS", "end enroll image successfully");
			}
			else
			{
				writeLog("VS", "end enroll image unsuccessfully");
			}
		}
		catch(Neurotec::NError &ex)
		{
			writeLog("VS", "End enroll error");
			Neurotec::NError::Report(ex.GetCode(), ex);
		}

		
	} 
	
	
	return enroll;
}
LINXDLL bool LinxFace::onCompare(VSAPI_DATA_PTR processedTemplate, VSAPI_DATA_PTR storedTemplate)
{
	try
	{	
		NBuffer pBuffer((const void *)processedTemplate->data, processedTemplate->length);
		NBuffer sBuffer((const void *)storedTemplate->data, storedTemplate->length);
			
		NSubject pSubject;
		NSubject sSubject;
		pSubject.SetTemplateBuffer(pBuffer);
		pSubject.SetId("Process Template ID");

		sSubject.SetTemplateBuffer(sBuffer);
		sSubject.SetId("Stored Template ID");

		NBiometricClient biometricClient;
		NBiometricTask enrollTask = biometricClient.CreateTask(nboEnroll, NULL);
		enrollTask.GetSubjects().Add(sSubject);

		biometricClient.PerformTask(enrollTask);

		if(enrollTask.GetStatus() != nbsOk)
		{
			writeLog("VS", "EnrollTask unsuccessfully");

			if(enrollTask.GetError() != NULL)
			{
				throw enrollTask.GetError();
			}

			//return !BioAPI_OK;
		}

		biometricClient.SetMatchingWithDetails(matchingWithDetail);
		biometricClient.SetMatchingMaximalResultCount(matchingMaxResult);
		biometricClient.SetFirstResultOnly(true);
		if(!isLoop)
		{
			biometricClient.SetMatchingThreshold(matchingThreshold);
			NBiometricStatus status = biometricClient.Identify(pSubject);
			if(status == nbsOk)
			{
				NSubject::MatchingResultCollection matchingResult = pSubject.GetMatchingResults();
				if(matchingResult.GetCount() > 0)
				{
					std::stringstream ss;
					ss  << "SCORE: "<< matchingResult.Get(0).GetScore();
					writeLog("NEURO", ss.str());
					writeLog("VS", "End Comparing sucessfully");
					return true;
				}
			}
		}
		else
		{
			int mThreshold[9] = {96,84,72,60,48};
			for(int i = 0; i < 5; i ++)
			{
				biometricClient.SetMatchingThreshold(mThreshold[i]);
				NBiometricStatus status = biometricClient.Identify(pSubject);
				if(status == nbsOk)
				{
					NSubject::MatchingResultCollection matchingResult = pSubject.GetMatchingResults();
					if(matchingResult.GetCount() > 0)
					{
						std::stringstream ss;
						ss  << "Threshold: " << mThreshold[i] << ", SCORE: "<< matchingResult.Get(0).GetScore();
						writeLog("NEURO", ss.str());
						writeLog("VS", "End Comparing sucessfully");
						return true;
					}
				}
			}
		}


	}
	catch(Neurotec::NError & ex)
	{
		writeLog("VS", "End Comparing error");
		Neurotec::NError::Report(ex.GetCode(), ex);
		
	}
	writeLog("VS", "End Comparing unsucessfully");
	
	return false;
}