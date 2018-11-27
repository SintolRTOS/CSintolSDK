#include "SDKManager.h"

namespace SintolRTI {

	SDKManager* SDKManager::_instance = NULL;
    Mutex SDKManager::_mutex;
	SDKManager* SDKManager::GetInstance()
	{
		ScopeLock scopeLock(_mutex);
		if (_instance == NULL)
			_instance = new	SDKManager();
		return _instance;
	}

	void SDKManager::Clear()
	{
		if (_instance != NULL)
		{
			delete _instance;
			_instance = NULL;
		}
	}

	bool SDKManager::waitForAllFederates()
	{
		_synchronized = 0;

		// FIXME need a test for concurrent announces
		try {
			ambassador->registerFederationSynchronizationPoint(_federateType, rti1516::VariableLengthData());
		}
		catch (const rti1516::Exception& e) {
			std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
			return false;
		}
		catch (...) {
			std::wcout << L"Unknown Exception!" << std::endl;
			return false;
		}
		ambassador->evokeCallback(10.0f);
		return true;
	}

	bool SDKManager::StopSDK()
	{
		// and now resign must work
		try {
			ambassador->resignFederationExecution(rti1516::CANCEL_THEN_DELETE_THEN_DIVEST);
		}
		catch (const rti1516::Exception& e) {
			std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
			return false;
		}
		catch (...) {
			std::wcout << L"Unknown Exception!" << std::endl;
			return false;
		}

		// destroy, must work once
		try {
			ambassador->destroyFederationExecution(_federateName);

		}
		catch (const rti1516::FederatesCurrentlyJoined&) {
			// Can happen in this test
			// Other threads just might have still joined.
		}
		catch (const rti1516::FederationExecutionDoesNotExist&) {
			// Can happen in this test
			// Other threads might have been faster
		}
		catch (const rti1516::Exception& e) {
			std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
			return false;
		}
		catch (...) {
			std::wcout << L"Unknown Exception!" << std::endl;
			return false;
		}

		_synchronized = 0;
		_isstarted = false;
		return true;
	}

	void SDKManager::Update(double approximateMinimumTimeInSeconds)
	{
		if (_isstarted)
			ambassador->evokeCallback(approximateMinimumTimeInSeconds);

	}

	bool SDKManager::execJoined()
	{
		return true;
	}

	bool SDKManager::InitSDK(
		rti1516::FederateAmbassador& federaambassador,
		std::wstring const & federationExecutionName,
		std::wstring const & federateType,
		std::wstring const & fullPathNameToTheFDDfile, 
		std::wstring const connectaddress,
		std::wstring const localtimefactory /*= L"HLAinteger64Time"*/)
	{
		std::wstring introduce =
L"|----------------------------------------------------------------------------------------------------------|\r\n\
|                                        Welcome to SintolSDK                                              |\r\n\
|                                      It's created by wangjingyi                                          |\r\n\
|                                      Email:langkexiaoyi@gmail.com                                        |\r\n\
|                                          Wechat:18513285865                                              |\r\n\
|                        It can help you create large-scale distributed computing                          |\r\n\
|                   It is the basic calculation of distributed cluster artificial intelligence             |\r\n\
|                              You can use sintolsdk to link sintolrtosnode                                |\r\n\
|----------------------------------------------------------------------------------------------------------|\r\n";


		std::wcout << introduce << std::endl;
		_federateName = federationExecutionName;
		_federateType = federateType;
		rti1516::RTIambassadorFactory rtifactory;
		std::vector<std::wstring> args;
		_federateList.clear();
		_federateList.push_back(_federateType);
		_federateSet.clear();
		_federateSet.insert(_federateType);
		args.push_back(L"protocol=rti");
		args.push_back(std::wstring(L"address=") + connectaddress);
		ambassador = rtifactory.createRTIambassador(args);

		// create, must work once
		try {
			ambassador->createFederationExecution(_federateName, fullPathNameToTheFDDfile, localtimefactory);
			std::wcout << L"createFederationExecution sucessful,federation:" << _federateName << ",fddfile:" << fullPathNameToTheFDDfile << std::endl;
		}
		catch (const rti1516::FederationExecutionAlreadyExists&) {
			// Can happen in this test
		}
		catch (const rti1516::Exception& e) {
			std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
			return false;
		}
		catch (...) {
			std::wcout << L"Unknown Exception!" << std::endl;
			return false;
		}
		// join must work
		try {
			_federateHandle = ambassador->joinFederationExecution(_federateType, _federateName, federaambassador);
			std::wcout << L"joinFederationExecution sucessful,getFederateType:" << _federateType << ",getFederationExecution:" << federationExecutionName << std::endl;
		}
		catch (const rti1516::Exception& e) {
			std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
			return false;
		}
		catch (...) {
			std::wcout << L"Unknown Exception!" << std::endl;
			return false;
		}

		if (!waitForAllFederates())
			return false;

		_isstarted = true;

		if (!execJoined())
			return false;

		return true;
	}

