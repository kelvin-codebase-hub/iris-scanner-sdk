#pragma once

namespace IrisScanner
{
    public enum class IrisEye
    {
        Unknown = 0,
        Left = 1,
        Right = 2,
        Both = 3
    };

    public enum class IrisWorkMode
    {
        Idle = -1,
        Enroll = 0,
        Identify = 1,
        Capture = 2
    };

    public enum class IrisLogLevel
    {
        Status = 1,
        Verbose = 2,
        Debug = 3,
        Info = 4,
        Warn = 5,
        Error = 6,
        Performance = 7
    };

    public enum class IrisInitMode
    {
        Iris = 0,
        Face = 1,
        Default = 2,
        Both = 3
    };

    public enum class CameraFrameFormat
    {
        Unknown = 0,
        Gray8 = 1,
        Bgr24 = 2,
        Rgb24 = 3,
        Bgra32 = 4,
        Rgba32 = 5,
        Mjpeg = 6
    };

    public enum class CameraFrameType
    {
        Unknown = 0,
        Light = 1,
        Dark = 2,
        Depth = 3,
        Infrared = 4,
        Iris = 5,
        IrisLeft = 6,
        IrisRight = 7,
        IrisBoth = 8
    };

    public ref class IrisScannerSettings sealed
    {
    public:
        IrisScannerSettings()
        {
            CaptureMode = IrisEye::Both;
            EnrollSampleCount = 3;
            TimeoutSeconds = 30;
            EnforceEnroll = false;
            AlignmentStepValue = 35;
            InvertTiltDirection = false;
            UseStatusOffForTiltDown = false;
        }

        property IrisEye CaptureMode;
        property int EnrollSampleCount;
        property int TimeoutSeconds;
        property bool EnforceEnroll;
        property int AlignmentStepValue;
        property bool InvertTiltDirection;
        property bool UseStatusOffForTiltDown;
    };

    public ref class IrisTemplateRecord sealed
    {
    public:
        IrisTemplateRecord()
        {
            SubjectId = System::String::Empty;
            Eye = IrisEye::Unknown;
            TemplateBytes = nullptr;
            PreviewImage = nullptr;
            Width = 0;
            Height = 0;
            FocusScore = 0;
            PercentVisible = 0;
            Brightness = 0;
            MotionBlur = 0;
            QualityScore = 0;
        }

        property System::String^ SubjectId;
        property IrisEye Eye;
        property cli::array<System::Byte>^ TemplateBytes;
        property cli::array<System::Byte>^ PreviewImage;
        property int Width;
        property int Height;
        property int FocusScore;
        property int PercentVisible;
        property int Brightness;
        property int MotionBlur;
        property int QualityScore;
    };

    public ref class ScannerLogEventArgs sealed : System::EventArgs
    {
    public:
        ScannerLogEventArgs(System::String^ category, IrisLogLevel level, System::String^ message)
        {
            Category = category;
            Level = level;
            Message = message;
        }

        property System::String^ Category;
        property IrisLogLevel Level;
        property System::String^ Message;
    };

    public ref class IrisStateUpdatedEventArgs sealed : System::EventArgs
    {
    public:
        IrisStateUpdatedEventArgs(
            IrisWorkMode workMode,
            double leftProgress,
            double rightProgress,
            int distance,
            int statusCode,
            System::String^ statusMessage)
        {
            WorkMode = workMode;
            LeftProgress = leftProgress;
            RightProgress = rightProgress;
            Distance = distance;
            StatusCode = statusCode;
            StatusMessage = statusMessage;
        }

        property IrisWorkMode WorkMode;
        property double LeftProgress;
        property double RightProgress;
        property int Distance;
        property int StatusCode;
        property System::String^ StatusMessage;
    };

