/*!
* @file IKBaseIrisAPIStruct.h
* @brief IrisAPI相关的一些数据结构定义以及错误码
*
* @author tangzhuan
* @version
* @date 2020.06.01
*/
#pragma once
#include <stdio.h>
#include <string>
#include <memory>
#include <memory.h>
#include <string.h>
#include <IKFrame.h>
using namespace std;
//////////////////////////////////////////虹膜相关信息结构///////////////////////////////////////////////
const int IK_VERSION_LENGTH = 500;/*!< 版本信息字符串长度*/
const int IK_UnknownInfoLen = 100;/*!< 预留信息长度*/
const int IK_CompleteInfoByte_IrisImFE = 520;/*!< 虹膜图像算法信息长度*/
const int IK_IRIS_ENROLL_FEATURE_SIZE = 512;/*!< 虹膜注册特征尺寸大小*/
const int IK_IRIS_IDEN_FEATURE_SIZE = 1024;/*!< 虹膜识别特征尺寸大小*/
const int IK_IRIS_IMG_HEIGHT = 480;/*!< VGA虹膜图像高*/
const int IK_IRIS_IMG_WIDTH = 640;/*!< VGA虹膜图像宽*/
const int IK_IRIS_IMG_SIZE = (IK_IRIS_IMG_HEIGHT * IK_IRIS_IMG_WIDTH);/*!< VGA虹膜图像尺寸大小*/
const int IK_IRIS_MIN_ENR_TIME = 0;/*!< 最小注册时间,单位:秒*/
const int IK_IRIS_MAX_ENR_TIME = 120;/*!< 最大注册时间*/
const int IK_IRIS_MIN_REC_TIME = 0;/*!< 最小识别时间*/
const int IK_IRIS_MAX_REC_TIME = 120;/*!< 最大识别时间*/
const int IK_IRIS_NEED_COUNT = 6;/*!< 默认虹膜数量*/

const int IKIris_Ori_ImH = 3000;/*!< 虹膜图像MAX宽*/
const int IKIris_Ori_ImW = 4000;/*!< 虹膜图像MAX高*/
const int IKIris_Ori_ImD = 1;/*!< 虹膜图像通道*/
const int IKIris_MAX_IM_SIZE	= (IKIris_Ori_ImH*IKIris_Ori_ImW*IKIris_Ori_ImD);/*!< 最大的允许虹膜图像尺寸*/

const int IKIris_RESERVEDINFO_LEN = 50;/*!< 预留信息长度*/
const int IKIris_RESERVEDINFO_DEBUG_FOCUSSCORE_L = 35;/*!< 清晰度值*/
const int IKIris_RESERVEDINFO_DEBUG_FOCUSSCORE_R = 36;/*!< 清晰度值*/
const int IKIris_RESERVEDINFO_DEBUG_PERCENTVISIBLE_L = 32;/*!< 遮挡程度*/
const int IKIris_RESERVEDINFO_DEBUG_PERCENTVISIBLE_R = 31;/*!< 遮挡程度*/
const int IKIris_RESERVEDINFO_DEBUG_BRIGHTNESS_L = 30;/*!< 图像亮度*/
const int IKIris_RESERVEDINFO_DEBUG_BRIGHTNESS_R = 29;/*!< 图像亮度*/
const int IKIris_RESERVEDINFO_DEBUG_MOTIONBLUR_L = 28;/*!< 运动模糊程度*/
const int IKIris_RESERVEDINFO_DEBUG_MOTIONBLUR_R = 27;/*!< 运动模糊程度*/
const int IKIris_RESERVEDINFO_DEBUG_GLASSFLAG_L = 26;/*!< 是否配带眼镜，1：戴眼镜；0：不戴眼镜*/
const int IKIris_RESERVEDINFO_DEBUG_GLASSFLAG_R = 25;/*!< 是否配带眼镜，1：戴眼镜；0：不戴眼镜*/
const int IKIris_RESERVEDINFO_DEBUG_QUALITYSCORE_L = 24;/*!< */
const int IKIris_RESERVEDINFO_DEBUG_QUALITYSCORE_R = 23;/*!< */

