#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __TestComponent_h__
#define __TestComponent_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__cplusplus)
#if defined(__MIDL_USE_C_ENUM)
#define MIDL_ENUM enum
#else
#define MIDL_ENUM enum class
#endif
#endif


/* Forward Declarations */ 

/* header files for imported files */
#include "Windows.ApplicationModel.Background.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_TestComponent_0000_0000 */
/* [local] */ 

#ifdef __cplusplus
namespace ABI {
namespace NMSPC {
namespace TestComponent {
class TestBackgroundTask;
} /*TestComponent*/
} /*NMSPC*/
}
#endif

#ifndef RUNTIMECLASS_NMSPC_TestComponent_TestBackgroundTask_DEFINED
#define RUNTIMECLASS_NMSPC_TestComponent_TestBackgroundTask_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_NMSPC_TestComponent_TestBackgroundTask[] = L"NMSPC.TestComponent.TestBackgroundTask";
#endif


/* interface __MIDL_itf_TestComponent_0000_0000 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_TestComponent_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_TestComponent_0000_0000_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


