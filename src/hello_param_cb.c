#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stat.h>

#define MAX_LEN 128

static unsigned int idx;
static unsigned int ch_val;
static char str[MAX_LEN] = "";
static char *my_str = str;

static int idx_set(const char *val, const struct kernel_param *kp) {
  int ret;
  unsigned int new_idx;

  // Парсим значение
  ret = kstrtouint(val, 10, &new_idx);
  if (ret < 0) {
    pr_err("Error: idx must be a number!\n");
    return ret;
  }

  // Проверяем, что позиция в пределах строки
  if (new_idx >= MAX_LEN - 1) {
    pr_err("Error: idx %u more than allowed. Max position = %d\n", new_idx,
           MAX_LEN - 1);
    return -EINVAL;
  }

  idx = new_idx;
  return 0;
}

static const struct kernel_param_ops idx_ops = {
    .set = idx_set,
    .get = param_get_uint,
};

static int ch_val_set(const char *val, const struct kernel_param *kp) {
  int ret;
  unsigned int new_ch_val;

  ret = kstrtouint(val, 10, &new_ch_val);
  if (ret < 0) {
    pr_err("Error: ch_val  must be a number!\n");
    return ret;
  }

  if (new_ch_val > 127 || new_ch_val == 0) {
    pr_err("Wrong ASCII code (1-127 allowed)\n");
    return -EINVAL;
  }

  ch_val = new_ch_val;

  my_str[idx] = (char)ch_val;

  pr_info("String changed my_str = %s", (char *)my_str);
  return 0;
}

static const struct kernel_param_ops ch_val_ops = {
    .set = ch_val_set,
    .get = param_get_uint,
};

module_param_cb(idx, &idx_ops, &idx, 0664);
MODULE_PARM_DESC(idx, "index of character (0 - MAX_LEN-1)");
module_param_cb(ch_val, &ch_val_ops, &ch_val, 0664);
MODULE_PARM_DESC(ch_val, "ascii code (0 - MAX_LEN-1)");
module_param(my_str, charp, 0444);
MODULE_PARM_DESC(my_str, "Result string");

static int __init mod_init(void) {
  pr_info("%s: init\n", KBUILD_MODNAME);
  return 0;
}

static void __exit mod_exit(void) { pr_info("%s: exit\n", KBUILD_MODNAME); }

module_init(mod_init);
module_exit(mod_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("IamMaver");
MODULE_DESCRIPTION("Hello world with callbacks");
