/**
 * simple.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>//_init�� _exit ��ũ�θ� �����Ѵ�.
#include <linux/module.h>//Ŀ�� ����� ����ϱ����� ž���ϱ� ���� �ʿ��� ��������̴�.
#include <linux/kernel.h>//KERN_INFO�� �����ϴ� �������

/* This function is called when the module is loaded. */
int simple_init(void)//�Ŀ� module_init�� ���� ����� ����Ǹ� ����� �Լ��̴�.
{
       printk(KERN_INFO "Loading Module\n");//"Loading Module"�� ����Ѵ�.
		//Ŀ�� �Լ����� ���Ǵ� ����Լ��̰� printk �Լ��� ���� ���ϰ� ����� �ִ� ����� ����̴�.
	   //Ŀ���� �޽����� ����ϰ� ������ �� �ִ� Ư���� �ִ�.
	   //KERN_INFO�� �α׷��� �� �ϳ��� ���ڰ� �������� �켱������ ����
	   //�α׷��� <6> �̸� �ý����� ������ �ǹ��Ѵ�.

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {//�Ŀ� module_exit�� ���� ����� ����Ǹ� ����� �Լ��̴�.
	printk(KERN_INFO "Removing Module\n");//"Removing Module"�� ����Ѵ�.
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );//module_init���� ����� �Լ��� ȣ�����ִ� ��ũ���̴�.
module_exit( simple_exit );//module_exit���� ����� �Լ��� ȣ�����ִ� ��ũ���̴�.

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

