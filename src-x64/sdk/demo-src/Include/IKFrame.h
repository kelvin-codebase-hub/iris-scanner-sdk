/*!
* @file IKFrame.h
* @brief CameraAPI相关的一些数据结构定义以及错误码
*
* @author tangzhuan
* @version
* @date 2020.06.01
*/
#pragma once
#include <string.h>
#if defined _WIN32 || defined _WIN64
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <memory>
/////////////////////////////////////////设备类型相关信息结构///////////////////////////////////////////
#define  DELETEOBJ(x)	\
    if((x) != nullptr)		\
{					\
    delete (x);		\
    (x) = nullptr;	\
    }

#define  DELETEARRAY(x)	\
    if((x) != nullptr)		\
{					\
    delete[] (x);	\
    (x) = nullptr;		\
    }

#define  FREE(x)		\
    if((x) != nullptr)		\
{					\
    free((x));		\
    (x) = nullptr;		\
    }

/*!
 * @brief Camera相关错误码只能定义在-4000至-4999段内
*/
const int E_CAMERA_OK = 0;/*!< 操作成功*/
const int E_CAMERA_BASE_ERROR = 0;/*!< 基础错误码*/
const int E_CAMERA_INVALID_OPERATE = 2;/*!< 操作没有执行成功，但不影响运行，不算做错误*/
const int E_CAMERA_INVALID_INPUT = E_CAMERA_BASE_ERROR - 4001;/*!< 无效的输入参数,比如指针为空*/
const int E_CAMERA_INVALID_DEVICETYPE = E_CAMERA_BASE_ERROR - 4002;/*!< 无效的设备类型*/
const int E_CAMERA_INIT_FAILED = E_CAMERA_BASE_ERROR - 4003;/*!< 初始化失败*/
const int E_CAMERA_DEVICE_NOTFOUND = E_CAMERA_BASE_ERROR - 4004;/*!< 没有发现设备*/
const int E_CAMERA_OPEN_FAILED = E_CAMERA_BASE_ERROR - 4005;/*!< 打开设备失败*/
const int E_CAMERA_INVALID_FORMAT_SETTING = E_CAMERA_BASE_ERROR - 4006;/*!< 无效的格式流*/
const int E_CAMERA_WAITCB_TIMEOUT = E_CAMERA_BASE_ERROR - 4007;/*!< 等待数据回调超时*/
const int E_CAMERA_NOT_SUPPORT = E_CAMERA_BASE_ERROR - 4008;/*!< 暂不支持此项功能*/
const int E_CAMERA_INTERFACE_FAILED = E_CAMERA_BASE_ERROR - 4009;/*!< 其他原因导致的接口失败*/
const int E_CAMERA_DEVICE_HAS_OPEND = E_CAMERA_BASE_ERROR - 4010;    /*!< 设备已经打开*/
const int E_CAMERA_DEVICE_CTRLCOM_ERROR = E_CAMERA_BASE_ERROR - 4011;/*!< 设备用于控制的串口打开失败*/
const int E_CAMERA_READ_DEVICE_PROP = E_CAMERA_BASE_ERROR - 4012;    /*!< 获取设备参数失败*/
const int E_CAMERA_WRITE_DEVICE_PROP = E_CAMERA_BASE_ERROR - 4013;    /*!< 设置设备参数失败*/
const int E_CAMERA_LIB_NOT_FOUND = E_CAMERA_BASE_ERROR - 4014;    /*!< 依赖库加载失败*/

#define DNSMPRATE 4 /*!< 图像降采样倍数*/
#define CBDEFAULTVALUE 9999 /*!< 信息默认值即数据没有意义可不处理*/

/*!
     * @brief Camera灯的亮度模式
     **/
enum IKLIGHTMODE
{
    MODE_DEGUALT = 0 ,/*!<默认*/
    MODE_ASYNFREQUENT = 1,/*!<异步频闪*/
    MODE_SYNFREQUENT = 2,/*!<同步频闪*/
    MODE_LOWCONTINUOUS = 3,/*!<弱常亮*/
    MODE_HIGHCONTINUOUS = 4,/*!<强常亮*/
};
/*!
     * @brief Camera部件
     **/
enum IKCOMPONENTTYPE
{
    IKCOMPONENT_InfraredLamp = 0,/*!< 红外灯*/
    IKCOMPONENT_FillLight= 1,/*!< 补光灯*/
    IKCOMPONENT_OUTPUTRGB=2,/*!< 针对以mjpeg输出的rgb相机*/
    IKCOMPONENT_IRISDIVECE_SN = 3,
    IKCOMPONENT_FACEDIVECE_SN = 4,    
    IKCOMPONENT_PTZ_CALIBRATION = 5, /*!< 舵机Y方向校准*/
    IKCOMPONENT_PTZ_HOMING = 6, /*!< 舵机Y方向复位*/
    IKCOMPONENT_PTZ_TEST = 7, /*!< 舵机Y方向测试用*/
};

