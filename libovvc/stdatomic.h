/* Minimal stdatomic compatibility for compilers/systems missing <stdatomic.h>
 * This header provides a lightweight fallback mapping to GCC's __atomic builtins.
 * It implements only the subset used by OpenVVC: atomic_uint, atomic_init,
 * atomic_fetch_add_explicit, atomic_load, atomic_store and memory order macros.
 */
#ifndef OPENVVC_STD_ATOMIC_COMPAT_H
#define OPENVVC_STD_ATOMIC_COMPAT_H

#include <stdint.h>

/* Memory order mapping */
#define memory_order_relaxed __ATOMIC_RELAXED
#define memory_order_acq_rel __ATOMIC_ACQ_REL
#define memory_order_release __ATOMIC_RELEASE
#define memory_order_acquire __ATOMIC_ACQUIRE
#define memory_order_seq_cst __ATOMIC_SEQ_CST

typedef unsigned int atomic_uint;

static inline void atomic_init(atomic_uint *obj, unsigned int val)
{
    __atomic_store_n(obj, val, memory_order_relaxed);
}

static inline unsigned int atomic_fetch_add_explicit(atomic_uint *obj, int arg, int order)
{
    return __atomic_fetch_add(obj, arg, order);
}

static inline unsigned int atomic_fetch_sub_explicit(atomic_uint *obj, int arg, int order)
{
    return __atomic_fetch_sub(obj, arg, order);
}

static inline unsigned int atomic_load(const atomic_uint *obj)
{
    return __atomic_load_n((atomic_uint *)obj, memory_order_relaxed);
}

static inline void atomic_store(atomic_uint *obj, unsigned int val)
{
    __atomic_store_n(obj, val, memory_order_relaxed);
}

static inline void atomic_store_explicit(atomic_uint *obj, unsigned int val, int order)
{
    __atomic_store_n(obj, val, order);
}

static inline unsigned int atomic_load_explicit(const atomic_uint *obj, int order)
{
    return __atomic_load_n((atomic_uint *)obj, order);
}

#define atomic_thread_fence(order) __atomic_thread_fence(order)

#endif /* OPENVVC_STD_ATOMIC_COMPAT_H */
