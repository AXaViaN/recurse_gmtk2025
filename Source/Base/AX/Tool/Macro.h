#ifndef _AX_TOOL_MACRO
#define _AX_TOOL_MACRO

  /****************************************************/
 /*****             PLATFORM HELPERS             *****/
/****************************************************/

#if AX_PLATFORM_WINDOWS
	#define AX_DebugBreak()		__debugbreak()
	#define AX_Abort()			exit(EXIT_FAILURE)
#elif AX_PLATFORM_WEB
	#define AX_DebugBreak()
	#define AX_Abort()			// TODO_ASC: Needs testing
#else
	#error No definition found for this OS
#endif // AX_PLATFORM_***

  /****************************************************/
 /*****    CONDITIONAL MACRO FUNCTION HELPERS    *****/
/****************************************************/

// To overcome VS macro expansion bug
#define AX_INTERNAL_MACRO_EXPAND(x) x
#define AX_INTERNAL_MACRO_EXPAND_VA_ARGS(...) __VA_ARGS__
#define AX_INTERNAL_MACRO_EXPAND_MIX_2(x, y) AX_INTERNAL_MACRO_EXPAND(x) ## AX_INTERNAL_MACRO_EXPAND(y)

// AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_x macros help to pick a macro function "dynamically"
// First arg params are to skip arguments and reach to the macro function
#define AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_1(arg1, macro_func, ...) macro_func
#define AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_2(arg1, arg2, macro_func, ...) macro_func
#define AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_3(arg1, arg2, arg3, macro_func, ...) macro_func
#define AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_4(arg1, arg2, arg3, arg4, macro_func, ...) macro_func
#define AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_5(arg1, arg2, arg3, arg4, arg5, macro_func, ...) macro_func
#define AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_6(arg1, arg2, arg3, arg4, arg5, arg6, macro_func, ...) macro_func
#define AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_7(arg1, arg2, arg3, arg4, arg5, arg6, arg7, macro_func, ...) macro_func
#define AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_8(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, macro_func, ...) macro_func
#define AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_9(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, macro_func, ...) macro_func

// TODO_ASC: This explanation is only valid for msvc. Didn't work with emcc because of direct expansion.
//			 So this macro is deprecated from now on.
//			 Use "AX_INTERNAL_MACRO_EXPAND( macro_picker(macro_args, __VA_ARGS__) (macro_args) )" directly.
// Usage (can examine AX_Assert as an example):
//		* Define macros with consecutive argument numbers
//		* Define the main macro as "#define AX_MacroName(...) AX_INTERNAL_CONDITIONAL_MACRO(a, b, c)" where;
//			- a is one of the AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_x, indicating max number of parameters this macro functions can have.
//			- b is __VA_ARGS__, actual parameters.
//			- c is the list of macro functions in descending argument count order.
//		!!! Known limitation: Zero argument functions are not supported.
//			- There is a workaround but it's dodgy. A new solution is needed if we want to use it.
// So the macro_picker here is a AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_x, macro_args are the actual parameters and __VA_ARGS__ is the
// macro function list. macro_picker returns one of the __VA_ARGS__ to call, and we call it with (macro_args)
#define AX_INTERNAL_CONDITIONAL_MACRO(macro_picker, macro_args, ...) AX_INTERNAL_MACRO_EXPAND( macro_picker(macro_args, __VA_ARGS__) (macro_args) )

  /****************************************************/
 /*****            NAMED ENUM HELPERS            *****/
/****************************************************/

#define AX_INTERNAL_NAMED_ENUM_VALUE_TO_ENUM(value) value,
#define AX_INTERNAL_NAMED_ENUM_VALUE_TO_ENUM_ASSIGNMENT(value) value = AX_INTERNAL_NAMED_ENUM_ASSIGNMENT_##value,
#define AX_INTERNAL_NAMED_ENUM_VALUE_TO_STRING(value) {value, #value},

#include <map>
#include <string>

// Usage (can examine Token::Type as an example):
//		* To define an enum like "enum class X { Val1, Val2 = 10 };"
//		* Define a macro function with 2 arguments like "#define AX_X_TYPES(value, assignment) \"
//		* List enum values as "value(Val1) \" and "assignment(Val2) \"
//		* Define a macro for each assignment value like "#define AX_INTERNAL_NAMED_ENUM_ASSIGNMENT_Val2 10"
//		* Call AX_INTERNAL_NAMED_ENUM with type name and types macro like "AX_INTERNAL_NAMED_ENUM(X, AX_X_TYPES)"
#define AX_INTERNAL_NAMED_ENUM(name, values) \
	enum class name { values(AX_INTERNAL_NAMED_ENUM_VALUE_TO_ENUM, AX_INTERNAL_NAMED_ENUM_VALUE_TO_ENUM_ASSIGNMENT) }; \
	static auto name##ToString(name v) -> const std::string& \
	{ \
		static auto strings = std::map<int, std::string>(); \
		if(strings.size() == 0) \
		{ \
			enum { values(AX_INTERNAL_NAMED_ENUM_VALUE_TO_ENUM, AX_INTERNAL_NAMED_ENUM_VALUE_TO_ENUM_ASSIGNMENT) }; \
			strings = std::map<int, std::string>{ values(AX_INTERNAL_NAMED_ENUM_VALUE_TO_STRING, AX_INTERNAL_NAMED_ENUM_VALUE_TO_STRING) }; \
		} \
		return strings[int(v)]; \
	}

  /****************************************************/
 /*****           RULE OF FIVE HELPERS           *****/
