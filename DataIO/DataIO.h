//#pragma once ����������������������룺#ifndef
#ifndef __DATA____IO___
#define __DATA____IO___

#include <algorithm>//�㷨��
#include <vector> //��������
#include <fstream> //�ļ���
#include "afxwin.h"//windows�ؼ�

using namespace std;

//�ֶ�����ö�٣����͡�ʵ�͡�����ʱ���͡��ִ���
enum EField { eInt, eDouble, eDataTime, eString };
enum ESelect { eEqual, eGreater, eLess, eGreaterOrEqual, eLessOrEqual, eLike };
//�ֶΣ��ֶ��� + �ֶ���������
struct CField {
	CString sFieldName;
	EField eFieldType;
};


//���ݼ���ģ����
template <class T>
class CDataset {
private:
	CDataset& operator =(const CDataset&) { return *this; }//������ֵ����
	//�Ƿ�����ɸѡ����
	bool isSelData(T& data, CString sNameField, ESelect eselect, CString sVal);

protected:
	vector<CField> aFields;
	vector<T> aData;

	CString sFileName;
public:
	virtual ~CDataset() { aFields.clear(); aData.clear(); }

public:
	vector<T>& getData() {
		return aData;
	}

	vector<CField>& getFields() {
		return aFields;
	}

public:
	//��������ɾ�����ݵ��麯��
	virtual void deleteOneRow(int nRow);

	//�����ֶ�����ȡ�ֶ���Ϣ
	virtual const CField& getFieldByName(CString sFieldName);

	//���� ����Ϊ���ڱȽϵ� ����ָ��
	virtual void dataOrder(bool comp(const T& b1, const T& b2));

	//���ڼ�ֵ��������ָ��
	virtual T* getItemByKeyVal(CString sKeyField, int nKeyVal);

	//���ڼ�ֵ��������ָ��
	virtual bool deleteItemByKeyVal(CString sKeyField, int nKeyVal);

	//ɸѡsNameField��ֵ����eSelect��sVal��������,�õ������ݼ���ŵ� aDataset
	virtual void selectData(vector<T>& aDataset, CString sNameField, ESelect eSelect, CString sVal);

	//����ȫ�����ݵ��ļ� ���ر���ļ�¼��
	virtual int saveAllDataToFile();

	//��¼�Ƿ����
	virtual bool itemExists(CString sNameField, CString sVal);

	//��¼�Ƿ����
	virtual bool itemExists(CString sNameField, int nVal);

	//��ȡĳ�ֶε����ֵ����Id�����ֶα��������ͣ��������쳣��
	virtual int getMaxVal(CString sNameField);

	//���ֶ�sNameFieldģ����ѯ��������浽aDataset��
	//��sNameField="����" sValLike="��",�� �ߵ���ѧ�����Դ��� ������
	virtual void likeQuery(CString sNameField, CString sValLike, vector<T>& aDataset);

	//��ȷ��ѯ���磺���� ������ Ϊ �����Ͻ�ͨ��ѧ�����硱 ��ȫ��ͼ�飬�Լ�Ū

	//����һ����¼���ļ�
	virtual bool saveOneItemToFile(T*);

	//��ȡȫ�����ݵ� aData ��
	virtual int readAllData();

	//�Ʊ���m_pData�е�������д���б�ؼ� pListCtrl ��
	virtual void dataToListCtrl(CListCtrl* pListCtrl, vector<T>* m_pData = NULL);

	//��д��ͷ
	virtual void doListHeader(CListCtrl* pListCtrl);


	//��̬��Ա����
public:
	//���ݵ�Ԫ�����ֳ����Զ������п�
	static void listAutoWidth(CListCtrl* pListCtrl, int nColSpace, int nMinWid);

	//����б�ؼ� pListCtrl ����ʾ������
	static void clearCListCtrl(CListCtrl* pListCtrl);

	//���EXE����λ��
	static CString getExePath();
};


