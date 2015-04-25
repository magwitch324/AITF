#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include "AttackManager.hpp"
#include "VictimManager.hpp"

/* nf_hook_ops format:
 * 		hook: the function to be called by the hook. Must return (NF_DROP, NF_ACCEPT, NF_STOLEN, NF_QUEUE, NF_REPEAT, NF_STOP)
 * 		hooknum: the type of hook (NF_IP_PRE_ROUTING, NF_IP_LOCAL_IN, NF_IP_FORWARD, NF_IP_POST_ROUTING, NF_IP_LOCAL_OUT)
 * 		pf: the protocol family (NFPROTO_UNSPEC, NFPROTO_IPV4, NFPROTO_ARP, NFPROTO_BRIDE, NFPROTO_IPV6, NFPROTO_DECNET)
 * 		priority: the priority of the function in descending priority (NF_IP_PRI_FIRST, NF_IP_PRI_CONNTRACK_DEFRAG, NF_IP_PRI_RAW, NF_IP_PRI_SELINUX_FIRST, NF_IP_PRI_CONNTRACK, NF_IP_PRI_MANGLE, NF_IP_PRI_NAT_DST, NF_IP_PRI_FILTER, NF_IP_PRI_SECURITY, NF_IP_PRI_NAT_SRC, NF_IP_PRI_SELINUX_LAST, NF_IP_PRI_CONNTRACK_CONFIRM, NF_IP_PRI_LAST)
 *
 */
static struct nf_hook_ops localOutHookStruct;
static struct nf_hook_ops localInHookStruct;

VictimManager * victimmanager;
AttackManager * attackmanager;


static bool isAITFCommunication( unsigned int hooknum,
		struct sk_buff *skb,
		const struct net_device *in,
		const struct net_device *out ) {

	return false;
}

static unsigned int localOutHookFunc( unsigned int hooknum,
		struct sk_buff *skb,
		const struct net_device *in,
		const struct net_device *out,
		int (*okfn)(struct sk_buff *) ) {

	return attackmanager->performFilter();
}

static unsigned int localInHookStruct( unsigned int hooknum,
		struct sk_buff *skb,
		const struct net_device *in,
		const struct net_device *out,
		int (*okfn)(struct sk_buff *) ) {

	if ( isAITFCommunication(hooknum, skb, in, out) ) {
		attackmanager->aitfCommunication();
		return NF_DROP;
	} else {
		victimmanager->performStatistics();
	}

	return NF_ACCEPT;
}

static int  init_main(void) {

	victimmanager = new VictimManager;
	attackmanager = new AttackManager;


	localOutHookStruct.hook     = localOutHookFunc;
	localOutHookStruct.hooknum  = NF_INET_LOCAL_OUT;
	localOutHookStruct.pf       = NFPROTO_IPV4;
	localOutHookStruct.priority = NF_IP_PRI_FIRST;

	localInHookStruct.hook     = localOutHookFunc;
	localInHookStruct.hooknum  = NF_INET_LOCAL_IN;
	localInHookStruct.pf       = NFPROTO_IPV4;
	localInHookStruct.priority = NF_IP_PRI_FIRST;

	nf_register_hook(&localOutHookStruct);
	nf_register_hook(&localInHookStruct);

	return 0;
}

static void cleanup_main(void) {
	nf_unregister_hook(&localOutHookStruct);
	nf_unregister_hook(&localInHookStruct);
}

module_init(init_main);
module_exit(cleanup_main);
