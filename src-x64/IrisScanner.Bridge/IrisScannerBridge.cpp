#include "IrisScannerBridge.h"

#include <msclr/gcroot.h>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

namespace
{
    String^ ToManagedString(const char* value)
    {
        if (value == nullptr)
        {
            return String::Empty;
        }

        return Marshal::PtrToStringAnsi(IntPtr(const_cast<char*>(value)));
    }

    String^ ToManagedString(const char* value, int length)
    {
        if (value == nullptr || length <= 0)
        {
            return String::Empty;
        }

        String^ managed = Marshal::PtrToStringAnsi(IntPtr(const_cast<char*>(value)), length);
        return managed == nullptr ? String::Empty : managed->Trim();
    }

    cli::array<Byte>^ CopyToManagedArray(const unsigned char* data, int length)
    {
        if (data == nullptr || length <= 0)
        {
            return gcnew cli::array<Byte>(0);
        }

        auto managed = gcnew cli::array<Byte>(length);
        Marshal::Copy(IntPtr(const_cast<unsigned char*>(data)), managed, 0, length);
        return managed;
    }
}

namespace IrisScanner
{
    using namespace msclr;
    using namespace System::Collections::Generic;

    class NativeCallback : public IKBaseProcessorCallback
    {
    public:
        explicit NativeCallback(IrisScannerBridge^ owner)
            : owner_(owner)
        {
        }

        void handleInitCompleted(int initRes) override
        {
            owner_->OnInitCompleted(initRes);
        }

        void handleProcessCompleted(IKBaseEnrollProcessedResult& result) override
        {
            owner_->OnEnrollCompleted(result);
        }

        void handleIdentProcessCompleted(const IKIdentProcessedResultStruct& result) override
        {
            owner_->OnIdentifyCompleted(result);
        }

        void handleStateUpdated(const IKProcessorState& state) override
        {
            owner_->OnStateUpdated(state);
        }

        void handleDebugInfoUpdated(const IKDebugInfo& debugInfo) override
        {
            owner_->OnDebugInfoUpdated(debugInfo);
        }

        void handleCameraUpdated(const IKCameraUpdated& cameraUpdate) override
        {
            owner_->OnCameraUpdated(cameraUpdate);
        }

        void handleLog(const char* logType, IKLog logLevel, const char* logInfo) override
        {
            owner_->OnLog(logType, logLevel, logInfo);
        }

    private:
        gcroot<IrisScannerBridge^> owner_;
    };

    IrisScannerBridge::IrisScannerBridge()
    {
        disposed_ = false;
        initialized_ = false;
        lastInitResult_ = IK_Err_INIT_FAILED;
        lastCameraEventCode_ = -1;
        lastCameraEventLoggedAtMs_ = Int64::MinValue;
        lastDistance_ = 0;
        lastLeftIrisX_ = 0;
        lastLeftIrisY_ = 0;
        lastLeftIrisRadius_ = 0;
        lastRightIrisX_ = 0;
        lastRightIrisY_ = 0;
        lastRightIrisRadius_ = 0;
        lastDeviceSn_ = String::Empty;
        lastDeviceType_ = String::Empty;
        lastDeviceEventCode_ = 0;
        initSignal_ = gcnew ManualResetEventSlim(false);
        settings_ = gcnew IrisScannerSettings();
        currentSubjectId_ = String::Empty;
        leftTemplates_ = gcnew List<IrisTemplateRecord^>();
        rightTemplates_ = gcnew List<IrisTemplateRecord^>();
        leftBuffer_ = new std::vector<unsigned char>();
        rightBuffer_ = new std::vector<unsigned char>();
        sdk_ = CreateSDKObj();

        if (sdk_ == nullptr)
        {
            delete leftBuffer_;
            delete rightBuffer_;
            throw gcnew InvalidOperationException("CreateSDKObj() returned null.");
        }

        nativeCallback_ = new NativeCallback(this);
    }

    IrisScannerBridge::~IrisScannerBridge()
    {
        this->!IrisScannerBridge();
        GC::SuppressFinalize(this);
    }

    IrisScannerBridge::!IrisScannerBridge()
    {
        if (disposed_)
        {
            return;
        }

        disposed_ = true;

        if (sdk_ != nullptr)
        {
            try
            {
                if (initialized_)
                {
                    sdk_->Release();
                }
            }
            catch (...)
            {
            }

            IKBaseSDKAPI* nativeSdk = sdk_;
            DestroySDKObj(&nativeSdk);
            sdk_ = nullptr;
        }

        if (nativeCallback_ != nullptr)
        {
            delete nativeCallback_;
            nativeCallback_ = nullptr;
        }

        if (leftBuffer_ != nullptr)
        {
            delete leftBuffer_;
            leftBuffer_ = nullptr;
        }

        if (rightBuffer_ != nullptr)
        {
            delete rightBuffer_;
            rightBuffer_ = nullptr;
        }
    }

