#pragma once
#include <algorithm>
#include <vector>
#include "Enums.h"
#include "FunctionHelper.h"
#include "TWCache.h"

class TWCacheDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TWCacheDlg)

public:
	TWCacheDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~TWCacheDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TWCacheDlg };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	BOOL OnInitDialog() override;
	BOOL PreTranslateMessage(MSG* pMsg) override;
	DECLARE_MESSAGE_MAP()

private:
	CButton m_CheckboxBuy;
	CButton m_CheckboxSell;
	CComboBox m_ComboboxChoice;
	CComboBox m_ComboboxQuery;
	CEdit m_Edit1;
	CEdit m_Edit2;
	CRichEditCtrl m_RichEdit1;
	CStatic m_StaticText1;
	CStatic m_StaticText2;
	CStatic m_StaticText3;
	std::vector<int> m_EnabledControls = { };

	FunctionHelper m_FunctionHelper;

	TWCache m_Cache;

	void FillComboBox(const std::vector<int>& list, CComboBox& combobox);
	void SetEnabled(const std::vector<int>& ctrlIdList, bool enabled);
	void ResetControl(int controlId);
	CString GetEditText(const CEdit& edit);
public:
	afx_msg void OnCbnSelchangeComboChoice();
	afx_msg void OnCbnSelchangeComboQuery();
	afx_msg void OnBnClickedOk();
	bool IsBuyChecked() { return m_CheckboxBuy.GetCheck() == TRUE; }
	bool IsSellChecked() { return m_CheckboxSell.GetCheck() == TRUE; }
	CString GetEdit1String() { return GetEditText(m_Edit1); }
	CString GetEdit2String() { return GetEditText(m_Edit2); }
	TWCache& GetCache() { return m_Cache; }
};
