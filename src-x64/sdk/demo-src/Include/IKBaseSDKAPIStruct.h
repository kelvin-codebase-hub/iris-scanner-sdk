/*!
* @file IKBaseSDKAPIStruct.h
* @brief SDKAPI相关的一些数据结构定义以及错误码
*
* @author tangzhuan
* @version
* @date 2020.06.01
*/
#pragma once
#include "IKBaseIrisAPIStruct.h"
#include "IKBaseFaceAPIStruct.h"
#include <map>
#define CONF_FILE_PATH	"IKConfig.ini" /*!< 配置文件*/
const int IK_Fun_Success = 0;/*!< 函数调用成功，并且完成了函数功能*/
const int IK_Err_BASE = 0;     /*!< 基础错误码*/
const int IK_Err_INIT_FAILED = IK_Err_BASE - 6001;/*!< SDK初始化失败或者尚未初始化*/
const int IK_Err_INVALID_PARAMETER = IK_Err_BASE - 6002;/*!< 无效的控制参数*/
const int IK_Err_CREATEIRISCAMERA_FAILED = IK_Err_BASE - 6003;/*!< 创建虹膜摄像头对象失败或没有创建*/
const int IK_Err_CREATEFACECAMERA_FAILED = IK_Err_BASE - 6004;/*!< 创建人脸摄像头对象失败或没有创建*/
const int IK_Err_CREATEDEPTHCAMERA_FAILED = IK_Err_BASE - 6005;/*!< 创建深度摄像头对象失败或没有创建*/
const int IK_Err_CREATEPTZ_FAILED = IK_Err_BASE - 6006;/*!< 创建PTZ对象失败或没有创建*/
const int IK_Err_INVALID_ENROLLIDENTMODE = IK_Err_BASE - 6007;/*!< 无效的或不支持的注册识别模式*/
const int IK_Err_INVALID_MATCHMODE = IK_Err_BASE - 6008;/*!< 无效的或不支持的比对模式*/
const int IK_Err_NOFOUND_CONFIGFILE = IK_Err_BASE - 6009;/*!< 没有找到配置文件*/
const int IK_Err_STARTIDENT_ModeUndef_FAILED = IK_Err_BASE - 6010;/*!< 人脸或虹膜识别,开启人脸识别,虹膜识别均失败，停止识别,检查输入参数*/
const int IK_Err_CONFIG_FAILED = IK_Err_BASE - 6011; /*!< 配置接口失败，请检查工作模式，idle模式下才能调用此接口*/
const int IK_Err_SETCONFIGFILEPATH_FAILED = IK_Err_BASE - 6012; /*!< 设置配置文件路经失败,请检查配置文件是否存在或是否在初始化之前配置*/
const int IK_Err_ASYNCTASK_BUSY = IK_Err_BASE - 6013;	/*!< 异步任务正在执行，还没有结果*/
const int IK_Err_NOTSUPPORT = IK_Err_BASE - 6014;	/*!< 不支持的操作*/
const int IK_Err_REPEATINIT = IK_Err_BASE - 6015;	/*!< 重复初始化*/
const int IK_Err_INVALID_DEVICETYPE = IK_Err_BASE - 6016;/*!< 无效的设备类型*/
const int IK_STARTIDENT_ModeUndef_IRISFAILED = 6100;     /*!< 识别模式为:虹膜或人脸，开启虹膜识别失败，人脸识别正在进行中*/
const int IK_STARTIDENT_ModeUndef_FACEFAILED = 6101;     /*!< 识别模式为:虹膜或人脸，开启人脸识别失败，虹膜识别正在进行中*/
/////////////////////////////////////////////回调注册结果信息类////////////////////////////////////////////////////////

