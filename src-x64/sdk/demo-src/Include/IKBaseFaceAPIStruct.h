/*!
* @file IKBaseFaceAPIStruct.h
* @brief FaceAPI相关的一些数据结构定义以及错误码
*
* @author tangzhuan
* @version
* @date 2020.06.01
*/
#pragma once
#include <vector>
#include <memory>
#include <list>

#include <string.h>
#include "IKFrame.h"
using namespace std;

/*!
 * @brief FaceAPI相关错误码只能定义在-3000至-3999段内
*/
const int IKFace_Fun_Success = 0;/*!< 函数返回成功 */
const int IKFace_Err_BASE = 0;/*!<基础错误码*/
const int IKFace_Err_ENR_IDEN_INTERRUPT = 1;/*<当人为调用停止注册，之前调用的注册将返回1*/
const int IKFace_Err_Invalid_ENRNUM	= IKFace_Err_BASE - 3001;	/*!< 阈值错误 */
const int IKFace_Err_Invalid_OVERTIME	= IKFace_Err_BASE - 3002;	/*!< 阈值错误 */
const int IKFace_Err_Invalid_DELETEAREA	= IKFace_Err_BASE - 3003;	/*!< 阈值错误 */
const int IKFace_Err_Invalid_ENRIDENTDISTANCE	= IKFace_Err_BASE - 3004;	/*!< 阈值错误 */
const int IKFace_Err_Invalid_PosMoveCheck	= IKFace_Err_BASE - 3005;	/*!< 阈值错误 */
const int IKFace_Err_Default = IKFace_Err_BASE - 3100;	/*!<人脸默认错误码*/
const int IKFace_Err_BadInput = IKFace_Err_BASE - 3101;/*!<输入参数错误，例如空指针*/
const int IKFace_Err_Match_Failed = IKFace_Err_BASE - 3103;/*!<剔除掉比对不成功的图像*/
const int IKFace_Err_EnrollPersonChange_CLEAR = IKFace_Err_BASE - 3104;/*!<在注册过程中检测到注册对象改变丢图*/
const int IKFace_Err_QUEUE_FULL_CLEAR = IKFace_Err_BASE - 3105; /*!<非空闲但队列满了导致丢图*/
const int IKFace_Err_ENROLLEND_CLEAR = IKFace_Err_BASE - 3106; /*!<注册识别开始或结束缓存清理*/
const int IKFace_Err_CLEARCACHE_FAILED = IKFace_Err_BASE - 3107;/*!<清理缓存失败，只有在idle模式才能清理*/
const int IKFace_Err_ASYNCTASK_BUSY = IKFace_Err_BASE - 3108;	/*!<异步任务正在执行，还没有结果*/
const int IKFace_Err_STOP = IKFace_Err_BASE - 3109;	/*!<停止工作失败，比如不是注册模式却调用停止注册或不是识别模式却调用停止识别*/
const int IKFace_Err_TimeOut = IKFace_Err_BASE - 3110;	/*!< 注册识别超时 */
const int IKFace_Err_EnrIden_Failed = IKFace_Err_BASE - 3111;	/*!< 注册识别失败 */
const int IKFace_Err_Init_Fail = IKFace_Err_BASE - 3112;	/*!<人脸算法初始化失败或尚未初始化*/
const int IKFace_Err_Detect_Init_Fail = IKFace_Err_BASE - 3113;	/*!<人脸检测算法初始化失败或尚未初始化*/
const int IKFace_Err_Detect_Invalid_License = IKFace_Err_BASE - 3114;/*!<人脸检测算法授权过期*/
const int IKFace_Err_Invalid_TpLen = IKFace_Err_BASE - 3115;/*!<人脸特征长度不正确*/
const int IKFace_Err_Feature_Init_Fail = IKFace_Err_BASE - 3116;/*!<人脸特征提取算法初始化失败*/
const int IKFace_Err_Feature_Repeat_Fail = IKFace_Err_BASE - 3117;	/*!<人脸特征重复*/
const int IKFace_Err_Feature_Invalid_License = IKFace_Err_BASE - 3118;/*!<人脸特征算法授权过期*/
const int IKFace_Err_NoMatch = IKFace_Err_BASE - 3121; /*!<比对失败*/
const int IKFace_Err_RESETBUSY = IKFace_Err_BASE - 3122; /*!<reset操作正在进行中*/
const int IKFace_Err_RESET_CLEAR = IKFace_Err_BASE - 3123; /*!<reset操作引发的清理已注册图像or队列缓存*/
const int IKFace_Err_RESET_FAILED = IKFace_Err_BASE - 3124;/*!<reset操作失败*/
const int IKFace_Err_SETCONFIGFILEPATH_FAILED = IKFace_Err_BASE - 3125;/*!< 路经设置必须在初始化之前*/
const int IKFace_Err_Data_Interrupted = IKFace_Err_BASE - 3126;/*!<人脸图像数据被非法篡改*/
const int IKFace_Err_CallSelect_Failed = IKFace_Err_BASE - 3127;/*!<人脸质量判断接口调用失败*/
const int IKFace_Err_LiveInit_Failed =  IKFace_Err_BASE - 3128;/*!<人脸活体检测初始化失败*/