    bool IrisScannerBridge::IsInitialized::get()
    {
        return initialized_;
    }

    void IrisScannerBridge::ApplySettings(IrisScannerSettings^ settings)
    {
        EnsureNotDisposed();
        settings_ = settings == nullptr ? gcnew IrisScannerSettings() : settings;
    }

    String^ IrisScannerBridge::GetVersion()
    {
        EnsureNotDisposed();

        if (sdk_ == nullptr)
        {
            throw gcnew InvalidOperationException("SDK object is not available.");
        }

        char version[IK_VERSION_LENGTH] = {};
        sdk_->GetVersion(version);
        return ToManagedString(version);
    }

    void IrisScannerBridge::Initialize()
    {
        // Initialize both processors so the M10 vendor SDK can run its
        // built-in face tracking/PTZ control while iris capture is active.
        // Iris-only initialization explicitly disables the servo according
        // to the vendor SDK documentation.
        Initialize(IrisInitMode::Both);
    }

    void IrisScannerBridge::Initialize(IrisInitMode mode)
    {
        EnsureNotDisposed();

        if (initialized_)
        {
            return;
        }

        lastInitResult_ = IK_Err_INIT_FAILED;
        initSignal_->Reset();

        // KioskApp historically calls Initialize(IrisInitMode::Iris)
        // explicitly.  On M10 hardware that mode starts only the iris
        // processor and leaves the face/PTZ tracker inactive.  Preserve the
        // public call for compatibility, but promote it to Both so the
        // camera can follow a user's face during iris capture.
        const auto effectiveMode = mode == IrisInitMode::Iris
            ? IrisInitMode::Both
            : mode;
        const auto nativeMode = static_cast<IKInitMode>(static_cast<int>(effectiveMode));
        const int initResult = sdk_->Init(nativeCallback_, nativeMode);
        ThrowIfImmediateError("Init", initResult);

        if (!initSignal_->Wait(TimeSpan::FromSeconds(30)))
        {
            throw gcnew TimeoutException("Timed out waiting for the SDK initialization callback.");
        }

        ThrowIfImmediateError("Init callback", lastInitResult_);
        initialized_ = true;
        RaiseLog("SDK", IrisLogLevel::Info, "Initialization completed.");
    }

    void IrisScannerBridge::StartEnroll(String^ subjectId)
    {
        EnsureNotDisposed();

        if (!initialized_)
        {
            throw gcnew InvalidOperationException("Initialize the SDK before enrolling.");
        }

        currentSubjectId_ = String::IsNullOrWhiteSpace(subjectId) ? "subject" : subjectId->Trim();
        const auto config = CreateConfig(IKIrisWorkModeEnroll);
        ThrowIfImmediateError("Config", sdk_->Config(config));
        ThrowIfImmediateError("StartEnroll", sdk_->StartEnroll());
    }

    void IrisScannerBridge::StopEnroll()
    {
        EnsureNotDisposed();

        if (!initialized_)
        {
            return;
        }

        sdk_->StopEnroll();
    }

    void IrisScannerBridge::StartIdentify(IEnumerable<IrisTemplateRecord^>^ templates)
    {
        EnsureNotDisposed();

        if (!initialized_)
        {
            throw gcnew InvalidOperationException("Initialize the SDK before identification.");
        }

        PrepareIdentifyInput(templates);

        if (leftTemplates_->Count == 0 && rightTemplates_->Count == 0)
        {
            throw gcnew InvalidOperationException("At least one left or right iris template is required.");
        }

        const auto config = CreateConfig(IKIrisWorkModeIdent);
        ThrowIfImmediateError("Config", sdk_->Config(config));

        unsigned char* leftPtr = leftBuffer_->empty() ? nullptr : leftBuffer_->data();
        unsigned char* rightPtr = rightBuffer_->empty() ? nullptr : rightBuffer_->data();

        ThrowIfImmediateError(
            "StartIdent",
            sdk_->StartIdent(
                leftPtr,
                leftTemplates_->Count,
                rightPtr,
                rightTemplates_->Count,
                nullptr,
                0));
    }

    void IrisScannerBridge::StopIdentify()
    {
        EnsureNotDisposed();

        if (!initialized_)
        {
            return;
        }

        sdk_->StopIdent();
    }

