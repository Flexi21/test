#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include "et_pin.h"

struct pin_private {
        char *name;
        int  count;
};

//debug
static ssize_t et_pin_show(struct device *pdev, 
                             struct device_attribute *attr, char *buf)
{
         int len = 0;
         struct pin_private *pin = dev_get_drvdata(pdev);         
 	 len += snprintf(buf+len, PAGE_SIZE-len, "%s \r\n",pin->name);    
	 len += snprintf(buf+len, PAGE_SIZE-len, "%d \r\n",pin->count);    
         return len;
}

static ssize_t et_pin_store(struct device *pdev, struct device_attribute *attr,
                                const char *buf, size_t lens)
{
        return lens;
}

static DEVICE_ATTR(pin_test, 0660, et_pin_show, et_pin_store);

static struct device_attribute *attribute_pin_list[] = {
        &dev_attr_pin_test,
};


static int et_pinctrl_probe(struct platform_device *pdev)
{
        int ret = -EINVAL;
        struct pin_private *pinprivate;

        LOGD("Func:%s Entry!\n",__func__);

        pinprivate = devm_kzalloc(&pdev->dev, sizeof(struct pin_private), GFP_KERNEL);
        if (!pinprivate)
            return -ENOMEM;

        platform_set_drvdata(pdev, pinprivate);

        pinprivate->name  = "just for test!";
        pinprivate->count = 999;

        ret = device_create_file(&pdev->dev, attribute_pin_list[0]);

        LOGD("Func:%s End!\n",__func__);
	return 0;
}

static const struct of_device_id et_pinctrl_ids[] = {
	{.compatible = "et,pinctrl",},
        {},
};
MODULE_DEVICE_TABLE(of, et_pinctrl_ids);

static struct platform_driver et_pinctrl_driver = {
	.probe = et_pinctrl_probe,
	.driver = {
		.name = "et_pinctrl",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(et_pinctrl_ids),
	},
};

static int _et_pinctrl_init(void)
{
        int ret = -EINVAL;
        ret = platform_driver_register(&et_pinctrl_driver);
        if (ret < 0){
            LOGE("func:%s Platform driver register error\n",__func__);
            return -ENODEV;
        }

        return 0;
}

static void _et_pinctrl_exit(void)
{
        platform_driver_unregister(&et_pinctrl_driver);
}
module_init(_et_pinctrl_init);
module_exit(_et_pinctrl_exit);
MODULE_AUTHOR("zwl");
MODULE_LICENSE("GPL");