const int IKFaceMaxEnrNum	 = 9;  /*!< 注册时允许的最大注册数目,当前仅支持1,2,3,6,9 */
const int IKFace_Ori_ImH	 = 2000;/*!< 人脸图像MAX宽*/
const int IKFace_Ori_ImW	 = 2000;/*!< 人脸图像MAX高*/
const int IKFace_Ori_ImD	 = 3;/*!< 人脸图像通道*/
const int IKFace_MAX_IM_SIZE = (IKFace_Ori_ImH*IKFace_Ori_ImW*IKFace_Ori_ImD);/*!< 最大的允许人脸图像尺寸*/
const int IKDepth_Ori_ImH	 = 1000;/*!< 深度图像MAX宽*/
const int IKDepth_Ori_ImW	 = 1000;/*!< 深度图像MAX高*/
const int IKDepth_Ori_ImD	 = 2;/*!< 深度图像通道*/
const int IKDepth_MAX_IM_SIZE = (IKDepth_Ori_ImH*IKDepth_Ori_ImW*IKDepth_Ori_ImD);/*!< 最大的允许深度图像尺寸*/
const int IKFACE_FEATURE_LENGTH	 = 512;/*!人脸注册特征的长度*/

/*!
* @brief 根据状态回调进行UIUE提示信息，注释中""为建议提示用语
* @note 状态回调throwAwayFrameStatus描述了每帧处理失败的原因,如果它的值在以下枚举类型中则需要在界面做必要的提示以引导使用者
*/
enum IKFaceUIUEMsgID {
    IKFaceUIUE_MsgID_SUITABLE = 0,/*!< "当前状态合适" */
    IKFaceUIUE_MsgID_Detect_MultipleFace = IKFace_Err_BASE - 3501,	/*!< "请保持画面内只有一张人脸" 检测到多张人脸*/
    IKFaceUIUE_MsgID_Detect_NoFace = IKFace_Err_BASE - 3502, /*!< 没有检测到人脸*/
    IKFaceUIUE_MsgID_IDLE_CLEAR = IKFace_Err_BASE - 3503,/*!< 空闲状态丢图*/
    IKFaceUIUE_MsgID_IMAGE_Invalid = IKFace_Err_BASE - 3504,/*!< "人脸回调图像和配置文件不一致" 输入的图像无效，如：类型，格式，大小*/
    IKFaceUIUE_MsgID_IMAGE_BLUE = IKFace_Err_BASE - 3505, /*!<  "正在扫描,请保持稳定" 运动模糊*/
    IKFaceUIUE_MsgID_IMAGE_OVERMOVE = IKFace_Err_BASE - 3506,/*!< "正在扫描,请保持稳定" 坐标超过运动阈值*/
    IKFaceUIUE_MsgID_ATTACK = IKFace_Err_BASE - 3507,/*!< "请先注册" */
    IKFaceUIUE_MsgID_Invalid_Im_Size = IKFace_Err_BASE - 3508, /*!< "原始图像尺寸过大" */
    IKFaceUIUE_MsgID_Bad_Pos_Move_Left = IKFace_Err_BASE - 3509,/*!< "请向左移动" 坐标超过限定的右边界*/
    IKFaceUIUE_MsgID_Bad_Pos_Move_Right = IKFace_Err_BASE - 3510,/*!< "请向右移动" 坐标超过限定的左边界*/
    IKFaceUIUE_MsgID_Bad_Pos_Move_Up = IKFace_Err_BASE - 3511,/*!< "请向上移动" 坐标超过限定的下边界*/
    IKFaceUIUE_MsgID_Bad_Pos_Move_Down = IKFace_Err_BASE - 3512,/*!< "请向下移动" 坐标超过限定的上边界*/
    IKFaceUIUE_MsgID_Pos_Move_CLOSE = IKFace_Err_BASE - 3513,/*!< "请靠近" 距离超过限定最远距离*/
    IKFaceUIUE_MsgID_Bad_Pos_Move_FAR = IKFace_Err_BASE - 3514,/*!< "请远离" 距离超过限定最近距离*/
    IKFaceUIUE_MsgID_Quality_Face_Invalid = IKFace_Err_BASE - 3515,/*!< "请靠近" 人脸质量校验人脸位置无效,人脸过小*/    
    IKFaceUIUE_MsgID_Quality_Angle_Fail = IKFace_Err_BASE - 3517,/*!< "尝试调整下角度" 人脸质量角度校验失败*/
    IKFaceUIUE_MsgID_AlignedDepthFace_Fail = IKFace_Err_BASE - 3518,/*!< 人脸深度对齐失败*/
    IKFaceUIUE_MsgID_DepthDetect_Fail = IKFace_Err_BASE - 3519,/*!< 深度图检测失败*/
    IKFaceUIUE_MsgID_DepthSelect_Fail = IKFace_Err_BASE - 3520,/*!< 深度图空洞率过高*/
    IKFaceUIUE_MsgID_LiveDetect_Fail = IKFace_Err_BASE - 3521,/*!< 活体检测失败*/
    IKFaceUIUE_MsgID_LiveScore_Fail = IKFace_Err_BASE - 3522,/*!< 活体检测分过低*/
    IKFaceUIUE_MsgID_INVALID = CBDEFAULTVALUE, /*!< 状态回调中UIUE无效,即此帧UIUE可忽略*/
};

