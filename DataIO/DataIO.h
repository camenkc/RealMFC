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
	virtual void dataToListCtrl(CListCtrl* pListCtrl, vector<T>* m_pData = NULL,int CtrlCode=0);

	//��д��ͷ
	virtual void doListHeader(CListCtrl* pListCtrl,int CtrlCode=0);


	//��̬��Ա����
public:
	//���ݵ�Ԫ�����ֳ����Զ������п�
	static void listAutoWidth(CListCtrl* pListCtrl, int nColSpace, int nMinWid);

	//����б�ؼ� pListCtrl ����ʾ������
	static void clearCListCtrl(CListCtrl* pListCtrl);

	//����б�ؼ�pListCtrl�ƶ��е�����
	static void clearByCol(CListCtrl* pListCtrl, int col);

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
	int    nLeft;//���
	CBookData(int id = 0, char* name = "", char* no = "", char* auth = "", char* pub = "", double price = 0,int left=0) {
		strcpy_s(sBookname, 31, name);
		strcpy_s(sBookNo, 31, no);
		strcpy_s(sAuthor, 11, auth);
		strcpy_s(sPubHouse, 31, pub);

		nBookId = id;
		fPrice = price;
		nLeft = left;
	}

	virtual ~CBookData() {}

	//���ി�麯����ʵ��
public:
	void DecBookNum() {
		nLeft--;
	}
	void IncBookNum() {
		nLeft++;
	}
	bool HasBookLeft() {
		if (nLeft==0) return false; return true;
	}
	//���ص�i���ֶε�ֵ
	virtual CString operator[](int i) {
		CString s;
		switch (i) {
		case 0:  s.Format("%d", nBookId);   break;
		case 5:  s.Format("%.2f", fPrice);  break;
		case 6:  s.Format("%d", nLeft);		break;
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
		char* aName[7] = { "Id","����","���","����","������","����","���" };

		int nIndex = 0;
		for (int i = 0; i < 7; i++) {
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
	static bool compByPriceAsc(const CBookData& b1, const CBookData& b2) {
		return b1.fPrice < b2.fPrice; //����
	}
	static bool compByPriceDec(const CBookData& b1, const CBookData& b2) {
		return b1.fPrice > b2.fPrice; //����
	}
	static bool compByNameAsc(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sBookname, b2.sBookname) < 0; //����
	}
	static bool compByNameDec(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sBookname, b2.sBookname) > 0; //����
	}
	static bool compByBookNoAsc(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sBookNo, b2.sBookNo) < 0; //����
	}
	static bool compByBookNoDec(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sBookNo, b2.sBookNo) > 0; //����
	}
	static bool compByAuthorAsc(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sAuthor, b2.sAuthor) < 0; //����
	}
	static bool compByAuthorDec(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sAuthor, b2.sAuthor) > 0; //����
	}
	static bool compByPubHouseAsc(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sPubHouse, b2.sPubHouse) < 0; //����
	}
	static bool compByPubHouseDec(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sPubHouse, b2.sPubHouse) > 0; //����
	}
	static bool compByPubLeftAsc(const CBookData& b1, const CBookData& b2) {
		return b1.nLeft < b2.nLeft; //����
	}
	static bool compByPubLeftDec(const CBookData& b1, const CBookData& b2) {
		return  b1.nLeft > b2.nLeft;  //����
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
	void ChangePassword(CString newPassword) {
		strcpy_s(sKeyword, 31, newPassword.GetBuffer());
	}
	void ChangeName(CString newName) {
		strcpy_s(sReaderName, 31, newName.GetBuffer());
	}
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
//****************************************************
//�ڽ��¼��
class CBorrowData :public CDataItem
{
public:
	int    nBorrowId;
	int    nBorrowReaderId;
	int    nBorrowBookId;
	char   strReaderName[31];
	char   strBookName[31];
	char   strBorrowDate[31];
	char   strBorrowTime[31];
	//���ұ������û�Ȩ��1Ϊ����Ա���û�Ȩ��Ϊ0Ϊ��ͨ����
	CBorrowData(int id = 0, int nReaderId = 0, int nBookId = 0, char* sReaderName = "", char* sBookName = "", char* strData = "", char* strTime = "")
	{
		nBorrowId = id;
		nBorrowReaderId = nReaderId;
		nBorrowBookId = nBookId;
		SYSTEMTIME st;
		GetLocalTime(&st);
		CString Tempa;
		CString Tempb;

		strcpy_s(strReaderName, 31, sReaderName);
		strcpy_s(strBookName, 31, sBookName);
		Tempa.Format("%d-%d-%d", st.wYear, st.wMonth, st.wDay);
		Tempb.Format("%d:%d:%d", st.wHour, st.wMinute, st.wSecond);
		strcpy_s(strBorrowDate, (LPSTR)(LPCTSTR)Tempa);
		strcpy_s(strBorrowTime, (LPSTR)(LPCTSTR)Tempb);
	}

	virtual ~CBorrowData() {}
public:

	//���ص�i���ֶε�ֵ  �������(Ψһʶ����)������Ķ���ʶ���롢��������š����ڡ�ʱ��
	virtual CString operator[](int i)
	{
		CString s;
		switch (i)
		{
		case 0:  s.Format("%d", nBorrowId);         break;
		case 1:  s.Format("%d", nBorrowReaderId);         break;
		case 2:  s.Format("%d", nBorrowBookId);         break;
		case 3:	 s = strReaderName;						break;
		case 4:	 s = strBookName;						break;
		case 5:  s = strBorrowDate;						break;
		case 6:  s = strBorrowTime;					break;
		default: throw CString("CBookData���ֶ�Խ��");
		}

		return s;
	}
	virtual CString operator[](CString sFieldName)
	{

		char* aName[7] = { "Id","����Id","ͼ��Id","��������","ͼ������","����","ʱ��" };


		for (int i = 0; i < 7; i++) {
			if (aName[i] == sFieldName)
			{
				return (*this)[i];
			}
		}

		throw CString("CBorrowData\n�Ƿ��ֶ�����" + sFieldName);
	}
};


//****************************************************
//��ʷ��¼����ʷ��¼��š�����˱�š�������š�������ڡ�ʱ�䡢�黹���ڡ�ʱ��
class CHistoryData :public CDataItem
{
public:
	int    nHistoryId;
	int    nBorrowReaderId;
	int    nBorrowBookId;
	char   strBorrowDate[31];
	char   strBorrowTime[31];
	char   strReturnDate[31];
	char   strReturnTime[31];
	CHistoryData(int id = 0, int nReaderId = 0, int nBookId = 0,char * BorrowDate="",char * BorrowTime="")
	{
		nHistoryId = id;
		nBorrowReaderId = nReaderId;
		nBorrowBookId = nBookId;
		
		strcpy_s(strBorrowDate, 31, BorrowDate);
		strcpy_s(strBorrowTime, 31, BorrowTime);
		
		SYSTEMTIME st;
		GetLocalTime(&st);
		CString Tempa;
		CString Tempb;

		Tempa.Format("%d-%d-%d", st.wYear, st.wMonth, st.wDay);
		Tempb.Format("%d:%d:%d", st.wHour, st.wMinute, st.wSecond);
		strcpy_s(strReturnDate, (LPSTR)(LPCTSTR)Tempa);
		strcpy_s(strReturnTime, (LPSTR)(LPCTSTR)Tempb);


	}

	virtual ~CHistoryData() {}
public:

	//���ص�i���ֶε�ֵ  ��ʷ��¼��š�����˱�š�������š�������ڡ�ʱ�䡢�黹���ڡ�ʱ��
	virtual CString operator[](int i)
	{
		CString s;
		switch (i)
		{
		case 0:  s.Format("%d", nHistoryId);         break;
		case 1:  s.Format("%d", nBorrowReaderId);         break;
		case 2:  s.Format("%d", nBorrowBookId);         break;
		case 3:  s = strBorrowDate;						break;
		case 4:  s = strBorrowTime;					break;
		case 5:  s = strReturnDate;						break;
		case 6:  s = strReturnTime;					break;
		default: throw CString("CBookData���ֶ�Խ��");
		}

		return s;
	}
	virtual CString operator[](CString sFieldName)
	{

		char* aName[7] = { "Id","����Id","ͼ��Id","�������","���ʱ��","�黹����","�黹ʱ��" };

		int nIndex = 0;
		for (int i = 0; i < 7; i++) {
			if (aName[i] == sFieldName)
			{
				return (*this)[i];
			}
		}

		throw CString("CHistoryData\n�Ƿ��ֶ�����" + sFieldName);
	}
};


//ͼ�����ݼ�����������¼�� ��ע�⣺������ģ���ࡿ
class CBookDataset : public CDataset<CBookData> {
public:
	CBookDataset();//���캯�� ��� aFields �ĳ�ʼ��

	virtual ~CBookDataset() {}

	int CheckIfHaveTheBookById(CString Id);
	CString CheckIfHaveTheBookByName(CString Name);//����ֵΪCString���͵��鼮Id ��û�з���""
	CString GetBookNameById(CString Id);
	CString GetBookIdByName(CString Id);
	void DecBookNum(CString BookId);
	void IncBookNum(CString BookId);
	void SetBookNoLeftToRed(CMyCListCtrl* pListCtrl);
	bool HasBookLeftById(CString BookId);
};
//�������ݼ�
class CReaderDataset : public CDataset<CReaderData>
{
public:
	CReaderDataset();//���캯�� ��� aFields �ĳ�ʼ��

	virtual ~CReaderDataset() {}

	CReaderData* getItemByName(CString name);
	char* getNameById(int n);
    int getClassById(int n);
	int CheckIfHasTheReader(CString name, CString password);
	int CheckIfHasTheReader(int Id, CString password);
	void ChangeNameById(int Id, CString name);
	void ChangePasswordById(int Id, CString password);

};


//���������ʷ��¼
class CHistoryDataset : public CDataset<CHistoryData> {
public:
	CHistoryDataset();//���캯�� ��� aFields �ĳ�ʼ��

	virtual ~CHistoryDataset() {}


};



//�����ڽ��¼
class CBorrowDataset : public CDataset<CBorrowData> {
public:
	CBorrowDataset();//���캯�� ��� aFields �ĳ�ʼ��

	virtual ~CBorrowDataset() {}

	CString CheckIfHasBorrowData(CString BorrowId, CString ReaderId);
	CString CheckIfHasBorrowDataByName(CString BookName, CString ReaderId);

};

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
			return atoi(data[sNameField]) > atoi(sVal);
		}
		else if (fd.eFieldType == eDouble) {
			return atof(data[sNameField]) > atof(sVal);
		}
		else {//���� Ŀǰ Ҳ���� �ִ�����
			return strcmp(data[sNameField], sVal.GetBuffer()) > 0;
		}
		break;
	case eLess:
		if (fd.eFieldType == eInt) {
			return atoi(data[sNameField]) < atoi(sVal);
		}
		else if (fd.eFieldType == eDouble) {
			return atof(data[sNameField]) < atof(sVal);
		}
		else {//���� Ŀǰ Ҳ���� �ִ�����
			return strcmp(data[sNameField], sVal.GetBuffer()) < 0;
		}
		break;
	case eGreaterOrEqual:
		if (fd.eFieldType == eInt) {
			return atoi(data[sNameField]) >= atoi(sVal);
		}
		else if (fd.eFieldType == eDouble) {
			return atof(data[sNameField]) >= atof(sVal);
		}
		else {//���� Ŀǰ Ҳ���� �ִ�����
			return strcmp(data[sNameField], sVal.GetBuffer()) >= 0;
		}
		break;
	case eLessOrEqual:
		if (fd.eFieldType == eInt) {
			return atoi(data[sNameField]) <= atoi(sVal);
		}
		else if (fd.eFieldType == eDouble) {
			return atof(data[sNameField]) <= atof(sVal);
		}
		else {//���� Ŀǰ Ҳ���� �ִ�����
			return strcmp(data[sNameField], sVal.GetBuffer()) <= 0;
		}
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