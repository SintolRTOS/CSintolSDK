#ifndef _SDKMANAGER_H_
#define _SDKMANAGER_H_

#include <ScopeLock.h>
#include <algorithm>
#include <cstring>
#include <iterator>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <RTI/FederateAmbassador.h>
#include <RTI/RTIambassadorFactory.h>
#include <RTI/RTIambassador.h>
#include <RTI/LogicalTime.h>
#include <RTI/LogicalTimeInterval.h>
#include <RTI/LogicalTimeFactory.h>
#include <RTI/RangeBounds.h>

namespace SintolRTI {
	class SintolRTI_LOCAL SDKManager {
	public:
		//获取单例实例
		static SDKManager* GetInstance();
		static void Clear();
		//初始化SDK核心
		bool InitSDK(
			rti1516::FederateAmbassador& federaambassador,
			std::wstring const & federationExecutionName,
			std::wstring const & federateType,
			std::wstring const & fullPathNameToTheFDDfile,
			std::wstring const connectaddress,
			std::wstring const localtimefactory = L"HLAinteger64Time");
		virtual ~SDKManager();
		//插入同步联邦到集合
		bool insertSyncfedearetionSet(std::wstring label);
		//通知同步节点到达
		void synchronizationPointAchieved(std::wstring label);
		//获取对象类的句柄
		rti1516::ObjectClassHandle getObjectClassHandle(std::wstring name);
		//获取类对象属性
		rti1516::AttributeHandle getAttributeHandle
		(rti1516::ObjectClassHandle rti1516ObjectClassHandle,
			std::wstring const & attributeName);
		//发布类对象属性，本地计算层的属性可以同步到联邦模型中
		void publishObjectClassAttributes(rti1516::ObjectClassHandle theClass,
			rti1516::AttributeHandleSet const & rti1516AttributeHandleSet);
		//从联邦模型中订阅对象的属性值
		void subscribeObjectClassAttributes(rti1516::ObjectClassHandle theClass,
			rti1516::AttributeHandleSet const & attributeList, bool active = true);
		//注册发布对象实例，标志在当前计算层中订阅了分布式联邦模型中的一个运算实体
		rti1516::ObjectInstanceHandle registerObjectInstance
		(rti1516::ObjectClassHandle rti1516ObjectClassHandle);
		//同步数据，同步当前计算层中计算实体的数据到分布式联邦模型中
		void updateAttributeValues(rti1516::ObjectInstanceHandle rti1516ObjectInstanceHandle,
			const rti1516::AttributeHandleValueMap& rti1516AttributeValues,
			const rti1516::VariableLengthData& rti1516Tag);
		//销毁对象实例，销毁当前计算层中的某个计算实体
		void deleteObjectInstance(rti1516::ObjectInstanceHandle rti1516ObjectInstanceHandle,
			const rti1516::VariableLengthData& rti1516Tag);
		//取消发布当前计算层中发布的模型类
		void unpublishObjectClass(rti1516::ObjectClassHandle rti1516ObjectClassHandle);
		//取消发布当前计算类中发布的模型属性
		void unpublishObjectClassAttributes
		(rti1516::ObjectClassHandle rti1516ObjectClassHandle,
			rti1516::AttributeHandleSet const & rti1516AttributeHandleSet);
		//取消从联邦模型中订阅的对象类，不再同步联邦中的属性
		void unsubscribeObjectClass(rti1516::ObjectClassHandle rti1516ObjectClassHandle);
		//获取RTI联邦代理，获取反馈接口
		std::auto_ptr<rti1516::RTIambassador> getRTIambassador()
		{
			return ambassador;
		}
		//获取当前联邦类型
		std::wstring getFedarateType()
		{
			return _federateType;
		}
		//获取联邦名称
		std::wstring getFaderateName()
		{
			return _federateName;
		}
		//获取联邦句柄
		rti1516::FederateHandle getFedarateHandle()
		{
			return _federateHandle;
		}
		//获取联邦中的成员集合
		const std::set<std::wstring>& getFedarateSet()
		{
			return _federateSet;
		}
		//获取联邦参数列表
		const std::vector<std::wstring>& getFedrateList()
		{
			return _federateList;
		}
		//停止关闭本地核心
		bool StopSDK();
		//同步核心接口，保持本地心跳，激活本地计算层的功能
		void Update(double approximateMinimumTimeInSeconds);
	private:
		//单例计算实例
		static SDKManager* _instance;
		//等待分布式联邦中的其他联邦成员
		bool waitForAllFederates();
		//SDK是否启动的标志
		bool _isstarted;
		//加入联邦成功接口
		bool execJoined();
		//联邦句柄对象
		rti1516::FederateHandle _federateHandle;
		//联邦类型
		std::wstring _federateType;
		//联邦名称
		std::wstring _federateName;
		//联邦代理反馈接口
		std::auto_ptr<rti1516::RTIambassador> ambassador;
		//联邦实例
		std::set<std::wstring> _federateSet;
		//联邦参数列表
		std::vector<std::wstring> _federateList;
		//联邦同步节点计数
		int _synchronized = 0;
		//多线程互斥体
		static Mutex _mutex;
		//单例对象不对外开放构造函数
		SDKManager();
	};
}

#endif // !_SDKMANAGER_H_

