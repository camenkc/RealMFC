#include "pch.h" //MFC框架下，任何CPP文件必须包含 本头文件
#include "DataIO.h"
#include "../MyControls/MyControls.h"


//--------------------------------------------------
/************************************************
   CDataset 成员函数 Start
*************************************************/
//--------------------------------------------------


//用于更具指定的nRow来将vector aData中指定一行删除
template <class T>
void CDataset<T>::deleteOneRow(int nRow) {
	vector<T>::iterator tmp = aData.begin() + nRow;
	aData.erase(tmp);
}
//基于字段名获取字段信息
template <class T>
const CField &CDataset<T> :: getFieldByName(CString sFieldName){
	int nNumField = aFields.size();
	for(int i = 0; i < nNumField; i++){
		if(aFields[i].sFieldName == sFieldName){
			return aFields[i];
		}
	}

	throw CString("字段不存在：" + sFieldName);
}

//记录是否存在
template <class T>
bool CDataset<T> :: itemExists(CString sNameField, CString sVal){
	int nNumItem = aData.size();
	for(int i = 0; i < nNumItem; i++){
		if(aData[i][sNameField] == sVal){
			return true;
		}
	}

	return false;
}

//记录是否存在
template <class T>
bool CDataset<T> :: itemExists(CString sNameField, int nVal){
	int nNumItem = aData.size();
	for(int i = 0; i < nNumItem; i++){
		if(atoi(aData[i][sNameField]) == nVal){
			return true;
		}
	}

	return false;
}

//获取某字段的最大值，如Id【该字段必须是整型，否则结果异常】
template <class T>
int CDataset<T> :: getMaxVal(CString sNameField){
	int nVal = 0;
	int nNumItem = aData.size();

	for(int i = 0; i < nNumItem; i++){
		if(i == 0){
			nVal = atoi(aData[i][sNameField]);
		}else if(nVal < atoi(aData[i][sNameField])){
			nVal = atoi(aData[i][sNameField]);
		}
	}

	return nVal;
}


//对字段sNameField模糊查询，结果保存到aDataset中
//如sNameField="书名" sValLike="数",则 高等数学、线性代数 均满足
template <class T>
void CDataset<T> :: likeQuery(CString sNameField, CString sValLike, vector<T> &aDataset){
	const CField &fd = getFieldByName(sNameField);

	if(fd.eFieldType != eString){
		throw CString("不能进行模糊查询：\n字段：" + sNameField + "  不是字串类型");
	}

	aDataset.clear();

	int nNumItem = aData.size();
	int nNumField = aFields.size();

	//遍历数据集
	for(int i = 0; i < nNumItem; i++){
		if(aData[i][sNameField].Find(sValLike) >= 0){
			aDataset.push_back(aData[i]);
		}
	}
}


//排序 参数为用于比较的 函数指针
template <class T>
void CDataset<T> ::dataOrder(bool comp(const T& b1, const T& b2)) {
	sort(aData.begin(), aData.end(), comp);
}

//基于键值查找数组指针
template <class T>
T* CDataset<T> ::getItemByKeyVal(CString sKeyField, int nKeyVal) {
	int nNumItem = aData.size();
	for (int i = 0; i < nNumItem; i++) {
		if (atoi(aData[i][sKeyField]) == nKeyVal) {
			return &aData[i];
		}
	}
	return NULL;
}

//保存全部数据到文件 返回保存的记录数
template <class T>
int CDataset<T> ::saveAllDataToFile() {

	//二进制追加方式打开文件sFileName(若文件不存在则创建,若存在则删除数据)	
	ofstream os(sFileName.GetBuffer(), ios::binary );
	int nNumofSaved = 0;
	if (!os) {
		throw "打开或创建文件失败：\n" + sFileName;
	}
	//用迭代器：
	vector<T>::iterator it = aData.begin();
	for (; it != aData.end(); it++) {
		nNumofSaved++;
		os.write((char*)&(*it), sizeof(T));

	}

	os.close();
	return nNumofSaved;

}


template <class T>
bool CDataset<T> ::deleteItemByKeyVal(CString sKeyField, int nKeyVal) {
	//用迭代器 遍历容器 若键值相等 则Erase
	return 1;
}

