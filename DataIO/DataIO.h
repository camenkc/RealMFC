//#pragma once 这个可以替代下面的条件编译：#ifndef
#ifndef __DATA____IO___
#define __DATA____IO___

#include <algorithm>//算法库
#include <vector> //向量容器
#include <fstream> //文件流
#include "afxwin.h"//windows控件

using namespace std;

//字段类型枚举：整型、实型、日期时间型、字串型
enum EField { eInt, eDouble, eDataTime, eString };
enum ESelect { eEqual, eGreater, eLess, eGreaterOrEqual, eLessOrEqual, eLike };
//字段：字段名 + 字段数据类型
struct CField {
	CString sFieldName;
	EField eFieldType;
};


//数据集，模板类
template <class T>
class CDataset {
private:
	CDataset& operator =(const CDataset&) { return *this; }//不允许赋值运算
	//是否满足筛选条件
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
	//增加用于删除数据的虚函数
	virtual void deleteOneRow(int nRow);

	//基于字段名获取字段信息
	virtual const CField& getFieldByName(CString sFieldName);

	//排序 参数为用于比较的 函数指针
	virtual void dataOrder(bool comp(const T& b1, const T& b2));

	//基于键值查找数组指针
	virtual T* getItemByKeyVal(CString sKeyField, int nKeyVal);

	//基于键值查找数组指针
	virtual bool deleteItemByKeyVal(CString sKeyField, int nKeyVal);

	//筛选sNameField的值满足eSelect（sVal）的数据,得到的数据集存放到 aDataset
	virtual void selectData(vector<T>& aDataset, CString sNameField, ESelect eSelect, CString sVal);

	//保存全部数据到文件 返回保存的记录数
	virtual int saveAllDataToFile();

	//记录是否存在
	virtual bool itemExists(CString sNameField, CString sVal);

	//记录是否存在
	virtual bool itemExists(CString sNameField, int nVal);

	//获取某字段的最大值，如Id【该字段必须是整型，否则结果异常】
	virtual int getMaxVal(CString sNameField);

	//对字段sNameField模糊查询，结果保存到aDataset中
	//如sNameField="书名" sValLike="数",则 高等数学、线性代数 均满足
	virtual void likeQuery(CString sNameField, CString sValLike, vector<T>& aDataset);

	//精确查询，如：查找 出版社 为 “西南交通大学出版社” 的全部图书，自己弄

	//保存一条记录到文件
	virtual bool saveOneItemToFile(T*);

	//读取全部数据到 aData 中
	virtual int readAllData();

	//制表：把m_pData中的数据填写到列表控件 pListCtrl 中
	virtual void dataToListCtrl(CListCtrl* pListCtrl, vector<T>* m_pData = NULL,int CtrlCode=0);

	//填写表头
	virtual void doListHeader(CListCtrl* pListCtrl,int CtrlCode=0);


	//静态成员函数
public:
	//根据单元格文字长度自动调整列宽
	static void listAutoWidth(CListCtrl* pListCtrl, int nColSpace, int nMinWid);

	//清除列表控件 pListCtrl 里显示的数据
	static void clearCListCtrl(CListCtrl* pListCtrl);

	//清除列表控件pListCtrl制定列的数据
	static void clearByCol(CListCtrl* pListCtrl, int col);

	//获得EXE所在位置
	static CString getExePath();
};


//接口类：一条记录，所有记录均应从此派生，以强制子类实现接口
class CDataItem {
public:
	//返回第i个字段的值
	virtual CString operator[](int i) = 0;

	//返回字段sFieldName的值
	virtual CString operator[](CString sFieldName) = 0;

	//析构没写代码，为什么必须写上呢？
	virtual ~CDataItem() {}
};

/*******************************************************************
以下封装的2个类 CBookData、CBookDataset用于管理【图书基本信息】
其它的如;用户信息、借阅信息等等请自行模仿处理
********************************************************************/
//图书数据(一条记录)【为保证每条记录字节数定长，字串使用字符数组，而不是CString】
class CBookData : public CDataItem {
public:
	int    nBookId;
	char   sBookname[31];
	char   sBookNo[31];
	char   sAuthor[11];
	char   sPubHouse[31];
	double fPrice; //图书单价
	int    nLeft;//库存
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

