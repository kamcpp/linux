#include <linux/kernel.h>  // For printk
#include <linux/string.h>  // For strcpy, strcat, strlen
#include <linux/slab.h>    // For kmalloc, kfree
#include <linux/uaccess.h> // For copy_from_user, copy_to_user

asmlinkage long sys_hello_world(const char __user *str,
                                const size_t str_len,
                                char __user *buf,
                                size_t buf_len) {
  void* name = NULL;
  void* message = NULL;

  name = kmalloc((str_len + 1) * sizeof(char), GFP_KERNEL);
  if (!name) {
    printk("Memory allocation failed.\n");
    return -1;
  }
  if (copy_from_user(name, str, str_len)) {
    printk("Copy from user space failed.\n");
    kfree(name);
    return -2;
  }
  message = kmalloc((16 + str_len + 1) * sizeof(char), GFP_KERNEL);;
  if (!message) {
    printk("Memory allocation failed.\n");
    kfree(name);
    return -1;
  }
  strcpy(message, "Hello World ");
  strcat(message, name);
  if (strlen(message) >= buf_len) {
    printk("Small user space buffer.\n");
    kfree(name);
    kfree(message);
    return -3;
  }
  if (copy_to_user(buf, message, strlen(message) + 1)) {
    printk("Copy to user space failed.\n");
    kfree(name);
    kfree(message);
    return -4;
  }
  printk("Written message: %s\n", (char*)message);
  kfree(name);
  kfree(message);
  return 0;
}