/*!
     * @brief Camera部件状态值
     **/
enum IKCOMPONENTSTATUS
{
    IKCOMPONENTUNKNOWN = -1,
    IKCOMPONENTOFF = 0,/*!< */
    IKCOMPONENTON = 1,/*!< */
};
/////////////////////////////////////////图像相关信息结构///////////////////////////////////////////
/*!
 * @brief 图像格式
 * @note 目前irisAPI只接收IK_Frame_Format_Gray_8类型图像数据;faceAPI只接收IK_Frame_Format_RGB类型图像数据
*/
enum IKFrameFormat
{
    IK_Frame_Format_YUV = 0,
    IK_Frame_Format_YUYV = 1,
    IK_Frame_Format_YUV20SP = 2,
    IK_Frame_Format_BGRA = 3,
    IK_Frame_Format_BGR = 4,
    IK_Frame_Format_RGBA = 5,
    IK_Frame_Format_RGB = 6,
    IK_Frame_Format_RGB565 = 7,
    IK_Frame_Format_RAW = 8,
    IK_Frame_Format_NV21 = 9,
    IK_Frame_Format_MJPEG = 10,
    IK_Frame_Format_Gray_8 = 11,
    IK_Frame_Format_Gray_16 = 12,
    IK_Frame_Format_DEPTH = 1024,
    IK_Frame_Formate_Unknown = CBDEFAULTVALUE,
};

/*!
 * @brief 图像类型
*/
enum IKFrameType
{
    IK_Frame_Light = 0,/*!< 人脸图像*/
    IK_Frame_Dark, /*!< */
    IK_Frame_Depth,/*!< 深度图像*/
    IK_Frame_IR,/*!< 红外图像*/
    IK_Frame_IRIS,/*!< 虹膜大图*/
    IK_Frame_IRIS_VGA_L,/*!< 虹膜VGA图左眼*/
    IK_Frame_IRIS_VGA_R,/*!< 虹膜VGA图右眼*/
    IK_Frame_IRIS_VGA_LR,/*!< 虹膜VGA图双眼，默认放置顺序左眼在前右眼在后*/
    IK_Frame_Unknown = CBDEFAULTVALUE,/*!< 未知*/
};