	//父类纯虚函数的实现
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
	//返回第i个字段的值
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
		default: throw CString("CBookData：字段越界");
		}

		return s;
	}

	//返回字段sFieldName的值
	virtual CString operator[](CString sFieldName) {
		//此处必须和CBookDataset构造函数里保持一致
		char* aName[7] = { "Id","书名","书号","作者","出版社","单价","库存" };

		int nIndex = 0;
		for (int i = 0; i < 7; i++) {
			if (aName[i] == sFieldName) {
				return (*this)[i];
			}
		}

		throw CString("CBookData\n非法字段名：" + sFieldName);
	}



	//用于排序的静态比较成员函数
public:
	static bool compByIdAsc(const CBookData& b1, const CBookData& b2) {
		return b1.nBookId < b2.nBookId; //升序
	}
	static bool compByIdDec(const CBookData& b1, const CBookData& b2) {
		return b1.nBookId > b2.nBookId; //降序
	}
	static bool compByPriceAsc(const CBookData& b1, const CBookData& b2) {
		return b1.fPrice < b2.fPrice; //升序
	}
	static bool compByPriceDec(const CBookData& b1, const CBookData& b2) {
		return b1.fPrice > b2.fPrice; //降序
	}
	static bool compByNameAsc(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sBookname, b2.sBookname) < 0; //升序
	}
	static bool compByNameDec(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sBookname, b2.sBookname) > 0; //降序
	}
	static bool compByBookNoAsc(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sBookNo, b2.sBookNo) < 0; //升序
	}
	static bool compByBookNoDec(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sBookNo, b2.sBookNo) > 0; //降序
	}
	static bool compByAuthorAsc(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sAuthor, b2.sAuthor) < 0; //升序
	}
	static bool compByAuthorDec(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sAuthor, b2.sAuthor) > 0; //降序
	}
	static bool compByPubHouseAsc(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sPubHouse, b2.sPubHouse) < 0; //升序
	}
	static bool compByPubHouseDec(const CBookData& b1, const CBookData& b2) {
		return strcmp(b1.sPubHouse, b2.sPubHouse) > 0; //降序
	}
	static bool compByLeftAsc(const CBookData& b1, const CBookData& b2) {
		return b1.nLeft < b2.nLeft; //升序
	}
	static bool compByLeftDec(const CBookData& b1, const CBookData& b2) {
		return  b1.nLeft > b2.nLeft;  //降序
	}


	//...............
};
/**************************************************/
//读者类的构建 待完善！--KevinZ 6.8
//第一眼看过去好像没什么要改变的  --Cake 6.10
class CReaderData :public CDataItem
{
public:
	int    nReaderId;
	char   sKeyword[31];
	char   sReaderName[31];
	int	   nClass;
	//姑且表明：用户权限1为管理员，用户权限为0为普通读者
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
	//返回第i个字段的值
	virtual CString operator[](int i)
	{
		CString s;
		switch (i)
		{
		case 0:  s.Format("%d", nReaderId);         break;
		case 1:  s = sKeyword;						break;
		case 2:  s = sReaderName;					break;
		case 3:  s.Format("%d", nClass);            break;
		default: throw CString("CBookData：字段越界");
		}

		return s;
	}

	//返回字段sFieldName的值
	virtual CString operator[](CString sFieldName)
	{

		char* aName[4] = { "Id","密码","姓名","用户权限" };

		int nIndex = 0;
		for (int i = 0; i < 4; i++) {
			if (aName[i] == sFieldName)
			{
				return (*this)[i];
			}
		}

		throw CString("CReaderData\n非法字段名：" + sFieldName);
	}

};
//****************************************************
//在借记录：
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

	//返回第i个字段的值  借出编码(唯一识别码)、借出的读者识别码、借出的书编号、日期、时间
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
		default: throw CString("CBookData：字段越界");
		}

		return s;
	}
	virtual CString operator[](CString sFieldName)
	{

		char* aName[7] = { "Id","读者Id","图书Id","读者姓名","图书名称","日期","时间" };


		for (int i = 0; i < 7; i++) {
			if (aName[i] == sFieldName)
			{
				return (*this)[i];
			}
		}

		throw CString("CBorrowData\n非法字段名：" + sFieldName);
	}
};


