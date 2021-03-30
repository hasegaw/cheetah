#include <linux/sched.h>
#include <linux/kthread.h>

MODULE_LICENSE("MIT");

#define NUM_CHEETAH_THREADS 4

struct task_struct *k[NUM_CHEETAH_THREADS];

static void kthread_main(void)
{
        u32 vmx_msr_low, vmx_msr_high;

        rdmsr(MSR_IA32_UCODE_REV, vmx_msr_low, vmx_msr_high);

        schedule();
}

static int kthread_func(void* arg)
{
    printk(KERN_INFO "start kthread\n");

    while (!kthread_should_stop()) {
        kthread_main();
    }

    printk(KERN_INFO "stop kthread\n");

    return 0;
}

static int __init cheetah_init(void)
{
        int i;

        for (i = 0; i < NUM_CHEETAH_THREADS; i++) {
                k[i] = kthread_run(kthread_func, NULL, "cheetah kthread");
                kthread_bind(k[i], i);
        }

        printk(KERN_INFO "cheetah loaded\n");

        return 0;
}

static void __exit cheetah_exit(void)
{
        int i;

        for (i = 0; i < NUM_CHEETAH_THREADS; i++) {
                kthread_stop(k[i]);
        }

        printk(KERN_INFO "cheetah unloaded\n");
}

module_init(cheetah_init);
module_exit(cheetah_exit);