//�ӿ��ࣺһ����¼�����м�¼��Ӧ�Ӵ���������ǿ������ʵ�ֽӿ�
class CDataItem {
public:
	//���ص�i���ֶε�ֵ
	virtual CString operator[](int i) = 0;

	//�����ֶ�sFieldName��ֵ
	virtual CString operator[](CString sFieldName) = 0;

	//����ûд���룬Ϊʲô����д���أ�
	virtual ~CDataItem() {}
};

/*******************************************************************
���·�װ��2���� CBookData��CBookDataset���ڹ���ͼ�������Ϣ��
��������;�û���Ϣ��������Ϣ�ȵ�������ģ�´���
********************************************************************/
//ͼ������(һ����¼)��Ϊ��֤ÿ����¼�ֽ����������ִ�ʹ���ַ����飬������CString��
class CBookData : public CDataItem {
public:
	int    nBookId;
	char   sBookname[31];
	char   sBookNo[31];
	char   sAuthor[11];
	char   sPubHouse[31];
	double fPrice; //ͼ�鵥��

	CBookData(int id = 0, char* name = "", char* no = "", char* auth = "", char* pub = "", double price = 0) {
		strcpy_s(sBookname, 31, name);
		strcpy_s(sBookNo, 31, no);
		strcpy_s(sAuthor, 11, auth);
		strcpy_s(sPubHouse, 31, pub);

		nBookId = id;
		fPrice = price;
	}

	virtual ~CBookData() {}

	//���ി�麯����ʵ��
public:
	//���ص�i���ֶε�ֵ
	virtual CString operator[](int i) {
		CString s;
		switch (i) {
		case 0:  s.Format("%d", nBookId);   break;
		case 5:  s.Format("%.2f", fPrice);  break;
		case 1:  s = sBookname;             break;
		case 2:  s = sBookNo;               break;
		case 3:  s = sAuthor;               break;
		case 4:  s = sPubHouse;             break;
		default: throw CString("CBookData���ֶ�Խ��");
		}

		return s;
	}

	//�����ֶ�sFieldName��ֵ
	virtual CString operator[](CString sFieldName) {
		//�˴������CBookDataset���캯���ﱣ��һ��
		char* aName[6] = { "Id","����","���","����","������","����" };

		int nIndex = 0;
		for (int i = 0; i < 6; i++) {
			if (aName[i] == sFieldName) {
				return (*this)[i];
			}
		}

		throw CString("CBookData\n�Ƿ��ֶ�����" + sFieldName);
	}



	//��������ľ�̬�Ƚϳ�Ա����
public:
	static bool compByIdAsc(const CBookData& b1, const CBookData& b2) {
		return b1.nBookId < b2.nBookId; //����
	}
	static bool compByIdDec(const CBookData& b1, const CBookData& b2) {
		return b1.nBookId > b2.nBookId; //����
	}
	static bool compByNameAsc(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sBookname, b2.sBookname) < 0; //����
	}
	//...............
};
/**************************************************/
//������Ĺ��� �����ƣ�--KevinZ 6.8
//��һ�ۿ���ȥ����ûʲôҪ�ı��  --Cake 6.10
class CReaderData :public CDataItem
{
public:
	int    nReaderId;
	char   sKeyword[31];
	char   sReaderName[31];
	int	   nClass;
	//���ұ������û�Ȩ��1Ϊ����Ա���û�Ȩ��Ϊ0Ϊ��ͨ����
	CReaderData(int id = 0, char* key = "", char* name = "", int nc = 1)
	{
		strcpy_s(sKeyword, 31, key);
		strcpy_s(sReaderName, 31, name);


		nReaderId = id;
		nClass = nc;
	}

