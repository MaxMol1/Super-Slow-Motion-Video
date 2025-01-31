// dialogs.h : dialog utilities
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//

#include <stdio.h>
#include <shlobj.h>
#include <windows.h>
#define MAX_STRING  1024

bool FAILMSG(HRESULT hr)
{
    if (FAILED(hr))
    {
        WCHAR   wszError[MAX_STRING];

        ::swprintf_s(wszError, MAX_STRING, L"Error code = %08X", hr);
        ::MessageBox(NULL, wszError, L"Error", MB_OK | MB_ICONERROR);
    }

    return FAILED(hr);
}
/////////////////////////////////////////////////////////////////////////////
// CFolderOpenDlg
class CFolderPickerDialog : public CDialogImpl<CFolderPickerDialog> {

    BEGIN_MSG_MAP(CFolderOpenDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnEndDialog)
        COMMAND_ID_HANDLER(IDCANCEL, OnEndDialog)
        COMMAND_ID_HANDLER(IDC_FOLDEROPEN_BROWSE, OnBrowse)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        ::SetFocus(GetDlgItem(IDC_FOLDEROPEN_EDIT));
        CenterWindow();
        return 0;
    }
    LRESULT OnEndDialog(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        GetDlgItemText(IDC_FOLDEROPEN_EDIT, m_bstrName.m_str);
        EndDialog(wID);
        return 0;
    }
    LRESULT OnBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        BROWSEINFO bi = { 0 };
        bi.lpszTitle = _T("Select Folder");
        LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
        if (pidl != 0)
        {
            // get the name of the folder
            TCHAR path[MAX_PATH];
            SHGetPathFromIDList(pidl, path);

            // free memory used
            IMalloc* imalloc = 0;
            if (SUCCEEDED(SHGetMalloc(&imalloc)))
            {
                imalloc->Free(pidl);
                imalloc->Release();
                m_bstrName = path;
            }
        }

        EndDialog(IDOK);

        return 0;
    }
    enum { IDD = IDD_FOLDEROPEN_DIALOG };

    CComBSTR m_bstrName;
};
/////////////////////////////////////////////////////////////////////////////
// CFileOpenDlg

class CFileOpenDlg : public CDialogImpl<CFileOpenDlg>
{
    BEGIN_MSG_MAP(CFileOpenDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnEndDialog)
        COMMAND_ID_HANDLER(IDCANCEL, OnEndDialog)
        COMMAND_ID_HANDLER(IDC_FILEOPEN_BROWSE, OnBrowse)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        ::SetFocus(GetDlgItem(IDC_FILEOPEN_EDIT));
        CenterWindow();
        return 0;
    }
    LRESULT OnEndDialog(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        GetDlgItemText(IDC_FILEOPEN_EDIT, m_bstrName.m_str);
        EndDialog(wID);
        return 0;
    }
    LRESULT OnBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        OPENFILENAME    ofn;
        WCHAR           wszFileName[_MAX_PATH];

        // display open dialog
        memset(&ofn, 0, sizeof(ofn)); // initialize structure to 0/NULL
        wszFileName[0] = L'\0';

        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = wszFileName;
        ofn.nMaxFile = sizeof(wszFileName) / sizeof(wszFileName[0]);
        ofn.lpstrDefExt = NULL;
        ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER;
        ofn.lpstrFilter = L"All files\0*.*\0";
        ofn.nFilterIndex = 0;
        ofn.hwndOwner = m_hWnd;
        ofn.hInstance = _Module.GetResourceInstance();

        if (!GetOpenFileName(&ofn))
            return 0;

        m_bstrName = wszFileName;
        EndDialog(IDOK);

        return 0;
    }

    enum { IDD = IDD_FILEOPEN_DIALOG };

    CComBSTR m_bstrName;
};

/////////////////////////////////////////////////////////////////////////////
// CStringDlg

class CStringDlg : public CDialogImpl<CStringDlg>
{
    BEGIN_MSG_MAP(CStringDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnEndDialog)
        COMMAND_ID_HANDLER(IDCANCEL, OnEndDialog)
    END_MSG_MAP()

    CStringDlg(const WCHAR* wszTitle = NULL, const WCHAR* wszInitialValue = NULL)
    {
        m_bstrTitle = wszTitle;
        m_bstrValue = wszInitialValue;
    }

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        if (m_bstrTitle)
        {
            SetWindowText(m_bstrTitle);
        }

        if (m_bstrValue1)
        {
            SetDlgItemText(IDC_STRING_EDIT1, m_bstrValue1);

        }
        if (m_bstrValue2)
        {
            SetDlgItemText(IDC_STRING_EDIT2, m_bstrValue2);

        }

        ::SetFocus(GetDlgItem(IDC_STRING_EDIT1));
        ::SetFocus(GetDlgItem(IDC_STRING_EDIT2));

        CenterWindow();

        return 0;
    }
    LRESULT OnEndDialog(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        GetDlgItemText(IDC_STRING_EDIT1, m_bstrValue1.m_str);
        GetDlgItemText(IDC_STRING_EDIT2, m_bstrValue2.m_str);

        EndDialog(wID);
        return 0;
    }

    enum { IDD = IDD_STRING_DIALOG };
    CComBSTR m_bstrValue;
    CComBSTR m_bstrValue1;
    CComBSTR m_bstrValue2;
    CComBSTR m_bstrTitle;
};

/////////////////////////////////////////////////////////////////////////////
// CBooleanDlg

class CBooleanDlg : public CDialogImpl<CBooleanDlg>
{
    BEGIN_MSG_MAP(CBooleanDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnEndDialog)
        COMMAND_ID_HANDLER(IDCANCEL, OnEndDialog)
    END_MSG_MAP()

    CBooleanDlg(const WCHAR* wszTitle = NULL, VARIANT_BOOL fInitialValue = VARIANT_FALSE)
    {
        m_bstrTitle = wszTitle;
        m_fValue = fInitialValue;
    }

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        if (m_bstrTitle)
        {
            SetWindowText(m_bstrTitle);
        }

        CheckRadioButton(IDC_BOOLEAN_TRUE, IDC_BOOLEAN_FALSE, (VARIANT_TRUE == m_fValue) ? IDC_BOOLEAN_TRUE : IDC_BOOLEAN_FALSE);

        CenterWindow();
        // Let the system set the focus
        return 1;
    }
    LRESULT OnEndDialog(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        m_fValue = IsDlgButtonChecked(IDC_BOOLEAN_TRUE) ? VARIANT_TRUE : VARIANT_FALSE;

        EndDialog(wID);
        return 0;
    }

    enum { IDD = IDD_BOOLEAN_DIALOG };

    VARIANT_BOOL m_fValue;
    CComBSTR m_bstrTitle;
};

