// TWCacheDlg.cpp : implementation file
//

#include "afxdialogex.h"
#include "pch.h"
#include "TWCacheMain.h"
#include "TWCacheDlg.h"

IMPLEMENT_DYNAMIC(TWCacheDlg, CDialogEx)

TWCacheDlg::TWCacheDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TW_CACHE_DLG, pParent),
	m_FunctionHelper(this)
{
	AfxInitRichEdit2();
}

TWCacheDlg::~TWCacheDlg()
{
}

void TWCacheDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_BUY, m_CheckboxBuy);
	DDX_Control(pDX, IDC_CHECK_SELL, m_CheckboxSell);
	DDX_Control(pDX, IDC_COMBO_CHOICE, m_ComboboxChoice);
	DDX_Control(pDX, IDC_COMBO_QUERY, m_ComboboxQuery);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_RICHEDIT1, m_RichEdit1);
	DDX_Control(pDX, IDC_STATIC1, m_StaticText1);
	DDX_Control(pDX, IDC_STATIC2, m_StaticText2);
	DDX_Control(pDX, IDC_STATIC3, m_StaticText3);
}

BOOL TWCacheDlg::OnInitDialog()
{
	auto val = CDialogEx::OnInitDialog();
	m_ComboboxChoice.ResetContent();
	m_ComboboxQuery.ResetContent();

	FillComboBox({ IDS_ADD_ORDER, IDS_CANCEL_ORDER, IDS_QUERY }, m_ComboboxChoice);
	FillComboBox({ IDS_QUERY_WAS_TRADED, IDS_QUERY_GREATER_THAN,
		IDS_QUERY_TOTAL_QUANTITY, IDS_QUERY_BUY_SELL_DIFF }, m_ComboboxQuery);
	return val;
}

