
//
// ===============================================================================
// clReflect, FunctionCall.h - A basic function call API for calling reflected
// functions at runtime.
// -------------------------------------------------------------------------------
// Copyright (c) 2011 Don Williamson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ===============================================================================
//

#pragma once


#include "Database.h"


namespace clcpp
{
	//
	// This code will call reflected functions with signatures that you assume the functions to hold.
	// If the signature you assume is different to the actual signature then your program is likely to
	// become unstable or present a security risk. It's not recommended that you use these functions
	// in the general case and instead build your own function library which performs parameter checking.
	//
	// Note that you can use Koenig lookup to avoid having to specify the clcpp namespace when calling
	// these functions.
	//


	//
	// Call a function with no parameters and to return value.
	//
	inline void CallFunction(const Function* function)
	{
		typedef void (*CallFunc)();
		CallFunc call_func = (CallFunc)function->address;
		call_func();
	}


	//
	// Call a function with one parameter and no return value.
	//
	template <typename A0> inline void CallFunction(const Function* function, A0 a0)
	{
		typedef void (*CallFunc)(A0);
		CallFunc call_func = (CallFunc)function->address;
		call_func(a0);
	}
}