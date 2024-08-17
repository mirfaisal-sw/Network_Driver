
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/kernel.h>
#include <linux/etherdevice.h>


struct net_device *virtual_nic;


int virtual_nic_open(struct net_device *dev) {
        printk("virtual_nic_open called\n");
        return 0;
}

int virtual_nic_release(struct net_device *dev) {
        printk("virtual_nic_release called\n");
        netif_stop_queue(dev);
        return 0;
}



int virtual_nic_xmit(struct sk_buff *skb, struct net_device *dev) {
        printk("dummy xmit function called...\n");
        dev_kfree_skb(skb);
        return 0;
}

int virtual_nic_init(struct net_device *dev) {
        printk("virtual_nic device initialized\n");
        return 0;
};

const struct net_device_ops my_netdev_ops = {
        .ndo_init = virtual_nic_init,
        .ndo_open = virtual_nic_open,
        .ndo_stop = virtual_nic_release,
        .ndo_start_xmit = virtual_nic_xmit,
};

static void virtual_nic_setup(struct net_device *net){
        net->netdev_ops = &my_netdev_ops;
}

int virtual_nic_init_module(void) {

        int result;

        virtual_nic = alloc_netdev(0, "virtnC%d", NET_NAME_UNKNOWN, virtual_nic_setup);

        if((result = register_netdev(virtual_nic))) {

                printk("virtual_nic: Error %d initalizing card ...", result);

                return result;

        }

        return 0;

}

void virtual_nic_cleanup (void)
{
        printk("<0> Cleaning Up the Module\n");
        unregister_netdev(virtual_nic);
}

module_init(virtual_nic_init_module);
module_exit(virtual_nic_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mir Faisal <mirfaisalfos@gmail.com>");