	SDKManager::~SDKManager()
	{
		if (_isstarted)
			StopSDK();
	}

	bool SDKManager::insertSyncfedearetionSet(std::wstring label)
	{
		if (_federateSet.find(label) == _federateSet.end())
		{
			_federateSet.insert(label);
			return true;
		}
		return false;
	}

	void SDKManager::synchronizationPointAchieved(std::wstring label)
	{
		if (_federateSet.find(label) == _federateSet.end())
		{
			std::wcout << L"start synchronizationPointAchieved:" << label << std::endl;
			ambassador->synchronizationPointAchieved(label);
		}
	}

	rti1516::ObjectClassHandle SDKManager::getObjectClassHandle(std::wstring name)
	{
		return ambassador->getObjectClassHandle(name);
	}

	rti1516::AttributeHandle SDKManager::getAttributeHandle(rti1516::ObjectClassHandle rti1516ObjectClassHandle, std::wstring const & attributeName)
	{
		return ambassador->getAttributeHandle(rti1516ObjectClassHandle, attributeName);
	}

	void SDKManager::publishObjectClassAttributes(rti1516::ObjectClassHandle theClass, rti1516::AttributeHandleSet const & rti1516AttributeHandleSet)
	{
		ambassador->publishObjectClassAttributes(theClass, rti1516AttributeHandleSet);
	}

	void SDKManager::subscribeObjectClassAttributes(rti1516::ObjectClassHandle theClass, rti1516::AttributeHandleSet const & attributeList, bool active /*= true*/)
	{
		ambassador->subscribeObjectClassAttributes(theClass, attributeList, active);
	}

	rti1516::ObjectInstanceHandle SDKManager::registerObjectInstance(rti1516::ObjectClassHandle rti1516ObjectClassHandle)
	{
		return ambassador->registerObjectInstance(rti1516ObjectClassHandle);
	}

	void SDKManager::updateAttributeValues(rti1516::ObjectInstanceHandle rti1516ObjectInstanceHandle, const rti1516::AttributeHandleValueMap& rti1516AttributeValues, const rti1516::VariableLengthData& rti1516Tag)
	{
		ambassador->updateAttributeValues(rti1516ObjectInstanceHandle, rti1516AttributeValues, rti1516Tag);
	}

	void SDKManager::deleteObjectInstance(rti1516::ObjectInstanceHandle rti1516ObjectInstanceHandle, const rti1516::VariableLengthData& rti1516Tag)
	{
		ambassador->deleteObjectInstance(rti1516ObjectInstanceHandle, rti1516Tag);
	}

	void SDKManager::unpublishObjectClass(rti1516::ObjectClassHandle rti1516ObjectClassHandle)
	{
		ambassador->unpublishObjectClass(rti1516ObjectClassHandle);
	}

	void SDKManager::unpublishObjectClassAttributes(rti1516::ObjectClassHandle rti1516ObjectClassHandle, rti1516::AttributeHandleSet const & rti1516AttributeHandleSet)
	{
		ambassador->unpublishObjectClassAttributes(rti1516ObjectClassHandle, rti1516AttributeHandleSet);
	}

	void SDKManager::unsubscribeObjectClass(rti1516::ObjectClassHandle rti1516ObjectClassHandle)
	{
		ambassador->unsubscribeObjectClass(rti1516ObjectClassHandle);
	}

	SDKManager::SDKManager() :
		_synchronized(0),
		_isstarted(false)
	{

	}
}