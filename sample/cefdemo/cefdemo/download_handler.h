#pragma once

#include "include/cef_base.h"

class CefDownloadHandler;

// Implement this interface to receive download notifications.
class DownloadListener : public virtual CefBase {
 public:
  // Called when the download is complete.
  virtual void NotifyDownloadComplete(const CefString& fileName) =0;

  // Called if the download fails.
  virtual void NotifyDownloadError(const CefString& fileName) =0;

  virtual bool NotifyGetDownloadPath(TCHAR* pszFolderPath,CefString& fileName) =0;

  virtual void NotifyDownloadProgress(long nReciveLength) = 0;
};

// Create a new download handler to manage download of a single file.
CefRefPtr<CefDownloadHandler> CreateDownloadHandler(
    CefRefPtr<DownloadListener> listener, const CefString& fileName);