    void IrisScannerBridge::MoveAlignmentUp()
    {
        EnsureNotDisposed();

        if (!initialized_)
        {
            throw gcnew InvalidOperationException("Initialize the SDK before moving the device alignment.");
        }

        auto effectiveSettings = settings_ == nullptr ? gcnew IrisScannerSettings() : settings_;
        int stepValue = effectiveSettings->AlignmentStepValue;
        if (stepValue < 0)
        {
            stepValue = -stepValue;
        }
        if (stepValue == 0)
        {
            stepValue = 35;
        }

        const int direction = effectiveSettings->InvertTiltDirection ? -1 : 1;
        const int value = stepValue * direction;

        RaiseLog("PTZ", IrisLogLevel::Info, String::Format("PTZ_TEST up: status=1, value={0}", value));
        const int moveResult = sdk_->WriteParameter(IKCOMPONENT_PTZ_TEST, IKCOMPONENTON, value, MODE_DEGUALT);
        RaiseLog("PTZ", IrisLogLevel::Info, String::Format("PTZ_TEST up -> {0}", moveResult));
        ThrowIfImmediateError("MoveAlignmentUp", moveResult);
    }

    void IrisScannerBridge::MoveAlignmentDown()
    {
        EnsureNotDisposed();

        if (!initialized_)
        {
            throw gcnew InvalidOperationException("Initialize the SDK before moving the device alignment.");
        }

        auto effectiveSettings = settings_ == nullptr ? gcnew IrisScannerSettings() : settings_;
        int stepValue = effectiveSettings->AlignmentStepValue;
        if (stepValue < 0)
        {
            stepValue = -stepValue;
        }
        if (stepValue == 0)
        {
            stepValue = 35;
        }

        const int direction = effectiveSettings->InvertTiltDirection ? -1 : 1;

        IKCOMPONENTSTATUS status = IKCOMPONENTON;
        int value = -stepValue * direction;
        if (effectiveSettings->UseStatusOffForTiltDown)
        {
            status = IKCOMPONENTOFF;
            value = stepValue * direction;
        }

        RaiseLog("PTZ", IrisLogLevel::Info, String::Format("PTZ_TEST down: status={0}, value={1}", static_cast<int>(status), value));
        const int moveResult = sdk_->WriteParameter(IKCOMPONENT_PTZ_TEST, status, value, MODE_DEGUALT);
        RaiseLog("PTZ", IrisLogLevel::Info, String::Format("PTZ_TEST down -> {0}", moveResult));
        ThrowIfImmediateError("MoveAlignmentDown", moveResult);
    }

    void IrisScannerBridge::HomeAlignment()
    {
        EnsureNotDisposed();

        if (!initialized_)
        {
            throw gcnew InvalidOperationException("Initialize the SDK before resetting the device alignment.");
        }

        RaiseLog("PTZ", IrisLogLevel::Info, "PTZ_HOMING requested.");
        const int moveResult = sdk_->WriteParameter(IKCOMPONENT_PTZ_HOMING, IKCOMPONENTON, 0, MODE_DEGUALT);
        RaiseLog("PTZ", IrisLogLevel::Info, String::Format("PTZ_HOMING -> {0}", moveResult));
        ThrowIfImmediateError("HomeAlignment", moveResult);
    }

    void IrisScannerBridge::Release()
    {
        EnsureNotDisposed();

        if (!initialized_ || sdk_ == nullptr)
        {
            return;
        }

        sdk_->Release();
        initialized_ = false;
        ResetIdentifyInput();
        RaiseLog("SDK", IrisLogLevel::Info, "Release completed.");
    }

    void IrisScannerBridge::OnInitCompleted(int result)
    {
        lastInitResult_ = result;
        initSignal_->Set();
    }

    void IrisScannerBridge::OnEnrollCompleted(IKBaseEnrollProcessedResult& result)
    {
        auto templates = gcnew List<IrisTemplateRecord^>();
        const int frameCount = result.getIrisFrameNum();

        for (int index = 0; index < frameCount; ++index)
        {
            IKIrisProcessedResult irisFrame;
            if (!result.getIrisResultData(index, irisFrame))
            {
                continue;
            }

            templates->Add(CreateTemplateRecord(irisFrame));
        }

        const bool success = result.getEnrollIdentResFlag() > IKEnrRecUnknown;
        const int vendorCode = result.getIrisRes();
        String^ message = success
            ? String::Format("Enrollment completed with {0} iris template(s).", templates->Count)
            : String::Format("Enrollment failed with vendor code {0}.", vendorCode);

        if (EnrollCompleted != nullptr)
        {
            EnrollCompleted(this, gcnew IrisEnrollCompletedEventArgs(success, vendorCode, message, templates));
        }
    }