/****************************************************/

#define AX_SemanticsSetCopyCtor(type, action) type(const type& other) = action;
#define AX_SemanticsSetCopyAssignment(type, action) type& operator=(const type& other) = action;
#define AX_SemanticsSetMoveCtor(type, action) type(type&& other) noexcept = action;
#define AX_SemanticsSetMoveAssignment(type, action) type& operator=(type&& other) noexcept = action;

#define AX_SemanticsSetCopy(type, action) \
	AX_SemanticsSetCopyCtor(type, action) \
	AX_SemanticsSetCopyAssignment(type, action)

#define AX_SemanticsSetMove(type, action) \
	AX_SemanticsSetMoveCtor(type, action) \
	AX_SemanticsSetMoveAssignment(type, action)

#define AX_SemanticsDeleteAll(type) \
	AX_SemanticsSetCopyCtor(type, delete) \
	AX_SemanticsSetCopyAssignment(type, delete) \
	AX_SemanticsSetMoveCtor(type, delete) \
	AX_SemanticsSetMoveAssignment(type, delete)

#define AX_SemanticsDefaultAll(type) \
	AX_SemanticsSetCopyCtor(type, default) \
	AX_SemanticsSetCopyAssignment(type, default) \
	AX_SemanticsSetMoveCtor(type, default) \
	AX_SemanticsSetMoveAssignment(type, default)

#define AX_SemanticsDeleteCopy(type) \
	AX_SemanticsSetCopy(type, delete)

#define AX_SemanticsDeleteMove(type) \
	AX_SemanticsSetMove(type, delete)

#define AX_SemanticsDefaultCopy(type) \
	AX_SemanticsSetCopy(type, default)

#define AX_SemanticsDefaultMove(type) \
	AX_SemanticsSetMove(type, default)

#define AX_INTERNAL_SEMANTICS_INIT_LIST_1(member) \
	member(std::move(other.member))
