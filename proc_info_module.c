#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>
#include <linux/sched/signal.h>
#include <linux/mm.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rumeysa Coskun, Bulent Arda Yilmaz");
MODULE_DESCRIPTION("displaying process information");
MODULE_VERSION("0.01");

static int upid = 2487;         // User process ID
static char upname[256] = ""; // User process name

module_param(upid, int, 0);
MODULE_PARM_DESC(upid, "User process ID");

module_param_string(upname, upname, sizeof(upname), 0);
MODULE_PARM_DESC(upname, "User process name");

void log_process_by_pid(pid_t pid) {
    struct task_struct *task;

    unsigned long memory_usage = 0;
    
    // Find the task_struct corresponding to the given PID
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    
    if (task == NULL) {
        printk(KERN_INFO "Process with PID %d not found.\n", pid);
        return;
    }
    
    // Log the process details
    printk(KERN_INFO "Process details for PID %d:\n", pid);
    printk(KERN_INFO "Process Name: %s\n", task->comm);

    // source: https://stackoverflow.com/questions/33594124/why-is-the-process-state-in-task-struct-stored-as-type-long
    volatile long state = task->__state;
    if(state == 0) {
        printk(KERN_INFO "Process state: RUNNING\n");
    } else if (state == 1) {
        printk(KERN_INFO "Process state: INTERRUPTIBLE\n");
    } else if (state == 2) {
        printk(KERN_INFO "Process state: TASK_UNINTERRUPTIBLE\n");
    } else if (state == 4) {
        printk(KERN_INFO "Process state: __TASK_STOPPED\n");
    } else if (state == 8) {
        printk(KERN_INFO "Process state: __TASK_TRACED\n");
    } else if (state == 16) {
        printk(KERN_INFO "Process state: EXIT_DEAD\n");
    } else if (state == 32) {
        printk(KERN_INFO "Process state: EXIT_ZOMBIE\n");
    } else if (state == 64) {
        printk(KERN_INFO "Process state: TASK_DEAD\n");
    } else if (state == 128) {
        printk(KERN_INFO "Process state: TASK_WAKEKILL\n");
    } else if (state == 256) {
        printk(KERN_INFO "Process state: TASK_WAKING\n");
    } else if (state == 512) {
        printk(KERN_INFO "Process state: TASK_PARKED\n");
    } else if (state == 1024) {
        printk(KERN_INFO "Process state: TASK_NOLOAD\n");
    } else if (state == 2048) {
        printk(KERN_INFO "Process state: TASK_STATE_MAX\n");
    }
    
    printk(KERN_INFO "UID: %d\n", task_uid(task).val);
    printk(KERN_INFO "PPID: %d\n", task->real_parent->pid);
    printk(KERN_INFO "Path: /proc/%d\n", task->pid);
    if (task->__state == 1)
    {
        struct mm_struct *mm = task->mm;
        memory_usage = (get_mm_rss(mm) << (PAGE_SHIFT - 10));
    }
    printk(KERN_INFO "Memory Usage: %luKB\n", memory_usage);
    // Add more details as per your requirements
}

void log_process_by_name(const char *name) {
    struct task_struct *task;
    unsigned long memory_usage = 0;
    // Iterate through all running processes
    for_each_process(task) {
        // Check if the process name matches
        if (strcmp(task->comm, name) == 0) {
            // Log the process details
            printk(KERN_INFO "Process details for process name '%s':\n", name);
            printk(KERN_INFO "PID: %d\n", task->pid);
            // Add more details as per your requirements

            if (task->__state == 1)
            {
                struct mm_struct *mm = task->mm;
                memory_usage = get_mm_rss(mm) << (PAGE_SHIFT - 10);
            }
            printk(KERN_INFO "Memory Usage: %luKB\n", memory_usage);
            return;
        }
    }
    
    // If no process with the given name is found
    printk(KERN_INFO "Process with name '%s' not found.\n", name);
}


static int __init process_logger_init(void) {
    // Log process details based on PID
    log_process_by_pid(upid);
    
    // Log process details based on process name
    log_process_by_name(upname);
    
    return 0;
}

static void __exit process_logger_exit(void) {
    // Module cleanup logic
    printk(KERN_INFO "rmmod proc_info_module...\n");
}

module_init(process_logger_init);
module_exit(process_logger_exit);