    void IrisScannerBridge::OnIdentifyCompleted(const IKIdentProcessedResultStruct& result)
    {
        bool success = result._enrollIdentResFlag > IKEnrRecUnknown;
        int vendorCode = result._irisIdentResult._nFunResult;
        String^ matchedSubjectId = nullptr;
        IrisEye matchedEye = IrisEye::Unknown;
        int matchScore = -1;
        int leftMatchIndex = result._irisIdentResult._matchOutputL.matchIndex;
        int rightMatchIndex = result._irisIdentResult._matchOutputR.matchIndex;
        String^ message;

        if (success)
        {
            switch (result._irisIdentResult._flag)
            {
            case IKIrisEnrRecLeftSuccess:
                matchedEye = IrisEye::Left;
                if (TryGetMatchedSubject(leftTemplates_, leftMatchIndex, matchedSubjectId))
                {
                    matchScore = result._irisIdentResult._matchOutputL.matchScore;
                }
                break;
            case IKIrisEnrRecRightSuccess:
                matchedEye = IrisEye::Right;
                if (TryGetMatchedSubject(rightTemplates_, rightMatchIndex, matchedSubjectId))
                {
                    matchScore = result._irisIdentResult._matchOutputR.matchScore;
                }
                break;
            case IKIrisEnrRecBothSuccess:
                matchedEye = IrisEye::Both;
                {
                    String^ leftSubject = nullptr;
                    String^ rightSubject = nullptr;

                    if (TryGetMatchedSubject(leftTemplates_, leftMatchIndex, leftSubject) &&
                        TryGetMatchedSubject(rightTemplates_, rightMatchIndex, rightSubject))
                    {
                        if (String::Equals(leftSubject, rightSubject, StringComparison::OrdinalIgnoreCase))
                        {
                            matchedSubjectId = leftSubject;
                        }
                        else
                        {
                            success = false;
                            message = "Left and right matches resolved to different subjects.";
                        }

                        matchScore = Math::Max(
                            result._irisIdentResult._matchOutputL.matchScore,
                            result._irisIdentResult._matchOutputR.matchScore);
                    }
                }
                break;
            default:
                break;
            }
        }

        if (String::IsNullOrWhiteSpace(message))
        {
            message = success
                ? String::Format("Identification succeeded. Match: {0}.", matchedSubjectId)
                : String::Format("Identification failed with vendor code {0}.", vendorCode);
        }

        if (IdentifyCompleted != nullptr)
        {
            IdentifyCompleted(
                this,
                gcnew IrisIdentifyCompletedEventArgs(
                    success,
                    vendorCode,
                    message,
                    matchedSubjectId,
                    matchedEye,
                    matchScore,
                    leftMatchIndex,
                    rightMatchIndex));
        }
    }

    void IrisScannerBridge::OnStateUpdated(const IKProcessorState& state)
    {
        if (state.dataValidFlag != IrisDataValid || state.pIrisState == nullptr)
        {
            return;
        }

        auto irisState = state.pIrisState;
        lastDistance_ = irisState->irisPos.dist;
        lastLeftIrisX_ = irisState->irisPos.leftIrisPos.xPixel;
        lastLeftIrisY_ = irisState->irisPos.leftIrisPos.yPixel;
        lastLeftIrisRadius_ = irisState->irisPos.leftIrisPos.radius;
        lastRightIrisX_ = irisState->irisPos.rightIrisPos.xPixel;
        lastRightIrisY_ = irisState->irisPos.rightIrisPos.yPixel;
        lastRightIrisRadius_ = irisState->irisPos.rightIrisPos.radius;
        if (StateUpdated != nullptr)
        {
            StateUpdated(
                this,
                gcnew IrisStateUpdatedEventArgs(
                    FromNativeWorkMode(irisState->workMode),
                    irisState->leftIrisProgress,
                    irisState->rightIrisProgress,
                    irisState->irisPos.dist,
                    irisState->throwAwayFrameStatus,
                    TranslateStateCode(irisState->throwAwayFrameStatus)));
        }
    }

    void IrisScannerBridge::OnDebugInfoUpdated(const IKDebugInfo& debugInfo)
    {
        if (debugInfo.dataValidFlag != IrisDataValid ||
            debugInfo.irisDebugInfo == nullptr ||
            debugInfo.irisDebugInfo->irisInfo == nullptr ||
            debugInfo.irisDebugInfo->irisInfoNum <= 0)
        {
            return;
        }

        int leftFocusScore = 0;
        int rightFocusScore = 0;
        int leftPercentVisible = 0;
        int rightPercentVisible = 0;
        int leftBrightness = 0;
        int rightBrightness = 0;
        int leftMotionBlur = 0;
        int rightMotionBlur = 0;
        int leftQualityScore = 0;
        int rightQualityScore = 0;

        auto irisInfo = debugInfo.irisDebugInfo->irisInfo;
        const int irisInfoCount = debugInfo.irisDebugInfo->irisInfoNum;

        for (int index = 0; index < irisInfoCount; index++)
        {
            const auto& info = irisInfo[index];
            const IrisEye eye = FromNativeEye(info.ImgType);

            if (eye == IrisEye::Left)
            {
                leftFocusScore = info.FocusScore;
                leftPercentVisible = info.PercentVisible;
                leftBrightness = info.Brightness;
                leftMotionBlur = info.MotionBlur;
                leftQualityScore = info.QualityScore;
            }
            else if (eye == IrisEye::Right)
            {
                rightFocusScore = info.FocusScore;
                rightPercentVisible = info.PercentVisible;
                rightBrightness = info.Brightness;
                rightMotionBlur = info.MotionBlur;
                rightQualityScore = info.QualityScore;
            }
        }

        if (IrisQualityUpdated != nullptr)
        {
            IrisQualityUpdated(
                this,
                gcnew IrisQualityUpdatedEventArgs(
                    lastDistance_,
                    leftFocusScore,
                    rightFocusScore,
                    leftPercentVisible,
                    rightPercentVisible,
                    leftBrightness,
                    rightBrightness,
                    leftMotionBlur,
                    rightMotionBlur,
                    leftQualityScore,
                    rightQualityScore,
                    lastLeftIrisX_,
                    lastLeftIrisY_,
                    lastLeftIrisRadius_,
                    lastRightIrisX_,
                    lastRightIrisY_,
                    lastRightIrisRadius_));
        }
    }