/*!
 * @brief 初始化打开设备的模式
*/
enum IKInitMode
{
    IKInitModeNone = -1,	/*!< 无效*/
    IKInitModeIris = 0,	/*!< 仅虹膜*/
    IKInitModeFace = 1,	/*!< 仅人脸*/
    IKInitModeDef = 2,	/*!< 打开SDK默认的camera，即支持哪些设备就初始化哪些*/
    IKInitModeBoth = 3		/*!< 虹膜+人脸*/
};
/*!
 * @brief 多模态注册识别模式
*/
enum IKEnrollIdentMode
{
    IKModeNone = -1,	/*!< 无效*/
    IKModeIris = 0,	/*!< 仅虹膜*/
    IKModeFace = 1,	/*!< 仅人脸*/
    IKModeUndef = 2,	/*!< 虹膜或人脸*/
    IKModeBoth = 3		/*!< 虹膜+人脸*/
};

/*!
 * @brief 比对模式
*/
enum IKMatchMode
{
    IKMatchModeIris = 0,	/*!< 虹膜*/
    IKMatchModeFace = 1,	/*!< 人脸*/
};
/*!
 * @brief 注册识别前需要的配置信息
*/
typedef struct IKConfigInfoStruct
{
    IKEnrollIdentMode mode; /*!< 多模态注册识别模式*/
    IKIrisMode irisMode; /*!< 虹膜注册识别模式*/
    bool irisEnrollIsEnforce;/*!< 虹膜注册是否强采,虹霸未明确说明需要强采则均设置为false*/
    IKIrisWorkMode irisWorkMode; /*!< 虹膜,传递给算法的工作模式，用户使用一般情况下不需要设置，需要设置的虹霸会特别提醒*/
    int irisCount;/*!<虹膜注册数量*/
    int faceCount;/*!<人脸注册数量*/
    int overTime;/*!<超时时间，单位:秒*/

    IKConfigInfoStruct()
    {
        mode = IKModeNone; /*!< 多模态注册识别模式*/
        irisMode = IKIrisModeNone; /*!< 虹膜注册识别模式*/
        irisEnrollIsEnforce = false;/*!< 虹膜注册是否强采,虹霸未明确说明需要强采则均设置为false*/
        irisWorkMode = IKIrisWorkModeIdle; /*!< 虹膜,传递给算法的工作模式，用户使用一般情况下不需要设置，需要设置的虹霸会特别提醒*/
        irisCount = 0;/*!<虹膜注册数量*/
        faceCount = 0;/*!<人脸注册数量*/
        overTime = 0;/*!<超时时间，单位:秒*/
    }

}IKConfigInfo;
/*!
 * @brief 比对输出信息
*/
typedef struct IKMatchOutputStruct
{
    int matchScore;/*!< 分数*/
    int matchIndex;/*!< 比对成功的特征在输入特征列表中的index*/

    IKMatchOutputStruct()
    {
        matchScore = -1;
        matchIndex = -1;
    }

}IKMatchOutput;

/*!
 * @brief 注册、识别时的成功,失败提示(描述多模态注册识别成功或失败的模态情况)
*/
enum IKEnrollIdentResultFlag
{
    IKEnrRecIrisFaceFailed = -3,    /*!< 虹膜人脸注册or识别均失败,所以结果失败*/
    IKEnrRecFaceFailed = -2,		/*!< 人脸注册or识别失败,所以结果失败*/
    IKEnrRecIrisFailed = -1,		/*!< 虹膜注册or识别失败,所以结果失败*/
    IKEnrRecUnknown = 0,			/*!< 未知*/
    IKEnrRecIrisSuccess = 1,		/*!< 虹膜注册or识别成功,所以结果成功*/
    IKEnrRecFaceSuccess = 2,     /*!< 人脸注册or识别成功,所以结果成功*/
    IKEnrRecIrisFaceSuccess = 3,  /*!< 虹膜人脸注册or识别均成功,所以结果成功*/
};

/*!
 * @brief 多模态注册结果信息类
*/
class IKBaseEnrollProcessedResult{
public:
    IKBaseEnrollProcessedResult(){}

    virtual ~IKBaseEnrollProcessedResult() = 0;

    /*!
        @brief 设置注册识别模式
        @param[in] mode 注册识别模式
        @return 无
        */
    virtual void setEnrollIdentMode(IKEnrollIdentMode mode) = 0;

    /*!
        @brief 获取注册识别模式
        @param 无
        @return 注册识别模式
       */
    virtual IKEnrollIdentMode getEnrollIdentMode() = 0;

