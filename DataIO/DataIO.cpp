#include "pch.h" //MFC����£��κ�CPP�ļ�������� ��ͷ�ļ�
#include "DataIO.h"
#include "../MyControls/MyControls.h"


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
void CDataset<T> :: dataToListCtrl(CListCtrl *pListCtrl, vector<T> *m_pData){
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	//�ؼ���ʱ��Ҫ���ƣ��������������ʱ������
	pListCtrl->SetRedraw(FALSE);
	
	//��ձ��ؼ�
	clearCListCtrl(pListCtrl);
	
	//��д���ؼ���ͷ
	doListHeader(pListCtrl);

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
			j == 0 ? pListCtrl->InsertItem(i,  aData[i][j]) : pListCtrl->SetItemText(i,  j,  aData[i][j]);
		}
	}

	//�м�� 13 pixels,��С�п�80pixels
	listAutoWidth(pListCtrl, 13, 80);

	pListCtrl->SetRedraw(TRUE);
	pListCtrl->InvalidateRect(NULL, FALSE);
}

//��д��ͷ
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
	EField aType[6] = {eInt, eString, eString, eString, eString, eDouble};
	char *aName[6] = {"Id","����","���","����","������","����"};
	 
	CField field;
	for(int i = 0; i < 6; i++){
		field.eFieldType = aType[i];
		field.sFieldName = aName[i];
		aFields.push_back(field);
	}

	//�������ݵ��ļ�����ִ���ļ�·�� + �ļ���
	sFileName = getExePath() + "\\Books.dat";
}

//--------------------------------------------------
/************************************************
   CBookDataset ��Ա���� End

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
//--------------------------------------------------
/************************************************
   CReaderDataset ��Ա���� End

   CHistoryDataset ��Ա����Start
*************************************************/
//--------------------------------------------------


//�������Ժ� �������ע��ȡ����
//KevinZ 6.8
/*

CHistoryDataset::CHistoryDataset()
{
	EField aType[6] = { eInt, eString, eInt, eString,eString,eInt };
	char* aName[6] = { "Id","��������","����ͼ��id","����ͼ������","��������","��������" };//0Ϊ���� 1Ϊ�黹
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
	char* aName[5] = { "Id","��������","����ͼ��id","����ͼ������","��������" };
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