    void IrisScannerBridge::OnLog(const char* logType, IKLog logLevel, const char* logInfo)
    {
        RaiseLog(ToManagedString(logType), static_cast<IrisLogLevel>(logLevel), ToManagedString(logInfo));
    }

    void IrisScannerBridge::OnCameraUpdated(const IKCameraUpdated& cameraUpdate)
    {
        if (!initialized_)
        {
            return;
        }

        const int cameraEventCode = static_cast<int>(cameraUpdate.event);
        String^ deviceSn = ToManagedString(cameraUpdate.deviceSN, cameraUpdate.deviceSNLen);
        String^ deviceType = ToManagedString(cameraUpdate.deviceTypeString, cameraUpdate.deviceTypeStringLen);

        if (cameraEventCode != lastDeviceEventCode_ ||
            !String::Equals(deviceSn, lastDeviceSn_, StringComparison::Ordinal) ||
            !String::Equals(deviceType, lastDeviceType_, StringComparison::Ordinal))
        {
            lastDeviceEventCode_ = cameraEventCode;
            lastDeviceSn_ = deviceSn;
            lastDeviceType_ = deviceType;

            if (DeviceInfoUpdated != nullptr)
            {
                DeviceInfoUpdated(this, gcnew DeviceInfoUpdatedEventArgs(deviceSn, deviceType, cameraEventCode));
            }
        }

        String^ message = TranslateCameraEventMessage(cameraUpdate.event);
        if (String::IsNullOrWhiteSpace(message))
        {
            RaisePreviewFrame(cameraUpdate);
            return;
        }

        IrisLogLevel level = cameraUpdate.event == IKUSB_HOTPLUG_EVENT_DEVICE_ARRIVED
            ? IrisLogLevel::Info
            : IrisLogLevel::Warn;

        if (ShouldLogCameraEvent(cameraUpdate.event))
        {
            RaiseLog("Camera", level, message);
        }
        RaisePreviewFrame(cameraUpdate);
    }

    void IrisScannerBridge::EnsureNotDisposed()
    {
        if (disposed_)
        {
            throw gcnew ObjectDisposedException("IrisScannerBridge");
        }
    }

    IKConfigInfo IrisScannerBridge::CreateConfig(IKIrisWorkMode workMode)
    {
        auto effectiveSettings = settings_ == nullptr ? gcnew IrisScannerSettings() : settings_;
        IKConfigInfo config;
        config.mode = IKModeIris;
        config.irisMode = ToNativeEye(effectiveSettings->CaptureMode);
        config.irisEnrollIsEnforce = effectiveSettings->EnforceEnroll;
        config.irisWorkMode = workMode;
        config.irisCount = effectiveSettings->EnrollSampleCount;
        config.faceCount = 0;
        config.overTime = effectiveSettings->TimeoutSeconds;
        return config;
    }

    void IrisScannerBridge::PrepareIdentifyInput(IEnumerable<IrisTemplateRecord^>^ templates)
    {
        ResetIdentifyInput();

        if (templates == nullptr)
        {
            return;
        }

        for each (IrisTemplateRecord ^ record in templates)
        {
            if (record->Eye == IrisEye::Left)
            {
                AppendIdentifyTemplate(record, leftTemplates_, leftBuffer_);
            }
            else if (record->Eye == IrisEye::Right)
            {
                AppendIdentifyTemplate(record, rightTemplates_, rightBuffer_);
            }
        }
    }

    void IrisScannerBridge::ResetIdentifyInput()
    {
        leftTemplates_->Clear();
        rightTemplates_->Clear();
        leftBuffer_->clear();
        rightBuffer_->clear();
    }