const int IKIris_IDQT_POS = 10;
const int IKIris_IDQT_FLAG = 1000;

/*!
* @brief IrisAPI相关错误码只能定义在-2000至-2999段内
*/
const int IKIris_Fun_Success = 0;/*!< 函数调用成功，并且完成了函数功能*/
const int IKIris_Err_BASE = 0;/*!< 基础错误码*/
//iris
const int IKIris_Err_ENR_IDEN_INTERRUPT = 1;/*!< 当人为调用停止注册，之前调用的注册将返回1*/
const int IKIris_Err_INIT_FAILED = IKIris_Err_BASE - 2001;/*!< IrisAPI初始化失败或者尚未初始化*/
const int IKIris_Err_INVALID_LICENSE = IKIris_Err_BASE - 2002;/*!< 无效的授权,授权过期,需要申请新的授权SDK*/
const int IKIris_Err_INVALID_PARAMETER = IKIris_Err_BASE - 2003; /*!< 无效的算法控制参数*/
const int IKIris_Err_INVALID_CONFIGTRACENUM = IKIris_Err_BASE - 2004;  /*!< 无效的跟踪数*/
const int IKIris_Err_INVALID_DELETEAREA = IKIris_Err_BASE - 2005;  /*!< 无效的检测区域*/
const int IKIris_Err_VERYFYDEV_ERROR = IKIris_Err_BASE - 2006; /*!< 设备校验出错*/
const int IKIris_Err_INVALID_OVERTIME = IKIris_Err_BASE - 2007;  /*!< 无效的超时时间*/
const int IKIris_Err_INVALID_ENRNUM = IKIris_Err_BASE - 2008;  /*!< 无效的注册数量*/
const int IKIris_Err_DEVICE_UPSIDEDOWN = IKIris_Err_BASE - 2009;	/*!< 设备上下翻转*/
const int IKIris_Err_ENR_IDEN_FAILED = IKIris_Err_BASE - 2010; /*!< 注册、识别、获取虹膜特征信息失败*/
const int IKIris_Err_INVALID_IRIS_DEVICETYPE = IKIris_Err_BASE - 2011;/*!< 无效的虹膜设备类型*/
const int IKIris_Err_FAIL2STOP = IKIris_Err_BASE - 2012;  /*!< 调用停止注册函数时失败*/
const int IKIris_Err_INVALID_IRIS_MODE = IKIris_Err_BASE - 2014; /*!< 无效的眼睛模式*/
const int IKIris_Err_ENROLLIDEN_OVERTIME = IKIris_Err_BASE - 2017; /*!< 注册识别时overtime*/
const int IKIris_Err_IDEN_UNSUPPORT_FEATURE_NUM = IKIris_Err_BASE - 2018; /*!< 识别时输入特征数目超过授权数目*/
const int IKIris_Err_ASYNCTASK_BUSY = IKIris_Err_BASE - 2022;	/*!< 异步任务正在执行，还没有结果*/
//alg
const int IKIris_Err_Fea_Unmatch = IKIris_Err_BASE - 2105;	//特征比对失败
const int IKIris_Err_Data_Interrupted = IKIris_Err_BASE - 2107;	//数据在线程之间传输时损坏
const int IKIris_Err_Invalid_Setting = IKIris_Err_BASE - 2108;	//无效的设置
const int IKIris_Err_Handle = IKIris_Err_BASE - 2109;	//算法API 的Handle配置有误或者为NULL
const int IKIris_Err_RESET_BUSY = IKIris_Err_BASE - 2110; //reset操作正在进行中
const int IKIris_Err_IMG_NUM_DIFFERROR = IKIris_Err_BASE - 2111; //图像上index标记比对失败
const int IKIris_Err_RESET_CLEAR = IKIris_Err_BASE - 2112; //reset操作引发的清理已注册图像or队列缓存
const int IKIris_Err_RESET_FAILED = IKIris_Err_BASE - 2113; //reset操作失败
const int IKIris_Err_QUEUE_FULL_CLEAR = IKIris_Err_BASE - 2114; //图像队列满了,引发的缓存清理
const int IKIris_Err_QUEUE_ENR_IDEN_END_CLEAR = IKIris_Err_BASE - 2115; //注册识别开始或结束缓存清理
const int IKIris_Err_CLEAR_CACHE_FAILED = IKIris_Err_BASE - 2116; //在工作模式不允许调用clearCache接口，失败
const int IKIris_Err_SELECTION_NOTREACHED = IKIris_Err_BASE - 2117; //质量选择选图像尚未达到目标