	virtual ~CReaderData() {}
public:
	//���ص�i���ֶε�ֵ
	virtual CString operator[](int i)
	{
		CString s;
		switch (i)
		{
		case 0:  s.Format("%d", nReaderId);         break;
		case 1:  s = sKeyword;						break;
		case 2:  s = sReaderName;					break;
		case 3:  s.Format("%d", nClass);            break;
		default: throw CString("CBookData���ֶ�Խ��");
		}

		return s;
	}

	//�����ֶ�sFieldName��ֵ
	virtual CString operator[](CString sFieldName)
	{

		char* aName[4] = { "Id","����","����","�û�Ȩ��" };

		int nIndex = 0;
		for (int i = 0; i < 4; i++) {
			if (aName[i] == sFieldName)
			{
				return (*this)[i];
			}
		}

		throw CString("CReaderData\n�Ƿ��ֶ�����" + sFieldName);
	}

};


//ͼ�����ݼ�����������¼�� ��ע�⣺������ģ���ࡿ
class CBookDataset : public CDataset<CBookData> {
public:
	CBookDataset();//���캯�� ��� aFields �ĳ�ʼ��

	virtual ~CBookDataset() {}

};
//�������ݼ�
class CReaderDataset : public CDataset<CReaderData>
{
public:
	CReaderDataset();//���캯�� ��� aFields �ĳ�ʼ��

	virtual ~CReaderDataset() {}

	CReaderData* getItemByName(CString name);

	int CheckIfHasTheReader(CString name, CString password);
};


//д�������ʷ�����Ժ�������ע��ȡ����
//KevinZ 6.8

/*
//���������ʷ��¼
class CHistoryDataset : public CDataset<CBookData> {
public:
	CHistoryDataset();//���캯�� ��� aFields �ĳ�ʼ��

	virtual ~CHistoryDataset() {}

};



//�����ڽ��¼
class CBorrowDataset : public CDataset<CBookData> {
public:
	CBorrowDataset();//���캯�� ��� aFields �ĳ�ʼ��

	virtual ~CBorrowDataset() {}

};
*/
template <class T>
bool CDataset<T> ::isSelData(T& data, CString sNameField, ESelect eSelect, CString sVal) {
	const CField& fd = getFieldByName(sNameField);

	switch (eSelect) {
	case eEqual:
		if (fd.eFieldType == eInt) {
			return atoi(data[sNameField]) == atoi(sVal);
		}
		else if (fd.eFieldType == eDouble) {
			return atof(data[sNameField]) == atof(sVal);
		}
		else {//���� Ŀǰ Ҳ���� �ִ�����
			return strcmp(data[sNameField], sVal.GetBuffer()) == 0;
		}
		break;
	case eGreater:
		if (fd.eFieldType == eInt) {
			return atoi(data[sNameField]) >= atoi(sVal);
		}
		else if (fd.eFieldType == eDouble) {
			return atof(data[sNameField]) >= atof(sVal);
		}
		else {//���� Ŀǰ Ҳ���� �ִ�����
			return strcmp(data[sNameField], sVal.GetBuffer()) > 0;
		}
		break;
	case eLess:
		throw CString(" CDataset<T> :: isSelData �Լ�д");
		break;
	case eGreaterOrEqual:
		throw CString(" CDataset<T> :: isSelData �Լ�д");
		break;
	case eLessOrEqual:
		throw CString(" CDataset<T> :: isSelData �Լ�д");
		break;
	}
}


template <class T>
//ɸѡsNameField��ֵ����eSelect��sVal��������,�õ������ݼ���ŵ� aDataset
void CDataset<T> ::selectData(vector<T>& aDataset, CString sNameField, ESelect eSelect, CString sVal) {
	if (eSelect == eLike) {
		return
			likeQuery(sNameField, sVal, aDataset);
	}

	aDataset.clear();

	int nNumItem = aData.size();
	int nNumField = aFields.size();

	//�������ݼ�
	for (int i = 0; i < nNumItem; i++) {
		if (isSelData(aData[i], sNameField, eSelect, sVal)) {
			aDataset.push_back(aData[i]);
		}
	}
}



#endif