    /*!
        @brief 设置注册识别结果标志
        @note 描述多模态注册识别成功或失败的模态情况
        @param[in] flag 标志
        @return 无
       */
    virtual void setEnrollIdentResFlag(IKEnrollIdentResultFlag flag) = 0;

    /*!
      @brief 获取注册识别结果标志
      @param 无
      @return  结果标志
      */
    virtual IKEnrollIdentResultFlag getEnrollIdentResFlag() = 0;

    /*!
        @brief 添加虹膜算法处理结果
        @param algResultData 虹膜算法数据
        @return 无
        */
    virtual void addIrisData(IKIrisProcessedResult& algResultData) = 0;

    /*!
         @brief 添加人脸算法处理结果
         @param[in] algResultData 虹膜算法数据
         @return 无
         */
    virtual void addFaceData(IKFaceProcessedResult& algResultData) = 0;

    /*!
         @brief 获取虹膜算法结果数据(index对应帧)
         @param[in] index 数据索引
         @param[in] data 算法结果数据
         @return 成功取到算法数据返回true，否则为false
         */
    virtual bool getIrisResultData(int index, IKIrisProcessedResult& data) = 0;

    /*!
         @brief 获取人脸算法结果数据(index对应帧)
         @param[in] index 数据索引
         @param[in] data 算法结果数据
         @return 成功取到算法数据返回true，否则为false
         */
    virtual bool getFaceResultData(int index, IKFaceProcessedResult& data) = 0;

    /*!
         @brief 获取虹膜注册结果数据的数量
         @param 无
         @return 注册结果数据的数量
         */
    virtual int getIrisFrameNum() = 0;

    /*!
         @brief 获取人脸注册结果数据的数量
         @param 无
         @return 注册结果数据的数量
         */
    virtual int getFaceFrameNum() = 0;

    /*!
         @brief 设置虹膜注册结果
         @param[in] 虹膜注册结果
         @return 无
         */
    virtual void setIrisRes(int res) = 0;
    /*!
         @brief 设置人脸注册结果
         @param[in] 人脸注册结果
         @return 无
         */
    virtual void setFaceRes(int res) = 0;
    /*!
         @brief 获取虹膜注册结果
         @param 无
         @return 注册结果,注册成功返回值为IKIris_Fun_Success,失败返回对应错误码
         */
    virtual int getIrisRes() = 0;
    /*!
         @brief 获取人脸注册结果
         @param 无
         @return 注册结果,注册成功返回值为IKFace_Fun_Success,失败返回对应错误码
         */
    virtual int getFaceRes() = 0;
};
/*!
 * @brief 工作模式枚举
*/
enum IKWorkMode
{
    IK_WorkMode_Enroll = 0,/*!< 注册模式*/
    IK_WorkMode_Iden = 1,/*!< 识别模式*/
    IK_WorkMode_Init = 2,/*!< 初始化模式*/
    IK_WorkMode_Idle = -1,/*!< 空闲模式*/
};
/*!
 * @brief 多模态识别结果信息结构体
*/
struct IKIdentProcessedResultStruct
{    
    IKEnrollIdentResultFlag _enrollIdentResFlag;/*!< 描述多模态识别结果*/
    IKEnrollIdentMode _enrollIdentMode;/*!< 识别模式*/
    IKIrisIdentProcessedResultStruct _irisIdentResult;/*!< 虹膜识别结果*/
    IKFaceIdentOutputStruct _faceIdentResult;/*!< 人脸识别结果*/

    IKIdentProcessedResultStruct()
    {
        _enrollIdentResFlag = IKEnrRecUnknown;/*!< 描述多模态识别结果*/
        _enrollIdentMode = IKModeNone;/*!< 识别模式*/
    }

};
/*!
 * @brief 回调信息中人脸虹膜信息有效枚举类型(每帧只有一种类型有效)
*/
enum IKIrisAndFaceDataValidFlag
{    
    DataNone = 0, /*!< 均无效*/
    IrisDataValid = 1,/*!< 虹膜状态信息有效*/
    FaceDataValid = 2,/*!< 人脸状态信息有效*/
};
/*!
 * @brief 多模态状态回调信息结构体
*/
struct IKProcessorState
{
    IKEnrollIdentMode mode;/*!< 注册识别模式*/
    IKIrisAndFaceDataValidFlag dataValidFlag;/*!< 人脸虹膜信息有效标志*/
    IKIrisProcessorState* pIrisState;/*!< 虹膜状态信息*/
    IKFaceProcessorState* pFaceState;/*!< 人脸状态信息*/

