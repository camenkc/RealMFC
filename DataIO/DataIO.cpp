#include "pch.h" //MFC����£��κ�CPP�ļ�������� ��ͷ�ļ�
#include "DataIO.h"
#include "../MyControls/MyControls.h"

#include <io.h> 

//--------------------------------------------------
/************************************************
   CDataset ��Ա���� Start
*************************************************/
//--------------------------------------------------


//���ڸ���ָ����nRow����vector aData��ָ��һ��ɾ��
template <class T>
void CDataset<T>::deleteOneRow(int nRow) {
	vector<T>::iterator tmp = aData.begin() + nRow;
	aData.erase(tmp);
}
//�����ֶ�����ȡ�ֶ���Ϣ
template <class T>
const CField &CDataset<T> :: getFieldByName(CString sFieldName){
	int nNumField = aFields.size();
	for(int i = 0; i < nNumField; i++){
		if(aFields[i].sFieldName == sFieldName){
			return aFields[i];
		}
	}

	throw CString("�ֶβ����ڣ�" + sFieldName);
}

//��¼�Ƿ����
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

//��¼�Ƿ����
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

//��ȡĳ�ֶε����ֵ����Id�����ֶα��������ͣ��������쳣��
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


//���ֶ�sNameFieldģ����ѯ��������浽aDataset��
//��sNameField="����" sValLike="��",�� �ߵ���ѧ�����Դ��� ������
template <class T>
void CDataset<T> :: likeQuery(CString sNameField, CString sValLike, vector<T> &aDataset){
	const CField &fd = getFieldByName(sNameField);

	if(fd.eFieldType != eString){
		throw CString("���ܽ���ģ����ѯ��\n�ֶΣ�" + sNameField + "  �����ִ�����");
	}

	aDataset.clear();

	int nNumItem = aData.size();
	int nNumField = aFields.size();

	//�������ݼ�
	for(int i = 0; i < nNumItem; i++){
		if(aData[i][sNameField].Find(sValLike) >= 0){
			aDataset.push_back(aData[i]);
		}
	}
}


//���� ����Ϊ���ڱȽϵ� ����ָ��
template <class T>
void CDataset<T> ::dataOrder(bool comp(const T& b1, const T& b2)) {
	sort(aData.begin(), aData.end(), comp);
}

//���ڼ�ֵ��������ָ��
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