/*!
 * @brief 人脸各角度
*/
struct IKFaceQulityInfo
{
    float   yaw;/*!< 人脸左右摇头姿态, 范围为(-50,50)*/
    float   pitch;/*!< 人脸上下抬低头姿态, 范围为(-50,50)*/
    float   roll;/*!< 人脸左右歪头姿态，范围为(-90, 90)*/
    float   confidence;/*!< 人脸得分，范围为(0,1)*/

    IKFaceQulityInfo()
    {
        clear();
    }

    void clear()
    {
        yaw = 0.0f;/*!< 人脸左右摇头姿态, 范围为(-50,50)*/
        pitch = 0.0f;/*!< 人脸上下抬低头姿态, 范围为(-50,50)*/
        roll = 0.0f;/*!< 人脸左右歪头姿态，范围为(-90, 90)*/
        confidence = 0.0f;/*!< 人脸得分，范围为(0,1)*/
    }

    IKFaceQulityInfo& operator=(const IKFaceQulityInfo& _fqInfo)
    {
        this->yaw = _fqInfo.yaw;
        this->pitch = _fqInfo.pitch;
        this->roll = _fqInfo.roll;
        this->confidence = _fqInfo.confidence;

        return *this;
    }

};

/*!
 * @brief 通用矩形坐标结构
*/
struct SELFRECT
{
    long    left;/*!< 左边界坐标*/
    long    top;/*!< 上边界坐标*/
    long    right;/*!< 右边界坐标*/
    long    bottom;/*!< 下边界坐标*/

    SELFRECT()
    {
        clear();
    }

    void clear()
    {
        left = 0;/*!< 左边界坐标*/
        top = 0;/*!< 上边界坐标*/
        right = 0;/*!< 右边界坐标*/
        bottom = 0;/*!< 下边界坐标*/
    }
};

/*!
 * @brief 通用点坐标结构
*/
struct SELFPOINT
{
    long  x;/*!< x坐标*/
    long  y;/*!< y坐标*/

    SELFPOINT()
    {
        clear();
    }

    void clear()
    {
        x = 0;
        y = 0;
    }
};

