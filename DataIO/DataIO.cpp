#include "pch.h" //MFC框架下，任何CPP文件必须包含 本头文件
#include "DataIO.h"
#include "../MyControls/MyControls.h"

#include <io.h> 

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
	//考虑到这里仅通过每种记录的唯一标识码进行删除 所以直接取[0]
	vector<T>::iterator it = aData.begin();
	CString sKeyVal; sKeyVal.Format("%d", nKeyVal);
	while (it != aData.end()) {
		if ((*it)[sKeyField].Compare(sKeyVal) == 0) {
			aData.erase(it);
			break;
		}
		it++;
	}
	
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
void CDataset<T> :: dataToListCtrl(CListCtrl *pListCtrl, vector<T> *m_pData,int CtrlCode){
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	//控件暂时不要绘制，否则数据量大的时候会很慢
	pListCtrl->SetRedraw(FALSE);
	
	//清空表格控件
	clearCListCtrl(pListCtrl);
	
	//填写表格控件的头
	if (CtrlCode == 1)//需要隐藏密码栏
	{
		doListHeader(pListCtrl, 1);
	}
	else {
		doListHeader(pListCtrl);
	}
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
			if (j == 1 && CtrlCode == 1)continue;
			if (j > 1 && CtrlCode == 1) {
				j == 0 ? pListCtrl->InsertItem(i, aData[i][j]) : pListCtrl->SetItemText(i, j-1, aData[i][j]);
				continue;
			}
			j == 0 ? pListCtrl->InsertItem(i, aData[i][j]) : pListCtrl->SetItemText(i, j, aData[i][j]);
			
		}
	}

	//列间距 13 pixels,最小列宽：80pixels
	listAutoWidth(pListCtrl, 13, 80);

	pListCtrl->SetRedraw(TRUE);
	pListCtrl->InvalidateRect(NULL, FALSE);
}


