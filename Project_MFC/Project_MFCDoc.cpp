
// Project_MFCDoc.cpp : implementation of the CProjectMFCDoc class
//
#pragma once

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Project_MFC.h"
#endif

#include "Project_MFCDoc.h"

#include <propkey.h>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CProjectMFCDoc

IMPLEMENT_DYNCREATE(CProjectMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(CProjectMFCDoc, CDocument)
END_MESSAGE_MAP()


// CProjectMFCDoc construction/destruction

CProjectMFCDoc::CProjectMFCDoc() noexcept
	: pointSize(12.0), fontSize(12.0)
{
	// TODO: add one-time construction code here
	memset(&font, 0, sizeof(LOGFONT));
	font.lfHeight = 20; 
	font.lfWidth = 10;  
	pDat = NULL;
	pExcept = GetExceptPtr();
}

CProjectMFCDoc::~CProjectMFCDoc()
{
	if (pDat)
		delete pDat;
	pDat = NULL;
}

BOOL CProjectMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	if (pDat)
		delete pDat;

	try {
		pDat = new MY_DATA(1);
		pExcept->ResetDat(pDat);
	}
	catch (std::bad_alloc)
	{
		pExcept->PutMessage(1002);
	}

	return TRUE;
}


void CProjectMFCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		size_t capacity = pDat->size();
		ar.Write(&capacity, sizeof(capacity));

		const MY_POINT* pTabTemp = pDat->getPtab();
		for (size_t i = 0; i < capacity; ++i)
		{
			ar.Write(&pTabTemp[i].x, sizeof(pTabTemp[i].x));
			ar.Write(&pTabTemp[i].y, sizeof(pTabTemp[i].y));
			ar.Write(&pTabTemp[i].numb, sizeof(pTabTemp[i].numb));
			ar.Write(&pTabTemp[i].color, sizeof(pTabTemp[i].color));
			ar.Write(&pointSize, sizeof(pointSize));
			ar.Write(&fontSize, sizeof(fontSize));
			ar.Write(&font, sizeof(font));

			CString nameStr = pTabTemp[i].name != nullptr ? CString(pTabTemp[i].name) : _T("");
			ar.WriteString(nameStr + _T("\n"));
		}

		pExcept->PutMessage(1001);
	}
	else
	{
		size_t capacity;
		ar.Read(&capacity, sizeof(capacity));

		for (size_t i = 0; i < capacity; ++i)
		{
			MY_POINT myPointObj;

			ar.Read(&myPointObj.x, sizeof(myPointObj.x));
			ar.Read(&myPointObj.y, sizeof(myPointObj.y));
			ar.Read(&myPointObj.numb, sizeof(myPointObj.numb));
			ar.Read(&myPointObj.color, sizeof(myPointObj.color));
			ar.Read(&pointSize, sizeof(pointSize));
			ar.Read(&fontSize, sizeof(fontSize));
			ar.Read(&font, sizeof(font));

			CString nameStr;
			ar.ReadString(nameStr);

			if (!nameStr.IsEmpty())
			{
				int length = nameStr.GetLength();
				myPointObj.name = new char[length + 1];
				strcpy_s(myPointObj.name, length + 1, CT2A(nameStr));
			}
			else
			{
				myPointObj.name = nullptr;
			}


			pDat->addObject(myPointObj);
		}
	}
}


#ifdef SHARED_HANDLERS

// Support for thumbnails
void CProjectMFCDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CProjectMFCDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CProjectMFCDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CProjectMFCDoc diagnostics

#ifdef _DEBUG
void CProjectMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CProjectMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CProjectMFCDoc commands
