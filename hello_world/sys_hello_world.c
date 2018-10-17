#include <linux/kernel.h>

asmlinkage long sys_hello_world(const char __user *str,
                                const size_t str_len,
                                char __user *buf,
                                size_t buf_len) {
  printk(">>>>>>>> Hello World!\n");
  return 0;
}
