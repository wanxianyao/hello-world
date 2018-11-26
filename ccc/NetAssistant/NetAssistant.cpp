#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "NetToolFrameWnd.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CNetToolFrameWnd* pFrame = new CNetToolFrameWnd();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("ÍøÂç¹¤¾ß"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 600);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}