//保存一条记录到文件
template <class T>
bool CDataset<T> :: saveOneItemToFile(T *dataItem){
	//二进制追加方式打开文件sFileName(若文件不存在则创建)
	//ios::app==》追加，打开后文件指针在末尾
	
	ofstream os(sFileName.GetBuffer(),ios::binary | ios::app);

	if(!os){
		throw "打开或创建文件失败：\n" + sFileName;
	}

	os.write((char *)dataItem, sizeof(T));

	os.close();

	//同时数据保持到 aData 中
	aData.push_back(*dataItem);

	return true;
}

//读取全部数据到 aData 中,返回读取的记录数
template <class T>
int CDataset<T> :: readAllData(){
	//二进制读方式打开文件sFileName(若文件不存在不创建文件，此时打开失败)
	ifstream is(sFileName.GetBuffer(),ios::binary | ios::_Nocreate);

	if(!is){
		AfxMessageBox("打开文件失败：\n" + sFileName);
		return 0;
	}

	T item;

	//清空容器里的数据
	aData.clear();

	//读文件，每次读一条记录
	while(is.good()){
		is.read((char *)&item, sizeof(T));
		if (!is.good()) break;
		aData.push_back(item);
	}

	is.close();

	return aData.size();
}


//制表：把m_pData中的数据填写到列表控件 pListCtrl 中
template <class T>
void CDataset<T> :: dataToListCtrl(CListCtrl *pListCtrl, vector<T> *m_pData){
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	//控件暂时不要绘制，否则数据量大的时候会很慢
	pListCtrl->SetRedraw(FALSE);
	
	//清空表格控件
	clearCListCtrl(pListCtrl);
	
	//填写表格控件的头
	doListHeader(pListCtrl);

	if(m_pData == NULL){
		m_pData = &aData;
	}

	//局部变量覆盖成员变量
	vector<T> &aData = *m_pData;

	int nNumItem = aData.size();
	int nNumField = aFields.size();

	//遍历数据集，制表
	for(int i = 0; i < nNumItem; i++){
		for(int j = 0; j < nNumField; j++){
			j == 0 ? pListCtrl->InsertItem(i,  aData[i][j]) : pListCtrl->SetItemText(i,  j,  aData[i][j]);
		}
	}

	//列间距 13 pixels,最小列宽：80pixels
	listAutoWidth(pListCtrl, 13, 80);

	pListCtrl->SetRedraw(TRUE);
	pListCtrl->InvalidateRect(NULL, FALSE);
}

//填写表头
template <class T>
void CDataset<T> :: doListHeader(CListCtrl *pListCtrl){
	if(pListCtrl->GetHeaderCtrl()){
		clearCListCtrl(pListCtrl);
	}
	CMyCListCtrl* pMylist = dynamic_cast<CMyCListCtrl*>(pListCtrl);

	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER); 

	int nNumField = aFields.size();
	for(int i = 0; i < nNumField; i++) {
		if (pMylist) {
			pMylist->InsertColumn(i + 1, (LPCTSTR)aFields[i].sFieldName, LVCFMT_CENTER, 80);
		}
		else {
			pListCtrl->InsertColumn(i + 1, (LPCTSTR)aFields[i].sFieldName, LVCFMT_CENTER, 80);

		}
	
	}
}

//-------------------------------------------------------
/////////////////////静态成员函数
//-------------------------------------------------------

