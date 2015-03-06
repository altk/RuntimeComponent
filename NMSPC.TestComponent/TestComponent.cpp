#include "pch.h"
#include "TestComponent.h"
#include <activation.h>
#include <new>

using namespace ABI::NMSPC::TestComponent;
using ABI::Windows::ApplicationModel::Background::IBackgroundTask;
using ABI::Windows::ApplicationModel::Background::IBackgroundTaskInstance;

ULONG m_objectsCount = 0;

class TestBackgroundTaskImpl sealed : public IBackgroundTask
{
	ULONG m_count;
public:
	TestBackgroundTaskImpl() throw()
		: m_count(1)
	{
		InterlockedIncrement(&m_objectsCount);
	}
	~TestBackgroundTaskImpl() throw()
	{
		InterlockedDecrement(&m_objectsCount);
	}

	STDMETHODIMP_(ULONG) AddRef() throw() override final
	{
		return InterlockedIncrement(&m_count);
	}
	STDMETHODIMP_(ULONG) Release() throw()  override
	{
		auto const count = InterlockedDecrement(&m_count);
		if (0 == count)
		{
			delete this;
		}
		return count;
	}

	STDMETHODIMP QueryInterface(const IID& riid, void** ppvObject) throw() override final
	{
		if (__uuidof(IUnknown) == riid || __uuidof(IInspectable) == riid || __uuidof(IBackgroundTask) == riid)
		{
			*ppvObject = this;
		}
		else
		{
			*ppvObject = nullptr;
			return E_NOINTERFACE;
		}
		static_cast<IInspectable*>(*ppvObject)->AddRef();
		return S_OK;
	}
	STDMETHODIMP GetIids(ULONG* iidCount, IID** iids) throw() override
	{
		*iids = static_cast<GUID*>(CoTaskMemAlloc(sizeof(GUID)));
		if (!*iids)
		{
			*iidCount = 0;
			return E_OUTOFMEMORY;
		}
		*iidCount = 1;
		(*iids)[0] = __uuidof(IBackgroundTask);
		return S_OK;
	}
	STDMETHODIMP GetRuntimeClassName(HSTRING* className) throw() override final
	{
		return WindowsCreateString(
			RuntimeClass_NMSPC_TestComponent_TestBackgroundTask,
			_countof(RuntimeClass_NMSPC_TestComponent_TestBackgroundTask),
			className);
	}
	STDMETHODIMP GetTrustLevel(TrustLevel* trustLevel) throw() override final
	{
		*trustLevel = BaseTrust;
		return S_OK;
	}
	STDMETHODIMP Run(IBackgroundTaskInstance*) throw() override final
	{
		OutputDebugStringW(L"Hello from background task\r\n");
		return S_OK;
	}
};

class TestBackgroundTaskFactory sealed : public IActivationFactory
{
	ULONG m_count;
public:
	TestBackgroundTaskFactory() throw()
		: m_count(1)
	{
		InterlockedIncrement(&m_objectsCount);
	}
	~TestBackgroundTaskFactory() throw()
	{
		InterlockedDecrement(&m_objectsCount);
	}

	STDMETHODIMP_(ULONG) AddRef() throw() override final
	{
		return InterlockedIncrement(&m_count);
	}
	STDMETHODIMP_(ULONG) Release() throw()  override
	{
		auto const count = InterlockedDecrement(&m_count);
		if (0 == count)
		{
			delete this;
		}
		return count;
	}

	STDMETHODIMP QueryInterface(const IID& riid, void** ppvObject) throw() override final
	{
		if (__uuidof(IUnknown) == riid || __uuidof(IInspectable) == riid || __uuidof(IActivationFactory) == riid)
		{
			*ppvObject = this;
		}
		else
		{
			*ppvObject = nullptr;
			return E_NOINTERFACE;
		}
		static_cast<IInspectable*>(*ppvObject)->AddRef();
		return S_OK;
	}
	STDMETHODIMP GetIids(ULONG* iidCount, IID** iids) throw() override final
	{
		*iids = static_cast<GUID*>(CoTaskMemAlloc(sizeof(GUID)));
		if (*iids)
		{
			*iidCount = 0;
			return E_OUTOFMEMORY;
		}
		*iidCount = 1;
		(*iids)[0] = __uuidof(IActivationFactory);
		return S_OK;
	}
	STDMETHODIMP GetRuntimeClassName(HSTRING*) throw() override final
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetTrustLevel(TrustLevel* trustLevel) throw() override final
	{
		*trustLevel = BaseTrust;
		return S_OK;
	}
	STDMETHODIMP ActivateInstance(IInspectable** instance) throw() override final
	{
		*instance = new (std::nothrow) TestBackgroundTaskImpl();
		return *instance ? S_OK : E_OUTOFMEMORY;
	}
};

HRESULT WINAPI DllGetActivationFactory(HSTRING activatableClassId, IActivationFactory **factory) throw()
{
	if (0 == wcscmp(RuntimeClass_NMSPC_TestComponent_TestBackgroundTask, WindowsGetStringRawBuffer(activatableClassId, nullptr)))
	{
		*factory = new (std::nothrow) TestBackgroundTaskFactory();
		return *factory ? S_OK : E_OUTOFMEMORY;
	}
	*factory = nullptr;
	return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT WINAPI DllCanUnloadNow() throw()
{
	return m_objectsCount ? S_FALSE : S_OK;
}