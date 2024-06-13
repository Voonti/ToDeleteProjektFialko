#include "pch.h"
#include "Project_MFC.h"
#include "framework.h"
#include "CDialogDetails.h"
#include "afxdialogex.h"
#include "Project_MFCDoc.h"

IMPLEMENT_DYNAMIC(CDialogDetails, CDialogEx)

CDialogDetails::CDialogDetails(CProjectMFCDoc* pDoc, double pointSize, double fontSize, CWnd* pParent /*= nullptr*/)
    : CDialogEx(IDD_DIALOG_DETAILS, pParent), m_PointSize(pointSize), m_FontSize(fontSize), pDocum(pDoc)
{
    if (pDoc)
    {
        pExcept = GetExceptPtr(); 
        pDat = pDoc->pDat; 
        m_PointSize = pDoc->pointSize;
        m_FontSize = pDoc->fontSize;
        m_Font = pDoc->font;
    }
    else
    {
        m_PointSize = 12.0;
        m_FontSize = 12.0;
    }
}


CDialogDetails::CDialogDetails(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_DETAILS, pParent), m_PointSize(12), m_FontSize(12)
{


}

CDialogDetails::~CDialogDetails()
{
}

void CDialogDetails::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_POINT_SIZE, m_PointSize);
    DDX_Text(pDX, IDC_EDIT_FONT_SIZE, m_FontSize);
}

BOOL CDialogDetails::OnInitDialog()
{
    m_PointSize = (m_PointSize > 0 && m_PointSize < 1000) ? m_PointSize : 12.0;
    m_FontSize = (m_FontSize > 0 && m_FontSize < 1000) ? m_FontSize : 12.0;

    UpdateData(FALSE); 

    CDialogEx::OnInitDialog();  

    return TRUE;
}

void CDialogDetails::OnBnClickedOk()
{
    CDialogEx::OnOK();

    try {
        UpdateData(TRUE);

        if (pDocum)
        {
            pDocum->pointSize = m_PointSize;
            pDocum->fontSize = m_FontSize;
            pDocum->font = m_Font; 

            pDocum->SetModifiedFlag(); 
            pDocum->UpdateAllViews(NULL); 
        }
    }
    catch (const std::exception& ex) {
        pExcept->PutMessage(IDS_EXCEPTION_WRONG_DATA); 
    }
}


BEGIN_MESSAGE_MAP(CDialogDetails, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDialogDetails::OnBnClickedOk)
END_MESSAGE_MAP()