/*!
 * @brief 图像数据帧包装类
*/
class IKFrame
{
public:
    /*!
         * @brief 无参构造，构造一个空类
         *
         * @param 无
         * @return 无
         **/
    IKFrame()
    {
        data_len = 0;
        width = 0;
        height = 0;
        rotation = 0;
        format = IK_Frame_Formate_Unknown;
        frame_type = IK_Frame_Unknown;
    }
    /*!
         * @brief 有参构造，根据输入数据开辟好空间
         *
         * @param[in] w 宽
         * @param[in] h 高
         * @param[in] angle 角度
         * @param[in] color_space 格式
         * @param[in] brightness 类型
         * @return 无
         **/
    IKFrame(int w, int h, int angle, IKFrameFormat color_space, IKFrameType brightness)
    {
        width = w;
        height = h;
        rotation = angle;
        format = color_space;
        frame_type = brightness;
        data_len = getDataLen();
        image_data = std::shared_ptr<unsigned char>(new unsigned char[data_len],deleteSharedPtr);
    }
    /*!
         * @brief 拷贝构造 浅拷贝
         *
         * @param[in] src IKFrame类
         * @return 无
         **/
    IKFrame(const IKFrame &src)
    {
        image_data = src.image_data;
        data_len = src.data_len;
        width = src.width;
        height = src.height;
        rotation = src.rotation;
        format = src.format;
        frame_type = src.frame_type;        
    }
    /*!
         * @brief 析构
         *
         * @param 无
         * @return 无
         **/
    ~IKFrame()
    {
    }
    /*!
         * @brief 赋值拷贝 深拷贝
         *
         * @param[in] src IKFrame类
         * @return IKFrame类
         **/
    IKFrame& operator=(const IKFrame &src)
    {
        if (&src == this)
        {
            return *this;
        }
        if(!empty() && data_len != src.data_len)
        {
            clear();
        }
        data_len = src.data_len;
        width = src.width;
        height = src.height;
        rotation = src.rotation;
        format = src.format;
        frame_type = src.frame_type;        
        if(empty())
        {
            image_data = std::shared_ptr<unsigned char>(new unsigned char[data_len],deleteSharedPtr);
        }
        memcpy(image_data.get(), src.image_data.get(), static_cast<size_t>(data_len));

        return *this;
    }
    /*!
         * @brief 输入图像包装类进行浅拷贝
         *
         * @param[in] src IKFrame类
         * @return IKFrame类
         **/
    IKFrame &simpleCpy(const IKFrame &src)
    {
        if (&src == this)
        {
            return *this;
        }
        image_data = src.image_data;
        data_len = src.data_len;
        width = src.width;
        height = src.height;
        rotation = src.rotation;
        format = src.format;
        frame_type = src.frame_type;
        return *this;
    }
    /*!
         * @brief 判断图像数据是否为NULL
         *
         * @param 无
         * @return true为空，false为非空
         **/
    bool empty() const
    {
        return image_data.get() == nullptr;
    }
    /*!
         * @brief 清理图像帧为NULL，即释放掉类内分配的空间
         *
         * @param 无
         * @return 清理是否成功
         **/
    bool clear()
    {
        image_data.reset();
        data_len = 0;
        width = 0;
        height = 0;
        rotation = 0;
        format = IK_Frame_Formate_Unknown;
        frame_type = IK_Frame_Unknown;
        return true;
    }    
    std::shared_ptr<unsigned char> image_data; /*!< 图像数据*/
    long data_len;/*!< 图像数据长度*/
    int width;/*!< 图像的宽*/
    int height;/*!< 图像的高*/
    int rotation;/*!< 旋转角度，暂时未使用*/
    IKFrameFormat format;/*!< 图像格式*/
    IKFrameType frame_type;/*!< 图像类型*/
private:
    static void deleteSharedPtr(unsigned char * p)
    {
        if (p != nullptr)
        {
            delete[] p;
            p = nullptr;            
        }
    }
    /*!
         * @brief 获取图像数据长度
         *
         * @param 无
         * @return 图像数据长度
         **/
    int getDataLen() const
    {
        int imageDataCount = 1;
        if(frame_type == IK_Frame_IRIS_VGA_LR)
        {
            imageDataCount = 2;
        }
        if (format == IK_Frame_Format_YUV || format == IK_Frame_Format_YUV20SP || format == IK_Frame_Format_NV21)
        {
            return width * height * 3 * imageDataCount / 2;
        }
        if (format == IK_Frame_Format_BGRA || format == IK_Frame_Format_RGBA)
        {
            return width * height * 4 * imageDataCount ;
        }
        if (format == IK_Frame_Format_BGR || format == IK_Frame_Format_RGB || format == IK_Frame_Format_MJPEG)
        {
            return width * height * 3 * imageDataCount ;
        }
        if (format == IK_Frame_Format_DEPTH ||  format == IK_Frame_Format_RGB565 || format == IK_Frame_Format_RAW)
        {
            return width * height * 2 * imageDataCount ;
        }
        if (format == IK_Frame_Format_Gray_8)
        {
            return width * height * imageDataCount ;
        }
        if (format == IK_Frame_Format_Gray_16)
        {
            return width * height * 2 * imageDataCount ;
        }
        return 0;
    }
};

/*!
 * @brief camera热插拔回调事件枚举类型
*/
enum IKCameraEvent{
    IKUSB_HOTPLUG_EVENT_DEVICE_ARRIVED = (1 << 0),/*!< 设备已插入并可以使用*/
    IKUSB_HOTPLUG_EVENT_DEVICE_LEFT = (1 << 1),/*!< 设备已离开，不再可用*/
    IKUSB_EVENT_DEVICE_UPSIDE = (1 << 2),/*!< 设备倒置*/
    IKUSB_MIRRORFLAG_CHECK_ERROR = (1 << 3),/*!< camera回调的图像方向和SDK设置的不一致，请检查*/
};

struct IKCameraUpdated{
    IKFrame* frame; /*!< 图像帧指针*/
    bool mirrorFlag; /*!< 图像帧是否为镜像图*/
    IKCameraEvent event; /*!< Camera事件回调*/
    int deviceType; /*!< Camera设备类型*/
    IKLIGHTMODE lightMode; /*!< 红外灯的亮灯模式*/
    char* deviceSN;
    int deviceSNLen;
    char* deviceTypeString;
    int deviceTypeStringLen;

    IKCameraUpdated()
    {
        frame = nullptr; /*!< 图像帧指针*/
        deviceSN = nullptr;
        deviceSNLen = 0;
        deviceTypeString = nullptr;
        deviceTypeStringLen = 0;
    }

};
