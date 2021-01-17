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

#include <linux/init.h>//_init과 _exit 매크로를 포함한다.
#include <linux/module.h>//커널 모듈을 사용하기위해 탑재하기 위해 필요한 헤더파일이다.
#include <linux/kernel.h>//KERN_INFO를 포함하는 헤더파일

/* This function is called when the module is loaded. */
int simple_init(void)//후에 module_init을 통해 모듈이 실행되면 실행될 함수이다.
{
       printk(KERN_INFO "Loading Module\n");//"Loading Module"을 출력한다.
		//커널 함수에서 사용되는 출력함수이고 printk 함수는 가장 편하고 영향력 있는 디버깅 방법이다.
	   //커널의 메시지를 출력하고 관리할 수 있는 특성이 있다.
	   //KERN_INFO는 로그레벨 주 하나로 숫자가 낮을수록 우선순위가 높다
	   //로그레벨 <6> 이며 시스템의 정보를 의미한다.

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {//후에 module_exit을 통해 모듈이 종료되면 실행될 함수이다.
	printk(KERN_INFO "Removing Module\n");//"Removing Module"을 출력한다.
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );//module_init으로 등록한 함수를 호출해주는 매크로이다.
module_exit( simple_exit );//module_exit으로 등록한 함수를 호출해주는 매크로이다.

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

