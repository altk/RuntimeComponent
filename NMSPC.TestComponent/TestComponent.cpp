#include "pch.h"
#include "TestComponent.h"

//Импортируем пространство имён нашего компонента
using namespace ABI::NMSPC::TestComponent;
//Импортируем интерфейсы из пространства имён ABI::Windows::ApplicationModel::Background
using ABI::Windows::ApplicationModel::Background::IBackgroundTask;
using ABI::Windows::ApplicationModel::Background::IBackgroundTaskInstance;

//Переменная для хранения числа текущих экземпляров объектов библиотеки.
//Значение данной переменной будем изменять в конструкторах объектов реализации 
//фоновой задачи и фабрики.
ULONG m_objectsCount = 0;

//Класс реализации фоновой задачи. 
//Реализует единственный "интерфейс" IBackgroundTask
class ABI::NMSPC::TestComponent::TestBackgroundTask sealed : public IBackgroundTask
{
	//Переменная для подсчёта ссылок на текущий объект
	ULONG m_count;
public:
	TestBackgroundTask() throw()
		: m_count(1)
	{
		//Увеличиваем общее количество экземпляров объектов библиотеки
		InterlockedIncrement(&m_objectsCount);
	}
	~TestBackgroundTask() throw()
	{
		//Уменьшаем общее количество экземпляров объектов библиотеки
		InterlockedDecrement(&m_objectsCount);
	}

	//Реализация COM метода увеличения счетчика ссылок на объект
	STDMETHODIMP_(ULONG) AddRef() throw() override final
	{
		//Увеличиваем количество ссылок на объект и возвращаем результат
		return InterlockedIncrement(&m_count);
	}
	//Реализация COM метода уменьшения счетчика ссылок на объект
	STDMETHODIMP_(ULONG) Release() throw()  override
	{
		//Получаем результат после уменьшения количества ссылок на объект
		auto const count = InterlockedDecrement(&m_count);
		//Если количество стало равным нулю
		if (0 == count)
		{
			//Уничтожаем объект
			delete this;
		}
		//Возвращаем количество ссылок
		return count;
	}
	//Реализация COM метода опроса на имплементацию заданного интерфейса
	STDMETHODIMP QueryInterface(const IID& riid, void** ppvObject) throw() override final
	{
		//Проверка запроса на равенство реализуемым интерфейсам
		//Проверяются три интерфеса так как IBackgroundTask наследует IInspectable
		//А IInspectable наследует IUnknown
		if (__uuidof(IUnknown) == riid || __uuidof(IInspectable) == riid || __uuidof(IBackgroundTask) == riid)
		{
			*ppvObject = this;
		}
		else
		{
			*ppvObject = nullptr;
			//Возвращаем константу означающую, что данный интерфейс не поддерживается
			return E_NOINTERFACE;
		}
		//Увеличиваем количество ссылок на объект
		//Это стандартное соглашение
		static_cast<IInspectable*>(*ppvObject)->AddRef();
		return S_OK;
	}

	//Реализация WINRT метода получения массива идентификаторов реализуемых интерфейсов
	STDMETHODIMP GetIids(ULONG* iidCount, IID** iids) throw() override
	{
		//Выделяем память для одного GUID, т.к. наш класс реализует только один интерфейс
		//Используетс функция CoTaskMemAlloc, т.к. вызывающий объект может очистить массив с помощью CoTaskMemFree
		*iids = static_cast<GUID*>(CoTaskMemAlloc(sizeof(GUID)));
		//Если указатель NULL 
		if (!*iids)
		{
			//Возвращаем ошибку отсутствия памяти
			return E_OUTOFMEMORY;
		}
		//Устанавливаем количество реализуемых интерфейсов
		*iidCount = 1;
		//Инициализируем значение идентификатором интерфейса IBackgroundTask
		(*iids)[0] = __uuidof(IBackgroundTask);
		return S_OK;
	}
	//Реализация WINRT метода получения имени Runtime класса
	STDMETHODIMP GetRuntimeClassName(HSTRING* className) throw() override final
	{
		//Проверяем результат возвращаемой функции
		//Документация рекомендует возвращает E_OUTOFMEMORY в любом случае неудачи 
		//Если это не фабрика или статический интерфейс
		if (S_OK != WindowsCreateString(
			RuntimeClass_NMSPC_TestComponent_TestBackgroundTask,
			_countof(RuntimeClass_NMSPC_TestComponent_TestBackgroundTask),
			className))
		{
			return E_OUTOFMEMORY;
		}
		return S_OK;
	}
	//Реализация WINRT метода получения TrustLevel объекта
	STDMETHODIMP GetTrustLevel(TrustLevel* trustLevel) throw() override final
	{
		*trustLevel = BaseTrust;
		return S_OK;
	}

