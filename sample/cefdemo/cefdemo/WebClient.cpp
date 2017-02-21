#include "StdAfx.h"
#include "WebClient.h"

CWebClient::CWebClient(void)
{
}


CWebClient::~CWebClient(void)
{
}
/*
void CWebClient::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefV8Context> context) {


  CefRefPtr<CefV8Value> object = context->GetGlobal();

  // Add the V8 bindings.
  // Create the new V8 object.
  CefRefPtr<CefV8Value> testObjPtr = CefV8Value::CreateObject(NULL);
  // Add the new V8 object to the global window object with the name
  // "cef_test".
  object->SetValue("cef_test", testObjPtr, V8_PROPERTY_ATTRIBUTE_NONE);

  // Create an instance of ClientV8FunctionHandler as the V8 handler.
  CefRefPtr<CefV8Handler> handlerPtr = new CClientV8FunctionHandler();

  // Add a new V8 function to the cef_test object with the name "Dump".
  testObjPtr->SetValue("Dump",
      CefV8Value::CreateFunction("Dump", handlerPtr),
      V8_PROPERTY_ATTRIBUTE_NONE);
  // Add a new V8 function to the cef_test object with the name "Call".
  testObjPtr->SetValue("Call",
      CefV8Value::CreateFunction("Call", handlerPtr),
      V8_PROPERTY_ATTRIBUTE_NONE);

  std::string url = frame->GetURL();
  if (url == performance_test::kTestUrl)
    performance_test::InitTest(browser, frame, object);
}
*/