/*!
* @brief 根据状态回调进行UIUE提示信息，注释中""为建议提示用语
* @note 状态回调throwAwayFrameStatus描述了每帧处理失败的原因,如果它的值在以下枚举类型中则需要在界面做必要的提示以引导使用者
*/
enum IKIrisUIUEMsgID {
    IKIrisUIUE_MsgID_SUITABLE = 0,/*!< "当前状态合适" */
    IKIrisUIUE_MsgID_EYE_NOT_FOUND = IKIris_Err_BASE - 2501, /*!< "没有检测到虹膜" */
    IKIrisUIUE_MsgID_EYE_MOVE_CLOSE = IKIris_Err_BASE - 2502,/*!< "请靠近" 距离超过限定最远距离或虹膜尺寸太大*/
    IKIrisUIUE_MsgID_EYE_MOVE_FAR = IKIris_Err_BASE - 2503,/*!< "请远离" 距离超过限定最近距离或虹膜尺寸太小*/
    IKIrisUIUE_MsgID_EYE_MOVE_UP = IKIris_Err_BASE - 2504,/*!< "请向上移动" 坐标超过限定的上边界*/
    IKIrisUIUE_MsgID_EYE_MOVE_DOWN = IKIris_Err_BASE - 2505,/*!< "请向下移动" 坐标超过限定的下边界*/
    IKIrisUIUE_MsgID_EYE_MOVE_LEFT = IKIris_Err_BASE - 2506, /*!< "请向左移动" 坐标超过限定的左边界*/
    IKIrisUIUE_MsgID_EYE_MOVE_RIGHT = IKIris_Err_BASE - 2507,/*!< "请向右移动" 坐标超过限定的右边界*/
    IKIrisUIUE_MsgID_MOTION_BLUR = IKIris_Err_BASE - 2508, /*!< "正在扫描,请保持稳定" 图像严重运动模糊 */
    IKIrisUIUE_MsgID_FOCUS_BLUR = IKIris_Err_BASE - 2509, /*!< "正在扫描,请保持稳定" 图像严重散焦*/
    IKIrisUIUE_MsgID_BAD_EYE_OPENNESS = IKIris_Err_BASE - 2510,/*!< "请睁大眼睛或调整下角度" 眼睛没有睁开遮挡比例过高*/
    IKIrisUIUE_MsgID_EXPOSURE_UNDER = IKIris_Err_BASE - 2512,/*!< "" 饱和度过低*/
    IKIrisUIUE_MsgID_EXPOSURE_OVER = IKIris_Err_BASE - 2513,/*!< "" 饱和度过高*/
    IKIrisUIUE_MsgID_WITH_GLASS = IKIris_Err_BASE - 2514, /*!< "尝试调整下角度或摘掉眼镜" 有光斑*/
    IKIrisUIUE_MsgID_IMAGE_Invalid =  IKIris_Err_BASE - 2515, /*!< "图像信息或格式无效" */
    IKIrisUIUE_MsgID_ATTACK = IKIris_Err_BASE - 2516, /*!< 请先注册*/
    IKIrisUIUE_MsgID_TRACEID_ERROR = IKIris_Err_BASE - 2517,
    IKIrisUIUE_MsgID_IDLE_CLEAR = IKIris_Err_BASE - 2518,/*!< 空闲*/
	IKIrisUIUE_MsgID_FAKE = IKIris_Err_BASE - 2519, /*!< 请勿用假体攻击*/
	IKIrisUIUE_MsgID_BP = IKIris_Err_BASE - 2520, /*!< 请摘掉美瞳*/
    IKIrisUIUE_MsgID_OFFANGLE = IKIris_Err_BASE - 2521, /*!< 请正视摄像头*/
    IKIrisUIUE_MsgID_INVALID = CBDEFAULTVALUE, /*!< 状态回调中UIUE无效,即此帧UIUE可忽略*/
};
/*!
* @brief 加密芯片校验状态
*/
enum IKIrisVerifyStatue
{
    IKIrisVerifyStatueNone,/*!< 无效，即尚未进行校验*/
    IKIrisVerifyStatueWork,/*!< 工作，即校验正在进行中*/
    IKIrisVerifyStatueSuccess,/*!< 成功,即校验成功*/
    IKIrisVerifyStatueINVALIDLISENCE,/*!< 无效的证书,即算法库过期*/
    IKIrisVerifyStatueFailed,/*!< 失败,即校验失败*/
};