#define AX_INTERNAL_SEMANTICS_INIT_LIST_2(member, ...) \
	AX_INTERNAL_SEMANTICS_INIT_LIST_1(member), \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_INIT_LIST_1(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_INIT_LIST_3(member, ...) \
	AX_INTERNAL_SEMANTICS_INIT_LIST_1(member), \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_INIT_LIST_2(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_INIT_LIST_4(member, ...) \
	AX_INTERNAL_SEMANTICS_INIT_LIST_1(member), \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_INIT_LIST_3(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_INIT_LIST_5(member, ...) \
	AX_INTERNAL_SEMANTICS_INIT_LIST_1(member), \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_INIT_LIST_4(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_INIT_LIST_6(member, ...) \
	AX_INTERNAL_SEMANTICS_INIT_LIST_1(member), \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_INIT_LIST_5(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_INIT_LIST_7(member, ...) \
	AX_INTERNAL_SEMANTICS_INIT_LIST_1(member), \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_INIT_LIST_6(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_INIT_LIST_8(member, ...) \
	AX_INTERNAL_SEMANTICS_INIT_LIST_1(member), \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_INIT_LIST_7(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_INIT_LIST_9(member, ...) \
	AX_INTERNAL_SEMANTICS_INIT_LIST_1(member), \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_INIT_LIST_8(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_INIT_LIST(...) \
	AX_INTERNAL_MACRO_EXPAND( \
		AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_9( \
			__VA_ARGS__, \
			AX_INTERNAL_SEMANTICS_INIT_LIST_9, \
			AX_INTERNAL_SEMANTICS_INIT_LIST_8, \
			AX_INTERNAL_SEMANTICS_INIT_LIST_7, \
			AX_INTERNAL_SEMANTICS_INIT_LIST_6, \
			AX_INTERNAL_SEMANTICS_INIT_LIST_5, \
			AX_INTERNAL_SEMANTICS_INIT_LIST_4, \
			AX_INTERNAL_SEMANTICS_INIT_LIST_3, \
			AX_INTERNAL_SEMANTICS_INIT_LIST_2, \
			AX_INTERNAL_SEMANTICS_INIT_LIST_1 \
		) (__VA_ARGS__) \
	)

#define AX_INTERNAL_SEMANTICS_RESET_VALUE_1(member) \
	other.member = std::move(decltype(other.member){});
#define AX_INTERNAL_SEMANTICS_RESET_VALUE_2(member, ...) \
	AX_INTERNAL_SEMANTICS_RESET_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_RESET_VALUE_1(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_RESET_VALUE_3(member, ...) \
	AX_INTERNAL_SEMANTICS_RESET_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_RESET_VALUE_2(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_RESET_VALUE_4(member, ...) \
	AX_INTERNAL_SEMANTICS_RESET_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_RESET_VALUE_3(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_RESET_VALUE_5(member, ...) \
	AX_INTERNAL_SEMANTICS_RESET_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_RESET_VALUE_4(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_RESET_VALUE_6(member, ...) \
	AX_INTERNAL_SEMANTICS_RESET_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_RESET_VALUE_5(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_RESET_VALUE_7(member, ...) \
	AX_INTERNAL_SEMANTICS_RESET_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_RESET_VALUE_6(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_RESET_VALUE_8(member, ...) \
	AX_INTERNAL_SEMANTICS_RESET_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_RESET_VALUE_7(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_RESET_VALUE_9(member, ...) \
	AX_INTERNAL_SEMANTICS_RESET_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_RESET_VALUE_8(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_RESET_VALUE(...) \
	AX_INTERNAL_MACRO_EXPAND( \
		AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_9( \
			__VA_ARGS__, \
			AX_INTERNAL_SEMANTICS_RESET_VALUE_9, \
			AX_INTERNAL_SEMANTICS_RESET_VALUE_8, \
			AX_INTERNAL_SEMANTICS_RESET_VALUE_7, \
			AX_INTERNAL_SEMANTICS_RESET_VALUE_6, \
			AX_INTERNAL_SEMANTICS_RESET_VALUE_5, \
			AX_INTERNAL_SEMANTICS_RESET_VALUE_4, \
			AX_INTERNAL_SEMANTICS_RESET_VALUE_3, \
			AX_INTERNAL_SEMANTICS_RESET_VALUE_2, \
			AX_INTERNAL_SEMANTICS_RESET_VALUE_1 \
		) (__VA_ARGS__) \
	)

#define AX_INTERNAL_SEMANTICS_MOVE_VALUE_1(member) \
	member = std::move(other.member);
#define AX_INTERNAL_SEMANTICS_MOVE_VALUE_2(member, ...) \
	AX_INTERNAL_SEMANTICS_MOVE_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_MOVE_VALUE_1(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_MOVE_VALUE_3(member, ...) \
	AX_INTERNAL_SEMANTICS_MOVE_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_MOVE_VALUE_2(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_MOVE_VALUE_4(member, ...) \
	AX_INTERNAL_SEMANTICS_MOVE_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_MOVE_VALUE_3(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_MOVE_VALUE_5(member, ...) \
	AX_INTERNAL_SEMANTICS_MOVE_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_MOVE_VALUE_4(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_MOVE_VALUE_6(member, ...) \
	AX_INTERNAL_SEMANTICS_MOVE_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_MOVE_VALUE_5(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_MOVE_VALUE_7(member, ...) \
	AX_INTERNAL_SEMANTICS_MOVE_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_MOVE_VALUE_6(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_MOVE_VALUE_8(member, ...) \
	AX_INTERNAL_SEMANTICS_MOVE_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_MOVE_VALUE_7(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_MOVE_VALUE_9(member, ...) \
	AX_INTERNAL_SEMANTICS_MOVE_VALUE_1(member) \
	AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_SEMANTICS_MOVE_VALUE_8(__VA_ARGS__) )
#define AX_INTERNAL_SEMANTICS_MOVE_VALUE(...) \
	AX_INTERNAL_MACRO_EXPAND( \
		AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_9( \
			__VA_ARGS__, \
			AX_INTERNAL_SEMANTICS_MOVE_VALUE_9, \
			AX_INTERNAL_SEMANTICS_MOVE_VALUE_8, \
			AX_INTERNAL_SEMANTICS_MOVE_VALUE_7, \
			AX_INTERNAL_SEMANTICS_MOVE_VALUE_6, \
			AX_INTERNAL_SEMANTICS_MOVE_VALUE_5, \
			AX_INTERNAL_SEMANTICS_MOVE_VALUE_4, \
			AX_INTERNAL_SEMANTICS_MOVE_VALUE_3, \
			AX_INTERNAL_SEMANTICS_MOVE_VALUE_2, \
			AX_INTERNAL_SEMANTICS_MOVE_VALUE_1 \
		) (__VA_ARGS__) \
	)

#define AX_SemanticsImplMoveCtor(type, ...) \
	inline type(type&& other) noexcept : \
		AX_INTERNAL_SEMANTICS_INIT_LIST(__VA_ARGS__) \
	{ \
		AX_INTERNAL_SEMANTICS_RESET_VALUE(__VA_ARGS__) \
	}

#define AX_SemanticsImplMoveAssignment(type, ...) \
	inline type& operator=(type&& other) noexcept \
	{ \
		if (this == &other) { return *this; } \
		AX_INTERNAL_SEMANTICS_MOVE_VALUE(__VA_ARGS__) \
		AX_INTERNAL_SEMANTICS_RESET_VALUE(__VA_ARGS__) \
		return *this; \
	}

#define AX_SemanticsImplMove(type, ...) \
	AX_SemanticsImplMoveCtor(type, __VA_ARGS__) \
	AX_SemanticsImplMoveAssignment(type, __VA_ARGS__)

#endif // !_AX_TOOL_MACRO