    void IrisScannerBridge::ThrowIfImmediateError(String^ operation, int code)
    {
        if (code == IK_Fun_Success)
        {
            return;
        }

        String^ vendorMessage = TranslateVendorError(code);
        if (String::IsNullOrWhiteSpace(vendorMessage))
        {
            throw gcnew InvalidOperationException(String::Format("{0} failed with vendor code {1}.", operation, code));
        }

        throw gcnew InvalidOperationException(
            String::Format("{0} failed with vendor code {1}: {2}", operation, code, vendorMessage));
    }

    IrisTemplateRecord^ IrisScannerBridge::CreateTemplateRecord(const IKIrisProcessedResult& irisFrame)
    {
        auto record = gcnew IrisTemplateRecord();
        record->SubjectId = currentSubjectId_;
        record->Eye = FromNativeEye(irisFrame.irisInfo.ImgType);
        record->Width = irisFrame.irisInfo.ImgWidth;
        record->Height = irisFrame.irisInfo.ImgHeight;
        record->FocusScore = irisFrame.irisInfo.FocusScore;
        record->PercentVisible = irisFrame.irisInfo.PercentVisible;
        record->Brightness = irisFrame.irisInfo.Brightness;
        record->MotionBlur = irisFrame.irisInfo.MotionBlur;
        record->QualityScore = irisFrame.irisInfo.QualityScore;

        const int imageLength = irisFrame.irisInfo.imgDataLen > 0 ? irisFrame.irisInfo.imgDataLen : IK_IRIS_IMG_SIZE;
        const int templateLength =
            irisFrame.irisInfo.irisEnrTemplateLen > 0 ? irisFrame.irisInfo.irisEnrTemplateLen : IK_IRIS_ENROLL_FEATURE_SIZE;

        record->PreviewImage = CopyToManagedArray(irisFrame.irisInfo.imgData, imageLength);
        record->TemplateBytes = CopyToManagedArray(irisFrame.irisInfo.irisEnrTemplate, templateLength);
        return record;
    }

    void IrisScannerBridge::AppendIdentifyTemplate(
        IrisTemplateRecord^ record,
        List<IrisTemplateRecord^>^ targetTemplates,
        std::vector<unsigned char>* targetBuffer)
    {
        if (record == nullptr || record->TemplateBytes == nullptr)
        {
            return;
        }

        if (record->TemplateBytes->Length != IK_IRIS_ENROLL_FEATURE_SIZE)
        {
            return;
        }

        targetTemplates->Add(record);
        const auto offset = targetBuffer->size();
        targetBuffer->resize(offset + IK_IRIS_ENROLL_FEATURE_SIZE);
        pin_ptr<Byte> pinned = &record->TemplateBytes[0];
        memcpy(targetBuffer->data() + offset, pinned, IK_IRIS_ENROLL_FEATURE_SIZE);
    }

    bool IrisScannerBridge::ShouldLogCameraEvent(int eventCode)
    {
        const Int64 nowMs = DateTime::Now.Ticks / TimeSpan::TicksPerMillisecond;

        if (eventCode == IKUSB_HOTPLUG_EVENT_DEVICE_ARRIVED || eventCode == IKUSB_HOTPLUG_EVENT_DEVICE_LEFT)
        {
            if (eventCode == lastCameraEventCode_)
            {
                return false;
            }

            lastCameraEventCode_ = eventCode;
            lastCameraEventLoggedAtMs_ = nowMs;
            return true;
        }

        const bool isRepeatedEvent = eventCode == lastCameraEventCode_;
        const bool isWithinSuppressionWindow = nowMs - lastCameraEventLoggedAtMs_ < 2000;
        if (isRepeatedEvent && isWithinSuppressionWindow)
        {
            return false;
        }

        lastCameraEventCode_ = eventCode;
        lastCameraEventLoggedAtMs_ = nowMs;
        return true;
    }

    void IrisScannerBridge::RaisePreviewFrame(const IKCameraUpdated& cameraUpdate)
    {
        if (PreviewFrameUpdated == nullptr || cameraUpdate.frame == nullptr || cameraUpdate.frame->empty())
        {
            return;
        }

        if (cameraUpdate.frame->image_data.get() == nullptr || cameraUpdate.frame->data_len <= 0)
        {
            return;
        }

        int width = cameraUpdate.frame->width;
        int height = cameraUpdate.frame->height;
        if (width <= 0 || height <= 0)
        {
            return;
        }

        if (cameraUpdate.frame->frame_type == IK_Frame_IRIS_VGA_LR)
        {
            width *= 2;
        }

        auto imageBytes = CopyToManagedArray(
            cameraUpdate.frame->image_data.get(),
            static_cast<int>(cameraUpdate.frame->data_len));

        PreviewFrameUpdated(
            this,
            gcnew CameraPreviewFrameEventArgs(
                imageBytes,
                width,
                height,
                cameraUpdate.frame->rotation,
                cameraUpdate.mirrorFlag,
                FromNativeFrameFormat(cameraUpdate.frame->format),
                FromNativeFrameType(cameraUpdate.frame->frame_type)));
    }