/*!
* @brief iris模式,表示虹膜注册/识别时需要哪只眼睛进行注册/识别,或表示眼图类型
*/
enum IKIrisMode
{
    IKIrisModeNone = -1,/*!< 无效*/
    IKIrisModeUndef = 0,/*!< 任意眼*/
    IKIrisModeLeft = 1,/*!< 左眼*/
    IKIrisModeRight = 2,/*!< 右眼*/
    IKIrisModeBoth = 3,/*!< 双眼*/
};

/*!
* @brief 比对结果信息
*/
typedef struct tagIKIrisMatchOutputStruct
{
    int matchScore;/*!< 分数*/
    int matchIndex;/*!< 在输入特征列表中的index值*/

    tagIKIrisMatchOutputStruct()
    {
        matchScore = -1;
        matchIndex = -1;
    }

}IKIrisMatchOutput;

/*!
* @brief 注册、识别、获取特征时的成功,失败提示
*/
enum IKIrisResultFlag
{
    IKIrisEnrRecBothFailed = -3,/*!< 注册、识别、获取特征时左右眼均注册或识别失败*/
    IKIrisEnrRecRightFailed = -2,/*!< 右眼注册、识别、获取特征失败导致注册、识别、获取特征失败,左眼情况未知*/
    IKIrisEnrRecLeftFailed = -1,/*!< 左眼注册、识别、获取特征失败导致注册、识别、获取特征失败,右眼情况未知*/

    IKIrisEnrRecUnknown = 0,	/*!< 注册、识别、获取特征结果未知*/

    IKIrisEnrRecLeftSuccess = 1,/*!< 注册、识别、获取特征时仅左眼注册、识别、获取特征成功*/
    IKIrisEnrRecRightSuccess = 2,/*!< 注册、识别、获取特征时仅右眼注册、识别、获取特征成功*/
    IKIrisEnrRecBothSuccess = 3, /*!< 注册、识别、获取特征时左右眼均注册、识别、获取特征成功*/
};

/*!
* @brief 虹膜位置信息
*/
typedef struct IKIrisPosStruct
{
    struct IrisPos{
        int xPixel;/*!< x坐标*/
        int yPixel;/*!< y坐标*/
        int radius;/*!< 半径*/

        IrisPos()
        {
            xPixel = 0;
            yPixel = 0;
            radius = 0;
        }

    }leftIrisPos, rightIrisPos; /*!< 左右眼虹膜中心坐标信息*/
    int dist; /*!< 虹膜与摄像头之间的估算距离*/

    IKIrisPosStruct()
    {
        dist = 0;
    }

}IKIrisPos;