    IKProcessorState()
    {
        mode = IKModeNone;/*!< 注册识别模式*/
        dataValidFlag = DataNone;/*!< 人脸虹膜信息有效标志*/
        pIrisState = nullptr;/*!< 虹膜状态信息*/
        pFaceState = nullptr;/*!< 人脸状态信息*/
    }
};
struct IKDebugInfo
{
    IKIrisAndFaceDataValidFlag dataValidFlag;/*!< 人脸虹膜信息有效标志*/
    IKIrisDebugInfo* irisDebugInfo;
    IKFaceDebugInfo* faceDebugInfo;

    IKDebugInfo()
    {
        dataValidFlag = DataNone;/*!< 人脸虹膜信息有效标志*/
        irisDebugInfo = nullptr;
        faceDebugInfo = nullptr;
    }
};
//////////////////////////////////////////////所有回调函数类///////////////////////////////////////////////////
/*!
 * @brief log类型枚举
*/
enum IKLog{
    IK_Log_STATUS = 1,/*!< 只输出帧处理情况的状态回调信息,即丢图过程及原因*/
    IK_Log_VERBOSE = 2,/*!< 在注册识别结束时会将整个注册过程中每一帧输入的图像处理结果综合输出*/
    IK_Log_DEBUG = 3, /*!< 注册识别的过程信息*/
    IK_Log_INFO = 4, /*!< 暂时只输出帧率*/
    IK_Log_WARN = 5, /*!< 暂未使用*/
    IK_Log_ERROR = 6, /*!< 检测到错误*/
    IK_Log_Performance = 7,/*!< 性能测试相关log输出，例如time*/
};
/*!
 * @brief 回调类
 * @note 所有的回调用户只能在回调中获取数据，不能做其他的阻塞or耗时or操作SDK接口
*/
class IKBaseProcessorCallback
{
public:
    virtual ~IKBaseProcessorCallback() {}
    /*!
         * @brief 初始化结果回调
         *
         * @param[in] initRes 初始化结果
         * @return 无
         **/
    virtual void handleInitCompleted(int initRes) = 0;
    /*!
         * @brief 多模态注册结果回调
         *
         * @param[in] result 多模态注册结果类
         * @note
         * @return 无
         **/
    virtual void handleProcessCompleted(IKBaseEnrollProcessedResult &result) = 0;
    /*!
         * @brief 多模态识别结果回调
         *
         * @param[in] result 多模态识别结果结构体
         * @return 无
         **/
    virtual void handleIdentProcessCompleted(const IKIdentProcessedResultStruct &result) = 0;
    /*!
         * @brief 多模态状态回调
         *
         * @param[in] result 多模态状态回调结构体
         * @return 无
         **/
    virtual void handleStateUpdated(const IKProcessorState &state)= 0;
    /*!
         * @brief 虹膜调试图像信息回调不需要则可以不用实现
         *
         * @param[in]
         * @return 无
         **/
    virtual void handleDebugInfoUpdated(const IKDebugInfo& debugInfo)
    {
        (void)debugInfo;
    }

    /*!
         * @brief camera回调
         *
         * @param[in] cameraUpdate camera相关数据和状态回调
         * @return 无
         **/
    virtual void handleCameraUpdated(const IKCameraUpdated &cameraUpdate)= 0;
    /*!
         * @brief log回调
         *
         * @param[in] logType log类型 "IKFace" "IKIris"
         * @param[in] logLevel log等级
         * @param[in] logInfo log内容
         * @return 无
         **/
    virtual void handleLog( const char* logType, IKLog logLevel, const char* logInfo)= 0;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
