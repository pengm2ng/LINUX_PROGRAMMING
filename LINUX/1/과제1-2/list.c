
#include <linux/init.h>//_init과 _exit 매크로를 포함한다.
#include <linux/module.h>//커널 모듈을 사용하기위해 탑재하기 위해 필요한 헤더파일이다.
#include <linux/kernel.h>//KERN_INFO를 포함하는 헤더파일
 #include <linux/slab.h>//kmalloc과 kfree를 사용하기 위한 헤더파일
 #include <linux/list.h>//list의 매크로들을 사용하기 위한 헤더파일
 struct birthday {//생일의 년월일을 담고 있는 구조체 선언
      
       int month;//구조체 변수 월
       int day;//구조체 변수 일
       int year;//구조체 변수  년      
       struct list_head list;  // 구조체 리스트 선언
       }; 
        struct birthday *ptr;//구조체 변수 포인터 선언
        struct birthday *person_one;//구조체 변수 사람1 선언
        struct birthday *person_two;//구조체 변수 사람2 선언
        struct birthday *person_three;//구조체 변수 사람3 선언
       struct birthday *person_four;//구조체 변수 사람4 선언
       struct birthday *next;
       int num=0;//사람 번호를 출력하기 위한 num 변수
    
    static LIST_HEAD(birthday_list);
	//사용자가 오브젝트를 추가할 수 있는 birthday_list의 list_head오브젝트를 정의하고 초기화하는 매크로
	
int simple_init(void)
{//후에 module_init을 통해 모듈이 실행되면 실행될 함수


	printk(KERN_INFO "Loading Module\n"); //"Loading Module"을 출력
		//커널 함수에서 사용되는 출력함수이고 printk 함수는 가장 편하고 영향력 있는 디버깅 방법
		//커널의 메시지를 출력하고 관리할 수 있는 특성이 있음.
		//KERN_INFO는 로그레벨 주 하나로 숫자가 낮을수록 우선순위가 높음.
		//로그레벨 <6> 이며 시스템의 정보를 의미
       
       
		person_one = kmalloc(sizeof(*person_one), GFP_KERNEL); 
		//person_one에 메모리 할당
		//GFP_KERNEL은 동적메모리 할당이 항상 성공하도록 요구
		//메모리가 충분하지 않을 겨우 프로세스 수행 중단

		person_one->month = 1;
		person_one->day = 23;
		person_one->year = 2000;//person_one의 생일은 2000년 1월 23일  
		INIT_LIST_HEAD(&person_one->list);//
		/*INIT_LIST_HEAD가 struct list head *를 가져와서 초기화.

		static inline void INIT_LIST_HEAD(struct list_head *list)
		{
		list->next = list;
		list->prev = list;
		}*/

		list_add_tail(&person_one->list, &birthday_list);
		/*list에 멤버 삽입,head뒤에 새로운 data를 삽입
	
		static inline void list_add_tail(struct list_head *new, struct list_head *head)
		{
			__list_add(new, head->prev, head);
		}*/


		person_two = kmalloc(sizeof(*person_two), GFP_KERNEL);//person_two에 메모리 할당
		person_two->month = 1;
		person_two->day = 3;
		person_two->year = 2000;  //person_two의 생일은 2000년 1월 3일
		INIT_LIST_HEAD(&person_two->list);
		list_add_tail(&person_two->list, &birthday_list);
		//list에 person_two data 저장


		person_three = kmalloc(sizeof(*person_three), GFP_KERNEL);//person_three에 메모리 할당 
		person_three->month = 7;
		person_three->day = 7;
		person_three->year = 2000;  //person_three의 생일은 2000년 7월 7일
		INIT_LIST_HEAD(&person_three->list);
		list_add_tail(&person_three->list, &birthday_list);
		//list에 person_three data 저장


		person_four = kmalloc(sizeof(*person_four), GFP_KERNEL); //person_four에 메모리 할당
		person_four->month = 12;
		person_four->day = 28;
		person_four->year = 2000;//person_four의 생일은 2000년 12월 28일
		INIT_LIST_HEAD(&person_four->list);
		list_add_tail(&person_four->list, &birthday_list);
		//list에 person_four data 저장



	list_for_each_entry(ptr, &birthday_list, list) { // list의 노드들을 한바퀴 순환하면서, 각 노드들을 참조하는 포인터를 시작주소 지점 옮기는 것
          
         num=num+1;//person1,person2,,,를 출력하도록 num을 하나씩 더하기
         printk(KERN_INFO "Person%d Birthday information\nMonth: %d\nday: %d\nyear: %d\n",num, ptr->month, ptr->day, ptr->year); 
        //list 정보 출력    
            
            }

	/*
		pos : 리스트의 현재 위치를 저장할 포인터. 리스트의 구조체를 가리키는 포인터
		head: next, prev를 member로 가지는 구조체의 포인터
		member : list의 구조체에서 pre와 next를 가지고 있는 구조체 변수명
	
	#define list_for_each_entry(pos, head, member)				
		for (pos = list_first_entry(head, typeof(*pos), member);	
			 &pos->member != (head);					
			 pos = list_next_entry(pos, member))
	*/

     
       return 0;
}


void simple_exit(void) {//후에 module_exit을 통해 모듈이 종료되면 실행될 함수

		printk(KERN_INFO "Removing Module\n");//"Removing Module"을 출력
		num=0; //다시 person1,person2,,,출력을 위해 0으로 초기화

	list_for_each_entry_safe(ptr, next, &birthday_list, list) { 
        
		num= num+1;//1씩 증가
        printk(KERN_INFO "Removing Person%d %d %d %d\n",num, ptr->month, ptr->day, ptr->year);//삭제되는 list의 정보를 출력
		list_del(&ptr->list); //list로 부터 entry 항목을 삭제
		/*
		static inline void list_del(struct list_head *entry)
		{
			__list_del(entry->prev, entry->next);
			entry->next = LIST_POISON1;
			entry->prev = LIST_POISON2;
		}
		*/
        kfree(ptr);//메모리 반환 함수
          
           }

	/* list_for_each_entry와 마찬가리고 for문을 이용하여 리스트를 순회
		entry를 삭제할 때 사용

	     #define list_for_each_entry_safe(pos, n, head, member)			
	for (pos = list_first_entry(head, typeof(*pos), member),	
		n = list_next_entry(pos, member);			
	     &pos->member != (head); 					
	     pos = n, n = list_next_entry(n, member))
	
	*/






}


module_init(simple_init);// module_init으로 simple_init 함수 호출
module_exit( simple_exit );//module_exit으로 simpel_exit 함수 호출

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