/*!
* @brief 虹膜图像结构信息
*/
typedef struct IKIrisInfoStruct
{
    int ImgHeight;/*!< 虹膜图像的高度,当前必须固定为480*/
    int ImgWidth;/*!< 虹膜图像的宽度,当前必须固定为640*/
    int ImgType;/*!< 虹膜图像类型，左眼、右眼OR未知*/
    int IrisRow;/*!< 虹膜图像中虹膜中心的行索引，范围从1到IK_IRIS_IMG_HEIGHT*/
    int IrisCol;/*!< 虹膜图像中虹膜中心的列索引，范围从1到IK_IRIS_IMG_WIDTH*/
    int IrisRadius;/*!< 虹膜图像中的虹膜半径*/
    int FocusScore;/*!< 虹膜图像聚焦级别的指示器，范围从0到100*/
    int PercentVisible;/*!< 虹膜可见部分的百分比，范围从0到100*/
    int Brightness;/*!< 亮度*/
    int MotionBlur;/*!< 运动模糊*/
    int QualityScore;
    unsigned char* imgData;/*!< 原始虹膜图像的像素强度值的一维数组*/
    int imgDataLen; /*!< IK_IRIS_IMG_SIZE*/
    unsigned char* imgDataMat;/*!< 虹膜图像算法信息*/
    int imgDataMatLen; /*!< IK_CompleteInfoByte_IrisImFE*/
    unsigned char* irisEnrTemplate;/*!< 虹膜图像特征模板已注册到数据库中，固定长度为512字节*/
    int irisEnrTemplateLen; /*!< IK_IRIS_ENROLL_FEATURE_SIZE*/
    unsigned char* irisRecTemplate;/*!< 识别图像的虹膜图像特征模板，固定长度1024字节*/
    int irisRecTemplateLen; /*!< IK_IRIS_IDEN_FEATURE_SIZE*/
    int* irisInfoUnkonwn;/*!< 预留信息: [10]标记是否进行IDQT处理，"1000"标识进行IDQT处理，其他均未处理*/
    int irisInfoUnkonwnLen; /*!< IK_UnknownInfoLen*/

    IKIrisInfoStruct()
    {
        ImgHeight = 0;/*!< 虹膜图像的高度,当前必须固定为480*/
        ImgWidth = 0;/*!< 虹膜图像的宽度,当前必须固定为640*/
        ImgType = 0;/*!< 虹膜图像类型，左眼、右眼OR未知*/
        IrisRow = 0;/*!< 虹膜图像中虹膜中心的行索引，范围从1到IK_IRIS_IMG_HEIGHT*/
        IrisCol = 0;/*!< 虹膜图像中虹膜中心的列索引，范围从1到IK_IRIS_IMG_WIDTH*/
        IrisRadius = 0;/*!< 虹膜图像中的虹膜半径*/
        FocusScore = 0;/*!< 虹膜图像聚焦级别的指示器，范围从0到100*/
        PercentVisible = 0;/*!< 虹膜可见部分的百分比，范围从0到100*/
        Brightness = 0;/*!< 亮度*/
        MotionBlur = 0;/*!< 运动模糊*/
        QualityScore = 0;
        imgData = nullptr;/*!< 原始虹膜图像的像素强度值的一维数组*/
        imgDataLen = 0; /*!< IK_IRIS_IMG_SIZE*/
        imgDataMat = nullptr;/*!< 虹膜图像算法信息*/
        imgDataMatLen = 0; /*!< IK_CompleteInfoByte_IrisImFE*/
        irisEnrTemplate = nullptr;/*!< 虹膜图像特征模板已注册到数据库中，固定长度为512字节*/
        irisEnrTemplateLen = 0; /*!< IK_IRIS_ENROLL_FEATURE_SIZE*/
        irisRecTemplate = nullptr;/*!< 识别图像的虹膜图像特征模板，固定长度1024字节*/
        irisRecTemplateLen = 0; /*!< IK_IRIS_IDEN_FEATURE_SIZE*/
        irisInfoUnkonwn = nullptr;/*!< 预留信息: [10]标记是否进行IDQT处理，"1000"标识进行IDQT处理，其他均未处理*/
        irisInfoUnkonwnLen = 0; /*!< IK_UnknownInfoLen*/
    }

    void creat()
    {
        imgDataLen = IK_IRIS_IMG_SIZE;
        imgDataMatLen = IK_CompleteInfoByte_IrisImFE;
        irisEnrTemplateLen = IK_IRIS_ENROLL_FEATURE_SIZE;
        irisRecTemplateLen = IK_IRIS_IDEN_FEATURE_SIZE;
        irisInfoUnkonwnLen = IK_UnknownInfoLen;
        if(imgDataLen!=0) imgData = new unsigned char[imgDataLen];
        if(imgDataMatLen!=0) imgDataMat = new unsigned char[imgDataMatLen];
        if(irisEnrTemplateLen!=0) irisEnrTemplate = new unsigned char[irisEnrTemplateLen];
        if(irisRecTemplateLen!=0) irisRecTemplate = new unsigned char[irisRecTemplateLen];
        if(irisInfoUnkonwnLen!=0) irisInfoUnkonwn = new int[irisInfoUnkonwnLen];
        return;
    }
    void destroy()
    {
        DELETEARRAY(imgData)
        DELETEARRAY(imgDataMat)
        DELETEARRAY(irisEnrTemplate)
        DELETEARRAY(irisRecTemplate)
        DELETEARRAY(irisInfoUnkonwn)
        return;
    }
    void Init()
    {
        ImgHeight = IK_IRIS_IMG_HEIGHT;
        ImgWidth = IK_IRIS_IMG_WIDTH;
        ImgType = IKIrisModeNone;
        IrisRow = 0;
        IrisCol = 0;
        IrisRadius = 0;
        FocusScore = 0;
        PercentVisible = 0;
        Brightness = 0;
        MotionBlur = 0;
        QualityScore = 0;
        if(imgData!= nullptr) memset(imgData, 0, imgDataLen);
        if(imgDataMat!= nullptr) memset(imgDataMat, 0, imgDataMatLen);
        if(irisEnrTemplate!= nullptr) memset(irisEnrTemplate, 0, irisEnrTemplateLen);
        if(irisRecTemplate!= nullptr) memset(irisRecTemplate, 0, irisRecTemplateLen);
        if(irisInfoUnkonwn!= nullptr) memset(irisInfoUnkonwn, 0, sizeof(IK_UnknownInfoLen));
    }

}IKIrisInfo,*pIKIrisInfo;