//****************************************************
//历史记录：历史记录编号、借出人编号、借出书编号、借出日期、时间、归还日期、时间
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

	//返回第i个字段的值  历史记录编号、借出人编号、借出书编号、借出日期、时间、归还日期、时间
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
		default: throw CString("CBookData：字段越界");
		}

		return s;
	}
	virtual CString operator[](CString sFieldName)
	{

		char* aName[7] = { "Id","读者Id","图书Id","借出日期","借出时间","归还日期","归还时间" };

		int nIndex = 0;
		for (int i = 0; i < 7; i++) {
			if (aName[i] == sFieldName)
			{
				return (*this)[i];
			}
		}

		throw CString("CHistoryData\n非法字段名：" + sFieldName);
	}
};


//图书数据集（若干条记录） 【注意：基类是模板类】
class CBookDataset : public CDataset<CBookData> {
public:
	CBookDataset();//构造函数 完成 aFields 的初始化

	virtual ~CBookDataset() {}

	int CheckIfHaveTheBookById(CString Id);
	CString CheckIfHaveTheBookByName(CString Name);//返回值为CString类型的书籍Id 若没有返回""
	CString GetBookNameById(CString Id);
	CString GetBookIdByName(CString Name);
	void DecBookNum(CString BookId);
	void IncBookNum(CString BookId);
	void SetBookNoLeftToRed(CMyCListCtrl* pListCtrl);
	bool HasBookLeftById(CString BookId);
};
//图书备份
class CBookBackup :public CBookDataset
{
public:

	CBookBackup();

	virtual ~CBookBackup() {}

	CString key;
	CString sNewFilename;


	bool BackupBykey(CString key);
	bool LoadingBykey(CString key);




};
//读者数据集
class CReaderDataset : public CDataset<CReaderData>
{
public:
	CReaderDataset();//构造函数 完成 aFields 的初始化

	virtual ~CReaderDataset() {}

	CReaderData* getItemByName(CString name);
	char* getNameById(int n);
    int getClassById(int n);
	int CheckIfHasTheReader(CString name, CString password);
	int CheckIfHasTheReader(int Id, CString password);
	void ChangeNameById(int Id, CString name);
	void ChangePasswordById(int Id, CString password);

};



//管理借阅历史记录
class CHistoryDataset : public CDataset<CHistoryData> {
public:
	CHistoryDataset();//构造函数 完成 aFields 的初始化

	virtual ~CHistoryDataset() {}


};



//管理在借记录
class CBorrowDataset : public CDataset<CBorrowData> {
public:
	CBorrowDataset();//构造函数 完成 aFields 的初始化

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
		else {//日期 目前 也当成 字串处理
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
		else {//日期 目前 也当成 字串处理
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
		else {//日期 目前 也当成 字串处理
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
		else {//日期 目前 也当成 字串处理
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
		else {//日期 目前 也当成 字串处理
			return strcmp(data[sNameField], sVal.GetBuffer()) <= 0;
		}
		break;
	}

}


template <class T>
//筛选sNameField的值满足eSelect（sVal）的数据,得到的数据集存放到 aDataset
void CDataset<T> ::selectData(vector<T>& aDataset, CString sNameField, ESelect eSelect, CString sVal) {
	if (eSelect == eLike) {
		return
			likeQuery(sNameField, sVal, aDataset);
	}

	aDataset.clear();

	int nNumItem = aData.size();
	int nNumField = aFields.size();

	//遍历数据集
	for (int i = 0; i < nNumItem; i++) {
		if (isSelData(aData[i], sNameField, eSelect, sVal)) {
			aDataset.push_back(aData[i]);
		}
	}
}



#endif