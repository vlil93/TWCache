#pragma once
#include <atlstr.h>
#include <string>

namespace Microsoft {
    namespace VisualStudio {
        namespace CppUnitTestFramework
        {
            template<> inline std::wstring ToString<CString>(const CString& t) { RETURN_WIDE_STRING(t); }
        }
    }
}