//工作模式，表示空闲/注册/识别
enum IKIrisWorkMode
{
    IKIrisWorkModeIdle =	-1,		/*!< 空闲模式*/
    IKIrisWorkModeEnroll = 0,		/*!< 注册模式*/
    IKIrisWorkModeIdent =	1,		/*!< 识别模式*/
    IKIrisWorkModeCapture = 2, /*!< 快速采集模式*/
};
/*!
* @brief 虹膜算法提示的状态信息
*/
struct IKIrisProcessorState{
    IKIrisWorkMode workMode;/*!< 工作模式,空闲还是注册or识别*/
    int detectType;/*!< 眼睛检测函数检测到的眼睛类型*/
    IKIrisMode irisMode;		/*!< 虹膜的图像类型*/
    float leftIrisProgress;	/*!< 左眼进度百分比*/
    float rightIrisProgress;		/*!< 右眼进度百分比*/    
    int imageW; /*!< 虹膜位置信息对应的图像的宽*/
    int imageH;  /*!< 虹膜位置信息对应的图像的高*/
    IKIrisPos irisPos;	/*!< 虹膜位置信息*/
    int throwAwayFrameStatus; /*!< 每帧处理失败的原因 <=0的数是有效数据*/
    int irisID;/*!< irisID*/
    int faceID;/*!< faceID*/
    int traceID;/*!< traceID*/
    int irisLeftMargin; /*!< 虹膜注册识别区域限制,图像上下左右预留宽度*/
    int irisRightMargin;
    int irisTopMargin;
    int irisBottomMargin;
    int reservedInfo[IKIris_RESERVEDINFO_LEN]; /*!< 预留信息*/