BEGIN_MESSAGE_MAP(TWCacheDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOICE, &TWCacheDlg::OnCbnSelchangeComboChoice)
	ON_CBN_SELCHANGE(IDC_COMBO_QUERY, &TWCacheDlg::OnCbnSelchangeComboQuery)
	ON_BN_CLICKED(IDOK, &TWCacheDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// TWCacheDlg message handlers


void TWCacheDlg::OnCbnSelchangeComboChoice()
{
	SetEnabled(m_EnabledControls, false);
	CString stringBuffer;
	std::vector<int> controlBuffer;
	switch (m_ComboboxChoice.GetCurSel())
	{
		case ComboboxOptions::Choice::Add:
		{
			controlBuffer = { IDC_EDIT1, IDC_EDIT2, IDC_STATIC1, IDC_STATIC2, 
				IDC_STATIC3, IDC_RICHEDIT1, IDC_CHECK_BUY, IDC_CHECK_SELL };
			SetEnabled(controlBuffer, true);
			stringBuffer.LoadStringW(IDS_BOND_ID);
			m_StaticText1.SetWindowTextW(stringBuffer);
			stringBuffer.LoadStringW(IDS_QUANTITY);
			m_StaticText2.SetWindowTextW(stringBuffer);
			m_FunctionHelper.SetFuntion(Function::AddOrder);
			break;
		}
		case ComboboxOptions::Choice::Cancel:
		{
			controlBuffer = { IDC_EDIT1, IDC_STATIC1, IDC_STATIC3, IDC_RICHEDIT1 };
			SetEnabled(controlBuffer, true);
			stringBuffer.LoadStringW(IDS_ORDER_ID);
			m_StaticText1.SetWindowTextW(stringBuffer);
			m_FunctionHelper.SetFuntion(Function::CancelOrder);
			break;
		}
		case ComboboxOptions::Choice::Query:
		{
			controlBuffer = { IDC_COMBO_QUERY };
			SetEnabled(controlBuffer, true);
			m_FunctionHelper.SetFuntion(Function::Unidentified);
			break;
		}
		default:
		{
			break;
		}
	}
}

void TWCacheDlg::OnCbnSelchangeComboQuery()
{
	std::vector<int> controlBuffer = m_EnabledControls;
	controlBuffer.erase(
		std::remove(controlBuffer.begin(), controlBuffer.end(), IDC_COMBO_QUERY),
		controlBuffer.end()
	);
	SetEnabled(controlBuffer, false);
	CString stringBuffer;

	switch (m_ComboboxQuery.GetCurSel())
	{
		case ComboboxOptions::Query::WasTraded:
		{
			controlBuffer = { IDC_EDIT1, IDC_STATIC1, 
				IDC_STATIC3, IDC_RICHEDIT1 };
			SetEnabled(controlBuffer, true);
			stringBuffer.LoadStringW(IDS_BOND_ID);
			m_StaticText1.SetWindowTextW(stringBuffer);
			m_FunctionHelper.SetFuntion(Function::QueryWasTraded);
			break;
		}
		case ComboboxOptions::Query::GreaterThan:
		{
			controlBuffer = { IDC_EDIT1, IDC_STATIC1, IDC_STATIC3, 
				IDC_RICHEDIT1, IDC_CHECK_BUY, IDC_CHECK_SELL };
			SetEnabled(controlBuffer, true);
			stringBuffer.LoadStringW(IDS_QUANTITY);
			m_StaticText1.SetWindowTextW(stringBuffer);
			m_FunctionHelper.SetFuntion(Function::QueryGreaterThan);
			break;
		}
		case ComboboxOptions::Query::TotalQuantity:
		{
			controlBuffer = { IDC_EDIT1, IDC_STATIC1, 
				IDC_STATIC2, IDC_STATIC3, IDC_RICHEDIT1, IDC_CHECK_BUY, IDC_CHECK_SELL };
			SetEnabled(controlBuffer, true);
			stringBuffer.LoadStringW(IDS_BOND_ID);
			m_StaticText1.SetWindowTextW(stringBuffer);
			stringBuffer.LoadStringW(IDS_NOTE_IF_VALUE_EMPTY);
			m_StaticText2.SetWindowTextW(stringBuffer);
			m_FunctionHelper.SetFuntion(Function::QueryTotalQuantity);
			break;
		}
		case ComboboxOptions::Query::BuySellDiff:
		{
			controlBuffer = { IDC_EDIT1, IDC_STATIC1, 
				IDC_STATIC2, IDC_STATIC3, IDC_RICHEDIT1 };
			SetEnabled(controlBuffer, true);
			stringBuffer.LoadStringW(IDS_BOND_ID);
			m_StaticText1.SetWindowTextW(stringBuffer);
			stringBuffer.LoadStringW(IDS_NOTE_IF_VALUE_EMPTY);
			m_StaticText2.SetWindowTextW(stringBuffer);
			m_FunctionHelper.SetFuntion(Function::QueryBuySellDiff);
			break;
		}
	}
}


BOOL TWCacheDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void TWCacheDlg::FillComboBox(const std::vector<int>& list, CComboBox& combobox)
{
	CString str;
	for (int strId : list)
	{
		str.LoadStringW(strId);
		combobox.AddString(str);
	}
}

void TWCacheDlg::SetEnabled(const std::vector<int>& ctrlIdList, bool enabled)
{
	if (ctrlIdList.empty())
	{
		return;
	}
	CWnd* control = nullptr;
	int state = enabled ? SW_SHOW : SW_HIDE;
	for (int ctrlId : ctrlIdList)
	{
		control = GetDlgItem(ctrlId);
		if (control != nullptr)
		{
			ResetControl(ctrlId);
			control->ShowWindow(state);
		}
	}

	if (enabled)
	{
		m_EnabledControls.insert(m_EnabledControls.end(), ctrlIdList.begin(), ctrlIdList.end());
	}
	else
	{
		m_EnabledControls.erase(
			std::remove_if(
				m_EnabledControls.begin(),
				m_EnabledControls.end(),
				[&ctrlIdList](int value)
				{
					return std::find(ctrlIdList.begin(), ctrlIdList.end(), value) != ctrlIdList.end();
				}
			),
			m_EnabledControls.end()
		);
	}
}

void TWCacheDlg::ResetControl(int controlId)
{
	switch (controlId)
	{
		case IDC_COMBO_CHOICE:
		{
			m_ComboboxChoice.SetCurSel(-1);
			break;
		}
		case IDC_COMBO_QUERY:
		{
			m_ComboboxQuery.SetCurSel(-1);
			break;
		}
		case IDC_EDIT1:
		{
			m_Edit1.SetWindowTextW(_T(""));
			break;
		}
		case IDC_EDIT2:
		{
			m_Edit2.SetWindowTextW(_T(""));
			break;
		}
		case IDC_RICHEDIT1:
		{
			m_RichEdit1.SetWindowTextW(_T(""));
			break;
		}
		case IDC_CHECK_BUY:
		{
			m_CheckboxBuy.SetCheck(FALSE);
			break;
		}
		case IDC_CHECK_SELL:
		{
			m_CheckboxSell.SetCheck(FALSE);
			break;
		}
	}
}

void TWCacheDlg::OnBnClickedOk()
{
	TWErr err;
	m_RichEdit1.SetWindowTextW(m_FunctionHelper.ExecuteFunction(err));
}

CString TWCacheDlg::GetEditText(const CEdit& edit)
{ 
	CString buffer; 
	edit.GetWindowTextW(buffer);
	return buffer; 
}