    public ref class IrisQualityUpdatedEventArgs sealed : System::EventArgs
    {
    public:
        IrisQualityUpdatedEventArgs(
            int distance,
            int leftFocusScore,
            int rightFocusScore,
            int leftPercentVisible,
            int rightPercentVisible,
            int leftBrightness,
            int rightBrightness,
            int leftMotionBlur,
            int rightMotionBlur,
            int leftQualityScore,
            int rightQualityScore,
            int leftIrisX,
            int leftIrisY,
            int leftIrisRadius,
            int rightIrisX,
            int rightIrisY,
            int rightIrisRadius)
        {
            Distance = distance;
            LeftFocusScore = leftFocusScore;
            RightFocusScore = rightFocusScore;
            LeftPercentVisible = leftPercentVisible;
            RightPercentVisible = rightPercentVisible;
            LeftBrightness = leftBrightness;
            RightBrightness = rightBrightness;
            LeftMotionBlur = leftMotionBlur;
            RightMotionBlur = rightMotionBlur;
            LeftQualityScore = leftQualityScore;
            RightQualityScore = rightQualityScore;
            LeftIrisX = leftIrisX;
            LeftIrisY = leftIrisY;
            LeftIrisRadius = leftIrisRadius;
            RightIrisX = rightIrisX;
            RightIrisY = rightIrisY;
            RightIrisRadius = rightIrisRadius;
        }

        property int Distance;
        property int LeftFocusScore;
        property int RightFocusScore;
        property int LeftPercentVisible;
        property int RightPercentVisible;
        property int LeftBrightness;
        property int RightBrightness;
        property int LeftMotionBlur;
        property int RightMotionBlur;
        property int LeftQualityScore;
        property int RightQualityScore;
        property int LeftIrisX;
        property int LeftIrisY;
        property int LeftIrisRadius;
        property int RightIrisX;
        property int RightIrisY;
        property int RightIrisRadius;
    };

    public ref class DeviceInfoUpdatedEventArgs sealed : System::EventArgs
    {
    public:
        DeviceInfoUpdatedEventArgs(
            System::String^ serialNumber,
            System::String^ deviceType,
            int cameraEventCode)
        {
            SerialNumber = serialNumber == nullptr ? System::String::Empty : serialNumber;
            DeviceType = deviceType == nullptr ? System::String::Empty : deviceType;
            CameraEventCode = cameraEventCode;
        }

        property System::String^ SerialNumber;
        property System::String^ DeviceType;
        property int CameraEventCode;
    };

    public ref class CameraPreviewFrameEventArgs sealed : System::EventArgs
    {
    public:
        CameraPreviewFrameEventArgs(
            cli::array<System::Byte>^ imageBytes,
            int width,
            int height,
            int rotation,
            bool mirrorFlag,
            CameraFrameFormat frameFormat,
            CameraFrameType frameType)
        {
            ImageBytes = imageBytes;
            Width = width;
            Height = height;
            Rotation = rotation;
            MirrorFlag = mirrorFlag;
            FrameFormat = frameFormat;
            FrameType = frameType;
        }

        property cli::array<System::Byte>^ ImageBytes;
        property int Width;
        property int Height;
        property int Rotation;
        property bool MirrorFlag;
        property CameraFrameFormat FrameFormat;
        property CameraFrameType FrameType;
    };

    public ref class IrisEnrollCompletedEventArgs sealed : System::EventArgs
    {
    public:
        IrisEnrollCompletedEventArgs(
            bool success,
            int vendorCode,
            System::String^ message,
            System::Collections::Generic::IList<IrisTemplateRecord^>^ templates)
        {
            Success = success;
            VendorCode = vendorCode;
            Message = message;
            Templates = templates;
        }

        property bool Success;
        property int VendorCode;
        property System::String^ Message;
        property System::Collections::Generic::IList<IrisTemplateRecord^>^ Templates;
    };

    public ref class IrisIdentifyCompletedEventArgs sealed : System::EventArgs
    {
    public:
        IrisIdentifyCompletedEventArgs(
            bool success,
            int vendorCode,
            System::String^ message,
            System::String^ matchedSubjectId,
            IrisEye matchedEye,
            int matchScore,
            int leftMatchIndex,
            int rightMatchIndex)
        {
            Success = success;
            VendorCode = vendorCode;
            Message = message;
            MatchedSubjectId = matchedSubjectId;
            MatchedEye = matchedEye;
            MatchScore = matchScore;
            LeftMatchIndex = leftMatchIndex;
            RightMatchIndex = rightMatchIndex;
        }

        property bool Success;
        property int VendorCode;
        property System::String^ Message;
        property System::String^ MatchedSubjectId;
        property IrisEye MatchedEye;
        property int MatchScore;
        property int LeftMatchIndex;
        property int RightMatchIndex;
    };
}