/*!
 * @brief 人脸ROI区域信息
*/
struct IKFaceROIInfo
{
    bool		    ifValid;/*!< 标记以下数据是否有效*/
    SELFRECT		rcFace;/*!< 脸部坐标*/
    SELFPOINT		ptLeftEye;/*!< 左眼坐标*/
    SELFPOINT		ptRightEye;/*!< 右眼坐标*/
    SELFPOINT		ptMouthLeft;/*!< 嘴巴坐标*/
    SELFPOINT		ptMouthRight;/*!< 嘴巴坐标*/
    SELFPOINT		ptNose;/*!< 鼻子坐标*/
    IKFaceQulityInfo	fAngle;/*!< 脸部角度*/

    IKFaceROIInfo()
    {
        ifValid = false;
    }

    void clear()
    {
        ifValid = false;
        rcFace.clear();
        ptLeftEye.clear();
        ptRightEye.clear();
        ptMouthLeft.clear();
        ptMouthRight.clear();
        ptNose.clear();
        fAngle.clear();
    }
};
/*!
 * @brief 生物模态信息结构体，包含有图像、特征、分数等必要信息
*/
typedef struct tagIKFaceProcessedResult
{
    
    SELFRECT		m_rcFace;/*!< 可检测区域脸部坐标*/
    IKFaceQulityInfo	 m_faceQuality;/*!< 人脸各角度*/
    unsigned char* m_detectImageData; /*!< 人脸的可检测区域图像数据*/
    int m_detectImageSize;/*!< 人脸可检测区域图像大小*/
    int m_detectImageH;/*!< 人脸可检测区域图像高*/
    int m_detectImageW;/*!< 人脸可检测区域图像宽*/

    unsigned char* m_imageData; /*!< 人脸的原始图像数据*/
    int m_imageSize;/*!< 人脸原始图像大小*/
    int m_imageH;/*!< 人脸原始图像高*/
    int m_imageW;/*!< 人脸原始图像宽*/

    unsigned char* m_enrTemplate;/*!< 人脸的注册特征*/
    int m_enrTemplateSize;	/*!< 人脸的注册特征长度*/
    unsigned char* m_recTemplate; /*!< 人脸的识别特征*/
    int m_recTemplateSize;/*!< 人脸的识别特征长度*/

    tagIKFaceProcessedResult()
    {
        m_detectImageData = nullptr; /*!< 人脸的可检测区域图像数据*/
        m_detectImageSize = 0;/*!< 人脸可检测区域图像大小*/
        m_detectImageH = 0;/*!< 人脸可检测区域图像高*/
        m_detectImageW = 0;/*!< 人脸可检测区域图像宽*/

        m_imageData = nullptr; /*!< 人脸的原始图像数据*/
        m_imageSize = 0;/*!< 人脸原始图像大小*/
        m_imageH = 0;/*!< 人脸原始图像高*/
        m_imageW = 0;/*!< 人脸原始图像宽*/

        m_enrTemplate = nullptr;/*!< 人脸的注册特征*/
        m_enrTemplateSize = 0;	/*!< 人脸的注册特征长度*/
        m_recTemplate = nullptr; /*!< 人脸的识别特征*/
        m_recTemplateSize = 0;/*!< 人脸的识别特征长度*/
    }

void creat(int enrTemplateSize, int recTemplateSize, 
        int bioImageH, int bioImageW, int bioImageSize, 
        int detectImageH, int detectImageW, int detectImageSize)
    {
        m_enrTemplateSize = enrTemplateSize;
        if(m_enrTemplateSize != 0) m_enrTemplate = new unsigned char[m_enrTemplateSize];

        m_recTemplateSize = recTemplateSize;
        if(m_recTemplateSize != 0) m_recTemplate = new unsigned char[m_recTemplateSize];

        m_imageSize = bioImageSize;
        if(m_imageSize != 0) m_imageData = new unsigned char[m_imageSize];
        m_imageH = bioImageH;
        m_imageW = bioImageW;

        m_detectImageSize = detectImageSize;
        if(m_detectImageSize != 0) m_detectImageData = new unsigned char[m_detectImageSize];
        m_detectImageH = detectImageH;
        m_detectImageW = detectImageW;
        return;
    }
    void init()
    {
        if(m_enrTemplate != nullptr) memset(m_enrTemplate, 0, m_enrTemplateSize);
        if(m_recTemplate != nullptr) memset(m_recTemplate, 0, m_recTemplateSize);
        if(m_imageData != nullptr) memset(m_imageData, 0, m_imageSize);
        if(m_detectImageData != nullptr) memset(m_detectImageData, 0, m_detectImageSize);
        m_rcFace.clear();
        m_faceQuality.clear();
        return;
    }
    void destroy()
    {
        DELETEARRAY(m_imageData)
        DELETEARRAY(m_detectImageData)
        DELETEARRAY(m_enrTemplate)
        DELETEARRAY(m_recTemplate)
        return;
    }
}IKFaceProcessedResult,*pIKFaceProcessedResult;
/*!
 * @brief 比对结果结构体
*/
typedef struct IKFaceMatchOutputStruct
{
    int matchScore;/*!< 比对成功的特征分数*/
    int matchIndex;/*!< 比对成功的特征在特征列表中的index*/

    IKFaceMatchOutputStruct()
    {
        matchScore = -1;
        matchIndex = -1;
    }

} IKFaceMatchOutput, *pIKFaceMatchOutput;

