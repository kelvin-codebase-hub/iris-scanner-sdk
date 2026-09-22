#pragma once

#include <vector>
#include "IKBaseSDKAPI.h"
#include "IrisScannerContracts.h"

namespace IrisScanner
{
    class NativeCallback;

    public ref class IrisScannerBridge sealed : System::IDisposable
    {
    public:
        IrisScannerBridge();
        ~IrisScannerBridge();
        !IrisScannerBridge();

        property bool IsInitialized
        {
            bool get();
        }

        void ApplySettings(IrisScannerSettings^ settings);
        System::String^ GetVersion();
        void Initialize();
        void Initialize(IrisInitMode mode);
        void StartEnroll(System::String^ subjectId);
        void StopEnroll();
        void StartIdentify(System::Collections::Generic::IEnumerable<IrisTemplateRecord^>^ templates);
        void StopIdentify();
        void MoveAlignmentUp();
        void MoveAlignmentDown();
        void HomeAlignment();
        void Release();

        System::EventHandler<ScannerLogEventArgs^>^ LogReceived;
        System::EventHandler<IrisStateUpdatedEventArgs^>^ StateUpdated;
        System::EventHandler<IrisQualityUpdatedEventArgs^>^ IrisQualityUpdated;
        System::EventHandler<DeviceInfoUpdatedEventArgs^>^ DeviceInfoUpdated;
        System::EventHandler<CameraPreviewFrameEventArgs^>^ PreviewFrameUpdated;
        System::EventHandler<IrisEnrollCompletedEventArgs^>^ EnrollCompleted;
        System::EventHandler<IrisIdentifyCompletedEventArgs^>^ IdentifyCompleted;

    internal:
        void OnInitCompleted(int result);
        void OnEnrollCompleted(IKBaseEnrollProcessedResult& result);
        void OnIdentifyCompleted(const IKIdentProcessedResultStruct& result);
        void OnStateUpdated(const IKProcessorState& state);
        void OnDebugInfoUpdated(const IKDebugInfo& debugInfo);
        void OnLog(const char* logType, IKLog logLevel, const char* logInfo);
        void OnCameraUpdated(const IKCameraUpdated& cameraUpdate);

    private:
        void EnsureNotDisposed();
        IKConfigInfo CreateConfig(IKIrisWorkMode workMode);
        void PrepareIdentifyInput(System::Collections::Generic::IEnumerable<IrisTemplateRecord^>^ templates);
        void ResetIdentifyInput();
        void ThrowIfImmediateError(System::String^ operation, int code);
        IrisTemplateRecord^ CreateTemplateRecord(const IKIrisProcessedResult& irisFrame);
        void AppendIdentifyTemplate(
            IrisTemplateRecord^ record,
            System::Collections::Generic::List<IrisTemplateRecord^>^ targetTemplates,
            std::vector<unsigned char>* targetBuffer);
        bool ShouldLogCameraEvent(int eventCode);
        void RaisePreviewFrame(const IKCameraUpdated& cameraUpdate);
        bool TryGetMatchedSubject(
            System::Collections::Generic::List<IrisTemplateRecord^>^ templates,
            int matchIndex,
            System::String^% matchedSubjectId);

        static IKIrisMode ToNativeEye(IrisEye eye);
        static IrisEye FromNativeEye(int nativeValue);
        static IrisWorkMode FromNativeWorkMode(IKIrisWorkMode nativeMode);
        static CameraFrameFormat FromNativeFrameFormat(IKFrameFormat nativeFormat);
        static CameraFrameType FromNativeFrameType(IKFrameType nativeType);
        static System::String^ TranslateVendorError(int code);
        static System::String^ TranslateStateCode(int statusCode);
        static System::String^ TranslateCameraEventMessage(int eventCode);

        void RaiseLog(System::String^ category, IrisLogLevel level, System::String^ message);

        NativeCallback* nativeCallback_;
        IKBaseSDKAPI* sdk_;
        bool disposed_;
        bool initialized_;
        int lastInitResult_;
        System::Threading::ManualResetEventSlim^ initSignal_;
        IrisScannerSettings^ settings_;
        System::String^ currentSubjectId_;
        System::Collections::Generic::List<IrisTemplateRecord^>^ leftTemplates_;
        System::Collections::Generic::List<IrisTemplateRecord^>^ rightTemplates_;
        std::vector<unsigned char>* leftBuffer_;
        std::vector<unsigned char>* rightBuffer_;
        int lastCameraEventCode_;
        System::Int64 lastCameraEventLoggedAtMs_;
        int lastDistance_;
        int lastLeftIrisX_;
        int lastLeftIrisY_;
        int lastLeftIrisRadius_;
        int lastRightIrisX_;
        int lastRightIrisY_;
        int lastRightIrisRadius_;
        System::String^ lastDeviceSn_;
        System::String^ lastDeviceType_;
        int lastDeviceEventCode_;
    };
}
