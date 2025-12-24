//////////////////////////////our code//////////////////////////
#include <linux/kernel.h>
#include<linux/sched.h>
#include <linux/list.h>


asmlinkage long sys_hello(void) {
    printk("Hello, World!\n");
    return 0;
}


asmlinkage long sys_set_weight(int weight){
    if(weight<0){
        return -EINVAL;
}
    current->weight= weight;
    return 0;
}


asmlinkage long sys_get_weight(void){
    return current->weight;
}


asmlinkage long sys_get_path_sum(pid_t target){

int sum ;
    struct task_struct *task;
struct task_struct *p;
    task = find_task_by_vpid(target);
    if (!task) {
        return -ECHILD;  // No such process
    }
     sum = task->weight;
  if (task == current) {
    //    sum+=current->weight;
        return sum;  // Successfully reached current, return the sum
    }
    p = task->parent;
    while(p && p!= current){
        if (p->pid==1) {
            return -ECHILD;  // Reached the init task
        }
        sum += p->weight;
        p = p->parent;
    }
    if (p == current) {
        sum+=current->weight;
        return sum;  // Successfully reached current, return the sum
    }
    return -ECHILD;

}


asmlinkage  pid_t sys_get_heaviest_sibling(void){

        //struct task_struct* sibling;
       // struct task_struct* parent;
	struct task_struct *sibling_task;
    	struct task_struct *parent_task;
        struct task_struct* current_task = current;
        struct task_struct *heaviest_sibling = current_task;
        int max_weight = current_task->weight;

        parent_task = current_task->parent;
        list_for_each_entry(sibling_task, &parent_task->children, sibling) {
            if (sibling_task->weight > max_weight ||
                (sibling_task->weight == max_weight && sibling_task->pid < heaviest_sibling->pid)) {
                heaviest_sibling = sibling_task;
                max_weight = sibling_task->weight;
            }
        }
        return heaviest_sibling->pid;

    }


