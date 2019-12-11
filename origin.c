#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>

struct my_node{
	struct list_head list;
	int data;
};

spinlock_t lock;
struct task_struct *thread1, *thread2;
struct list_head my_list;
struct my_node *tmp;
struct my_node *current_node;
int i;
u64 st1, et1, st2, et2, st3, et3;

static int insert(void *data){
	for(i=0; i < 100000; i++){
		struct my_node *new = kmalloc(sizeof(struct my_node), GFP_KERNEL);
		new->data = i;
		list_add(&new->list, &my_list);
	}
	do_exit(0);
}

static int search(void *data){
	list_for_each_entry(current_node, &my_list, list){
		spin_lock(&lock);
		spin_unlock(&lock);
	}
	do_exit(0);
}

static int delete(void *data){
	list_for_each_entry_safe(current_node, tmp, &my_list, list){
		list_del(&current_node->list);
		kfree(current_node);
	}
	//printk("all deleted\n");
	do_exit(0);
}

/*
static int delete(void *data){
	while(&current_node != (&my_list)){
		list_del(&current_node->list);
		kfree(current_node);
		current_node = tmp, tmp = list_next_entry(tmp, list);
	}
	//printk("all deleted\n");
	do_exit(0);
}*/

int __init hello_module_init(void)
{
	
	spin_lock_init(&lock);
	printk("Origin Init\n");
	INIT_LIST_HEAD(&my_list);
	st1 = ktime_get_ns();

	//i=0;
	thread1 = kthread_run(insert, NULL, "insert");

	et1 = ktime_get_ns();


	st2 = ktime_get_ns();

	thread1 = kthread_run(search, NULL, "search");	
	thread2 = kthread_run(search, NULL, "search");	

	et2 = ktime_get_ns();


	st3 = ktime_get_ns();
	/*	
	current_node = list_first_entry(&my_list, struct my_node, list);
	tmp = list_next_entry(current_node, list);
	*/
	thread1 = kthread_run(delete, NULL, "delete");

	et3 = ktime_get_ns();

	//printk("insert time : %llu ns \n", et1 - st1);
	printk("traverse time : %llu ns \n", et2 - st2);
	//printk("delete time : %llu ns \n", et3-st3);
	//printk("total time : %llu ns \n", et3-st3+et2-st2+et1-st1);
	return 0;
}

void __exit hello_module_cleanup(void)
{
	printk("Terminate\n\n");
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);
MODULE_LICENSE("GPL");
