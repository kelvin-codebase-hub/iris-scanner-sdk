/*!
* @file IKBaseSDKAPI.h
* @defgroup IKBaseSDKAPI
* @brief 多模态SDKAPI基类
*
* @author tangzhuan
* @version
* @date 2020.06.01
*/

#pragma once
#include <stdio.h>
#include <memory>

#include "IKBaseSDKAPIStruct.h"


#if defined _WIN32 ||defined _WIN64
#ifdef  IKBaseSDKAPI_EXPORTS
#define  __IKBaseSDKAPI_CLASS_ extern "C" __declspec(dllexport)
#else
#define  __IKBaseSDKAPI_CLASS_ extern "C" __declspec(dllimport)
#endif
#define __IK_STYLE_CALL  __stdcall
#else
#define  __IKBaseSDKAPI_CLASS_ extern "C"
#define __IK_STYLE_CALL
#endif

class IKBaseSDKAPI{
public:
    explicit IKBaseSDKAPI(){}
    virtual ~IKBaseSDKAPI() = 0;

    /*!
         * @brief 算法初始化
         *
         * @param[in] callback 回调函数类，包含初始化结果回调,注册结果回调，识别结果回调，状态回调，camera回调，log回调         
         * @param[in] initDeviceMode 初始化设备的模式
         * @return 返回初始化线程开启是否成功,成功返回IK_Fun_Success,失败返回错误码
         **/
    virtual int Init(IKBaseProcessorCallback* callback,IKInitMode initDeviceMode) = 0;

    /*!
         * @brief 注册识别配置信息
         *
         * @param[in] config 配置信息
         * @return 返回初始化是否成功,成功返回IK_Fun_Success,失败返回错误码
         **/
    virtual int Config(IKConfigInfo config) = 0;
    /*!
         * @brief 开启注册线程
         *
         * @param 无
         * @return 返回注册线程开启是否成功,成功返回IK_Fun_Success,失败返回错误码
         **/
    virtual int StartEnroll() = 0;

    /*!
         * @brief 停止注册线程
         *
         * @param 无
         * @return 返回注册线程停止是否成功,成功返回IK_Fun_Success,失败返回错误码
         **/
    virtual int StopEnroll() = 0;

    /*!
         * @brief 开启识别线程
         *
         * @param[in] pCodeListL 左眼注册特征列表
         * @param[in] codeNumL  左眼注册特征数量
         * @param[in] pCodeListR 右眼注册特征列表
         * @param[in] codeNumR  右眼注册特征列表
         * @param[in] pCodeListF 人脸注册特征列表
         * @param[in] codeNumF  人脸注册特征列表
         * @return 返回识别线程开启是否成功,成功返回IK_Fun_Success,失败返回错误码
         **/
     virtual int StartIdent(unsigned char *pCodeListL,int codeNumL,unsigned char *pCodeListR,int codeNumR,unsigned char *pCodeListF,int codeNumF) = 0;

    /*!
         * @brief 停止识别线程
         *
         * @param 无
         * @return 返回识别线程停止是否成功,成功返回IK_Fun_Success,失败返回错误码
         **/
    virtual int StopIdent() = 0;

    /*!
         * @brief 清除缓存
         *
         * @param 无
         * @return 返回清除缓存是否成功,成功返回IK_Fun_Success,失败返回错误码
         **/
    virtual int Reset() = 0;

    /*!
          * @brief 算法比对
          *
          * @param[in] code   待比对的特征
          * @param[in] pCodeList 特征列表
          * @param[in] codeNum  特征列表数量
          * @param[out] pMatchOutput 比对结果
          * @param[in] matchMode 比对模式
          * @return 返回算法比对是否成功,成功返回IK_Fun_Success,失败返回错误码
          **/
    virtual int MatchFeature(unsigned char *code,unsigned char *pCodeList,int codeNum,IKMatchOutput *pMatchOutput,IKMatchMode matchMode) = 0;

    /*!
         * @brief 释放所有资源
         *
         * @param 无
         * @return 返回释放所有资源是否成功,成功返回IK_Fun_Success,失败返回错误码
         **/
    virtual int Release() = 0;

    /*!
         * @brief 获取版本
         *
         * @param[in] version 指向版本信息的指针，空间由用户分配
         * @return 无
         **/
    virtual void GetVersion(char* version) = 0;

    /*!
         * @brief 设备部件操作
         *
         * @param[in] type 设置部件类型
         * @param[in] status 设置部件开关状态
         * @param[in] value 设置部件值
         * @param[in] mode 设置部件模式
         * @note 不同设备对输入参数需求不同，根据具体项目或设备对应使用即可
         * @return 成功返回IK_Fun_Success,失败返回错误码
         **/
    virtual int WriteParameter(IKCOMPONENTTYPE type,IKCOMPONENTSTATUS status,int value = 0,IKLIGHTMODE mode = MODE_DEGUALT) = 0;    
};

/*!
     * @brief 创建多模态SDKAPI实例
     *
     * @param 无
     * @return 创建成功返回多模态SDKAPI实例,创建失败返回NULL
     **/
__IKBaseSDKAPI_CLASS_ IKBaseSDKAPI* __IK_STYLE_CALL CreateSDKObj();

/*!
     * @brief 释放多模态SDKAPI实例
     *
     * @param[in] irisApi 多模态SDKAPI实例指针的地址
     * @return 无
     **/
__IKBaseSDKAPI_CLASS_ void __IK_STYLE_CALL DestroySDKObj(IKBaseSDKAPI** api);