    bool IrisScannerBridge::TryGetMatchedSubject(
        List<IrisTemplateRecord^>^ templates,
        int matchIndex,
        String^% matchedSubjectId)
    {
        if (matchIndex < 0 || matchIndex >= templates->Count)
        {
            return false;
        }

        matchedSubjectId = templates[matchIndex]->SubjectId;
        return true;
    }

    IKIrisMode IrisScannerBridge::ToNativeEye(IrisEye eye)
    {
        switch (eye)
        {
        case IrisEye::Left:
            return IKIrisModeLeft;
        case IrisEye::Right:
            return IKIrisModeRight;
        case IrisEye::Both:
            return IKIrisModeBoth;
        default:
            return IKIrisModeUndef;
        }
    }

    IrisEye IrisScannerBridge::FromNativeEye(int nativeValue)
    {
        switch (nativeValue)
        {
        case IKIrisModeLeft:
            return IrisEye::Left;
        case IKIrisModeRight:
            return IrisEye::Right;
        case IKIrisModeBoth:
            return IrisEye::Both;
        default:
            return IrisEye::Unknown;
        }
    }

    IrisWorkMode IrisScannerBridge::FromNativeWorkMode(IKIrisWorkMode nativeMode)
    {
        switch (nativeMode)
        {
        case IKIrisWorkModeEnroll:
            return IrisWorkMode::Enroll;
        case IKIrisWorkModeIdent:
            return IrisWorkMode::Identify;
        case IKIrisWorkModeCapture:
            return IrisWorkMode::Capture;
        default:
            return IrisWorkMode::Idle;
        }
    }

    CameraFrameFormat IrisScannerBridge::FromNativeFrameFormat(IKFrameFormat nativeFormat)
    {
        switch (nativeFormat)
        {
        case IK_Frame_Format_Gray_8:
            return CameraFrameFormat::Gray8;
        case IK_Frame_Format_BGR:
            return CameraFrameFormat::Bgr24;
        case IK_Frame_Format_RGB:
            return CameraFrameFormat::Rgb24;
        case IK_Frame_Format_BGRA:
            return CameraFrameFormat::Bgra32;
        case IK_Frame_Format_RGBA:
            return CameraFrameFormat::Rgba32;
        case IK_Frame_Format_MJPEG:
            return CameraFrameFormat::Mjpeg;
        default:
            return CameraFrameFormat::Unknown;
        }
    }

    CameraFrameType IrisScannerBridge::FromNativeFrameType(IKFrameType nativeType)
    {
        switch (nativeType)
        {
        case IK_Frame_Light:
            return CameraFrameType::Light;
        case IK_Frame_Dark:
            return CameraFrameType::Dark;
        case IK_Frame_Depth:
            return CameraFrameType::Depth;
        case IK_Frame_IR:
            return CameraFrameType::Infrared;
        case IK_Frame_IRIS:
            return CameraFrameType::Iris;
        case IK_Frame_IRIS_VGA_L:
            return CameraFrameType::IrisLeft;
        case IK_Frame_IRIS_VGA_R:
            return CameraFrameType::IrisRight;
        case IK_Frame_IRIS_VGA_LR:
            return CameraFrameType::IrisBoth;
        default:
            return CameraFrameType::Unknown;
        }
    }