	//Реализация IBackgroundTask метода запуска фоновой задачи
	STDMETHODIMP Run(IBackgroundTaskInstance* task_instance) throw() override final
	{
		//Просто пишем строку в отладочное окно
		OutputDebugStringW(L"Hello from background task.\r\n");
		return S_OK;
	}
};

//Класс реализации фабрики фоновых задач.
class TestBackgroundTaskFactory sealed : public IActivationFactory
{
	//Переменная для подсчёта ссылок на текущий объект
	ULONG m_count;
public:
	TestBackgroundTaskFactory() throw()
		: m_count(1)
	{
		//Увеличиваем общее количество экземпляров объектов библиотеки
		InterlockedIncrement(&m_objectsCount);
	}
	~TestBackgroundTaskFactory() throw()
	{
		//Уменьшаем общее количество экземпляров объектов библиотеки
		InterlockedDecrement(&m_objectsCount);
	}

	//Реализация COM метода увеличения счетчика ссылок на объект
	STDMETHODIMP_(ULONG) AddRef() throw() override final
	{
		//Увеличиваем количество ссылок на объект и возвращаем результат
		return InterlockedIncrement(&m_count);
	}
	//Реализация COM метода уменьшения счетчика ссылок на объект
	STDMETHODIMP_(ULONG) Release() throw()  override
	{
		//Получаем результат после уменьшения количества ссылок на объект
		auto const count = InterlockedDecrement(&m_count);
		//Если количество стало равным нулю
		if (0 == count)
		{
			//Уничтожаем объект
			delete this;
		}
		//Возвращаем количество ссылок
		return count;
	}
	//Реализация COM метода опроса на имплементацию заданного интерфейса
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

	//Реализация WINRT метода получения массива идентификаторов реализуемых интерфейсов
	STDMETHODIMP GetIids(ULONG* iidCount, IID** iids) throw() override final
	{
		//Выделяем память для одного GUID, т.к. наш класс реализует только один интерфейс
		//Используетс функция CoTaskMemAlloc, т.к. вызывающий объект может очистить массив с помощью CoTaskMemFree
		*iids = static_cast<GUID*>(CoTaskMemAlloc(sizeof(GUID)));
		//Если указатель NULL 
		if (*iids)
		{
			//Возвращаем ошибку отсутствия памяти
			return E_OUTOFMEMORY;
		}
		//Устанавливаем количество реализуемых интерфейсов
		*iidCount = 1;
		//Инициализируем значение идентификатором интерфейса IBackgroundTask
		(*iids)[0] = __uuidof(IActivationFactory);
		return S_OK;
	}
	//Реализация WINRT метода получения имени Runtime класса
	STDMETHODIMP GetRuntimeClassName(HSTRING*) throw() override final
	{
		//Возвращаем данную константу, т.к. вызовается метод фабрики
		return E_ILLEGAL_METHOD_CALL;
	}
	//Реализация WINRT метода получения TrustLevel объекта
	STDMETHODIMP GetTrustLevel(TrustLevel* trustLevel) throw() override final
	{
		*trustLevel = BaseTrust;
		return S_OK;
	}

	//Реализация IActivationFactory метода инстанциирования экземпляра
	STDMETHODIMP ActivateInstance(IInspectable** instance) throw() override final
	{
		//Если указатель равено null
		if (nullptr == instance)
		{
			//Возвращаем ошибку
			return E_INVALIDARG;
		}
		//Создаём объект 
		//При этом указываем признак того, что не надо генерировать исключение
		*instance = new (std::nothrow) TestBackgroundTask();
		//Возвращаем результат в зависимости от успешности создания объекта
		return *instance ? S_OK : E_OUTOFMEMORY;
	}
};

//Реализация экспортируемой функции получения фабрики объектов класса, имеющего идентификатор activatableClassId
HRESULT WINAPI DllGetActivationFactory(HSTRING activatableClassId, IActivationFactory **factory) throw()
{
	//Проверяем идентфикатор класса и указатель на фабрику
	if (WindowsIsStringEmpty(activatableClassId) || nullptr == factory)
	{
		//Если идентификатор не задан или указатель нулевой
		return E_INVALIDARG;
	}
	//Проверяем на равенство строки идентификатора класса и определенного нами класса
	if (0 == wcscmp(RuntimeClass_NMSPC_TestComponent_TestBackgroundTask, WindowsGetStringRawBuffer(activatableClassId, nullptr)))
	{
		//Инициализируем указатель
		*factory = new (std::nothrow) TestBackgroundTaskFactory();
		return *factory ? S_OK : E_OUTOFMEMORY;
	}
	*factory = nullptr;
	return E_NOINTERFACE;
}

//Реализация экспортируемой функции опроса возможности выгрузки библиотеки
HRESULT WINAPI DllCanUnloadNow() throw()
{
	//Возвращаем признак в зависимости от количества текущий экземпляров
	return m_objectsCount ? S_FALSE : S_OK;
}