//填写表头
template <class T>
void CDataset<T> :: doListHeader(CListCtrl *pListCtrl,int CtrlCode){
	if(pListCtrl->GetHeaderCtrl()){
		clearCListCtrl(pListCtrl);
	}
	CMyCListCtrl* pMylist = dynamic_cast<CMyCListCtrl*>(pListCtrl);

	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER); 

	int nNumField = aFields.size();
	for(int i = 0; i < nNumField; i++) {
		if (i == 1 && CtrlCode == 1) continue;
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

//清除列表控件制定列的数据
template<class T>
void CDataset<T>::clearByCol(CListCtrl* pListCtrl, int col)
{
	pListCtrl->DeleteColumn(col);
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
	EField aType[7] = {eInt, eString, eString, eString, eString, eDouble,eString};
	char *aName[7] = {"Id","书名","书号","作者","出版社","单价","库存"};
	 
	CField field;
	for(int i = 0; i < 7; i++){
		field.eFieldType = aType[i];
		field.sFieldName = aName[i];
		aFields.push_back(field);
	}

	//保存数据的文件：可执行文件路径 + 文件名
	sFileName = getExePath() + "\\Books.dat";
}
int CBookDataset::CheckIfHaveTheBookById(CString Id) {
	vector<CBookData>::iterator it = aData.begin();
	while (it != aData.end()) {
		if ((*it)[0].Compare(Id) == 0) {
			return 1;
		}
		it++;
	}return 0;
}
//返回值为CString类型的书籍Id 若没有返回""
CString CBookDataset::CheckIfHaveTheBookByName(CString Name) {
	vector<CBookData>::iterator it = aData.begin();
	while (it != aData.end()) {
		if ((*it)[1].Compare(Name) == 0) {
			return (*it)[0];
		}
		it++;
	}return CString("");
}
CString CBookDataset::GetBookNameById(CString Id) {
	vector<CBookData>::iterator it = aData.begin();
	while (it != aData.end()) {
		if ((*it)[0].Compare(Id) == 0) {
			return (*it)[1];
		}
		it++;
	}return CString("");
}
CString CBookDataset::GetBookIdByName(CString Name)
{
	vector<CBookData>::iterator it = aData.begin();
	while (it != aData.end()) {
		if ((*it)[1].Compare(Name) == 0) {
			return (*it)[0];
		}
		it++;
	}return CString("");
}
void CBookDataset::DecBookNum(CString BookId) {
	vector<CBookData>::iterator it = aData.begin();
	while (it != aData.end()) {
		if ((*it)[0].Compare(BookId) == 0) {
			(*it).DecBookNum();
			return;
		}
		it++;
	}
}
void CBookDataset::IncBookNum(CString BookId) {
	vector<CBookData>::iterator it = aData.begin();
	while (it != aData.end()) {
		if ((*it)[0].Compare(BookId) == 0) {
			(*it).IncBookNum();
			return;
		}
		it++;
	}
}
void CBookDataset::SetBookNoLeftToRed(CMyCListCtrl* pListCtrl) {
	vector<CBookData>::iterator it = aData.begin();
	int nowRow = 0;
	while (it != aData.end()) {
		if ((*it)[6].Compare("0") == 0) {
			for (int i=0;i<7;i++)
				pListCtrl->SetItemTextColor(i, nowRow, RGB(255, 0, 0));
		}
		nowRow++;
		it++;
	}
}
bool CBookDataset::HasBookLeftById(CString BookId) {
	vector<CBookData>::iterator it = aData.begin();
	while (it != aData.end()) {
		if ((*it)[0].Compare(BookId) == 0) {
			return (*it).HasBookLeft();
		}
		it++;
	}
	return 0;
}
//--------------------------------------------------
/************************************************
   CBookDataset 成员函数 End

   CBookBackup 成员函数Start
*************************************************/
//--------------------------------------------------
CBookBackup::CBookBackup()
{
	CBookDataset();
	sNewFilename = getExePath() + "\\Backup.dat";
	key.Format("0");

}
bool CBookBackup::BackupBykey(CString key)
{

	char* kagari, fbuffer[1];
	char ckey;
	kagari = (LPTSTR)(LPCSTR)key;
	int keylen = strlen(kagari);
	int index = 0;
	if ((_access(getExePath() + "\\Books.dat", 0) == -1))
	{
		return 0;
	}
	else
	{

		FILE* fsource = fopen(getExePath() + "\\Books.dat", "rb");
		FILE* fdest = fopen(getExePath() + "\\Backup.dat", "wb");

		while (!feof(fsource))
		{
			fread(fbuffer, 1, 1, fsource);
			if (!feof(fsource))
			{
				ckey = kagari[index % keylen];
				*fbuffer = *fbuffer + ckey*ckey;
				fwrite(fbuffer, 1, 1, fdest);
				index++;
			}
		}
		fclose(fsource);
		fclose(fdest);
		return 1;

	}
}
bool CBookBackup::LoadingBykey(CString key)
{

	char* kagari, fbuffer[1];
	char ckey;
	kagari = (LPTSTR)(LPCSTR)key;
	int keylen = strlen(kagari);
	int index = 0;
	if ((_access(getExePath() + "\\Backup.dat", 0) == -1))
	{
		return 0;
	}
	else
	{

		FILE* fsource = fopen(getExePath() + "\\Backup.dat", "rb");
		FILE* fdest = fopen(getExePath() + "\\Books.dat", "wb");

		while (!feof(fsource))
		{
			fread(fbuffer, 1, 1, fsource);
			if (!feof(fsource))
			{
				ckey = kagari[index % keylen];
				*fbuffer = *fbuffer -ckey*ckey;
				fwrite(fbuffer, 1, 1, fdest);
				index++;
			}
		}
		fclose(fsource);
		fclose(fdest);
		return 1;

	}
}
//--------------------------------------------------
/************************************************
   CBookBackup 成员函数 End

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


CReaderData* CReaderDataset::getItemByName(CString name) {
	vector<CReaderData>::iterator tmp=aData.begin();
	while (tmp != aData.end()) {
		if ((*tmp)[2] == name)
		{
			return &(*tmp);
		}
		tmp++;
	}
	return NULL;
}
char* CReaderDataset::getNameById(int n)
{
	vector<CReaderData>::iterator tmp = aData.begin();
	CString n1; n1.Format("%d", n);
	while (tmp != aData.end()) {
		if ((*tmp)[0].Compare(n1)==0 )
		{
			return (*tmp).sReaderName;
		}
		tmp++;
	}
	return NULL;
}

int  CReaderDataset::getClassById(int n)
{
	vector<CReaderData>::iterator temp = aData.begin();
	CString n1; n1.Format("%d", n);
	while (temp!=aData.end())
	{
		if ((*temp)[0].Compare(n1)==0)
		{
			return (*temp).nClass;
		}
		temp++;
	}
	return NULL;
}
int CReaderDataset::CheckIfHasTheReader(CString name, CString password) {
	vector<CReaderData>::iterator tmp = aData.begin();
	int now = 1;
	while (tmp != aData.end())
	{
		if ((*tmp)[2].Compare(name)==0 && (*tmp)[1].Compare(password)==0) {
			return now;
		}
		now++;
		tmp++;
	}
	return 0;
}
int CReaderDataset::CheckIfHasTheReader(int Id, CString password) {
	vector<CReaderData>::iterator tmp = aData.begin();
	int now = 1; CString Sid; Sid.Format("%d", Id);
	while (tmp != aData.end())
	{
		if ((*tmp)[0].Compare(Sid) == 0 && (*tmp)[1].Compare(password) == 0) {
			return now;
		}
		now++;
		tmp++;
	}
	return 0;
}
void CReaderDataset::ChangeNameById(int Id, CString name) {
	CString sId;
	sId.Format("%d", Id);
	vector<CReaderData>::iterator tmp = aData.begin();
	while (tmp != aData.end())
	{
		if ((*tmp)[0].Compare(sId) == 0) {
			(*tmp).ChangeName(name);
		}
		tmp++;
	}
}
void CReaderDataset::ChangePasswordById(int Id, CString password) {
	CString sId;
	sId.Format("%d", Id);
	vector<CReaderData>::iterator tmp = aData.begin();
	while (tmp != aData.end())
	{
		if ((*tmp)[0].Compare(sId) == 0) {
			(*tmp).ChangePassword(password);
		}
		tmp++;
	}
}
//--------------------------------------------------
/************************************************
   CReaderDataset 成员函数 End

   CHistoryDataset 成员函数Start
*************************************************/
//--------------------------------------------------



CHistoryDataset::CHistoryDataset()
{
	EField aType[7] = { eInt, eInt, eInt, eString,eString,eString,eString };
	char* aName[7] = { "Id","读者Id","图书Id","借出日期","借出时间","归还日期","归还时间" };
	CField field;
	for (int i = 0; i < 7; i++)
	{
		field.eFieldType = aType[i];
		field.sFieldName = aName[i];
		aFields.push_back(field);
	}

	sFileName = getExePath() + "\\History.dat";

}


//--------------------------------------------------
/************************************************
   CHistoryDataset 成员函数 End

   CBorrowDataset 成员函数Start
*************************************************/
//--------------------------------------------------

CBorrowDataset::CBorrowDataset()
{
	EField aType[7] = { eInt, eInt, eInt,eString, eString, eString,eString };
	char* aName[7] = { "Id","读者Id","图书Id","读者姓名","图书名称","日期","时间" };
	CField field;
	for (int i = 0; i < 7; i++)
	{
		field.eFieldType = aType[i];
		field.sFieldName = aName[i];
		aFields.push_back(field);
	}

	sFileName = getExePath() + "\\Borrow.dat";

}

CString CBorrowDataset::CheckIfHasBorrowData(CString BookId, CString ReaderId) {
	vector<CBorrowData>::iterator it = aData.begin();
	while (it != aData.end()) {
		if ((*it)[2].Compare(BookId) == 0 && (*it)[1].Compare(ReaderId) == 0) {
			return (*it)[0];
		}
		it++;
	}
	return "";
}
CString CBorrowDataset::CheckIfHasBorrowDataByName(CString BookName, CString ReaderId) 
{
	vector<CBorrowData>::iterator it = aData.begin();
	while (it != aData.end()) 
	{
		if ((*it)[4].Compare(BookName) == 0 && (*it)[1].Compare(ReaderId) == 0) 
		{
			return (*it)[0];
		}
		it++;
	}
	return "";
}