/*!
 * @brief 识别结果结构体
*/
typedef struct tagIKFaceIdentOutputStruct
{
    int identResult;/*!< 识别结果返回值，成功返回IKFace_Fun_Success,失败返回错误码*/
    IKFaceMatchOutput matchResult;/*!< 识别结果特征信息*/
    IKFaceProcessedResult* resultInfo;

    tagIKFaceIdentOutputStruct()
    {
        identResult = -1;
        resultInfo = nullptr;
    }

}IKFaceIdentOutputStruct;

/*!
 * @brief 人脸工作模式枚举
*/
enum IKFaceWorkMode
{
    IKFace_WorkMode_Enroll = 0,/*!< 注册模式*/
    IKFace_WorkMode_Iden = 1,/*!< 识别模式*/
    IKFace_WorkMode_Idle = -1,/*!< 空闲模式*/
};
/*!
 * @brief 人脸状态回调信息结构体
*/
struct IKFaceProcessorState
{
    IKFaceWorkMode workMode;/*!< 工作模式*/
    IKFaceROIInfo faceROIInfo; /*!< 检测的人脸ROI区域信息*/
    int dist; /*!< 人脸与摄像头之间的估算距离*/
    float lightScore; /*!< 图像亮度*/
    int imageW; /*!< 原始图像宽*/
    int imageH; /*!< 原始图像高*/
    int detectBeginX; /*!< 检测起始x坐标*/
    int detectBeginY; /*!< 检测起始y坐标*/
    int detectW; /*!< 检测图像宽*/
    int detectH; /*!< 检测图像高*/
    float faceProgress; /*!< 注册识别进度百分比*/
    int throwAwayFrameStatus;/*!< 每帧处理失败的原因 <=0的数是有效数据*/
    int imageIndex;/*!< */

    IKFaceProcessorState()
    {
        workMode = IKFace_WorkMode_Idle;/*!< 工作模式*/
        faceROIInfo.clear(); /*!< 检测的人脸ROI区域信息*/
        dist = 0; /*!< 人脸与摄像头之间的估算距离*/
        lightScore = 0; /*!< 图像亮度*/
        imageW =  0; /*!< 原始图像宽*/
        imageH =  0; /*!< 原始图像高*/
        detectBeginX = 0; /*!< 检测起始x坐标*/
        detectBeginY = 0; /*!< 检测起始y坐标*/
        detectW = 0; /*!< 检测图像宽*/
        detectH = 0; /*!< 检测图像高*/
        faceProgress = 0; /*!< 注册识别进度百分比*/
        throwAwayFrameStatus = CBDEFAULTVALUE;/*!< 每帧处理失败的原因 <=0的数是有效数据*/
        imageIndex = 0;/*!< */
    }
    void Init()
    {
        dist = 0;
        lightScore = 0;
        throwAwayFrameStatus = CBDEFAULTVALUE;
        imageIndex = 0;
    }


};
/*!
* @brief 人脸调试图像信息
*/
struct IKFaceDebugInfo{
    int throwAwayFrameStatus;
    int faceInfoNum;
    IKFaceProcessedResult* faceInfo;

    IKFaceDebugInfo()
    {
        throwAwayFrameStatus = CBDEFAULTVALUE;
        faceInfoNum = 0;
        faceInfo = nullptr;
    }
};