//根据单元格文字长度自动调整列宽
template <class T>
void CDataset<T> :: listAutoWidth(CListCtrl *pListCtrl, int nColSpace, int nMinWid){
	pListCtrl->SetRedraw(FALSE);
	
	int nColumnWidth, nHeaderWidth;
	int nColumnCount = 0;
	
	//获取表格列数
	CHeaderCtrl* pHeaderCtrl = pListCtrl->GetHeaderCtrl();
	if(!pHeaderCtrl){//无表头
		return;
	}

	nColumnCount = pHeaderCtrl->GetItemCount();
	if(nColumnCount < 1){//无列表
		return;
	}
	
	for (int i = 0; i < nColumnCount - 1; i++){	
		pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE); 
		
		nColumnWidth = pListCtrl->GetColumnWidth(i) + nColSpace;
		pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		nHeaderWidth = pListCtrl->GetColumnWidth(i) + nColSpace; 
		
		nColumnWidth = max(nColumnWidth, nHeaderWidth);
		if(nColumnWidth < nMinWid){
			nColumnWidth = nMinWid;
		}

		pListCtrl->SetColumnWidth(i, nColumnWidth);		
	}

	pListCtrl->SetRedraw(TRUE);

	pListCtrl->InvalidateRect(NULL, FALSE);
}


 //清除列表控件 pListCtrl 里显示的数据
template <class T>
void CDataset<T> :: clearCListCtrl(CListCtrl *pListCtrl){
	int nColumnCount =  pListCtrl->GetHeaderCtrl()->GetItemCount();

	pListCtrl->DeleteAllItems(); 

	for (int i = 0;i < nColumnCount; i++) {
		pListCtrl->DeleteColumn(0);
	}
}

//获得EXE所在位置
template <class T>
CString CDataset<T> :: getExePath(){
	char szPath[MAX_PATH];

	::GetModuleFileName(NULL, szPath, sizeof(szPath));//返回exe文件所在路径

	CString strPath = szPath;

	int nPos = strPath.ReverseFind('\\');

	strPath = strPath.Left(nPos).TrimRight(_T("\\"));

	return strPath;	
}
//--------------------------------------------------
/************************************************
    CDataset 成员函数 End
	
	CBookDataset 成员函数 Start
*************************************************/
//--------------------------------------------------
CBookDataset :: CBookDataset(){ //构造函数 完成 aFields 的初始化
	//务必与CBookData数成员类型对应
	EField aType[6] = {eInt, eString, eString, eString, eString, eDouble};
	char *aName[6] = {"Id","书名","书号","作者","出版社","单价"};
	 
	CField field;
	for(int i = 0; i < 6; i++){
		field.eFieldType = aType[i];
		field.sFieldName = aName[i];
		aFields.push_back(field);
	}

	//保存数据的文件：可执行文件路径 + 文件名
	sFileName = getExePath() + "\\Books.dat";
}

//--------------------------------------------------
/************************************************
   CBookDataset 成员函数 End

   CReaderDataset 成员函数Start
*************************************************/
//--------------------------------------------------

CReaderDataset :: CReaderDataset()
{
	EField aType[4] = { eInt, eString, eString, eInt };
	char* aName[4] = { "Id","密码","姓名","用户权限" };//用户权限分2级 0为admin 1为普通用户
	CField field;
	for (int i = 0; i < 4; i++)
	{
		field.eFieldType = aType[i];
		field.sFieldName = aName[i];
		aFields.push_back(field);
	}

	sFileName = getExePath() + "\\Readers.dat";

}
//--------------------------------------------------
/************************************************
   CReaderDataset 成员函数 End

   CHistoryDataset 成员函数Start
*************************************************/
//--------------------------------------------------


//完善类以后 把下面的注释取消掉
//KevinZ 6.8
/*

CHistoryDataset::CHistoryDataset()
{
	EField aType[6] = { eInt, eString, eInt, eString,eString,eInt };
	char* aName[6] = { "Id","读者姓名","借阅图书id","借阅图书名称","借阅日期","借阅类型" };//0为借阅 1为归还
	CField field;
	for (int i = 0; i < 5; i++)
	{
		field.eFieldType = aType[i];
		field.sFieldName = aName[i];
		aFields.push_back(field);
	}

	sFileName = getExePath() + "\\History.dat";

}CBorrowDataset::CBorrowDataset()
{
	EField aType[5] = { eInt, eString, eInt, eString,eString };
	char* aName[5] = { "Id","读者姓名","借阅图书id","借阅图书名称","借阅日期" };
	CField field;
	for (int i = 0; i < 5; i++)
	{
		field.eFieldType = aType[i];
		field.sFieldName = aName[i];
		aFields.push_back(field);
	}

	sFileName = getExePath() + "\\Borrow.dat";

}
*/