    IKIrisProcessorState()
    {
        workMode = IKIrisWorkModeIdle;/*!< 工作模式,空闲还是注册or识别*/
        detectType = IKIrisModeNone;/*!< 眼睛检测函数检测到的眼睛类型*/
        irisMode = IKIrisModeNone;		/*!< 虹膜的图像类型*/
        leftIrisProgress = 0;	/*!< 左眼进度百分比*/
        rightIrisProgress = 0;		/*!< 右眼进度百分比*/    
        imageW = 0; /*!< 虹膜位置信息对应的图像的宽*/
        imageH = 0;  /*!< 虹膜位置信息对应的图像的高*/
        throwAwayFrameStatus = CBDEFAULTVALUE; /*!< 每帧处理失败的原因 <=0的数是有效数据*/
        irisID = 0;/*!< irisID*/
        faceID = 0;/*!< faceID*/
        traceID = 0;/*!< traceID*/
        irisLeftMargin = 0; /*!< 虹膜注册识别区域限制,图像上下左右预留宽度*/
        irisRightMargin = 0;
        irisTopMargin = 0;
        irisBottomMargin = 0;
        memset(reservedInfo, 0, IKIris_RESERVEDINFO_LEN*sizeof(int)); /*!< 预留信息*/
    }
};
/*!
* @brief 虹膜调试图像信息
*/
struct IKIrisDebugInfo{
    int throwAwayFrameStatus;
    int irisInfoNum;
    IKIrisInfo* irisInfo;

    IKIrisDebugInfo()
    {
        throwAwayFrameStatus = CBDEFAULTVALUE;
        irisInfoNum = 0;
        irisInfo = nullptr;
    }

};

/*!
* @brief 虹膜注册结果帧数据
*/
typedef struct tagIKIrisProcessedResult
{        
    IKIrisMode irisMode;/*!< 注册模式*/
    IKIrisResultFlag flag;/*!< 虹膜注册结果标志，解释虹膜注册成功或失败的具体原因*/
    IKIrisInfo irisInfo;/*!< 虹膜图像对应的具体信息*/

    tagIKIrisProcessedResult()
    {
        irisMode = IKIrisModeNone;
        flag = IKIrisEnrRecUnknown;
    }

}IKIrisProcessedResult,*pIKIrisProcessedResult;

/*!
* @brief 虹膜识别结果数据
*/
typedef struct tagIKIrisIdentProcessedResultStruct
{
    int _nFunResult;/*!< 识别结果返回值,成功返回IKIris_Fun_Success,失败返回错误码*/
    IKIrisMode _irisMode;/*!< 识别模式*/
    IKIrisResultFlag _flag;/*!< 虹膜识别结果标志，解释虹膜识别成功或失败的具体原因*/
    IKIrisMatchOutput _matchOutputL;
    IKIrisMatchOutput _matchOutputR;/*!< 识别结果左右眼具体信息*/
    pIKIrisInfo _irisInfoL;
    pIKIrisInfo _irisInfoR;

    tagIKIrisIdentProcessedResultStruct()
    {
        _nFunResult = -1;/*!< 识别结果返回值,成功返回IKIris_Fun_Success,失败返回错误码*/
        _irisMode = IKIrisModeNone;/*!< 识别模式*/
        _flag = IKIrisEnrRecUnknown;/*!< 虹膜识别结果标志，解释虹膜识别成功或失败的具体原因*/
        _irisInfoL = nullptr;
        _irisInfoR = nullptr;
    }

}IKIrisIdentProcessedResultStruct;
