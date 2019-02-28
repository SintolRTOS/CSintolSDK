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
		//��ȡ����ʵ��
		static SDKManager* GetInstance();
		static void Clear();
		//��ʼ��SDK����
		bool InitSDK(
			rti1516::FederateAmbassador& federaambassador,
			std::wstring const & federationExecutionName,
			std::wstring const & federateType,
			std::wstring const & fullPathNameToTheFDDfile,
			std::wstring const connectaddress,
			std::wstring const localtimefactory = L"HLAinteger64Time");
		virtual ~SDKManager();
		//����ͬ���������
		bool insertSyncfedearetionSet(std::wstring label);
		//֪ͨͬ���ڵ㵽��
		void synchronizationPointAchieved(std::wstring label);
		//��ȡ������ľ��
		rti1516::ObjectClassHandle getObjectClassHandle(std::wstring name);
		//��ȡ���������
		rti1516::AttributeHandle getAttributeHandle
		(rti1516::ObjectClassHandle rti1516ObjectClassHandle,
			std::wstring const & attributeName);
		//������������ԣ����ؼ��������Կ���ͬ��������ģ����
		void publishObjectClassAttributes(rti1516::ObjectClassHandle theClass,
			rti1516::AttributeHandleSet const & rti1516AttributeHandleSet);
		//������ģ���ж��Ķ��������ֵ
		void subscribeObjectClassAttributes(rti1516::ObjectClassHandle theClass,
			rti1516::AttributeHandleSet const & attributeList, bool active = true);
		//ע�ᷢ������ʵ������־�ڵ�ǰ������ж����˷ֲ�ʽ����ģ���е�һ������ʵ��
		rti1516::ObjectInstanceHandle registerObjectInstance
		(rti1516::ObjectClassHandle rti1516ObjectClassHandle);
		//ͬ�����ݣ�ͬ����ǰ������м���ʵ������ݵ��ֲ�ʽ����ģ����
		void updateAttributeValues(rti1516::ObjectInstanceHandle rti1516ObjectInstanceHandle,
			const rti1516::AttributeHandleValueMap& rti1516AttributeValues,
			const rti1516::VariableLengthData& rti1516Tag);
		//���ٶ���ʵ�������ٵ�ǰ������е�ĳ������ʵ��
		void deleteObjectInstance(rti1516::ObjectInstanceHandle rti1516ObjectInstanceHandle,
			const rti1516::VariableLengthData& rti1516Tag);
		//ȡ��������ǰ������з�����ģ����
		void unpublishObjectClass(rti1516::ObjectClassHandle rti1516ObjectClassHandle);
		//ȡ��������ǰ�������з�����ģ������
		void unpublishObjectClassAttributes
		(rti1516::ObjectClassHandle rti1516ObjectClassHandle,
			rti1516::AttributeHandleSet const & rti1516AttributeHandleSet);
		//ȡ��������ģ���ж��ĵĶ����࣬����ͬ�������е�����
		void unsubscribeObjectClass(rti1516::ObjectClassHandle rti1516ObjectClassHandle);
		//��ȡRTI���������ȡ�����ӿ�
		std::auto_ptr<rti1516::RTIambassador> getRTIambassador()
		{
			return ambassador;
		}
		//��ȡ��ǰ��������
		std::wstring getFedarateType()
		{
			return _federateType;
		}
		//��ȡ��������
		std::wstring getFaderateName()
		{
			return _federateName;
		}
		//��ȡ������
		rti1516::FederateHandle getFedarateHandle()
		{
			return _federateHandle;
		}
		//��ȡ�����еĳ�Ա����
		const std::set<std::wstring>& getFedarateSet()
		{
			return _federateSet;
		}
		//��ȡ��������б�
		const std::vector<std::wstring>& getFedrateList()
		{
			return _federateList;
		}
		//ֹͣ�رձ��غ���
		bool StopSDK();
		//ͬ�����Ľӿڣ����ֱ�������������ؼ����Ĺ���
		void Update(double approximateMinimumTimeInSeconds);
	private:
		//��������ʵ��
		static SDKManager* _instance;
		//�ȴ��ֲ�ʽ�����е����������Ա
		bool waitForAllFederates();
		//SDK�Ƿ������ı�־
		bool _isstarted;
		//��������ɹ��ӿ�
		bool execJoined();
		//����������
		rti1516::FederateHandle _federateHandle;
		//��������
		std::wstring _federateType;
		//��������
		std::wstring _federateName;
		//����������ӿ�
		std::auto_ptr<rti1516::RTIambassador> ambassador;
		//����ʵ��
		std::set<std::wstring> _federateSet;
		//��������б�
		std::vector<std::wstring> _federateList;
		//����ͬ���ڵ����
		int _synchronized = 0;
		//���̻߳�����
		static Mutex _mutex;
		//�������󲻶��⿪�Ź��캯��
		SDKManager();
	};
}

#endif // !_SDKMANAGER_H_

