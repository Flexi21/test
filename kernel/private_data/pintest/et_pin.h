#ifndef  __ET_PIN_
#define  __ET_PIN_

#define ET_PIN_DEBUG
#if defined(ET_PIN_DEBUG)
#define TAG              "et_pinctrl: "
#define LOGD(x...)        pr_debug(TAG x)
#define LOGE(x...)        pr_err(TAG x)
#else
#define LOGD(...)
#define LOGE(...)
#endif


/**********************Extern Part*******************/
extern void et_set_gpio_value(struct platform_device *ppdev, int pin, int val);
extern void et_get_gpio_value(struct platform_device *ppdev, int pin, int val);
extern void et_set_gpio_dir(struct platform_device *ppdev, int pin, int val);
extern void et_set_gpio_mode(struct platform_device *ppdev, int pin, int val);
#endif
