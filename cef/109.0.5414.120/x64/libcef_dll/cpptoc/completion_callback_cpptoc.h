// Copyright (c) 2023 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=407df18b90244b245e73c4f69a199663df079f0d$
//

#ifndef CEF_LIBCEF_DLL_CPPTOC_COMPLETION_CALLBACK_CPPTOC_H_
#define CEF_LIBCEF_DLL_CPPTOC_COMPLETION_CALLBACK_CPPTOC_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_callback_capi.h"
#include "include/cef_callback.h"
#include "libcef_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class CefCompletionCallbackCppToC
    : public CefCppToCRefCounted<CefCompletionCallbackCppToC,
                                 CefCompletionCallback,
                                 cef_completion_callback_t> {
 public:
  CefCompletionCallbackCppToC();
  virtual ~CefCompletionCallbackCppToC();
};

#endif  // CEF_LIBCEF_DLL_CPPTOC_COMPLETION_CALLBACK_CPPTOC_H_
