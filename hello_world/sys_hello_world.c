#include <linux/kernel.h>   // For printk
#include <linux/string.h>   // For strcpy, strcat, strlen
#include <linux/slab.h>     // For kmalloc, kfree
#include <linux/uaccess.h>  // For copy_from_user, copy_to_user
#include <linux/syscalls.h> // For SYSCALL_DEFINE4

SYSCALL_DEFINE4(hello_world, 
		const char __user *, str, 
		const unsigned int, str_len, 
		char __user *, buf, 
		unsigned int, buf_len) { 
/*asmlinkage long sys_hello_world(const char __user *str,
                                const int str_len,
                                char __user *buf,
                                int buf_len) {*/
  char name[64];
  char message[96];

  printk("System call fired! %d.\n", str_len);
  if (str_len >= 64) {
    printk("Too long input string.\n");
    return -1;
  }

  if (copy_from_user(name, str, str_len)) {
    printk("Copy from user space failed.\n");
    return -2;
  }

  strcpy(message, "Hello ");
  strcat(message, name);
  strcat(message, "!");

  if (strlen(message) >= (buf_len - 1)) {
    printk("Too small output buffer.\n");
    return -3;
  }

  if (copy_to_user(buf, message, strlen(message) + 1)) {
    printk("Copy to user space failed.\n");
    return -4;
  }

  printk("Message: %s\n", message);
  return 0;
}