    String^ IrisScannerBridge::TranslateVendorError(int code)
    {
        switch (code)
        {
        case IK_Err_INIT_FAILED:
            return "SDK initialization failed or the SDK is not initialized.";
        case IK_Err_INVALID_PARAMETER:
            return "Invalid initialization or configuration parameter.";
        case IK_Err_CREATEIRISCAMERA_FAILED:
            return "The SDK could not create the iris camera object.";
        case IK_Err_CREATEFACECAMERA_FAILED:
            return "The SDK could not create the face camera object.";
        case IK_Err_CREATEDEPTHCAMERA_FAILED:
            return "The SDK could not create the depth camera object.";
        case IK_Err_CREATEPTZ_FAILED:
            return "The SDK could not create the PTZ control object.";
        case IK_Err_INVALID_ENROLLIDENTMODE:
            return "The requested enroll or identify mode is invalid.";
        case IK_Err_INVALID_MATCHMODE:
            return "The requested match mode is invalid.";
        case IK_Err_NOFOUND_CONFIGFILE:
            return "IKConfig.ini was not found beside the executable.";
        case IK_Err_STARTIDENT_ModeUndef_FAILED:
            return "Identification startup failed for both supported modes; check the input parameters.";
        case IK_Err_CONFIG_FAILED:
            return "Config failed. The SDK expects the device to be idle before Config is called.";
        case IK_Err_SETCONFIGFILEPATH_FAILED:
            return "Setting the SDK config file path failed.";
        case IK_Err_ASYNCTASK_BUSY:
            return "The SDK is still busy with a previous asynchronous task.";
        case IK_Err_NOTSUPPORT:
            return "The SDK does not support this operation.";
        case IK_Err_REPEATINIT:
            return "The SDK has already been initialized.";
        case IK_Err_INVALID_DEVICETYPE:
            return "The device type in IKConfig.ini is invalid for this hardware.";
        case E_CAMERA_INVALID_INPUT:
            return "The camera API received an invalid input parameter.";
        case E_CAMERA_INVALID_DEVICETYPE:
            return "The camera device type is invalid.";
        case E_CAMERA_INIT_FAILED:
            return "Camera environment initialization failed.";
        case E_CAMERA_DEVICE_NOTFOUND:
            return "No compatible camera device was found.";
        case E_CAMERA_OPEN_FAILED:
            return "The camera device could not be opened.";
        case E_CAMERA_INVALID_FORMAT_SETTING:
            return "The configured camera stream format is invalid.";
        case E_CAMERA_WAITCB_TIMEOUT:
            return "Timed out waiting for the camera callback.";
        case E_CAMERA_NOT_SUPPORT:
            return "The camera API does not support this operation.";
        case E_CAMERA_INTERFACE_FAILED:
            return "A camera API call failed for an unspecified reason.";
        case E_CAMERA_DEVICE_HAS_OPEND:
            return "The device is already open in another process or already opened by this SDK instance.";
        case E_CAMERA_DEVICE_CTRLCOM_ERROR:
            return "The scanner control serial port could not be opened.";
        case E_CAMERA_READ_DEVICE_PROP:
            return "Reading device properties failed.";
        case E_CAMERA_WRITE_DEVICE_PROP:
            return "Writing device properties failed.";
        case E_CAMERA_LIB_NOT_FOUND:
            return "A required camera dependency DLL could not be loaded.";
        default:
            return String::Empty;
        }
    }

    String^ IrisScannerBridge::TranslateStateCode(int statusCode)
    {
        switch (statusCode)
        {
        case IKIrisUIUE_MsgID_SUITABLE:
            return "Aligned";
        case IKIrisUIUE_MsgID_EYE_NOT_FOUND:
            return "Eye not found";
        case IKIrisUIUE_MsgID_EYE_MOVE_CLOSE:
            return "Move closer";
        case IKIrisUIUE_MsgID_EYE_MOVE_FAR:
            return "Move farther";
        case IKIrisUIUE_MsgID_EYE_MOVE_UP:
            return "Move up";
        case IKIrisUIUE_MsgID_EYE_MOVE_DOWN:
            return "Move down";
        case IKIrisUIUE_MsgID_EYE_MOVE_LEFT:
            return "Move left";
        case IKIrisUIUE_MsgID_EYE_MOVE_RIGHT:
            return "Move right";
        case IKIrisUIUE_MsgID_MOTION_BLUR:
            return "Hold steady";
        case IKIrisUIUE_MsgID_FOCUS_BLUR:
            return "Refocus";
        case IKIrisUIUE_MsgID_BAD_EYE_OPENNESS:
            return "Open eyes wider";
        case IKIrisUIUE_MsgID_WITH_GLASS:
            return "Adjust or remove glasses";
        case IKIrisUIUE_MsgID_ATTACK:
            return "Register before identifying";
        case IKIrisUIUE_MsgID_FAKE:
            return "Spoof detected";
        case IKIrisUIUE_MsgID_BP:
            return "Remove contact lenses";
        case IKIrisUIUE_MsgID_OFFANGLE:
            return "Look directly at the camera";
        case IKIrisUIUE_MsgID_IDLE_CLEAR:
            return "Idle";
        case CBDEFAULTVALUE:
            return String::Empty;
        default:
            return String::Format("Vendor state {0}", statusCode);
        }
    }

    String^ IrisScannerBridge::TranslateCameraEventMessage(int eventCode)
    {
        switch (eventCode)
        {
        case IKUSB_HOTPLUG_EVENT_DEVICE_ARRIVED:
            return "Device attached.";
        case IKUSB_HOTPLUG_EVENT_DEVICE_LEFT:
            return "Device detached.";
        case IKUSB_EVENT_DEVICE_UPSIDE:
            return "Device is upside down.";
        case IKUSB_MIRRORFLAG_CHECK_ERROR:
            return "Mirror flag mismatch detected.";
        default:
            return String::Empty;
        }
    }

    void IrisScannerBridge::RaiseLog(String^ category, IrisLogLevel level, String^ message)
    {
        if (LogReceived != nullptr)
        {
            LogReceived(this, gcnew ScannerLogEventArgs(category, level, message));
        }
    }
}
