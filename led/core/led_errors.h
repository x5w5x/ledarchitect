#ifndef _LED_ERRORS_H
#define _LED_ERRORS_H


#ifdef __cplusplus
extern "C" {
#endif

//错误码定义 V2.1新增

typedef enum{
    LED_OK=0,
    LED_ERR_INVALID_ARG, //无效参数
    LED_ERR_OUT_OF_MEMORY, //内存池耗尽
    LED_ERR_NOT_INITTALIZED, //LED管理器未初始化
    LED_ERR_DRIVE_FAILED, //驱动层操作失败
    LED_ERR_UNSUPPORTED_MODE, //不支持的LED模式
}led_err_t;







#ifdef __cplusplus
}
#endif

#endif
