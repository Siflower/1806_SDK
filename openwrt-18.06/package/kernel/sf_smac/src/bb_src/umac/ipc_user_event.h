/*
 * =====================================================================================
 *
 *       Filename:  ipc_user_event.h
 *
 *    Description:  define user event of ipc message
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower
 *
 * =====================================================================================
 */
#include "ipc_shared.h"

/*  IPC USER EVENT BIT set in ipc_shared_env_tag.user_data */
// event when lmac request deep debug enable
#define IPC_USER_EVENT_DEEP_DEBUG_SET (1 << 0)

__INLINE void set_ipc_event_user_bit(struct ipc_shared_env_tag *env, uint32_t event)
{
    env->user_data |= event;
}

__INLINE void clear_ipc_event_user_bit(struct ipc_shared_env_tag *env, uint32_t event)
{
    env->user_data &= (~event);
}

__INLINE void wait_ipc_event_user_bit(struct ipc_shared_env_tag *env, uint32_t event)
{
    while (env->user_data & event)
        ;
}

__INLINE void set_ipc_event_user_reply(struct ipc_shared_env_tag *env, uint32_t reply)
{
    env->user_reply = reply;
}