//����ȫ�����ݵ��ļ� ���ر���ļ�¼��
template <class T>
int CDataset<T> ::saveAllDataToFile() {

	//������׷�ӷ�ʽ���ļ�sFileName(���ļ��������򴴽�,��������ɾ������)	
	ofstream os(sFileName.GetBuffer(), ios::binary );
	int nNumofSaved = 0;
	if (!os) {
		throw "�򿪻򴴽��ļ�ʧ�ܣ�\n" + sFileName;
	}
	//�õ�������
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
	//�õ����� �������� ����ֵ��� ��Erase 
	//���ǵ������ͨ��ÿ�ּ�¼��Ψһ��ʶ�����ɾ�� ����ֱ��ȡ[0]
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

//����һ����¼���ļ�
template <class T>
bool CDataset<T> :: saveOneItemToFile(T *dataItem){
	//������׷�ӷ�ʽ���ļ�sFileName(���ļ��������򴴽�)
	//ios::app==��׷�ӣ��򿪺��ļ�ָ����ĩβ
	
	ofstream os(sFileName.GetBuffer(),ios::binary | ios::app);

	if(!os){
		throw "�򿪻򴴽��ļ�ʧ�ܣ�\n" + sFileName;
	}

	os.write((char *)dataItem, sizeof(T));

	os.close();

	//ͬʱ���ݱ��ֵ� aData ��
	aData.push_back(*dataItem);

	return true;
}

//��ȡȫ�����ݵ� aData ��,���ض�ȡ�ļ�¼��
template <class T>
int CDataset<T> :: readAllData(){
	//�����ƶ���ʽ���ļ�sFileName(���ļ������ڲ������ļ�����ʱ��ʧ��)
	ifstream is(sFileName.GetBuffer(),ios::binary | ios::_Nocreate);

	if(!is){
		AfxMessageBox("���ļ�ʧ�ܣ�\n" + sFileName);
		return 0;
	}

	T item;

	//��������������
	aData.clear();

	//���ļ���ÿ�ζ�һ����¼
	while(is.good()){		
		is.read((char *)&item, sizeof(T));
		if (!is.good()) break;
		aData.push_back(item);
	}

	is.close();

	return aData.size();
}


//�Ʊ���m_pData�е�������д���б�ؼ� pListCtrl ��
template <class T>
void CDataset<T> :: dataToListCtrl(CListCtrl *pListCtrl, vector<T> *m_pData,int CtrlCode){
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	//�ؼ���ʱ��Ҫ���ƣ��������������ʱ������
	pListCtrl->SetRedraw(FALSE);
	
	//��ձ��ؼ�
	clearCListCtrl(pListCtrl);
	
	//��д���ؼ���ͷ
	if (CtrlCode == 1)//��Ҫ����������
	{
		doListHeader(pListCtrl, 1);
	}
	else {
		doListHeader(pListCtrl);
	}
	if(m_pData == NULL){
		m_pData = &aData;
	}

	//�ֲ��������ǳ�Ա����
	vector<T> &aData = *m_pData;

	int nNumItem = aData.size();
	int nNumField = aFields.size();

	//�������ݼ����Ʊ�
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

	//�м�� 13 pixels,��С�п�80pixels
	listAutoWidth(pListCtrl, 13, 80);

	pListCtrl->SetRedraw(TRUE);
	pListCtrl->InvalidateRect(NULL, FALSE);
}


//��д��ͷ
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
/////////////////////��̬��Ա����
//-------------------------------------------------------

//���ݵ�Ԫ�����ֳ����Զ������п�
template <class T>
void CDataset<T> :: listAutoWidth(CListCtrl *pListCtrl, int nColSpace, int nMinWid){
	pListCtrl->SetRedraw(FALSE);
	
	int nColumnWidth, nHeaderWidth;
	int nColumnCount = 0;
	
	//��ȡ�������
	CHeaderCtrl* pHeaderCtrl = pListCtrl->GetHeaderCtrl();
	if(!pHeaderCtrl){//�ޱ�ͷ
		return;
	}

	nColumnCount = pHeaderCtrl->GetItemCount();
	if(nColumnCount < 1){//���б�
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


 //����б�ؼ� pListCtrl ����ʾ������
template <class T>
void CDataset<T> :: clearCListCtrl(CListCtrl *pListCtrl){
	int nColumnCount =  pListCtrl->GetHeaderCtrl()->GetItemCount();

	pListCtrl->DeleteAllItems(); 

	for (int i = 0;i < nColumnCount; i++) {
		pListCtrl->DeleteColumn(0);
	}
}

//����б�ؼ��ƶ��е�����
template<class T>
void CDataset<T>::clearByCol(CListCtrl* pListCtrl, int col)
{
	pListCtrl->DeleteColumn(col);
}

//���EXE����λ��
template <class T>
CString CDataset<T> :: getExePath(){
	char szPath[MAX_PATH];

	::GetModuleFileName(NULL, szPath, sizeof(szPath));//����exe�ļ�����·��

	CString strPath = szPath;

	int nPos = strPath.ReverseFind('\\');

	strPath = strPath.Left(nPos).TrimRight(_T("\\"));

	return strPath;	
}
//--------------------------------------------------
/************************************************
    CDataset ��Ա���� End
	
	CBookDataset ��Ա���� Start
*************************************************/
//--------------------------------------------------
CBookDataset :: CBookDataset(){ //���캯�� ��� aFields �ĳ�ʼ��
	//�����CBookData����Ա���Ͷ�Ӧ
	EField aType[7] = {eInt, eString, eString, eString, eString, eDouble,eString};
	char *aName[7] = {"Id","����","���","����","������","����","���"};
	 
	CField field;
	for(int i = 0; i < 7; i++){
		field.eFieldType = aType[i];
		field.sFieldName = aName[i];
		aFields.push_back(field);
	}

	//�������ݵ��ļ�����ִ���ļ�·�� + �ļ���
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
//����ֵΪCString���͵��鼮Id ��û�з���""
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
   CBookDataset ��Ա���� End

   CBookBackup ��Ա����Start
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
   CBookBackup ��Ա���� End

   CReaderDataset ��Ա����Start
*************************************************/
//--------------------------------------------------

CReaderDataset :: CReaderDataset()
{
	EField aType[4] = { eInt, eString, eString, eInt };
	char* aName[4] = { "Id","����","����","�û�Ȩ��" };//�û�Ȩ�޷�2�� 0Ϊadmin 1Ϊ��ͨ�û�
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
   CReaderDataset ��Ա���� End

   CHistoryDataset ��Ա����Start
*************************************************/
//--------------------------------------------------



CHistoryDataset::CHistoryDataset()
{
	EField aType[7] = { eInt, eInt, eInt, eString,eString,eString,eString };
	char* aName[7] = { "Id","����Id","ͼ��Id","�������","���ʱ��","�黹����","�黹ʱ��" };
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
   CHistoryDataset ��Ա���� End

   CBorrowDataset ��Ա����Start
*************************************************/
//--------------------------------------------------

CBorrowDataset::CBorrowDataset()
{
	EField aType[7] = { eInt, eInt, eInt,eString, eString, eString,eString };
	char* aName[7] = { "Id","����Id","ͼ��Id","��������","ͼ������","����","ʱ��" };
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