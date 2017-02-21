#pragma once
//����cef��һ�����ࣻ
class CWebClient 
	: public CefClient
	, public CefLifeSpanHandler
{
protected:
	CefRefPtr<CefBrowser> m_Browser;

public:
	CWebClient(void);
	virtual ~CWebClient(void);

	CefRefPtr<CefBrowser> GetBrowser() { return m_Browser; }

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
	{ return this; }

	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) {};
	
	// ���CEF��SP�麯��
	IMPLEMENT_REFCOUNTING(CWebClient);
	IMPLEMENT_LOCKING(CWebClient);
};
